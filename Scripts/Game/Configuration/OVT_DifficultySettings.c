enum OVT_QRFFastTravelMode
{
	FREE,
	NOQRF,
	DISABLED
}
[BaseContainerProps(configRoot: true)]
class OVT_DifficultySettings : ScriptAndConfig
{	
	[Attribute()]
	string name;
	
	[Attribute()]
	string description;
	
	[Attribute()]
	bool showPlayerOnMap;
	
	//Wanted system
	[Attribute(defvalue: "30000", desc: "Timeout in ms for wanted levels 2-5 (per level)", category: "Wanted System")]
	int wantedTimeout;
	[Attribute(defvalue: "120000", desc: "Timeout in ms for wanted level 1", category: "Wanted System")]
	int wantedOneTimeout;
	
	//OF
	[Attribute(defvalue: "3000", desc: "OF starting resources per base", category: "Occupying Faction")]
	int startingResources;
	[Attribute(defvalue: "250", desc: "OF resources per 6 hrs", category: "Occupying Faction")]
	int baseResourcesPerTick;
	[Attribute(defvalue: "500", desc: "Additional OF resources per 6 hrs (* threat)", category: "Occupying Faction")]
	int resourcesPerTick;
	[Attribute(defvalue: "10", desc: "Base resource cost", category: "Occupying Faction")]
	int baseResourceCost;	
	[Attribute(defvalue: "1500", desc: "Radio Tower Range", category: "Occupying Faction")]
	float radioTowerRange;
	[Attribute(defvalue: "750", desc: "Base Support Range", category: "Occupying Faction")]
	float baseSupportRange;
	[Attribute(defvalue: "2", desc: "Minimum number of defense groups at radio towers", category: "Occupying Faction")]
	int patrolGroupsMin;
	[Attribute(defvalue: "3", desc: "Maximum number of defense groups at radio towers", category: "Occupying Faction")]
	int patrolGroupsMax;	
	[Attribute(defvalue: "5", desc: "Maximum number of defense positions at bases", category: "Occupying Faction")]
	int defenseGroupsBaseMax;
	[Attribute("280", category: "Occupying Faction")]
	float baseRange;
	[Attribute("220", category: "Occupying Faction")]
	float baseCloseRange;
	[Attribute("120", category: "Occupying Faction")]
	float counterAttackTimeout;
	
	//Economy
	[Attribute(defvalue: "100", desc: "Player starting cash", category: "Economy")]
	int startingCash;
	[Attribute(defvalue: "5", desc: "Money taken from player per respawn", category: "Economy")]
	int respawnCost;
	[Attribute(defvalue: "5", desc: "Cost to fast travel", category: "Economy")]
	int fastTravelCost;
	[Attribute(defvalue: "1", desc: "Cost of placeables is multiplied by this value", category: "Economy")]
	float placeableCostMultiplier;
	[Attribute(defvalue: "1", desc: "Cost of buildables is multiplied by this value", category: "Economy")]
	float buildableCostMultiplier;
	[Attribute(defvalue: "0.5", desc: "Cost of Real Estate is multiplied by this value", category: "Economy")]
	float realEstateCostMultiplier;
	[Attribute(defvalue: "10", desc: "Donation income per civilian supporter", category: "Economy")]
	int donationIncome;
	[Attribute(defvalue: "25", desc: "Tax income per civilian", category: "Economy")]
	int taxIncome;
	[Attribute(defvalue: "5", desc: "Bus ticket price per km", category: "Economy")]
	int busTicketPrice;
	[Attribute(defvalue: "250", desc: "Base price for AI recruit", category: "Economy")]
	int baseRecruitCost;
	[Attribute(defvalue: "0.5", desc: "Multiplier when selling to a gun dealer", category: "Economy")]
	float gunDealerSellPriceMultiplier;
	[Attribute(defvalue: "0.8", desc: "Multiplier when buying vehicles at an owned base", category: "Economy")]
	float procurementMultiplier;
	[Attribute(defvalue: "1.0", desc: "Multiplier for vehicle purchase prices", category: "Economy")]
	float vehiclePriceMultiplier;
	
	//RF
	[Attribute(defvalue: "0", desc: "Base RF threat", category: "Resistance Faction")]
	int baseThreat;	
	
	[Attribute(defvalue: "0.004", desc: "Threat reduction factor", category: "Resistance Faction")]
	float threatReductionFactor;	
	
	[Attribute(defvalue: "500", desc: "Minimum fast travel distance", category: "Resistance Faction")]
	float minFastTravelDistance;
	
	//QRF
	[Attribute("1", UIWidgets.ComboBox, "QRF Fast Travel Mode", "", ParamEnumArray.FromEnum(OVT_QRFFastTravelMode), category: "QRF" )]
	OVT_QRFFastTravelMode QRFFastTravelMode;
	
	[Attribute(defvalue: "100", desc: "QRF Points needed to win a battle (lower = shorter battles)", category: "QRF")]
	int QRFPointsToWin;
	
	[Attribute(defvalue: "1000", desc: "Max size of QRF in resources", category: "QRF")]
	int maxQRF;
	
