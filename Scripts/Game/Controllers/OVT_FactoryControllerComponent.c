class OVT_FactoryControllerComponentClass: OVT_ComponentClass
{
};

class OVT_FactoryControllerComponent: OVT_Component
{
	[Attribute(defvalue: "100", desc: "Radius in meters for garrison check and capture")]
	float m_fCaptureRadius;

	[Attribute(defvalue: "1", desc: "Supply multiplier for this factory (higher = more strategic value)")]
	float m_fSupplyMultiplier;
}
