class OVT_FactoryManagerComponentClass: OVT_ComponentClass
{
};

class OVT_FactoryData : Managed
{
	[NonSerialized()]
	int id;

	int faction;
	vector location;

	[NonSerialized()]
	EntityID entId;

	[NonSerialized()]
	ref array<EntityID> garrison = {};

	bool IsOccupyingFaction()
	{
		return faction == OVT_Global.GetConfig().GetOccupyingFactionIndex();
	}
}

class OVT_FactoryManagerComponent : OVT_Component
{
	ref array<ref OVT_FactoryData> m_aFactories = {};

	protected int m_iProductionTicks = 0;
	protected int m_iHourPaidProduction = -1;
	protected TimeAndWeatherManagerEntity m_Time;

	const int UPDATE_FREQUENCY = 60000;

	static OVT_FactoryManagerComponent s_Instance;

	static OVT_FactoryManagerComponent GetInstance()
	{
		if (!s_Instance)
		{
			BaseGameMode pGameMode = GetGame().GetGameMode();
			if (pGameMode)
				s_Instance = OVT_FactoryManagerComponent.Cast(pGameMode.FindComponent(OVT_FactoryManagerComponent));
		}
		return s_Instance;
	}

	void Init(IEntity owner)
	{
		GetGame().GetCallqueue().CallLater(PostGameStart, 1000);

		if(!Replication.IsServer()) return;

		float timeMul = 6;
		OVT_TimeAndWeatherHandlerComponent tw = OVT_TimeAndWeatherHandlerComponent.Cast(GetGame().GetGameMode().FindComponent(OVT_TimeAndWeatherHandlerComponent));
		if(tw) timeMul = tw.GetDayTimeMultiplier();

		GetGame().GetCallqueue().CallLater(CheckUpdate, UPDATE_FREQUENCY / timeMul, true, GetOwner());
		GetGame().GetCallqueue().CallLater(CheckFactories, 9000, true, GetOwner());
	}

	protected void PostGameStart()
	{
		GetGame().GetWorld().QueryEntitiesBySphere("0 0 0", 99999999, CheckFactoryAdd, FilterFactoryEntities, EQueryEntitiesFlags.STATIC);
	}

	protected bool CheckFactoryAdd(IEntity ent)
	{
		OVT_FactoryData data = new OVT_FactoryData;
		data.id = m_aFactories.Count();
		data.location = ent.GetOrigin();
		data.faction = OVT_Global.GetConfig().GetOccupyingFactionIndex();
		data.entId = ent.GetID();
		m_aFactories.Insert(data);
		return true;
	}

	protected bool FilterFactoryEntities(IEntity entity)
	{
		OVT_FactoryControllerComponent ctrl = OVT_FactoryControllerComponent.Cast(entity.FindComponent(OVT_FactoryControllerComponent));
		if(ctrl) return true;
		return false;
	}

	void CheckFactories()
	{
		if(!Replication.IsServer()) return;

		OVT_Faction faction = OVT_Global.GetConfig().GetOccupyingFaction();
		foreach(OVT_FactoryData factory : m_aFactories)
		{
			if(!factory.IsOccupyingFaction()) continue;

			bool inrange = OVT_Global.PlayerInRange(factory.location, OVT_Global.GetConfig().m_iMilitarySpawnDistance);
			if(inrange)
			{
				if(factory.garrison.Count() == 0)
				{
					vector pos = factory.location + "5 0 0";
					float surfaceY = GetGame().GetWorld().GetSurfaceY(pos[0], pos[2]);
					if(pos[1] < surfaceY) pos[1] = surfaceY;

					int numGroups = OVT_Global.GetDifficulty().factoryDefenderGroups;
					for(int t = 0; t < numGroups; t++)
					{
						IEntity group = OVT_Global.SpawnEntityPrefab(faction.m_aTowerDefensePatrolPrefab, pos);
						factory.garrison.Insert(group.GetID());
						SCR_AIGroup aigroup = SCR_AIGroup.Cast(group);
						AIWaypoint wp = OVT_Global.GetConfig().SpawnDefendWaypoint(pos);
						aigroup.AddWaypoint(wp);
					}
				}else{
					array<EntityID> remove = {};
					foreach(EntityID id : factory.garrison)
					{
						IEntity ent = GetGame().GetWorld().FindEntityByID(id);
						SCR_AIGroup group = SCR_AIGroup.Cast(ent);
						if(!group)
						{
							remove.Insert(id);
						}else if(group.GetAgentsCount() == 0)
						{
							SCR_EntityHelper.DeleteEntityAndChildren(group);
							remove.Insert(id);
						}
					}
					foreach(EntityID id : remove)
					{
						factory.garrison.RemoveItem(id);
					}
					if(factory.garrison.Count() == 0)
					{
						CaptureFactory(factory);
					}
				}
			}else{
				if(factory.garrison.Count() > 0)
				{
					foreach(EntityID id : factory.garrison)
					{
						IEntity ent = GetGame().GetWorld().FindEntityByID(id);
						SCR_EntityHelper.DeleteEntityAndChildren(ent);
					}
					factory.garrison.Clear();
				}
			}
		}
	}

