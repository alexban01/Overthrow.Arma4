[EPF_ComponentSaveDataType(OVT_FactoryManagerComponent), BaseContainerProps()]
class OVT_FactorySaveDataClass : EPF_ComponentSaveDataClass
{
};

[EDF_DbName.Automatic()]
class OVT_FactorySaveData : EPF_ComponentSaveData
{
	ref array<ref OVT_FactoryData> m_Factories;

	override EPF_EReadResult ReadFrom(IEntity owner, GenericComponent component, EPF_ComponentSaveDataClass attributes)
	{
		OVT_FactoryManagerComponent mgr = OVT_FactoryManagerComponent.Cast(component);
		m_Factories = new array<ref OVT_FactoryData>;
		foreach(OVT_FactoryData factory : mgr.m_aFactories)
		{
			m_Factories.Insert(factory);
		}
		return EPF_EReadResult.OK;
	}

	override EPF_EApplyResult ApplyTo(IEntity owner, GenericComponent component, EPF_ComponentSaveDataClass attributes)
	{
		OVT_FactoryManagerComponent mgr = OVT_FactoryManagerComponent.Cast(component);
		if(!m_Factories) return EPF_EApplyResult.OK;
		foreach(OVT_FactoryData saved : m_Factories)
		{
			OVT_FactoryData existing = mgr.GetNearestFactory(saved.location);
			if(!existing) continue;
			existing.faction = saved.faction;
		}
		return EPF_EApplyResult.OK;
	}
}
