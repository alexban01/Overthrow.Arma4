class OVT_WarInfo : SCR_InfoDisplay
{
	protected OVT_OccupyingFactionManager m_OccupyingFaction;

	protected float m_fUpdateCounter = 0;

	private override event void UpdateValues(IEntity owner, float timeSlice)
	{
		if (!m_wRoot) return;

		m_fUpdateCounter += timeSlice;
		if (m_fUpdateCounter < 2.0) return;
		m_fUpdateCounter = 0;

		if (!m_OccupyingFaction)
			m_OccupyingFaction = OVT_OccupyingFactionManager.GetInstance();

		if (!m_OccupyingFaction) return;

		UpdateWarLevel();
		UpdateAggression();
		UpdateHR();
	}

	protected void UpdateWarLevel()
	{
		int wl = m_OccupyingFaction.GetWarLevel();
		for (int i = 0; i < 5; i++)
		{
			Widget w = m_wRoot.FindAnyWidget("Star" + i.ToString());
			if (w) w.SetVisible(i < wl);
		}
	}

	protected void UpdateAggression()
	{
		ProgressBarWidget bar = ProgressBarWidget.Cast(m_wRoot.FindAnyWidget("AggressionBar"));
		if (!bar) return;
		bar.SetMax(100);
		bar.SetCurrent(m_OccupyingFaction.GetAggression());
	}

	protected void UpdateHR()
	{
		ProgressBarWidget bar = ProgressBarWidget.Cast(m_wRoot.FindAnyWidget("HRBar"));
		if (!bar) return;
		int hrMax = OVT_Global.GetDifficulty().hrMax;
		float pct = 0;
		if (hrMax > 0)
			pct = (float)m_OccupyingFaction.GetHR() / (float)hrMax * 100.0;
		bar.SetMax(100);
		bar.SetCurrent(pct);
	}
}
