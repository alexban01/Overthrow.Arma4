class OVT_FactoryStatusContext : OVT_UIContext
{
	[Attribute(uiwidget: UIWidgets.ResourceNamePicker, desc: "Layout for factory rows", params: "layout")]
	ResourceName m_RowLayout;

	OVT_FactoryManagerComponent m_FactoryManager;

	override void PostInit()
	{
		m_FactoryManager = OVT_FactoryManagerComponent.GetInstance();
	}

	override void OnShow()
	{
		Widget closeButton = m_wRoot.FindAnyWidget("CloseButton");
		if (closeButton)
		{
			SCR_InputButtonComponent action = SCR_InputButtonComponent.Cast(closeButton.FindHandler(SCR_InputButtonComponent));
			if (action)
				action.m_OnActivated.Insert(CloseLayout);
		}

		Refresh();
	}

	protected void Refresh()
	{
		if (!m_FactoryManager)
			m_FactoryManager = OVT_FactoryManagerComponent.GetInstance();

		if (!m_FactoryManager) return;

		int playerFaction = OVT_Global.GetConfig().GetPlayerFactionIndex();
		OVT_DifficultySettings diff = OVT_Global.GetDifficulty();
		int baseRate = Math.Round(diff.factorySuppliesPerTick);

		int controlled = 0;
		int total = m_FactoryManager.m_aFactories.Count();
		int totalPerTick = 0;

		foreach (OVT_FactoryData factory : m_FactoryManager.m_aFactories)
		{
			if (factory.faction == playerFaction)
			{
				controlled++;
				totalPerTick += baseRate;
			}
		}

		TextWidget summary = TextWidget.Cast(m_wRoot.FindAnyWidget("SummaryText"));
		if (summary)
			summary.SetText(controlled.ToString() + "/" + total.ToString() + " factories controlled  |  +" + totalPerTick.ToString() + " supplies/tick");

		Widget container = m_wRoot.FindAnyWidget("FactoryListContainer");
		if (!container) return;

		Widget child = container.GetChildren();
		while (child)
		{
			container.RemoveChild(child);
			child = container.GetChildren();
		}

		WorkspaceWidget workspace = GetGame().GetWorkspace();

		foreach (OVT_FactoryData factory : m_FactoryManager.m_aFactories)
		{
			Widget row = workspace.CreateWidgets(m_RowLayout, container);
			if (!row) continue;

			TextWidget nameText = TextWidget.Cast(row.FindAnyWidget("FactoryNameText"));
			if (nameText)
				nameText.SetText("Factory " + (factory.id + 1).ToString());

			TextWidget factionText = TextWidget.Cast(row.FindAnyWidget("FactionText"));
			if (factionText)
			{
				if (factory.faction == playerFaction)
				{
					factionText.SetText("Resistance");
					factionText.SetColor(new Color(0, 0.8, 0, 1));
				}
				else
				{
					factionText.SetText("Occupying");
					factionText.SetColor(new Color(0.8, 0, 0, 1));
				}
			}

			TextWidget garrisonText = TextWidget.Cast(row.FindAnyWidget("GarrisonText"));
			if (garrisonText)
				garrisonText.SetText(factory.garrisonCount.ToString());

			TextWidget suppliesText = TextWidget.Cast(row.FindAnyWidget("SuppliesText"));
			if (suppliesText)
			{
				if (factory.faction == playerFaction)
					suppliesText.SetText("+" + baseRate.ToString());
				else
					suppliesText.SetText("0");
			}
		}
	}
}