	void CaptureFactory(OVT_FactoryData factory)
	{
		factory.faction = OVT_Global.GetConfig().GetPlayerFactionIndex();
		Rpc(RpcDo_SetFactoryFaction, factory.id, factory.faction);
		OVT_Global.GetNotify().SendTextNotification("FactoryCaptured", -1, "");
		OVT_Global.GetNotify().SendExternalNotifications("FactoryCaptured", "");
		OVT_OccupyingFactionManager of = OVT_Global.GetOccupyingFaction();
		OVT_DifficultySettings diff = OVT_Global.GetDifficulty();
		of.AddWarPoints(diff.warPointsPerBase);
		of.m_fAggression = Math.Min(100, of.m_fAggression + diff.aggressionPerCapture);
	}

	void CheckUpdate()
	{
		if(!Replication.IsServer()) return;

		if(!m_Time)
		{
			ChimeraWorld world = GetOwner().GetWorld();
			m_Time = world.GetTimeAndWeatherManager();
		}

		PlayerManager mgr = GetGame().GetPlayerManager();
		if(mgr.GetPlayerCount() == 0) return;

		TimeContainer time = m_Time.GetTime();

		if((time.m_iHours == 0
			|| time.m_iHours == 6
			|| time.m_iHours == 12
			|| time.m_iHours == 18)
			&&
			m_iHourPaidProduction != time.m_iHours)
		{
			m_iHourPaidProduction = time.m_iHours;
			ProduceSupplies();
		}
	}

	protected void ProduceSupplies()
	{
		m_iProductionTicks++;
		int interval = Math.Max(1, OVT_Global.GetDifficulty().factoryProductionIntervalTicks);
		if(m_iProductionTicks < interval) return;
		m_iProductionTicks = 0;

		int playerFaction = OVT_Global.GetConfig().GetPlayerFactionIndex();
		foreach(OVT_FactoryData factory : m_aFactories)
		{
			if(factory.faction != playerFaction) continue;
			IEntity ent = GetGame().GetWorld().FindEntityByID(factory.entId);
			if(!ent) continue;
			OVT_FactoryControllerComponent ctrl = OVT_FactoryControllerComponent.Cast(ent.FindComponent(OVT_FactoryControllerComponent));
			if(!ctrl) continue;
			OVT_DifficultySettings diff = OVT_Global.GetDifficulty();
			int produced = Math.Round(diff.factorySuppliesPerTick * ctrl.m_fSupplyMultiplier);
			OVT_Global.GetEconomy().AddResistanceSupplies(produced);
			Print("[Overthrow.FactoryManager] Factory produced " + produced + " supplies. Total: " + OVT_Global.GetEconomy().GetResistanceSupplies());
		}
	}

	OVT_FactoryData GetNearestFactory(vector pos)
	{
		OVT_FactoryData nearest;
		float nearestDist = -1;
		foreach(OVT_FactoryData factory : m_aFactories)
		{
			float dist = vector.Distance(factory.location, pos);
			if(nearestDist == -1 || dist < nearestDist)
			{
				nearestDist = dist;
				nearest = factory;
			}
		}
		if(nearestDist > 50) return null;
		return nearest;
	}

	override bool RplSave(ScriptBitWriter writer)
	{
		writer.WriteInt(m_aFactories.Count());
		foreach(OVT_FactoryData factory : m_aFactories)
		{
			writer.WriteVector(factory.location);
			writer.WriteInt(factory.faction);
		}
		return true;
	}

	override bool RplLoad(ScriptBitReader reader)
	{
		int length;
		if(!reader.ReadInt(length)) return false;
		for(int i = 0; i < length; i++)
		{
			OVT_FactoryData factory = new OVT_FactoryData;
			factory.id = i;
			vector v;
			if(!reader.ReadVector(v)) return false;
			factory.location = v;
			int fac;
			if(!reader.ReadInt(fac)) return false;
			factory.faction = fac;
			m_aFactories.Insert(factory);
		}
		return true;
	}

	[RplRpc(RplChannel.Reliable, RplRcver.Broadcast)]
	protected void RpcDo_SetFactoryFaction(int factoryId, int faction)
	{
		if(factoryId < m_aFactories.Count())
			m_aFactories[factoryId].faction = faction;
	}
}
