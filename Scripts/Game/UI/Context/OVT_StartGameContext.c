class OVT_StartGameContext : OVT_UIContext
{
	protected FactionManager m_Factions;

	protected ref array<FactionKey> m_FactionKeys;

	protected ref array<int> m_aStartingCashValues;
	protected ref array<int> m_aRespawnCostValues;
	protected ref array<int> m_aFastTravelCostValues;
	protected ref array<float> m_aDetectionRangeValues;
	protected ref array<float> m_aDisguiseValues;
	protected ref array<int> m_aMaxQRFValues;

	override void OnShow()
	{
#ifdef PLATFORM_CONSOLE
		Widget xbox = m_wRoot.FindAnyWidget("XBOXWarning");
		xbox.SetVisible(true);
#endif

		Widget startButton = m_wRoot.FindAnyWidget("StartButton");
		SCR_InputButtonComponent action = SCR_InputButtonComponent.Cast(startButton.FindHandler(SCR_InputButtonComponent));

		if(action)
			action.m_OnActivated.Insert(StartGame);

		m_Factions = GetGame().GetFactionManager();
		int i = 0;

		autoptr array<Faction> factions = new array<Faction>;
		m_Factions.GetFactionsList(factions);

		Widget of = m_wRoot.FindAnyWidget("OccupyingFactionSpinner");
		SCR_SpinBoxComponent ofSpin = SCR_SpinBoxComponent.Cast(of.FindHandler(SCR_SpinBoxComponent));
		ofSpin.m_OnChanged.Insert(OnSpinOccupyingFaction);

		Widget sf = m_wRoot.FindAnyWidget("SupportingFactionSpinner");
		SCR_SpinBoxComponent sfSpin = SCR_SpinBoxComponent.Cast(sf.FindHandler(SCR_SpinBoxComponent));
		sfSpin.m_OnChanged.Insert(OnSpinSupportingFaction);

		int selectedOccupyingFaction = 0;
		int selectedSupportingFaction = 0;

		OVT_OverthrowConfigComponent config = OVT_Global.GetConfig();

		config.SetOccupyingFaction(config.m_sDefaultOccupyingFaction);

		foreach(Faction fac : factions)
		{
			OVT_Faction faction = OVT_Global.GetFactions().GetOverthrowFactionByKey(fac.GetFactionKey());
			if(!faction) continue;
			if(faction.IsPlayable()) continue;
			if(faction.GetFactionKey() == "CIV") continue;

			ofSpin.AddItem(fac.GetUIInfo().GetName(),false,fac);
			sfSpin.AddItem(fac.GetUIInfo().GetName(),false,fac);

			if(faction.GetFactionKey() == config.m_sDefaultOccupyingFaction) selectedOccupyingFaction = i;
			if(faction.GetFactionKey() == config.m_sDefaultSupportingFaction) selectedSupportingFaction = i;

			i++;
		}
		ofSpin.SetCurrentItem(selectedOccupyingFaction);
		sfSpin.SetCurrentItem(selectedSupportingFaction);

		Widget diff = m_wRoot.FindAnyWidget("DifficultySpinner");
		SCR_SpinBoxComponent spin = SCR_SpinBoxComponent.Cast(diff.FindHandler(SCR_SpinBoxComponent));
		spin.m_OnChanged.Insert(OnSpinDifficulty);

		foreach(OVT_DifficultySettings preset : config.m_aDifficultyPresets)
		{
			spin.AddItem(preset.name, false, preset);
		}

		Widget description = m_wRoot.FindAnyWidget("DifficultyDescription");
		TextWidget text = TextWidget.Cast(description);

		OVT_DifficultySettings defaultPreset;
		if (RplSession.Mode() == RplMode.None)
		{
			spin.SetCurrentItem(0);
			defaultPreset = config.m_aDifficultyPresets.Get(0);
		} else {
			spin.SetCurrentItem(1);
			defaultPreset = config.m_aDifficultyPresets.Get(1);
		}
		text.SetText(defaultPreset.description);
		config.m_Difficulty = defaultPreset;

		SetupCustomSpinboxes();
		UpdateCustomPanelVisibility(defaultPreset);
	}

	protected SCR_SpinBoxComponent GetCustomSpin(string name)
	{
		Widget w = m_wRoot.FindAnyWidget(name);
		return SCR_SpinBoxComponent.Cast(w.FindHandler(SCR_SpinBoxComponent));
	}

	protected void SetupCustomSpinboxes()
	{
		m_aStartingCashValues = new array<int>();
		m_aStartingCashValues.Insert(0);
		m_aStartingCashValues.Insert(100);
		m_aStartingCashValues.Insert(200);
		m_aStartingCashValues.Insert(300);
		m_aStartingCashValues.Insert(400);
		m_aStartingCashValues.Insert(500);
		m_aStartingCashValues.Insert(750);
		m_aStartingCashValues.Insert(1000);
		m_aStartingCashValues.Insert(1500);
		m_aStartingCashValues.Insert(2000);
		m_aStartingCashValues.Insert(3000);
		m_aStartingCashValues.Insert(5000);

		m_aRespawnCostValues = new array<int>();
		m_aRespawnCostValues.Insert(0);
		m_aRespawnCostValues.Insert(5);
		m_aRespawnCostValues.Insert(10);
		m_aRespawnCostValues.Insert(25);
		m_aRespawnCostValues.Insert(50);
		m_aRespawnCostValues.Insert(100);
		m_aRespawnCostValues.Insert(200);
		m_aRespawnCostValues.Insert(500);

		m_aFastTravelCostValues = new array<int>();
		m_aFastTravelCostValues.Insert(0);
		m_aFastTravelCostValues.Insert(5);
		m_aFastTravelCostValues.Insert(10);
		m_aFastTravelCostValues.Insert(25);
		m_aFastTravelCostValues.Insert(50);
		m_aFastTravelCostValues.Insert(100);
		m_aFastTravelCostValues.Insert(200);
		m_aFastTravelCostValues.Insert(500);

		m_aDetectionRangeValues = new array<float>();
		m_aDetectionRangeValues.Insert(0.10);
		m_aDetectionRangeValues.Insert(0.25);
		m_aDetectionRangeValues.Insert(0.50);
		m_aDetectionRangeValues.Insert(0.75);
		m_aDetectionRangeValues.Insert(1.00);
		m_aDetectionRangeValues.Insert(1.25);
		m_aDetectionRangeValues.Insert(1.50);
		m_aDetectionRangeValues.Insert(1.75);
		m_aDetectionRangeValues.Insert(2.00);

		m_aDisguiseValues = new array<float>();
		m_aDisguiseValues.Insert(0.10);
		m_aDisguiseValues.Insert(0.20);
		m_aDisguiseValues.Insert(0.30);
		m_aDisguiseValues.Insert(0.40);
		m_aDisguiseValues.Insert(0.50);
		m_aDisguiseValues.Insert(0.60);
		m_aDisguiseValues.Insert(0.70);
		m_aDisguiseValues.Insert(0.80);
		m_aDisguiseValues.Insert(0.90);
		m_aDisguiseValues.Insert(1.00);

		m_aMaxQRFValues = new array<int>();
		m_aMaxQRFValues.Insert(100);
		m_aMaxQRFValues.Insert(200);
		m_aMaxQRFValues.Insert(300);
		m_aMaxQRFValues.Insert(400);
		m_aMaxQRFValues.Insert(500);
		m_aMaxQRFValues.Insert(750);
		m_aMaxQRFValues.Insert(1000);
		m_aMaxQRFValues.Insert(1500);
		m_aMaxQRFValues.Insert(2000);

		SCR_SpinBoxComponent showMapSpin = GetCustomSpin("CustomShowOnMap");
		showMapSpin.AddItem("No", false, null);
		showMapSpin.AddItem("Yes", false, null);
		showMapSpin.m_OnChanged.Insert(OnSpinShowOnMap);

		SCR_SpinBoxComponent cashSpin = GetCustomSpin("CustomStartingCash");
		foreach(int cv : m_aStartingCashValues)
			cashSpin.AddItem(cv.ToString(), false, null);
		cashSpin.m_OnChanged.Insert(OnSpinStartingCash);

		SCR_SpinBoxComponent respawnSpin = GetCustomSpin("CustomRespawnCost");
		foreach(int rv : m_aRespawnCostValues)
			respawnSpin.AddItem(rv.ToString(), false, null);
		respawnSpin.m_OnChanged.Insert(OnSpinRespawnCost);

		SCR_SpinBoxComponent ftSpin = GetCustomSpin("CustomFastTravelCost");
		foreach(int fv : m_aFastTravelCostValues)
			ftSpin.AddItem(fv.ToString(), false, null);
		ftSpin.m_OnChanged.Insert(OnSpinFastTravelCost);

		SCR_SpinBoxComponent patrolMinSpin = GetCustomSpin("CustomPatrolMin");
		for(int p = 1; p <= 10; p++)
			patrolMinSpin.AddItem(p.ToString(), false, null);
		patrolMinSpin.m_OnChanged.Insert(OnSpinPatrolMin);

		SCR_SpinBoxComponent patrolMaxSpin = GetCustomSpin("CustomPatrolMax");
		for(int p = 1; p <= 10; p++)
			patrolMaxSpin.AddItem(p.ToString(), false, null);
		patrolMaxSpin.m_OnChanged.Insert(OnSpinPatrolMax);

		SCR_SpinBoxComponent detectionSpin = GetCustomSpin("CustomDetectionRange");
		foreach(float dv : m_aDetectionRangeValues)
			detectionSpin.AddItem("" + dv, false, null);
		detectionSpin.m_OnChanged.Insert(OnSpinDetectionRange);

		SCR_SpinBoxComponent disguiseSpin = GetCustomSpin("CustomDisguise");
		foreach(float gv : m_aDisguiseValues)
			disguiseSpin.AddItem("" + gv, false, null);
		disguiseSpin.m_OnChanged.Insert(OnSpinDisguise);

		SCR_SpinBoxComponent qrfModeSpin = GetCustomSpin("CustomQRFMode");
		qrfModeSpin.AddItem("Free", false, null);
		qrfModeSpin.AddItem("No QRF", false, null);
		qrfModeSpin.AddItem("Disabled", false, null);
		qrfModeSpin.m_OnChanged.Insert(OnSpinQRFMode);

		SCR_SpinBoxComponent maxQRFSpin = GetCustomSpin("CustomMaxQRF");
		foreach(int qv : m_aMaxQRFValues)
			maxQRFSpin.AddItem(qv.ToString(), false, null);
		maxQRFSpin.m_OnChanged.Insert(OnSpinMaxQRF);
	}

	protected void UpdateCustomPanelVisibility(OVT_DifficultySettings preset)
	{
		Widget panel = m_wRoot.FindAnyWidget("CustomDifficultyPanel");
		bool isCustom = preset.name == "Custom";
		panel.SetVisible(isCustom);
		if(isCustom)
			PopulateCustomSpinboxes();
	}

	protected void PopulateCustomSpinboxes()
	{
		OVT_DifficultySettings d = OVT_Global.GetConfig().m_Difficulty;

		int showOnMapIdx = 0;
		if(d.showPlayerOnMap) showOnMapIdx = 1;
		GetCustomSpin("CustomShowOnMap").SetCurrentItem(showOnMapIdx);
		GetCustomSpin("CustomStartingCash").SetCurrentItem(FindClosestInt(m_aStartingCashValues, d.startingCash));
		GetCustomSpin("CustomRespawnCost").SetCurrentItem(FindClosestInt(m_aRespawnCostValues, d.respawnCost));
		GetCustomSpin("CustomFastTravelCost").SetCurrentItem(FindClosestInt(m_aFastTravelCostValues, d.fastTravelCost));
		GetCustomSpin("CustomPatrolMin").SetCurrentItem(Math.Clamp(d.patrolGroupsMin - 1, 0, 9));
		GetCustomSpin("CustomPatrolMax").SetCurrentItem(Math.Clamp(d.patrolGroupsMax - 1, 0, 9));
		GetCustomSpin("CustomDetectionRange").SetCurrentItem(FindClosestFloat(m_aDetectionRangeValues, d.detectionRangeMultiplier));
		GetCustomSpin("CustomDisguise").SetCurrentItem(FindClosestFloat(m_aDisguiseValues, d.baseDisguiseEffectiveness));
		GetCustomSpin("CustomQRFMode").SetCurrentItem(d.QRFFastTravelMode);
		GetCustomSpin("CustomMaxQRF").SetCurrentItem(FindClosestInt(m_aMaxQRFValues, d.maxQRF));
	}

	protected int FindClosestInt(array<int> values, int target)
	{
		int bestIdx = 0;
		int bestDist = Math.AbsInt(values[0] - target);
		for(int i = 1; i < values.Count(); i++)
		{
			int dist = Math.AbsInt(values[i] - target);
			if(dist < bestDist) { bestDist = dist; bestIdx = i; }
		}
		return bestIdx;
	}

	protected int FindClosestFloat(array<float> values, float target)
	{
		int bestIdx = 0;
		float bestDist = Math.AbsFloat(values[0] - target);
		for(int i = 1; i < values.Count(); i++)
		{
			float dist = Math.AbsFloat(values[i] - target);
			if(dist < bestDist) { bestDist = dist; bestIdx = i; }
		}
		return bestIdx;
	}

	protected void CopyDifficultySettings(OVT_DifficultySettings src, OVT_DifficultySettings dst)
	{
		dst.name = src.name;
		dst.description = src.description;
		dst.showPlayerOnMap = src.showPlayerOnMap;
		dst.wantedTimeout = src.wantedTimeout;
		dst.wantedOneTimeout = src.wantedOneTimeout;
		dst.startingResources = src.startingResources;
		dst.baseResourcesPerTick = src.baseResourcesPerTick;
		dst.resourcesPerTick = src.resourcesPerTick;
		dst.baseResourceCost = src.baseResourceCost;
		dst.radioTowerRange = src.radioTowerRange;
		dst.baseSupportRange = src.baseSupportRange;
		dst.patrolGroupsMin = src.patrolGroupsMin;
		dst.patrolGroupsMax = src.patrolGroupsMax;
		dst.defenseGroupsBaseMax = src.defenseGroupsBaseMax;
		dst.baseRange = src.baseRange;
		dst.baseCloseRange = src.baseCloseRange;
		dst.counterAttackTimeout = src.counterAttackTimeout;
		dst.startingCash = src.startingCash;
		dst.respawnCost = src.respawnCost;
		dst.fastTravelCost = src.fastTravelCost;
		dst.placeableCostMultiplier = src.placeableCostMultiplier;
		dst.buildableCostMultiplier = src.buildableCostMultiplier;
		dst.realEstateCostMultiplier = src.realEstateCostMultiplier;
		dst.donationIncome = src.donationIncome;
		dst.taxIncome = src.taxIncome;
		dst.busTicketPrice = src.busTicketPrice;
		dst.baseRecruitCost = src.baseRecruitCost;
		dst.gunDealerSellPriceMultiplier = src.gunDealerSellPriceMultiplier;
		dst.procurementMultiplier = src.procurementMultiplier;
		dst.vehiclePriceMultiplier = src.vehiclePriceMultiplier;
		dst.baseThreat = src.baseThreat;
		dst.threatReductionFactor = src.threatReductionFactor;
		dst.minFastTravelDistance = src.minFastTravelDistance;
		dst.QRFFastTravelMode = src.QRFFastTravelMode;
		dst.QRFPointsToWin = src.QRFPointsToWin;
		dst.maxQRF = src.maxQRF;
		dst.disguiseDetectionDistance = src.disguiseDetectionDistance;
		dst.baseDisguiseEffectiveness = src.baseDisguiseEffectiveness;
		dst.wantedReductionMultiplier = src.wantedReductionMultiplier;
		dst.detectionRangeMultiplier = src.detectionRangeMultiplier;
		dst.startingItems = src.startingItems;
	}

	protected void OnSpinOccupyingFaction(SCR_SpinBoxComponent spinner, int index)
	{
		Faction data = Faction.Cast(spinner.GetItemData(index));

		// Check if this conflicts with supporting faction
		string currentSupporting = OVT_Global.GetConfig().m_sSupportingFaction;
		if(data.GetFactionKey() == currentSupporting)
		{
			// Find supporting faction spinner and change it to a different faction
			Widget sf = m_wRoot.FindAnyWidget("SupportingFactionSpinner");
			SCR_SpinBoxComponent sfSpin = SCR_SpinBoxComponent.Cast(sf.FindHandler(SCR_SpinBoxComponent));

			// Find a different faction (not the one we just selected)
			for(int i = 0; i < sfSpin.GetNumItems(); i++)
			{
				Faction altFaction = Faction.Cast(sfSpin.GetItemData(i));
				if(altFaction.GetFactionKey() != data.GetFactionKey())
				{
					sfSpin.SetCurrentItem(i);
					OVT_Global.GetConfig().SetSupportingFaction(altFaction.GetFactionKey());
					break;
				}
			}
		}

		OVT_Global.GetConfig().SetOccupyingFaction(data.GetFactionKey());
	}

	protected void OnSpinSupportingFaction(SCR_SpinBoxComponent spinner, int index)
	{
		Faction data = Faction.Cast(spinner.GetItemData(index));

		// Check if this conflicts with occupying faction
		string currentOccupying = OVT_Global.GetConfig().m_sOccupyingFaction;
		if(data.GetFactionKey() == currentOccupying)
		{
			// Find occupying faction spinner and change it to a different faction
			Widget of = m_wRoot.FindAnyWidget("OccupyingFactionSpinner");
			SCR_SpinBoxComponent ofSpin = SCR_SpinBoxComponent.Cast(of.FindHandler(SCR_SpinBoxComponent));

			// Find a different faction (not the one we just selected)
			for(int i = 0; i < ofSpin.GetNumItems(); i++)
			{
				Faction altFaction = Faction.Cast(ofSpin.GetItemData(i));
				if(altFaction.GetFactionKey() != data.GetFactionKey())
				{
					ofSpin.SetCurrentItem(i);
					OVT_Global.GetConfig().SetOccupyingFaction(altFaction.GetFactionKey());
					break;
				}
			}
		}

		OVT_Global.GetConfig().SetSupportingFaction(data.GetFactionKey());
	}

	protected void OnSpinDifficulty(SCR_SpinBoxComponent spinner, int index)
	{
		OVT_DifficultySettings preset = OVT_DifficultySettings.Cast(spinner.GetItemData(index));

		if(preset.name == "Custom")
		{
			OVT_DifficultySettings copy = new OVT_DifficultySettings();
			CopyDifficultySettings(preset, copy);
			OVT_Global.GetConfig().m_Difficulty = copy;
		} else {
			OVT_Global.GetConfig().m_Difficulty = preset;
		}

		Widget description = m_wRoot.FindAnyWidget("DifficultyDescription");
		TextWidget text = TextWidget.Cast(description);
		text.SetText(preset.description);

		UpdateCustomPanelVisibility(preset);
	}

	protected void OnSpinShowOnMap(SCR_SpinBoxComponent spinner, int index)
	{
		OVT_Global.GetConfig().m_Difficulty.showPlayerOnMap = (index == 1);
	}

	protected void OnSpinStartingCash(SCR_SpinBoxComponent spinner, int index)
	{
		OVT_Global.GetConfig().m_Difficulty.startingCash = m_aStartingCashValues[index];
	}

	protected void OnSpinRespawnCost(SCR_SpinBoxComponent spinner, int index)
	{
		OVT_Global.GetConfig().m_Difficulty.respawnCost = m_aRespawnCostValues[index];
	}

	protected void OnSpinFastTravelCost(SCR_SpinBoxComponent spinner, int index)
	{
		OVT_Global.GetConfig().m_Difficulty.fastTravelCost = m_aFastTravelCostValues[index];
	}

	protected void OnSpinPatrolMin(SCR_SpinBoxComponent spinner, int index)
	{
		OVT_DifficultySettings diff = OVT_Global.GetConfig().m_Difficulty;
		diff.patrolGroupsMin = index + 1;
		if(diff.patrolGroupsMin > diff.patrolGroupsMax)
		{
			diff.patrolGroupsMax = diff.patrolGroupsMin;
			GetCustomSpin("CustomPatrolMax").SetCurrentItem(index);
		}
	}

	protected void OnSpinPatrolMax(SCR_SpinBoxComponent spinner, int index)
	{
		OVT_DifficultySettings diff = OVT_Global.GetConfig().m_Difficulty;
		diff.patrolGroupsMax = index + 1;
		if(diff.patrolGroupsMax < diff.patrolGroupsMin)
		{
			diff.patrolGroupsMin = diff.patrolGroupsMax;
			GetCustomSpin("CustomPatrolMin").SetCurrentItem(index);
		}
	}

	protected void OnSpinDetectionRange(SCR_SpinBoxComponent spinner, int index)
	{
		OVT_Global.GetConfig().m_Difficulty.detectionRangeMultiplier = m_aDetectionRangeValues[index];
	}

	protected void OnSpinDisguise(SCR_SpinBoxComponent spinner, int index)
	{
		OVT_Global.GetConfig().m_Difficulty.baseDisguiseEffectiveness = m_aDisguiseValues[index];
	}

	protected void OnSpinQRFMode(SCR_SpinBoxComponent spinner, int index)
	{
		OVT_Global.GetConfig().m_Difficulty.QRFFastTravelMode = index;
	}

	protected void OnSpinMaxQRF(SCR_SpinBoxComponent spinner, int index)
	{
		OVT_Global.GetConfig().m_Difficulty.maxQRF = m_aMaxQRFValues[index];
	}

	protected void StartGame()
	{
		Print("[Overthrow] StartGame button clicked - closing menu and starting game");
		CloseLayout();

		OVT_OverthrowGameMode mode = OVT_OverthrowGameMode.Cast(GetGame().GetGameMode());

		Print("[Overthrow] Calling DoStartNewGame()");
		mode.DoStartNewGame();

		Print("[Overthrow] Calling DoStartGame()");
		mode.DoStartGame();

		Print("[Overthrow] Game start complete");
	}
}
