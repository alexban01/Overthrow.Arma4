class OVT_ShopContext : OVT_UIContext
{
	protected OVT_ShopComponent m_Shop;
	protected int m_iPageNum = 0;
	protected int m_SelectedResource = -1;
	protected ResourceName m_SelectedResourceName;
	protected int m_iNumPages = 0;

	protected ref array<int> m_aSortedIds = new array<int>();
	protected ref map<int, ResourceName> m_mIdToResource = new map<int, ResourceName>();
	protected ref map<int, string> m_mIdToNameLower = new map<int, string>();
	protected ref array<int> m_aFilteredIds = new array<int>();

	protected SCR_EditBoxComponent m_SearchBox;
	protected string m_sSearchQuery = "";

	override void PostInit()
	{
		if(SCR_Global.IsEditMode()) return;
		m_Economy.m_OnPlayerMoneyChanged.Insert(OnPlayerMoneyChanged);
	}

	protected void OnPlayerMoneyChanged(string playerId, int amount)
	{
		if(playerId == m_sPlayerID && m_bIsActive)
		{
			TextWidget money = TextWidget.Cast(m_wRoot.FindAnyWidget("PlayerMoney"));
			money.SetText("$" + amount);
		}
	}

	override void OnShow()
	{
		m_iPageNum = 0;
		m_SelectedResource = -1; // Reset selection to ensure first item gets selected
		m_sSearchQuery = "";

		// Set up the buy button
		Widget buyButton = m_wRoot.FindAnyWidget("BuyButton");
		SCR_InputButtonComponent action = SCR_InputButtonComponent.Cast(buyButton.FindHandler(SCR_InputButtonComponent));
		action.m_OnActivated.Insert(Buy);

		// Set up the sell button with updated visibility logic
		Widget sellButton = m_wRoot.FindAnyWidget("SellButton");
		bool showSellButton = ShouldShowSellButton(m_Shop.m_ShopType);
		sellButton.SetVisible(showSellButton);
		if (showSellButton)
		{
			SCR_InputButtonComponent sellAction = SCR_InputButtonComponent.Cast(sellButton.FindHandler(SCR_InputButtonComponent));
			sellAction.m_OnActivated.Insert(Sell);
		}

		// Set up the previous button
		Widget prevButton = m_wRoot.FindAnyWidget("PrevButton");
		SCR_InputButtonComponent btn = SCR_InputButtonComponent.Cast(prevButton.FindHandler(SCR_InputButtonComponent));
		btn.m_OnActivated.Insert(PreviousPage);

		// Set up the next button
		Widget nextButton = m_wRoot.FindAnyWidget("NextButton");
		btn = SCR_InputButtonComponent.Cast(nextButton.FindHandler(SCR_InputButtonComponent));
		btn.m_OnActivated.Insert(NextPage);

		// Set up the close button
		Widget closeButton = m_wRoot.FindAnyWidget("CloseButton");
		btn = SCR_InputButtonComponent.Cast(closeButton.FindHandler(SCR_InputButtonComponent));
		btn.m_OnActivated.Insert(CloseLayout);

		// Set up search box (polled in OnActiveFrame for cross-API compatibility)
		m_SearchBox = SCR_EditBoxComponent.GetEditBoxComponent("SearchBox", m_wRoot);
		if(m_SearchBox)
			m_SearchBox.SetValue("");

		BuildSortedItemList();
		ApplyFilter();
		Refresh();
	}

	// Helper function to determine sell button visibility
	private bool ShouldShowSellButton(OVT_ShopType shopType)
	{
		if (shopType == OVT_ShopType.SHOP_VEHICLE)
		{
			return false; // Hide for vehicle shops
		}
		if (shopType == OVT_ShopType.SHOP_GUNDEALER)
		{
			float multiplier = OVT_Global.GetConfig().m_Difficulty.gunDealerSellPriceMultiplier;
			return multiplier != 0; // Show only if multiplier is not zero
		}
		return true; // Show for all other shop types
	}


	void PreviousPage()
	{
		if(!m_wRoot) return;
		m_iPageNum--;
		if(m_iPageNum < 0) m_iPageNum = 0;

		Refresh();
	}

	void NextPage()
	{
		if(!m_wRoot) return;
		m_iPageNum++;
		if(m_iPageNum > m_iNumPages-1) m_iPageNum = m_iNumPages-1;

		Refresh();
	}

	override protected void OnActiveFrame(float timeSlice)
	{
		if(!m_SearchBox) return;
		string current = m_SearchBox.GetValue();
		if(current == m_sSearchQuery) return;

		m_sSearchQuery = current;
		m_iPageNum = 0;
		m_SelectedResource = -1;
		ApplyFilter();
		Refresh();
	}

	// Builds the master id->resource map and a sorted array of ids (alphabetical by display name).
	// Called once on open; the inventory composition does not change while the menu is up.
	protected void BuildSortedItemList()
	{
		m_aSortedIds.Clear();
		m_mIdToResource.Clear();
		m_mIdToNameLower.Clear();

		array<int> ids = new array<int>();
		array<ResourceName> resources = new array<ResourceName>();

		if(m_Shop.m_bProcurement)
		{
			OVT_ParkingComponent parking = EPF_Component<OVT_ParkingComponent>.Find(m_Shop.GetOwner());
			array<OVT_ParkingType> parkingTypes();
			parking.GetParkingTypes(parkingTypes);

			array<ResourceName> allVehicles();
			m_Economy.GetAllNonOccupyingFactionVehicles(allVehicles, true);

			foreach(ResourceName res : allVehicles)
			{
				int id = m_Economy.GetInventoryId(res);
				OVT_ParkingType parkingType = m_Economy.GetParkingType(id);
				if(!parkingTypes.Contains(parkingType)) continue;
				ids.Insert(id);
				resources.Insert(res);
			}

			// Filter out Mobile FOB vehicles if restricted to officers only
			if(OVT_Global.GetConfig().m_ConfigFile.mobileFOBOfficersOnly && !OVT_Global.GetPlayers().LocalPlayerIsOfficer())
			{
				for(int j = resources.Count() - 1; j >= 0; j--)
				{
					string vehiclePath = resources[j];
					if(vehiclePath.Contains("OverthrowMobileFOB"))
					{
						resources.Remove(j);
						ids.Remove(j);
					}
				}
			}
		}
		else
		{
			for(int i = 0; i < m_Shop.m_aInventory.Count(); i++)
			{
				int id = m_Shop.m_aInventory.GetKey(i);
				ids.Insert(id);
				resources.Insert(m_Economy.GetResource(id));
			}
		}

		array<string> sortKeys = new array<string>();
		for(int i = 0; i < ids.Count(); i++)
		{
			ResourceName res = resources[i];
			string displayName = ResolveDisplayName(res);
			string lower = displayName;
			lower.ToLower();
			sortKeys.Insert(lower);
			m_mIdToNameLower[ids[i]] = lower;
			m_mIdToResource[ids[i]] = res;
		}

		// Sort indices alphabetically by lowercased display name
		// (insertion sort — shop inventories are small, low hundreds at most)
		array<int> order = new array<int>();
		for(int i = 0; i < ids.Count(); i++) order.Insert(i);
		for(int i = 1; i < order.Count(); i++)
		{
			int cur = order[i];
			string curKey = sortKeys[cur];
			int j = i - 1;
			while(j >= 0 && sortKeys[order[j]] > curKey)
			{
				order[j+1] = order[j];
				j--;
			}
			order[j+1] = cur;
		}

		foreach(int idx : order)
		{
			m_aSortedIds.Insert(ids[idx]);
		}
	}

	protected string ResolveDisplayName(ResourceName res)
	{
		if(m_Economy.IsVehicle(res))
		{
			SCR_EditableVehicleUIInfo vinfo = OVT_Global.GetVehicleUIInfo(res);
			if(vinfo) return vinfo.GetName();
			SCR_EditableEntityUIInfo einfo = OVT_Global.GetEditableUIInfo(res);
			if(einfo) return einfo.GetName();
		}
		else
		{
			UIInfo info = OVT_Global.GetItemUIInfo(res);
			if(info) return info.GetName();
		}
		return res;
	}

	protected void ApplyFilter()
	{
		m_aFilteredIds.Clear();
		string q = m_sSearchQuery;
		q.ToLower();
		foreach(int id : m_aSortedIds)
		{
			if(q != "" && !m_mIdToNameLower[id].Contains(q)) continue;
			m_aFilteredIds.Insert(id);
		}
	}

	void Refresh()
	{
		if(!m_Shop) return;
		if(!m_wRoot) return;

		TextWidget money = TextWidget.Cast(m_wRoot.FindAnyWidget("PlayerMoney"));
		money.SetText("$" + m_Economy.GetPlayerMoney(m_sPlayerID));

		TextWidget pages = TextWidget.Cast(m_wRoot.FindAnyWidget("Pages"));
		Widget grid = m_wRoot.FindAnyWidget("BrowserGrid");

		int total = m_aFilteredIds.Count();
		m_iNumPages = Math.Ceil(total / 15.0);
		if(m_iNumPages < 1) m_iNumPages = 1;
		if(m_iPageNum >= m_iNumPages) m_iPageNum = m_iNumPages - 1;
		if(m_iPageNum < 0) m_iPageNum = 0;

		pages.SetText((m_iPageNum + 1).ToString() + "/" + m_iNumPages);

		int wi = 0;
		for(int i = m_iPageNum * 15; i < (m_iPageNum + 1) * 15 && i < total; i++)
		{
			int id = m_aFilteredIds[i];
			ResourceName res = m_mIdToResource[id];

			if(wi == 0 && m_SelectedResource == -1)
				SelectItem(res);

			Widget w = grid.FindWidget("ShopMenu_Card" + wi);
			w.SetOpacity(1);
			OVT_ShopMenuCardComponent card = OVT_ShopMenuCardComponent.Cast(w.FindHandler(OVT_ShopMenuCardComponent));

			int buy = m_Economy.GetShopBuyPrice(id, m_Shop, m_Shop.GetOwner().GetOrigin(), m_iPlayerID);
			int qty;
			if(m_Shop.m_bProcurement)
				qty = 100;
			else
				qty = m_Shop.GetStock(id);

			card.Init(res, buy, qty, this);
			wi++;
		}

		for(; wi < 15; wi++)
		{
			Widget w = grid.FindWidget("ShopMenu_Card" + wi);
			w.SetOpacity(0);
		}
	}

	override void SelectItem(ResourceName res)
	{
		int id = m_Economy.GetInventoryId(res);
		m_SelectedResource = id;
		m_SelectedResourceName = res;
		TextWidget typeName = TextWidget.Cast(m_wRoot.FindAnyWidget("SelectedTypeName"));
		TextWidget details = TextWidget.Cast(m_wRoot.FindAnyWidget("SelectedDetails"));
		TextWidget desc = TextWidget.Cast(m_wRoot.FindAnyWidget("SelectedDescription"));

		int buy, sell, qty, max;

		if(m_Shop.m_bProcurement)
		{
			buy = m_Economy.GetShopBuyPrice(id, m_Shop, m_Shop.GetOwner().GetOrigin(), m_iPlayerID);
			sell = buy;
			qty = 100;
			max = 100;
		}else{
			buy = m_Economy.GetShopBuyPrice(id, m_Shop, m_Shop.GetOwner().GetOrigin(), m_iPlayerID);
			sell = m_Economy.GetSellPrice(id, m_Shop.GetOwner().GetOrigin());
			if(m_Shop.m_ShopType == OVT_ShopType.SHOP_GUNDEALER)
			{
				sell = sell * OVT_Global.GetConfig().m_Difficulty.gunDealerSellPriceMultiplier;
			}
			qty = m_Shop.GetStock(id);
			OVT_TownData town = m_Shop.GetTown();
			int townID = OVT_Global.GetTowns().GetTownID(town);
			max = m_Economy.GetTownMaxStock(townID, id);
		}

		if(m_Economy.IsVehicle(res))
		{
			SCR_EditableVehicleUIInfo info = OVT_Global.GetVehicleUIInfo(res);
			if(info)
			{
				typeName.SetText(info.GetName());
				desc.SetText(info.GetDescription());
			}else{
				SCR_EditableEntityUIInfo uiinfo = OVT_Global.GetEditableUIInfo(res);
				typeName.SetText(uiinfo.GetName());
				desc.SetText(uiinfo.GetDescription());
			}
			if(m_Shop.m_bProcurement)
			{
				details.SetText("$" + buy);
			}else{
				details.SetText("$" + buy + "\n" + qty + " #OVT-Shop_InStock");
			}
		}else{
			UIInfo info = OVT_Global.GetItemUIInfo(res);
			if(info)
			{
				typeName.SetText(info.GetName());
				details.SetText("#OVT-Shop_Buying: $" + buy + "\n#OVT-Shop_Selling: $" + sell + "\n" + qty + "/" + max + " #OVT-Shop_InStock");
				desc.SetText(info.GetDescription());
			}
		}
	}

	void SetShop(OVT_ShopComponent shop)
	{
		m_Shop = shop;
	}

	void Buy(Widget src, float value = 1, EActionTrigger reason = EActionTrigger.DOWN)
	{
		if(!m_Shop.m_bProcurement && m_Shop.GetStock(m_SelectedResource) < 1) return;

		int playerId = OVT_Global.GetPlayers().GetPlayerIDFromPersistentID(m_sPlayerID);
		IEntity player = GetGame().GetPlayerManager().GetPlayerControlledEntity(playerId);
		if(!player) return;

		int cost = m_Economy.GetShopBuyPrice(m_SelectedResource, m_Shop, m_Shop.GetOwner().GetOrigin(), m_iPlayerID);

		if(!m_Economy.PlayerHasMoney(m_sPlayerID, cost)) return;

		SCR_InventoryStorageManagerComponent inventory = SCR_InventoryStorageManagerComponent.Cast(player.FindComponent( SCR_InventoryStorageManagerComponent ));
		if(!inventory) return;

		if(m_Shop.m_ShopType == OVT_ShopType.SHOP_VEHICLE)
		{
			OVT_Global.GetServer().BuyVehicle(m_Shop, m_SelectedResource, m_iPlayerID);
			CloseLayout();
			return;
		}

		OVT_Global.GetServer().Buy(m_Shop, m_SelectedResource, 1, m_iPlayerID);
		SelectItem(m_SelectedResourceName);
	}

	void Sell(Widget src, float value = 1, EActionTrigger reason = EActionTrigger.DOWN)
	{
		int playerId = OVT_Global.GetPlayers().GetPlayerIDFromPersistentID(m_sPlayerID);
		IEntity player = GetGame().GetPlayerManager().GetPlayerControlledEntity(playerId);
		if(!player) return;

		int cost = m_Economy.GetSellPrice(m_SelectedResource, m_Shop.GetOwner().GetOrigin());
		if(m_Shop.m_ShopType == OVT_ShopType.SHOP_GUNDEALER)
		{
			cost = cost * OVT_Global.GetConfig().m_Difficulty.gunDealerSellPriceMultiplier;
		}

		SCR_InventoryStorageManagerComponent inventory = SCR_InventoryStorageManagerComponent.Cast(player.FindComponent( SCR_InventoryStorageManagerComponent ));
		if(!inventory) return;

		autoptr array<IEntity> items = new array<IEntity>;
		inventory.GetItems(items);

		ResourceName res = m_Economy.GetResource(m_SelectedResource);

		foreach(IEntity ent : items)
		//Chris - Make this work better for variants
		{
			string prefab = ent.GetPrefabData().GetPrefabName();
			if (prefab == "{63E8322E2ADD4AA7}Prefabs/Weapons/Rifles/AK74/Rifle_AK74_GP25.et")
			{
			prefab = "{FA5C25BF66A53DCF}Prefabs/Weapons/Rifles/AK74/Rifle_AK74.et";
			}
			if (prefab == "{EB404DC9E1BCB750}Prefabs/Weapons/Rifles/AK74/Rifle_AK74N_1P29.et" || prefab == "{BC6C9476FB3219A7}Prefabs/Weapons/Rifles/AK74/Rifle_AK74N_GP25.et")
			{
			prefab = "{96DFD2E7E63B3386}Prefabs/Weapons/Rifles/AK74/Rifle_AK74N.et";
			}
			if (res == "{7A82FE978603F137}Prefabs/Weapons/Launchers/RPG7/Launcher_RPG7.et" && prefab == "{E8A55396050E1762}Prefabs/Weapons/Launchers/RPG7/Launcher_RPG7_PGO7.et")
			{
			prefab = res;
			}
			if (res == "{E8A55396050E1762}Prefabs/Weapons/Launchers/RPG7/Launcher_RPG7_PGO7.et" && prefab == "{7A82FE978603F137}Prefabs/Weapons/Launchers/RPG7/Launcher_RPG7.et")
			{
			prefab = res;
			}
			if(prefab == res)
			{
				if(inventory.TryDeleteItem(ent))
				{
					m_Economy.AddPlayerMoney(m_iPlayerID, cost, true);
					m_Shop.AddToInventory(m_SelectedResource, 1);
					SelectItem(m_SelectedResourceName);
					break;
				}
			}
		}
	}

}
