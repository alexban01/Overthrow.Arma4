class OVT_CaptureFactoryAction : ScriptedUserAction
{
	override void PerformAction(IEntity pOwnerEntity, IEntity pUserEntity)
	{
		RplComponent rpl = RplComponent.Cast(pUserEntity.FindComponent(RplComponent));
		if (!rpl || !rpl.IsOwner()) return;

		OVT_FactoryManagerComponent mgr = OVT_FactoryManagerComponent.GetInstance();
		if (!mgr) return;
		OVT_FactoryData factory = mgr.GetNearestFactory(pOwnerEntity.GetOrigin());
		if (!factory) return;

		if (factory.garrisonCount > 0)
		{
			string msg = "There are " + factory.garrisonCount + " enemy groups defending this factory. Clear them out first!";
			SCR_HintManagerComponent.ShowCustomHint(msg, "Factory", 5);
			return;
		}

		mgr.RequestCaptureFactory(pOwnerEntity.GetOrigin());
	}

	override bool CanBeShownScript(IEntity user)
	{
		OVT_FactoryManagerComponent mgr = OVT_FactoryManagerComponent.GetInstance();
		if (!mgr) return false;
		OVT_FactoryData factory = mgr.GetNearestFactory(GetOwner().GetOrigin());
		if (!factory) return false;
		return factory.IsOccupyingFaction();
	}

	override bool CanBePerformedScript(IEntity user)
	{
		RplComponent rpl = RplComponent.Cast(user.FindComponent(RplComponent));
		if (!rpl || !rpl.IsOwner()) return false;
		return true;
	}

	override bool HasLocalEffectOnlyScript() { return true; }
}