	//Undercover System
	[Attribute(defvalue: "15", desc: "Distance at which enemies can see through disguises (meters)", category: "Undercover System")]
	float disguiseDetectionDistance;
	[Attribute(defvalue: "0.8", desc: "Base disguise effectiveness when fully disguised", category: "Undercover System")]
	float baseDisguiseEffectiveness;
	[Attribute(defvalue: "0.7", desc: "Maximum wanted level reduction from disguises", category: "Undercover System")]
	float wantedReductionMultiplier;
	[Attribute(defvalue: "0.6", desc: "Detection range multiplier when disguised", category: "Undercover System")]
	float detectionRangeMultiplier;
	
	[Attribute("", UIWidgets.ResourcePickerThumbnail, "Items given to player when first spawned in")]
	ref array<ResourceName> startingItems;

	// War Level
	[Attribute(defvalue: "150", desc: "Cumulative war points to reach War Level 2", category: "War Progression")]
	int warLevelThreshold2;
	[Attribute(defvalue: "400", desc: "Cumulative war points to reach War Level 3", category: "War Progression")]
	int warLevelThreshold3;
	[Attribute(defvalue: "800", desc: "Cumulative war points to reach War Level 4", category: "War Progression")]
	int warLevelThreshold4;
	[Attribute(defvalue: "1500", desc: "Cumulative war points to reach War Level 5", category: "War Progression")]
	int warLevelThreshold5;
	[Attribute(defvalue: "5", desc: "War points awarded per AI kill", category: "War Progression")]
	int warPointsPerKill;
	[Attribute(defvalue: "50", desc: "War points for liberating a village", category: "War Progression")]
	int warPointsPerVillage;
	[Attribute(defvalue: "150", desc: "War points for liberating a town", category: "War Progression")]
	int warPointsPerTown;
	[Attribute(defvalue: "350", desc: "War points for liberating a city", category: "War Progression")]
	int warPointsPerCity;
	[Attribute(defvalue: "100", desc: "War points for capturing a base", category: "War Progression")]
	int warPointsPerBase;

	// Aggression
	[Attribute(defvalue: "0.1", desc: "Aggression gained per AI kill (0-100 scale)", category: "Aggression")]
	float aggressionPerKill;
	[Attribute(defvalue: "2.0", desc: "Aggression gained per town or base captured", category: "Aggression")]
	float aggressionPerCapture;
	[Attribute(defvalue: "0.1", desc: "Aggression lost every 15-minute tick", category: "Aggression")]
	float aggressionDecayPerTick;
	[Attribute(defvalue: "70.0", desc: "Aggression level at which enemy launches proactive attacks on player targets", category: "Aggression")]
	float aggressionAttackThreshold;

	// HR (Human Resources)
	[Attribute(defvalue: "1000", desc: "Maximum enemy HR (manpower pool)", category: "HR")]
	int hrMax;
	[Attribute(defvalue: "1000", desc: "Starting HR value", category: "HR")]
	int hrStart;
	[Attribute(defvalue: "1", desc: "HR lost per AI kill", category: "HR")]
	int hrLossPerKill;
	[Attribute(defvalue: "100", desc: "HR regenerated per regen interval", category: "HR")]
	int hrRegenPerWeek;
	[Attribute(defvalue: "4", desc: "HR regen interval in 6-hour ticks (1=6h, 4=daily, 28=weekly)", category: "HR")]
	int hrRegenIntervalTicks;
	[Attribute(defvalue: "0.3", desc: "QRF resource multiplier when HR is at 0 (scales linearly to 1.0 at full HR)", category: "HR")]
	float hrMinQRFMultiplier;

	// Factories
	[Attribute(defvalue: "2", desc: "Number of OF defender groups per factory", category: "Factories")]
	int factoryDefenderGroups;
	[Attribute(defvalue: "4", desc: "Production interval in 6-hour ticks (1=6h, 4=daily)", category: "Factories")]
	int factoryProductionIntervalTicks;
	[Attribute(defvalue: "50", desc: "Resistance supplies produced per factory per production tick", category: "Factories")]
	int factorySuppliesPerTick;
	[Attribute(defvalue: "10", desc: "Resistance supplies produced per fully-supported town per production tick", category: "Factories")]
	int townSuppliesPerTick;

	// Resistance HR
	[Attribute(defvalue: "500", desc: "Maximum resistance HR (manpower pool)", category: "Resistance HR")]
	int resistanceHRMax;
	[Attribute(defvalue: "0", desc: "Starting resistance HR", category: "Resistance HR")]
	int resistanceHRStart;
	[Attribute(defvalue: "1", desc: "Resistance HR regenerated per 1 town support point per 6h tick", category: "Resistance HR")]
	float resistanceHRRegenPerSupport;
	[Attribute(defvalue: "10", desc: "HR cost to recruit one AI soldier", category: "Resistance HR")]
	int recruitHRCost;
	[Attribute(defvalue: "25", desc: "Supply cost to recruit one AI soldier (base, scales with loadout)", category: "Resistance HR")]
	int recruitSupplyCost;
}