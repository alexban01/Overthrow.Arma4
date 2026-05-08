class OVT_GarrisonFactoryAction : ScriptedUserAction
{
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		OVT_UIManagerComponent ui = OVT_UIManagerComponent.Cast(pUserEntity.FindComponent(OVT_UIManagerComponent));
		if (!ui) return;

		OVT_FactoryManagerComponent mgr = OVT_FactoryManagerComponent.GetInstance();
		if (!mgr) return;

		OVT_FactoryData factory = mgr.GetNearestFactory(pOwnerEntity.GetOrigin());
		if (!factory) return;

		OVT_FactoryGarrisonContext context = OVT_FactoryGarrisonContext.Cast(ui.GetContext(OVT_FactoryGarrisonContext));
		if (!context) return;

		context.m_Factory = factory;
		context.ShowLayout();
	}

	override bool CanBeShownScript(IEntity user)
	{
		OVT_FactoryManagerComponent mgr = OVT_FactoryManagerComponent.GetInstance();
		if (!mgr) return false;
		OVT_FactoryData factory = mgr.GetNearestFactory(GetOwner().GetOrigin());
		if (!factory) return false;
		return !factory.IsOccupyingFaction();
	}

	override bool CanBePerformedScript(IEntity user)
	{
		RplComponent rpl = RplComponent.Cast(user.FindComponent(RplComponent));
		return rpl && rpl.IsOwner();
	}

	override bool GetActionNameScript(out string outName)
	{
		outName = "Garrison Factory";
		return true;
	}

	override bool HasLocalEffectOnlyScript() { return true; }
}
