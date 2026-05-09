class OVT_DevMenuContext : OVT_UIContext
{
	protected OVT_TownManagerComponent m_Towns;
	protected OVT_FactoryManagerComponent m_Factories;
	protected OVT_OccupyingFactionManager m_OF;
	protected OVT_ResistanceFactionManager m_RF;

	protected SCR_SpinBoxComponent m_TownSpin;
	protected SCR_SpinBoxComponent m_FactorySpin;
	protected TextWidget m_StatusText;

	override void PostInit()
	{
		m_Towns = OVT_Global.GetTowns();
		m_Factories = OVT_FactoryManagerComponent.GetInstance();
		m_OF = OVT_Global.GetOccupyingFaction();
		m_RF = OVT_Global.GetResistanceFaction();
	}

	override void OnShow()
	{
		m_StatusText = TextWidget.Cast(m_wRoot.FindAnyWidget("StatusText"));

		// Close button
		Widget closeButton = m_wRoot.FindAnyWidget("CloseButton");
		if (closeButton)
		{
			SCR_InputButtonComponent action = SCR_InputButtonComponent.Cast(closeButton.FindHandler(SCR_InputButtonComponent));
			if (action) action.m_OnActivated.Insert(CloseLayout);
		}

		SCR_ButtonTextComponent comp;

		// Resources
		comp = SCR_ButtonTextComponent.GetButtonText("HRMinus500", m_wRoot);
		if (comp) comp.m_OnClicked.Insert(HRMinus500);
		comp = SCR_ButtonTextComponent.GetButtonText("HRMinus100", m_wRoot);
		if (comp) comp.m_OnClicked.Insert(HRMinus100);
		comp = SCR_ButtonTextComponent.GetButtonText("HRPlus100", m_wRoot);
		if (comp) comp.m_OnClicked.Insert(HRPlus100);
		comp = SCR_ButtonTextComponent.GetButtonText("HRPlus500", m_wRoot);
		if (comp) comp.m_OnClicked.Insert(HRPlus500);

		comp = SCR_ButtonTextComponent.GetButtonText("SupMinus1000", m_wRoot);
		if (comp) comp.m_OnClicked.Insert(SupMinus1000);
		comp = SCR_ButtonTextComponent.GetButtonText("SupMinus500", m_wRoot);
		if (comp) comp.m_OnClicked.Insert(SupMinus500);
		comp = SCR_ButtonTextComponent.GetButtonText("SupPlus500", m_wRoot);
		if (comp) comp.m_OnClicked.Insert(SupPlus500);
		comp = SCR_ButtonTextComponent.GetButtonText("SupPlus1000", m_wRoot);
		if (comp) comp.m_OnClicked.Insert(SupPlus1000);

		comp = SCR_ButtonTextComponent.GetButtonText("CashMinus500", m_wRoot);
		if (comp) comp.m_OnClicked.Insert(CashMinus500);
		comp = SCR_ButtonTextComponent.GetButtonText("CashPlus500", m_wRoot);
		if (comp) comp.m_OnClicked.Insert(CashPlus500);
		comp = SCR_ButtonTextComponent.GetButtonText("CashPlus5000", m_wRoot);
		if (comp) comp.m_OnClicked.Insert(CashPlus5000);

		// War State
		comp = SCR_ButtonTextComponent.GetButtonText("WL1", m_wRoot);
		if (comp) comp.m_OnClicked.Insert(WL1);
		comp = SCR_ButtonTextComponent.GetButtonText("WL2", m_wRoot);
		if (comp) comp.m_OnClicked.Insert(WL2);
		comp = SCR_ButtonTextComponent.GetButtonText("WL3", m_wRoot);
		if (comp) comp.m_OnClicked.Insert(WL3);
		comp = SCR_ButtonTextComponent.GetButtonText("WL4", m_wRoot);
		if (comp) comp.m_OnClicked.Insert(WL4);
		comp = SCR_ButtonTextComponent.GetButtonText("WL5", m_wRoot);
		if (comp) comp.m_OnClicked.Insert(WL5);

		comp = SCR_ButtonTextComponent.GetButtonText("Agg0", m_wRoot);
		if (comp) comp.m_OnClicked.Insert(Agg0);
		comp = SCR_ButtonTextComponent.GetButtonText("Agg25", m_wRoot);
		if (comp) comp.m_OnClicked.Insert(Agg25);
		comp = SCR_ButtonTextComponent.GetButtonText("Agg50", m_wRoot);
		if (comp) comp.m_OnClicked.Insert(Agg50);
		comp = SCR_ButtonTextComponent.GetButtonText("Agg75", m_wRoot);
		if (comp) comp.m_OnClicked.Insert(Agg75);
		comp = SCR_ButtonTextComponent.GetButtonText("Agg100", m_wRoot);
		if (comp) comp.m_OnClicked.Insert(Agg100);

		comp = SCR_ButtonTextComponent.GetButtonText("EnemyHRZero", m_wRoot);
		if (comp) comp.m_OnClicked.Insert(EnemyHRZero);
		comp = SCR_ButtonTextComponent.GetButtonText("EnemyHRMinus100", m_wRoot);
		if (comp) comp.m_OnClicked.Insert(EnemyHRMinus100);
		comp = SCR_ButtonTextComponent.GetButtonText("EnemyHRPlus100", m_wRoot);
		if (comp) comp.m_OnClicked.Insert(EnemyHRPlus100);
		comp = SCR_ButtonTextComponent.GetButtonText("EnemyHRFull", m_wRoot);
		if (comp) comp.m_OnClicked.Insert(EnemyHRFull);

		// Towns
		Widget townSpinWidget = m_wRoot.FindAnyWidget("TownSpin");
		if (townSpinWidget)
			m_TownSpin = SCR_SpinBoxComponent.Cast(townSpinWidget.FindHandler(SCR_SpinBoxComponent));
		if (m_TownSpin && m_Towns)
		{
			m_TownSpin.ClearAll();
			foreach (OVT_TownData town : m_Towns.m_Towns)
			{
				int townId = m_Towns.GetTownID(town);
				m_TownSpin.AddItem(m_Towns.GetTownName(townId));
			}
		}
		comp = SCR_ButtonTextComponent.GetButtonText("TownTeleport", m_wRoot);
		if (comp) comp.m_OnClicked.Insert(TownTeleport);
		comp = SCR_ButtonTextComponent.GetButtonText("TownSup0", m_wRoot);
		if (comp) comp.m_OnClicked.Insert(TownSup0);
		comp = SCR_ButtonTextComponent.GetButtonText("TownSup50", m_wRoot);
		if (comp) comp.m_OnClicked.Insert(TownSup50);
		comp = SCR_ButtonTextComponent.GetButtonText("TownSup100", m_wRoot);
		if (comp) comp.m_OnClicked.Insert(TownSup100);

		// Factories
		Widget factSpinWidget = m_wRoot.FindAnyWidget("FactorySpin");
		if (factSpinWidget)
			m_FactorySpin = SCR_SpinBoxComponent.Cast(factSpinWidget.FindHandler(SCR_SpinBoxComponent));
		if (m_FactorySpin && m_Factories)
		{
			m_FactorySpin.ClearAll();
			int count = m_Factories.m_aFactories.Count();
			for (int i = 0; i < count; i++)
			{
				m_FactorySpin.AddItem("Factory " + (i + 1).ToString());
			}
		}
		comp = SCR_ButtonTextComponent.GetButtonText("FactoryTeleport", m_wRoot);
		if (comp) comp.m_OnClicked.Insert(FactoryTeleport);
		comp = SCR_ButtonTextComponent.GetButtonText("FactorySetResistance", m_wRoot);
		if (comp) comp.m_OnClicked.Insert(FactorySetResistance);
		comp = SCR_ButtonTextComponent.GetButtonText("FactorySetEnemy", m_wRoot);
		if (comp) comp.m_OnClicked.Insert(FactorySetEnemy);

		RefreshStatus();
	}

	protected void RefreshStatus()
	{
		if (!m_StatusText) return;
		OVT_EconomyManagerComponent economy = OVT_Global.GetEconomy();
		int rHR = 0;
		int rSup = 0;
		if (m_RF) rHR = m_RF.GetResistanceHR();
		if (economy) rSup = economy.GetResistanceSupplies();
		int wl = 1;
		float agg = 0;
		int eHR = 0;
		if (m_OF)
		{
			wl = m_OF.GetWarLevel();
			agg = m_OF.GetAggression();
			eHR = m_OF.GetHR();
		}
		m_StatusText.SetText("HR: " + rHR.ToString() + "  |  Sup: " + rSup.ToString() + "  |  WL: " + wl.ToString() + "  |  Agg: " + agg.ToString() + "  |  EnemyHR: " + eHR.ToString());
	}

	// ---- RESOURCES ----
	protected void HRMinus500() { OVT_Global.GetServer().DevAddResistanceHR(-500); RefreshStatus(); }
	protected void HRMinus100() { OVT_Global.GetServer().DevAddResistanceHR(-100); RefreshStatus(); }
	protected void HRPlus100()  { OVT_Global.GetServer().DevAddResistanceHR(100);  RefreshStatus(); }
	protected void HRPlus500()  { OVT_Global.GetServer().DevAddResistanceHR(500);  RefreshStatus(); }

	protected void SupMinus1000() { OVT_Global.GetServer().DevAddResistanceSupplies(-1000); RefreshStatus(); }
	protected void SupMinus500()  { OVT_Global.GetServer().DevAddResistanceSupplies(-500);  RefreshStatus(); }
	protected void SupPlus500()   { OVT_Global.GetServer().DevAddResistanceSupplies(500);   RefreshStatus(); }
	protected void SupPlus1000()  { OVT_Global.GetServer().DevAddResistanceSupplies(1000);  RefreshStatus(); }

	protected void CashMinus500() { OVT_Global.GetServer().AddPlayerMoney(m_iPlayerID, -500); }
	protected void CashPlus500()  { OVT_Global.GetServer().AddPlayerMoney(m_iPlayerID, 500); }
	protected void CashPlus5000() { OVT_Global.GetServer().AddPlayerMoney(m_iPlayerID, 5000); }

	// ---- WAR STATE ----
	protected void WL1() { OVT_Global.GetServer().DevSetWarLevel(1); RefreshStatus(); }
	protected void WL2() { OVT_Global.GetServer().DevSetWarLevel(2); RefreshStatus(); }
	protected void WL3() { OVT_Global.GetServer().DevSetWarLevel(3); RefreshStatus(); }
	protected void WL4() { OVT_Global.GetServer().DevSetWarLevel(4); RefreshStatus(); }
	protected void WL5() { OVT_Global.GetServer().DevSetWarLevel(5); RefreshStatus(); }

	protected void Agg0()   { OVT_Global.GetServer().DevSetAggression(0);   RefreshStatus(); }
	protected void Agg25()  { OVT_Global.GetServer().DevSetAggression(25);  RefreshStatus(); }
	protected void Agg50()  { OVT_Global.GetServer().DevSetAggression(50);  RefreshStatus(); }
	protected void Agg75()  { OVT_Global.GetServer().DevSetAggression(75);  RefreshStatus(); }
	protected void Agg100() { OVT_Global.GetServer().DevSetAggression(100); RefreshStatus(); }

	protected void EnemyHRZero()    { OVT_Global.GetServer().DevAddEnemyHR(-99999);  RefreshStatus(); }
	protected void EnemyHRMinus100(){ OVT_Global.GetServer().DevAddEnemyHR(-100);    RefreshStatus(); }
	protected void EnemyHRPlus100() { OVT_Global.GetServer().DevAddEnemyHR(100);     RefreshStatus(); }
	protected void EnemyHRFull()    { OVT_Global.GetServer().DevAddEnemyHR(99999);   RefreshStatus(); }

	// ---- TOWNS ----
	protected OVT_TownData GetSelectedTown()
	{
		if (!m_TownSpin || !m_Towns) return null;
		int idx = m_TownSpin.GetCurrentItem();
		idx = idx - 1;
		if (idx < 0 || idx >= m_Towns.m_Towns.Count()) return null;
		return m_Towns.m_Towns[idx];
	}

	protected void TownTeleport()
	{
		Print("[OVT DevMenu] TownTeleport clicked, playerID=" + m_iPlayerID);
		OVT_TownData town = GetSelectedTown();
		if (!town) { Print("[OVT DevMenu] TownTeleport: no town selected"); return; }
		Print("[OVT DevMenu] TownTeleport to " + town.location.ToString());
		OVT_Global.GetServer().DevTeleport(m_iPlayerID, town.location);
	}

	protected void TownSup0()
	{
		OVT_TownData town = GetSelectedTown();
		if (!town) return;
		OVT_Global.GetServer().DevSetTownSupport(town.location, 0);
	}

	protected void TownSup50()
	{
		OVT_TownData town = GetSelectedTown();
		if (!town) return;
		OVT_Global.GetServer().DevSetTownSupport(town.location, 50);
	}

	protected void TownSup100()
	{
		OVT_TownData town = GetSelectedTown();
		if (!town) return;
		OVT_Global.GetServer().DevSetTownSupport(town.location, 100);
	}

	// ---- FACTORIES ----
	protected int GetSelectedFactoryId()
	{
		if (!m_FactorySpin || !m_Factories) return -1;
		int id = m_FactorySpin.GetCurrentItem();
		return id - 1;
	}

	protected void FactoryTeleport()
	{
		Print("[OVT DevMenu] FactoryTeleport clicked, playerID=" + m_iPlayerID);
		int id = GetSelectedFactoryId();
		if (id < 0 || id >= m_Factories.m_aFactories.Count()) { Print("[OVT DevMenu] FactoryTeleport: invalid id=" + id); return; }
		Print("[OVT DevMenu] FactoryTeleport to " + m_Factories.m_aFactories[id].location.ToString());
		OVT_Global.GetServer().DevTeleport(m_iPlayerID, m_Factories.m_aFactories[id].location);
	}

	protected void FactorySetResistance()
	{
		int id = GetSelectedFactoryId();
		if (id < 0) return;
		OVT_Global.GetServer().DevSetFactoryFaction(id, OVT_Global.GetConfig().GetPlayerFactionIndex());
	}

	protected void FactorySetEnemy()
	{
		int id = GetSelectedFactoryId();
		if (id < 0) return;
		OVT_Global.GetServer().DevSetFactoryFaction(id, OVT_Global.GetConfig().GetOccupyingFactionIndex());
	}
}
