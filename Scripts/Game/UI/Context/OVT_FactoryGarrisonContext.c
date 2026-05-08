class OVT_FactoryGarrisonContext : OVT_UIContext
{
	OVT_FactoryData m_Factory;

	protected ref SCR_SpinBoxComponent m_GroupSpin;
	protected ref SCR_ButtonTextComponent m_GarrisonButton;

	override void OnShow()
	{
		Widget w = m_wRoot.FindAnyWidget("GarrisonSpin");
		m_GroupSpin = SCR_SpinBoxComponent.Cast(w.FindHandler(SCR_SpinBoxComponent));

		m_GroupSpin.GetOnLeftArrowClick().Insert(UpdateInfo);
		m_GroupSpin.GetOnRightArrowClick().Insert(UpdateInfo);

		m_GarrisonButton = SCR_ButtonTextComponent.Cast(m_wRoot.FindAnyWidget("AddToGarrison").FindHandler(SCR_ButtonTextComponent));
		m_GarrisonButton.m_OnClicked.Insert(AddToGarrison);

		Widget closeButton = m_wRoot.FindAnyWidget("CloseButton");
		SCR_InputButtonComponent btn = SCR_InputButtonComponent.Cast(closeButton.FindHandler(SCR_InputButtonComponent));
		btn.m_OnClicked.Insert(CloseLayout);

		Refresh();
	}

	override void OnClose()
	{
		m_GroupSpin.GetOnLeftArrowClick().Remove(UpdateInfo);
		m_GroupSpin.GetOnRightArrowClick().Remove(UpdateInfo);
		m_GarrisonButton.m_OnClicked.Remove(AddToGarrison);
	}

	protected void Refresh()
	{
		OVT_Faction faction = OVT_Global.GetConfig().GetPlayerFaction();
		OVT_DifficultySettings diff = OVT_Global.GetDifficulty();

		foreach (int i, ResourceName res : faction.m_aGroupPrefabSlots)
		{
			IEntity spawn = OVT_Global.SpawnEntityPrefab(res, "0 0 0", "0 0 0", false);
			SCR_AIGroup aigroup = SCR_AIGroup.Cast(spawn);
			int numSoldiers = aigroup.m_aUnitPrefabSlots.Count();
			int hrCost = diff.recruitHRCost * numSoldiers;
			int supplyCost = diff.recruitSupplyCost * numSoldiers;

			SCR_EditableGroupComponent group = SCR_EditableGroupComponent.Cast(spawn.FindComponent(SCR_EditableGroupComponent));
			if (group)
				m_GroupSpin.AddItem(group.GetDisplayName(), false, new OVT_GroupUIInfo(res, hrCost, supplyCost, numSoldiers));

			SCR_EntityHelper.DeleteEntityAndChildren(spawn);
		}

		UpdateInfo();
	}

	protected void AddToGarrison()
	{
		OVT_GroupUIInfo uiinfo = OVT_GroupUIInfo.Cast(m_GroupSpin.GetCurrentItemData());
		if (!uiinfo) return;

		OVT_ResistanceFactionManager rf = OVT_Global.GetResistanceFaction();
		if (rf.GetResistanceHR() < uiinfo.soldierCost)
		{
			CloseLayout();
			SCR_HintManagerComponent.ShowCustomHint("Not enough resistance HR to garrison this group.", "Factory Garrison", 5);
			return;
		}

		OVT_EconomyManagerComponent economy = OVT_Global.GetEconomy();
		if (economy.GetResistanceSupplies() < uiinfo.equipmentCost)
		{
			CloseLayout();
			SCR_HintManagerComponent.ShowCustomHint("Not enough resistance supplies to garrison this group.", "Factory Garrison", 5);
			return;
		}

		if (!m_Factory) return;

		OVT_Global.GetServer().AddGarrisonFactory(m_Factory, uiinfo.resource);
		CloseLayout();
	}

	protected void UpdateInfo()
	{
		OVT_GroupUIInfo uiinfo = OVT_GroupUIInfo.Cast(m_GroupSpin.GetCurrentItemData());
		if (!uiinfo) return;

		TextWidget w = TextWidget.Cast(m_wRoot.FindAnyWidget("SoldierCost"));
		if (w) w.SetText(uiinfo.soldierCost.ToString() + " HR");

		w = TextWidget.Cast(m_wRoot.FindAnyWidget("EquipmentCost"));
		if (w) w.SetText(uiinfo.equipmentCost.ToString() + " Supplies");

		w = TextWidget.Cast(m_wRoot.FindAnyWidget("TotalCost"));
		if (w) w.SetText(uiinfo.soldierCost.ToString() + " HR / " + uiinfo.equipmentCost.ToString() + " Supplies");
	}
}
