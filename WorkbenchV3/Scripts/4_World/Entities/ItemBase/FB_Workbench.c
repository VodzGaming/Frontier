class FB_Workbench: TerritoryFlag
{
    // TERRITORY ========================================
    const static ref array<string> POWER_SOURCE_ATTACHMENTS = {
        "FB_Battery_1","FB_Battery_2","FB_Battery_3"
    };

    protected const int m_CheckBasePartsFrequency = 60;
    protected const int m_CheckBatteriesFrequency = 5;

    protected int m_CheckBasePartsCounter = 0;
    protected int m_CheckBatteriesCounter = 0;
    
    protected const int SECONDS_IN_A_DAY = 86400;
    protected const int SECONDS_IN_AN_HOUR = 3600;
    protected const int SECONDS_IN_A_MINUTE = 60;
    protected const int NO_DRAIN_TIMELEFT = 2147483647; // effectively "infinite" when powered but no consumption

    protected float m_EnergyDrainPerSecond;
    protected bool m_HasEnergyLeft;
    protected int m_BaseLifetimeLeft;
    protected ref array<EntityAI> m_TrackedEntities;
    protected FrontierWorkbenchTrigger m_WorkbenchTrigger;
    protected bool m_PowerScanDirty;
    protected int m_PeriodicFullScanSeconds;
    protected float m_PeriodicFullScanAccumulator;
    // Limit tracked items to those that actually matter for power/refresh.
    protected static ref array<typename> TRACK_ALLOWED_BASES = { ItemBase };

    // CRAFTING ========================================    
    protected bool m_IsWorkbenchBusy;
    protected ref array<ref FB_CraftJob> m_CraftQueue;
    protected float m_QueueProgressAccumulator;
    protected ref array<string> m_WatcherIds;
    protected ref array<string> m_AuthorizedIds;
    protected string m_WorkbenchOwnerId;
    protected float m_AutoRefreshAccumulator;
    protected float m_AutoRefreshInterval;
    protected int m_CeRefreshInterval;
    protected bool m_HasOwnerFlag;
    protected float m_PowerDrainMultiplier;
    protected float m_CraftSpeedMultiplier;
    protected int m_QueueBonus;
    protected int m_BaseQueueLimit;
    protected float m_TerritoryRadiusBonus;
    protected int m_WorkbenchTier;
    protected int m_RequiredCraftTier;
    protected bool m_CraftingUnlocked;
    protected bool m_UseFrameCraftTick;
    protected int m_ActiveJobSequence;
    protected const int CRAFT_STORE_VERSION = 1; // bump if queue serialization changes
    protected static const bool WORKBENCH_UPGRADES_ENABLED = true;
    protected const int AUTH_LIST_STORE_LIMIT = 64; // prevent runaway auth growth in persistence

    void FB_Workbench()
    {
        SetAllowDamage( false );

        RegisterNetSyncVariableFloat("m_EnergyDrainPerSecond");
        RegisterNetSyncVariableBool("m_IsWorkbenchBusy");
        RegisterNetSyncVariableBool("m_HasOwnerFlag");
        RegisterNetSyncVariableBool("m_CraftingUnlocked");
        RegisterNetSyncVariableInt("m_BaseLifetimeLeft");

        m_CraftQueue = new array<ref FB_CraftJob>();
        m_QueueProgressAccumulator = 0;
        m_WatcherIds = new array<string>();
        m_AuthorizedIds = new array<string>();
        m_WorkbenchOwnerId = "";
        m_AutoRefreshAccumulator = 0;
        m_HasOwnerFlag = false;
        m_PowerDrainMultiplier = 1.0;
        m_CraftSpeedMultiplier = 1.0;
        m_QueueBonus = 0;
        m_BaseQueueLimit = 1;
        m_TerritoryRadiusBonus = 0;
        m_WorkbenchTier = 1;
        m_RequiredCraftTier = 1;
        m_CraftingUnlocked = true;
        m_AutoRefreshInterval = 600;
        m_CeRefreshInterval = 0;
        m_TrackedEntities = new array<EntityAI>();
        m_PowerScanDirty = true;
        m_UseFrameCraftTick = true;
        m_ActiveJobSequence = 0;
#ifdef SERVER
        SetEventMask(EntityEvent.FRAME | EntityEvent.POSTFRAME);
        LoadRefreshSettings();
    LoadPowerScanSettings();
        UpdateUpgradeEffects();
        RefreshBaseQueueLimit();
#endif
    }

    override void EOnFrame(IEntity other, float timeSlice)
    {
        super.EOnFrame(other, timeSlice);

    #ifdef SERVER
        if (m_UseFrameCraftTick && timeSlice > 0)
            ProcessCraftQueue(timeSlice);
    #endif
    }

    bool IsWorkbenchBusy()
    {
        return m_IsWorkbenchBusy;
    }    

    bool IsCraftingUnlocked()
    {
        return true;
    }

    int GetWorkbenchTier()
    {
        return 1;
    }

    int GetCraftingTierRequirement()
    {
        return 1;
    }

    string GetCraftingLockMessage()
    {
        return "Workbench is ready";
    }

    protected void LoadRefreshSettings()
    {
        FrontierRefreshConfig refreshCfg = ModuleFrontierConfigHub.Get().GetRefreshConfig();
        if (refreshCfg)
        {
            // Guard against absurd values; minimum 60s to prevent spam.
            m_AutoRefreshInterval = Math.Max(60, refreshCfg.autoRefreshIntervalSeconds);
            m_CeRefreshInterval = refreshCfg.ceRefreshIntervalSeconds;
        }
    }

    protected void LoadPowerScanSettings()
    {
        ModuleFrontierPowerConfig powerCfg = ModuleFrontierConfigHub.Get().GetPowerConfig();
        if (powerCfg && powerCfg.enablePeriodicFullScan && powerCfg.periodicFullScanSeconds > 0)
            m_PeriodicFullScanSeconds = Math.Max(powerCfg.periodicFullScanSeconds, 30);
        else
            m_PeriodicFullScanSeconds = 0;

        m_PeriodicFullScanAccumulator = 0;
    }

    protected void EnsureTracking()
    {
        if (!m_TrackedEntities)
            m_TrackedEntities = new array<EntityAI>();

        if (!g_Game || !g_Game.IsServer())
            return;

        if (!m_WorkbenchTrigger)
        {
            m_WorkbenchTrigger = FrontierWorkbenchTrigger.Cast(g_Game.CreateObjectEx("FrontierWorkbenchTrigger", GetPosition(), ECE_NONE));
            if (m_WorkbenchTrigger)
            {
                m_WorkbenchTrigger.SetWorkbench(this);
                UpdateTriggerRadius();
                SeedTrackedEntities();
            }
        }
    }

    protected void UpdateTriggerRadius()
    {
        if (!g_Game || !g_Game.IsServer())
            return;

        if (!m_WorkbenchTrigger)
            return;

        m_WorkbenchTrigger.SetCollisionSphere(GetTerritoryRadius());
    }

    protected void SeedTrackedEntities()
    {
        if (!g_Game || !g_Game.IsServer())
            return;

        array<Object> objectsInRadius = new array<Object>();
        GetGame().GetObjectsAtPosition(GetPosition(), GetTerritoryRadius(), objectsInRadius, NULL);

        foreach (Object obj : objectsInRadius)
        {
            ItemBase item = ItemBase.Cast(obj);
            RegisterTrackedEntity(item);
        }
    }

    protected void FullReseedTrackedEntities()
    {
        if (!g_Game || !g_Game.IsServer())
            return;

        if (m_TrackedEntities)
            m_TrackedEntities.Clear();

        SeedTrackedEntities();
        m_PowerScanDirty = true;
    }

    void RegisterTrackedEntity(ItemBase item)
    {
        if (!g_Game || !g_Game.IsServer())
            return;

        if (!item || item == this)
            return;

        if (!ShouldTrackItem(item))
            return;

        EnsureTracking();

        if (m_TrackedEntities.Find(item) == -1)
        {
            m_TrackedEntities.Insert(item);
            m_PowerScanDirty = true;
        }
    }

    void UnregisterTrackedEntity(ItemBase item)
    {
        if (!g_Game || !g_Game.IsServer())
            return;

        if (!m_TrackedEntities || !item)
            return;

        int idx = m_TrackedEntities.Find(item);
        if (idx > -1)
        {
            m_TrackedEntities.Remove(idx);
            m_PowerScanDirty = true;
        }
    }

    protected bool ShouldTrackItem(ItemBase item)
    {
        if (!item || item == this)
            return false;

        if (TRACK_ALLOWED_BASES && TRACK_ALLOWED_BASES.Count() > 0)
        {
            bool matchesBase = false;
            foreach (typename baseType : TRACK_ALLOWED_BASES)
            {
                if (item.IsInherited(baseType))
                {
                    matchesBase = true;
                    break;
                }
            }

            if (!matchesBase)
                return false;
        }

        ModuleFrontierWorkbenchPower powerModule = ModuleFrontierWorkbenchPower.Get();
        if (!powerModule)
            return true;

        // Only track items (or their attachments) that actually draw power.
        if (powerModule.GetItemPowerConsumptionPerHour(item.GetType()) > 0)
            return true;

        if (!item.GetInventory())
            return false;

        int attachmentCount = item.GetInventory().AttachmentCount();
        for (int j = 0; j < attachmentCount; j++)
        {
            EntityAI attachment = item.GetInventory().GetAttachmentFromIndex(j);
            if (!attachment)
                continue;

            if (powerModule.GetItemPowerConsumptionPerHour(attachment.GetType()) > 0)
                return true;
        }

        return false;
    }

    override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
    {
        super.OnRPC(sender, rpc_type, ctx);

#ifdef SERVER
        if (rpc_type == FBRpc.CLIENT_REQUEST_WORKBENCH_DATA)
        {
            HandleWorkbenchDataRequest(sender);
            return;
        }

        if (rpc_type == FBRpc.CLIENT_REQUEST_CRAFT_RECIPE)
        {
            HandleCraftRequest(sender, ctx);
            return;
        }

        if (rpc_type == FBRpc.CLIENT_REQUEST_AUTH_ADD)
        {
            HandleAuthorizationRequest(sender, ctx, true);
            return;
        }

        if (rpc_type == FBRpc.CLIENT_REQUEST_AUTH_REMOVE)
        {
            HandleAuthorizationRequest(sender, ctx, false);
            return;
        }

        if (rpc_type == FBRpc.CLIENT_REQUEST_AUTH_CLEAR)
        {
            HandleAuthorizationClear(sender);
            return;
        }
#endif
    }

    override void EEInit()
    {
        super.EEInit();

#ifdef SERVER
        EnsureTracking();
        UpdateTriggerRadius();
    UpdateLifetimeCache(true);
#endif
    }

    protected void EnsureAuthorizedList()
    {
        if (!m_AuthorizedIds)
            m_AuthorizedIds = new array<string>();
    }

    protected void WriteAuthorizedList(ParamsWriteContext ctx)
    {
        EnsureAuthorizedList();

        int entryCount = m_AuthorizedIds.Count();
        if (entryCount > AUTH_LIST_STORE_LIMIT)
        {
            entryCount = AUTH_LIST_STORE_LIMIT;
        }

        ctx.Write(entryCount);

        for (int i = 0; i < entryCount; i++)
        {
            string entry = m_AuthorizedIds[i];
            ctx.Write(entry);
        }
    }

    protected bool ReadAuthorizedList(ParamsReadContext ctx)
    {
        // Legacy saves wrote the entire array in one ctx entry. Try that first so older benches keep working.
        array<string> legacyList;
        if (ctx.Read(legacyList) && legacyList)
        {
            EnsureAuthorizedList();
            m_AuthorizedIds.Clear();

            for (int legacyIdx = 0; legacyIdx < legacyList.Count(); legacyIdx++)
            {
                string legacyId = legacyList[legacyIdx];
                legacyId.Trim();
                if (!legacyId || legacyId == "")
                    continue;

                if (m_AuthorizedIds.Count() >= AUTH_LIST_STORE_LIMIT)
                    break;

                if (m_AuthorizedIds.Find(legacyId) == -1)
                    m_AuthorizedIds.Insert(legacyId);
            }

            return true;
        }

        // New saves store count + entries to avoid ctx array corruption.
        int serializedCount;
        if (!ctx.Read(serializedCount))
            return false;

        EnsureAuthorizedList();
        m_AuthorizedIds.Clear();

        if (serializedCount <= 0)
            return true;

        string serializedId;
        for (int i = 0; i < serializedCount; i++)
        {
            if (!ctx.Read(serializedId))
                return false;

            serializedId.Trim();
            if (!serializedId || serializedId == "")
                continue;

            if (m_AuthorizedIds.Count() >= AUTH_LIST_STORE_LIMIT)
                break;

            if (m_AuthorizedIds.Find(serializedId) == -1)
                m_AuthorizedIds.Insert(serializedId);
        }

        return true;
    }

    protected string GetPlayerSteamId(PlayerBase player)
    {
        if (!player)
            return "";

        PlayerIdentity identity = player.GetIdentity();
        if (!identity)
            return "";

        string id = identity.GetPlainId();
        if (!id || id == "")
            id = identity.GetId();
        return id;
    }

    bool HasOwner()
    {
#ifdef SERVER
        return (m_WorkbenchOwnerId && m_WorkbenchOwnerId != "");
#else
        return m_HasOwnerFlag;
#endif
    }

    protected bool IsAuthorized(PlayerBase player)
    {
        if (!player)
            return false;

        if (IsOwner(player))
            return true;

        string playerId = GetPlayerSteamId(player);
        if (playerId == "")
            return false;

#ifdef SERVER
        ModuleFrontierAdminBypass adminBypass = ModuleFrontierAdminBypass.Get();
        if (adminBypass && adminBypass.IsSteamIdBypassed(playerId))
            return true;
#endif

        EnsureAuthorizedList();
        return m_AuthorizedIds.Find(playerId) > -1;
    }

    bool IsOwner(PlayerBase player)
    {
        if (!player)
            return false;

        string ownerId = m_WorkbenchOwnerId;
        if (!ownerId || ownerId == "")
            return true; // legacy benches without owner behave as open-access

        string playerId = GetPlayerSteamId(player);
        if (playerId == "")
            return false;

#ifdef SERVER
        ModuleFrontierAdminBypass adminBypass = ModuleFrontierAdminBypass.Get();
        if (adminBypass && adminBypass.IsSteamIdBypassed(playerId))
            return true;
#endif

        return ownerId == playerId;
    }

    void FrontierAssignOwner(PlayerBase player)
    {
#ifdef SERVER
        if (!player)
            return;

        string playerId = GetPlayerSteamId(player);
        if (playerId == "")
            return;

        m_WorkbenchOwnerId = playerId;
        m_HasOwnerFlag = true;
        AddAuthorizedPlayer(playerId);
        SetSynchDirty();
#endif
    }

    protected void AddAuthorizedPlayer(string playerId)
    {
#ifdef SERVER
        EnsureAuthorizedList();

        if (playerId == "")
            return;

        if (m_AuthorizedIds.Find(playerId) > -1)
            return;

        m_AuthorizedIds.Insert(playerId);
        OnAuthorizationChanged();
#endif
    }

    protected void RemoveAuthorizedPlayer(string playerId)
    {
#ifdef SERVER
        if (!m_AuthorizedIds || playerId == "")
            return;

        int idx = m_AuthorizedIds.Find(playerId);
        if (idx == -1)
            return;

        m_AuthorizedIds.RemoveOrdered(idx);
        OnAuthorizationChanged();
#endif
    }

    void ClearAuthorizedPlayers()
    {
#ifdef SERVER
        EnsureAuthorizedList();
        m_AuthorizedIds.Clear();
        m_WorkbenchOwnerId = "";
        m_HasOwnerFlag = false;
        OnAuthorizationChanged();
#endif
    }

    protected void OnAuthorizationChanged()
    {
#ifdef SERVER
        SetSynchDirty();
#endif
    }

#ifdef SERVER
    protected void HandleWorkbenchDataRequest(PlayerIdentity sender)
    {
        if (!g_CraftServerManager)
            return;

        PlayerBase player = FindPlayerByIdentity(sender);
        if (!player || !IsAuthorized(player))
            return;

        g_CraftServerManager.SendWorkbenchData(player, this);
    }

    protected void HandleCraftRequest(PlayerIdentity sender, ParamsReadContext ctx)
    {
        if (!g_CraftServerManager || !ctx)
            return;

        Param1<string> recipePayload;
        if (!ctx.Read(recipePayload))
            return;

        string recipeId = recipePayload.param1;
        if (recipeId == "")
            return;

        PlayerBase crafter = FindPlayerByIdentity(sender);
        if (!crafter || !IsAuthorized(crafter))
            return;

        string resultMessage;
        g_CraftServerManager.TryCraft(crafter, this, recipeId, resultMessage);
    }
#endif

#ifdef SERVER
    protected void HandleAuthorizationRequest(PlayerIdentity sender, ParamsReadContext ctx, bool add)
    {
        if (!ctx)
            return;

        Param1<string> payload;
        if (!ctx.Read(payload))
            return;

        string targetId = payload.param1;
        targetId.Trim();
        PlayerBase requester = FindPlayerByIdentity(sender);
        if (!requester || !IsOwner(requester))
            return;

        if (targetId == "")
            return;

        if (add)
            AddAuthorizedPlayer(targetId);
        else
            RemoveAuthorizedPlayer(targetId);
    }

    protected void HandleAuthorizationClear(PlayerIdentity sender)
    {
        PlayerBase requester = FindPlayerByIdentity(sender);
        if (!requester || !IsOwner(requester))
            return;

        ClearAuthorizedPlayers();
    }
#endif

#ifdef SERVER
    protected PlayerBase FindPlayerByIdentity(PlayerIdentity sender)
    {
        if (!sender)
            return NULL;

        DayZPlayer player = DayZPlayer.Cast(sender.GetPlayer());
        return PlayerBase.Cast(player);
    }
#endif

    void CraftItems(ParamsReadContext ctx)
    {
        Param1<ref CraftItem> rpb;
        if (!ctx.Read(rpb))
            return;

        CraftItem craftData = rpb.param1;

        //if (!ScanItems(craftData))
        //    return;

        //DeleteIngredients(craftData);
    }

    /*
    void DeleteIngredients(CraftItem arr, ItemBase work)
    {
        CraftItem crtItems = arr;
        ItemBase PCHP = work;
        ItemBase cargoItemIB;
        CargoBase cargoBase;
        int cargoCount;

        cargoBase = PCHP.GetInventory().GetCargo();
        cargoCount = cargoBase.GetItemCount();

        for (int k = 0; k < cargoCount; k++)
        {
            EntityAI crgEnt = cargoBase.GetItem(k);
            if (crgEnt && (!crgEnt.IsRuined() || crgEnt.IsWeapon()))
            {
                //CargoItems.Insert(crgEnt);
            }
        }

        for (int i = 0; i < crtItems.CraftComponents.Count(); i++)
        {
            CraftComponent cc = crtItems.CraftComponents.Get(i);
            cargoItemIB = ItemBase.Cast(GetGame().CreateObject(cc.Classname, vector.Zero, false));
            if (cargoItemIB)
            {
                if ((QuantityConversions.HasItemQuantity(cargoItemIB) == 0) || cargoItemIB.IsInherited(Magazine))
                {
                    ItemWithoutQuantity(cc);
                }
                else
                {
                    ItemWithCount(cc, PCHP);
                }

                GetGame().ObjectDelete(cargoItemIB);
            }
        }
        DeletItems.Clear();

        GiveResults(arr);
    }

    void GiveResults(CraftItem itemData)
    {
        CraftItem cragtItem = itemData;
        ItemBase itemReward;
        ItemBase tempIB;
        Magazine pileReward;
        string resultType;
        float needQuantity;
        float q_cur, q_min, q_max;

        resultType = cragtItem.Result;
        needQuantity = cragtItem.ResultCount;

        tempIB = ItemBase.Cast(GetGame().CreateObject(resultType, vector.Zero, false));
        if (!tempIB)
            return;

        QuantityConversions.GetItemQuantity(tempIB, q_cur, q_min, q_max);
        tempIB.Delete();

		if (!q_max)
            q_max = 1;

        while (needQuantity)
        {
            if (q_max < needQuantity)
            {
                itemReward = ItemBase.Cast(GetGame().CreateObject(resultType, this.GetPosition(), false));
                needQuantity -= q_max;
            }
            else
            {
                itemReward = ItemBase.Cast(GetGame().CreateObject(resultType, this.GetPosition(), false));
                if (Class.CastTo(pileReward, itemReward))
                {
                    pileReward.ServerSetAmmoCount(needQuantity);
                }
                else
                {
                    itemReward.SetQuantity(needQuantity);
                }
                needQuantity = 0;
            }
        }
    }

    void ItemWithoutQuantity(CraftComponent cc)
    {
        int amount = cc.Amount;
        string name = cc.Classname;
        int count = 1;//CargoItems.Count();
        for (int i = 0; i < count; i++)
        {
            EntityAI ent;// = CargoItems.Get(i);
            if (ent && ent.GetType() == name)
            {
                ItemBase item = ItemBase.Cast(ent);
                if (item)
                {
                    if (cc.Destroy)
                        GetGame().ObjectDelete(item);    
                    else
                        item.AddHealth(cc.Changehealth);

                    amount--;
                    if (!amount)
                        break;
                }
            }
        }
    }


    void ItemWithCount(CraftComponent cc, ItemBase PCHP)
    {
        int amount = cc.Amount;
        string name = cc.Classname;
        int count = 00;//CargoItems.Count();
        for (int i = 0; i < count; i++)
        {
            EntityAI ent;// = CargoItems.Get(i);
            if (ent && (ent.GetType() == name))
            {
                ItemBase item = ItemBase.Cast(ent);
                if (item)
                {
                    if (cc.Destroy)
                    {
                        if (item.GetQuantity() > amount)
                        {
                            item.AddQuantity(-amount);
                            break;
                        }
                        else
                        {
                            amount -= item.GetQuantity();
                            GetGame().ObjectDelete(item); 
                        }
                    }
                    else
                    {
                        if (item.GetQuantity() > amount)
                        {
                            item.AddQuantity(-amount);
                            ItemBase tempItm = ItemBase.Cast(PCHP.GetInventory().CreateInInventory(item.GetType()));

                            if (!tempItm)
                                tempItm = ItemBase.Cast(GetGame().CreateObject(item.GetType(), PCHP.GetPosition(), false));

                            tempItm.SetQuantity(amount);
                            tempItm.AddHealth(cc.Changehealth);
                            break;
                        }
                        else
                        {
                            amount -= item.GetQuantity();
                            item.AddHealth(cc.Changehealth); 
                        }
                    }
                }
            }
        }
    }

    bool ScanItems(CraftItem arr)
    {
        CargoBase cargoBase;
        int cargoCount;
        int quant;
        
        cargoBase = GetInventory().GetCargo();
        cargoCount = cargoBase.GetItemCount();
        for (int c = 0; c < cargoCount; c++)
        {
            ItemBase cargoItemIB;
			if ( Class.CastTo(cargoItemIB, cargoBase.GetItem(c)) && cargoItemIB.IsItemBase() )
            {
                if (cargoItemIB.IsRuined())
                    continue;
                
                if (cargoItemIB.HasQuantity())
                {
                    quant = cargoItemIB.GetQuantity();
                }
                else
                {
                    quant = 1;
                }

                if (!DeletItems.Contains(cargoItemIB.GetType()))
                {
                    DeletItems.Insert(cargoItemIB.GetType(), quant);
                }
                else
                {
                    int nowQuant = DeletItems.Get(cargoItemIB.GetType());
                    DeletItems.Set(cargoItemIB.GetType(), (nowQuant + quant));
                }
            }
        }

        for (int i = 0; i < arr.CraftComponents.Count(); i++)
        {
            CraftComponent cc = arr.CraftComponents.Get(i);
            int count = DeletItems.Get(cc.Classname);
            if ( count < cc.Amount )
                return false;
        }

        DeletItems.Clear();
        return true;
    }
    */

    void RegisterWatcher(string playerId)
    {
        if (!m_WatcherIds)
            m_WatcherIds = new array<string>();

        if (!playerId || playerId == "")
            return;

        if (m_WatcherIds.Find(playerId) == -1)
            m_WatcherIds.Insert(playerId);
    }

    ref array<string> GetWatchers()
    {
        if (!m_WatcherIds)
            m_WatcherIds = new array<string>();
        return m_WatcherIds;
    }

    ref array<ref FB_CraftJob> GetCraftQueue()
    {
        if (!m_CraftQueue)
            m_CraftQueue = new array<ref FB_CraftJob>();
        return m_CraftQueue;
    }

    void EnqueueCraftJob(FB_CraftJob job)
    {
        if (!job)
            return;

        if (!m_CraftQueue)
            m_CraftQueue = new array<ref FB_CraftJob>();

        m_CraftQueue.Insert(job);
        if (m_CraftQueue.Count() == 1)
        {
            ActivateNextJob();
        }
        else if (g_CraftServerManager)
        {
            g_CraftServerManager.OnJobProgress(this);
        }
    }

    protected void SerializeJob(ParamsWriteContext ctx, FB_CraftJob job)
    {
        ctx.Write(job.RecipeId);
        ctx.Write(job.RecipeName);
        ctx.Write(job.RequestedById);
        ctx.Write(job.RequestedByName);
        ctx.Write(job.TotalTime);
        ctx.Write(job.TimeRemaining);
        ctx.Write(job.Active);

        int resCount = 0;
        if (job.Results)
            resCount = job.Results.Count();
        ctx.Write(resCount);
        if (job.Results && resCount > 0)
        {
            for (int i = 0; i < resCount; i++)
            {
                FB_ResultItem res = job.Results[i];
                ctx.Write(res.Type);
                ctx.Write(res.Count);
            }
        }
    }

    protected bool DeserializeJob(ParamsReadContext ctx, out FB_CraftJob job)
    {
        job = null;
        string recipeId;
        string recipeName;
        string reqById;
        string reqByName;
        float totalTime;
        float timeRemaining;
        bool active;

        if (!ctx.Read(recipeId)) return false;
        if (!ctx.Read(recipeName)) return false;
        if (!ctx.Read(reqById)) return false;
        if (!ctx.Read(reqByName)) return false;
        if (!ctx.Read(totalTime)) return false;
        if (!ctx.Read(timeRemaining)) return false;
        if (!ctx.Read(active)) return false;

        int resCount;
        if (!ctx.Read(resCount)) return false;

        FB_CraftJob newJob = new FB_CraftJob();
        newJob.RecipeId = recipeId;
        newJob.RecipeName = recipeName;
        newJob.RequestedById = reqById;
        newJob.RequestedByName = reqByName;
        newJob.TotalTime = totalTime;
        newJob.TimeRemaining = timeRemaining;
        newJob.Active = active;

        if (resCount > 0)
        {
            for (int i = 0; i < resCount; i++)
            {
                string resType;
                int resCountVal;
                if (!ctx.Read(resType) || !ctx.Read(resCountVal))
                {
                    return false;
                }
                newJob.Results.Insert(new FB_ResultItem(resType, resCountVal));
            }
        }

        job = newJob;
        return true;
    }

    protected void ActivateNextJob()
    {
        if (!m_CraftQueue || m_CraftQueue.Count() == 0)
        {
            m_IsWorkbenchBusy = false;
            SetSynchDirty();
            if (g_CraftServerManager)
                g_CraftServerManager.OnJobProgress(this);
            return;
        }

        FB_CraftJob job = m_CraftQueue[0];
        if (!job)
        {
            m_CraftQueue.RemoveOrdered(0);
            ActivateNextJob();
            return;
        }

        job.Active = true;
        job.TimeRemaining = Math.Max(job.TotalTime, 0.01);
        m_IsWorkbenchBusy = true;
        m_QueueProgressAccumulator = 0;
        ScheduleJobCompletion(job.TimeRemaining);
        SetSynchDirty();
        if (g_CraftServerManager)
            g_CraftServerManager.OnJobProgress(this);
    }

    // Schedule a one-shot completion so the item spawns exactly when the timer ends, independent of CE tick jitter.
    protected void ScheduleJobCompletion(float seconds)
    {
        if (!GetGame() || !GetGame().IsServer())
            return;

        float clampedSeconds = Math.Max(seconds, 0.001);
        m_ActiveJobSequence++;

        // Cancel any prior scheduled finish.
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(this.OnJobTimerElapsed);
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(this.OnJobTimerElapsed, clampedSeconds * 1000, false, m_ActiveJobSequence);
    }

    protected void OnJobTimerElapsed(int seq)
    {
        if (!GetGame() || !GetGame().IsServer())
            return;

        // Ignore stale callbacks after queue changes.
        if (seq != m_ActiveJobSequence)
            return;

        if (!m_CraftQueue || m_CraftQueue.Count() == 0)
            return;

        FB_CraftJob job = m_CraftQueue[0];
        if (!job || !job.Active)
            return;

        job.TimeRemaining = 0;
        ProcessCraftQueue(0); // force completion path
    }

    protected void ProcessCraftQueue(float deltaTime)
    {
        if (!GetGame() || !GetGame().IsServer())
            return;

        if (!m_CraftQueue || m_CraftQueue.Count() == 0)
        {
            if (m_IsWorkbenchBusy)
            {
                m_IsWorkbenchBusy = false;
                SetSynchDirty();
            }
            return;
        }

        FB_CraftJob job = m_CraftQueue[0];
        if (!job)
        {
            m_CraftQueue.RemoveOrdered(0);
            ActivateNextJob();
            return;
        }

        if (!job.Active)
        {
            ActivateNextJob();
            return;
        }

        job.TimeRemaining = Math.Max(0, job.TimeRemaining - deltaTime);
        m_QueueProgressAccumulator += deltaTime;

        if (job.TimeRemaining <= 0)
        {
            job.Active = false;
            m_CraftQueue.RemoveOrdered(0);
            if (g_CraftServerManager)
                g_CraftServerManager.OnJobCompleted(this, job);
            ActivateNextJob();
        }
        else if (m_QueueProgressAccumulator >= 1.0)
        {
            m_QueueProgressAccumulator = 0;
            if (g_CraftServerManager)
                g_CraftServerManager.OnJobProgress(this);
        }
    }

    /**
     *  Custom Territory Upkeep Logic ========================================================================
     */

    override void OnCEUpdate()
	{
#ifdef SERVER
        EnsureTracking();
#endif

		int refreshTimeCounterTemp = m_RefreshTimeCounter;
        m_RefresherTimeRemaining = 0;
        
        super.OnCEUpdate();

        bool shouldRecalcDrain = m_PowerScanDirty;
        m_CheckBasePartsCounter += Math.Round(m_ElapsedSinceLastUpdate);
        if ( m_CheckBasePartsCounter > m_CheckBasePartsFrequency )
        {
            shouldRecalcDrain = true;
            m_CheckBasePartsCounter = 0;
        }

        if ( shouldRecalcDrain )
        {
            EnergyConsumptionCalculated();
            m_PowerScanDirty = false;
        }

    #ifdef SERVER
        if (m_PeriodicFullScanSeconds > 0)
        {
            m_PeriodicFullScanAccumulator += m_ElapsedSinceLastUpdate;
            if (m_PeriodicFullScanAccumulator >= m_PeriodicFullScanSeconds)
            {
                FullReseedTrackedEntities();
                m_PeriodicFullScanAccumulator = 0;
            }
        }
    #endif

        m_CheckBatteriesCounter += Math.Round(m_ElapsedSinceLastUpdate);
        if ( m_CheckBatteriesCounter > m_CheckBatteriesFrequency )
        {
            ConsumeEnergy(m_CheckBatteriesCounter);
            m_CheckBatteriesCounter = 0;
        }

    #ifdef SERVER
        UpdateLifetimeCache();
    #endif

        if ( m_HasEnergyLeft )
        {
            CEApi ceApi = GetCEApi();
            int checkFrequency = m_CeRefreshInterval;
            if ( checkFrequency <= 0 && ceApi )
            {
                checkFrequency = ceApi.GetCEGlobalInt("FlagRefreshFrequency");
            }
            if ( !checkFrequency )
                checkFrequency = 1800;

		    m_RefreshTimeCounter = refreshTimeCounterTemp;
            m_RefreshTimeCounter += Math.Round(m_ElapsedSinceLastUpdate);
            if ( m_RefreshTimeCounter > checkFrequency && ceApi )
			{
                ceApi.RadiusLifetimeReset(GetPosition(), GetTerritoryRadius());
				m_RefreshTimeCounter = 0;
            }

            m_AutoRefreshAccumulator += m_ElapsedSinceLastUpdate;
            if (m_AutoRefreshAccumulator >= m_AutoRefreshInterval)
            {
                PerformAutoRefresh();
                m_AutoRefreshAccumulator = 0;
            }
        }
        else
        {
            m_AutoRefreshAccumulator = 0;
        }

        float craftDelta = m_ElapsedSinceLastUpdate;
        // If per-frame ticking is disabled, fall back to the coarse CE tick.
        if (craftDelta > 0 && !m_UseFrameCraftTick)
            ProcessCraftQueue(craftDelta);

        SetRefresherActiveCustom( m_HasEnergyLeft );
        
        SetSynchDirty();
	}

    override void SetRefresherActive( bool state ) {}

    void SetRefresherActiveCustom( bool state )
    {
        if ( m_RefresherActive != state )
		{
			m_RefresherActive = state;
			
			//update on refresher activation / last update on refresher deactivation
			if (GetCEApi())
                GetCEApi().RadiusLifetimeReset(GetPosition(), GetTerritoryRadius());
		}
    }

    protected void PerformAutoRefresh()
    {
        CEApi api = GetCEApi();
        if (api)
        {
            api.RadiusLifetimeReset(GetPosition(), GetTerritoryRadius());
            m_RefreshTimeCounter = 0;
        }
    }
    
    override string GetDisplayName()
    {
        string name = super.GetDisplayName();

        // Show remaining time only; no colored flag tags.
        name = name + " - " + GetBaseTimeLeftFormatted();

        return name;
    }

    string DebugTime(int timeleft)
    {
        int minutes = (timeleft % SECONDS_IN_AN_HOUR) / SECONDS_IN_A_MINUTE;
        int seconds = timeleft % SECONDS_IN_A_MINUTE;

        return string.Format("%1:%2", minutes, seconds );
    }

    string GetBaseTimeLeftFormatted()
    {
        int timeleft = BaseLifeTimeLeft();
        if (timeleft <= 0)
            return "[DECAYING]";

        if (timeleft == NO_DRAIN_TIMELEFT)
            return "[POWERED]";

        int days = timeleft / SECONDS_IN_A_DAY;
        int hours = (timeleft % SECONDS_IN_A_DAY) / SECONDS_IN_AN_HOUR;
        int minutes = (timeleft % SECONDS_IN_AN_HOUR) / SECONDS_IN_A_MINUTE;

        return string.Format("[%1D %2H %3M]", days.ToStringLen(2), hours.ToStringLen(2), minutes.ToStringLen(2));
    }

    void UpdateRefresherTimeRemaining()
    {
        EnergyConsumptionCalculated();

#ifdef SERVER
        UpdateLifetimeCache();
#endif

        SetSynchDirty();
    }

#ifdef SERVER
    void ForceStatusSync()
    {
        // Mark dirty then recompute so clients see an updated timer when inspecting.
        m_PowerScanDirty = true;
        EnergyConsumptionCalculated();
        GetEnergyLeftInBattery();
        UpdateLifetimeCache(true);
        SetSynchDirty();
    }
#endif

    protected int ComputeLifetimeSeconds()
    {
        int energyLeft = GetEnergyLeftInBattery();
        if (energyLeft <= 0)
            return 0;

        if (m_EnergyDrainPerSecond <= 0)
            return NO_DRAIN_TIMELEFT; // effectively infinite while powered with no active drain

        return Math.Max(0, energyLeft / m_EnergyDrainPerSecond );
    }

#ifdef SERVER
    protected void UpdateLifetimeCache(bool force = false)
    {
        int computed = ComputeLifetimeSeconds();
        if (force || computed != m_BaseLifetimeLeft)
        {
            m_BaseLifetimeLeft = computed;
            SetSynchDirty();
        }
    }
#endif

    int BaseLifeTimeLeft()
    {
#ifdef SERVER
        UpdateLifetimeCache();
#endif

        return m_BaseLifetimeLeft;
    }

    float EnergyConsumptionCalculated()
    {
        float timeElapsed = 0;
        bool objfound;

        if (!m_TrackedEntities)
            m_TrackedEntities = new array<EntityAI>();

        float radius = GetTerritoryRadius();
        float radiusSq = radius * radius;

        for (int i = m_TrackedEntities.Count() - 1; i >= 0; i--)
        {
            EntityAI entity = m_TrackedEntities[i];
            if (!entity || entity.IsPendingDeletion())
            {
                m_TrackedEntities.Remove(i);
                continue;
            }

            // Safety: drop anything that drifted outside the bubble without triggering leave (e.g., cleanup moves).
            if (vector.DistanceSq(entity.GetPosition(), GetPosition()) > radiusSq)
            {
                m_TrackedEntities.Remove(i);
                continue;
            }

            float powerConsumptionPerHour = ModuleFrontierWorkbenchPower.Get().GetItemPowerConsumptionPerHour(entity.GetType());
            if (powerConsumptionPerHour > 0)
            {
                objfound = true;
                timeElapsed += powerConsumptionPerHour;
            }

            if (entity.GetInventory())
            {
                int count = entity.GetInventory().AttachmentCount();
                for (int j = 0; j < count; j++)
                {
                    EntityAI attachment = entity.GetInventory().GetAttachmentFromIndex(j);
                    if (!attachment)
                        continue;

                    powerConsumptionPerHour = ModuleFrontierWorkbenchPower.Get().GetItemPowerConsumptionPerHour(attachment.GetType());
                    if (powerConsumptionPerHour > 0)
                    {
                        objfound = true;
                        timeElapsed += powerConsumptionPerHour;
                    }
                }
            }
        }

        if (objfound)
        {
            m_EnergyDrainPerSecond = (timeElapsed * GetPowerDrainMultiplier()) / 3600;
        }
        else
        {
            // Fallback: if we have power but no tracked consumers, treat the bench as having a tiny idle drain
            // so we still surface a time estimate instead of showing [POWERED] indefinitely.
            if (m_HasEnergyLeft)
                m_EnergyDrainPerSecond = (1 * GetPowerDrainMultiplier()) / 3600; // 1 unit/hour idle drain
            else
                m_EnergyDrainPerSecond = -1;
        }

#ifdef SERVER
        UpdateLifetimeCache();
#endif

        return m_EnergyDrainPerSecond;       
    }

    void ConsumeEnergy( float timeSpent )
    {
        if (m_EnergyDrainPerSecond <= 0)
            return;
        
        int energyToConsume = Math.Floor(timeSpent * m_EnergyDrainPerSecond);
        if (energyToConsume < 1)
            return;

        foreach( string batterySlotName : POWER_SOURCE_ATTACHMENTS )
        {
            ItemBase battery = ItemBase.Cast( GetInventory().FindAttachmentByName(batterySlotName) );
            if ( !battery )
                continue;

            int energyLeft = battery.GetQuantity();

            battery.AddQuantity(-energyToConsume);

            energyToConsume -= energyLeft;
            
            if (energyToConsume < 1)
                break;
        }

        GetEnergyLeftInBattery();

#ifdef SERVER
    UpdateLifetimeCache();
#endif
    }

    int GetEnergyLeftInBattery()
    {
        if (!GetInventory())
            return 0;
        
        int energy = 0;
        foreach( string batterySlotName: POWER_SOURCE_ATTACHMENTS )
        {
            ItemBase battery = ItemBase.Cast( GetInventory().FindAttachmentByName(batterySlotName) );
            if ( !battery )
                continue;
            
            energy += battery.GetQuantity();
        }

        m_HasEnergyLeft = energy > 0;

        return energy;
    }

    bool HasPower()
    {
        return GetEnergyLeftInBattery() > 0;
    }

#ifdef SERVER
    override void EEItemAttached( EntityAI item, string slot_name )
    {
		super.EEItemAttached( item, slot_name);
        
        if ( POWER_SOURCE_ATTACHMENTS.Find(slot_name) > -1 )
        {
            UpdateRefresherTimeRemaining();
            SetRefresherActiveCustom(m_HasEnergyLeft);
        }
        // Track newly attached items so their power consumption contributes to the timer even if they never enter the trigger volume.
        RegisterTrackedEntity(ItemBase.Cast(item));
        UpdateUpgradeEffects();
	}

    override void EEItemDetached( EntityAI item, string slot_name )
    {
        super.EEItemDetached( item, slot_name);
        
        if ( POWER_SOURCE_ATTACHMENTS.Find(slot_name) > -1 )
        {
            UpdateRefresherTimeRemaining();
            SetRefresherActiveCustom(m_HasEnergyLeft);
        }
        // Stop tracking detached items; they no longer draw power from this bench.
        UnregisterTrackedEntity(ItemBase.Cast(item));
        UpdateUpgradeEffects();
    }
#endif

    /**
     *  3rd Party ========================================================================
     */

    override void EEOnAfterLoad()
    {
        super.EEOnAfterLoad();

        SetLifetime( GetEconomyProfile().GetLifetime() );

#ifdef SERVER
        EnsureTracking();
#endif
    }

    override bool CanReleaseAttachment(EntityAI attachment) return true;    
    override bool CanRemoveFromCargo(EntityAI parent) return true;
    override bool CanReleaseCargo(EntityAI cargo) return true;
    override bool CanReceiveAttachment( EntityAI attachment, int slotId ) return true;
    override float GetRefresherTime01() return 0;
    
    /**
     * Vanilla TerritoryFlag overwrites. ========================================================================
     */

	override void SetPartsAfterStoreLoad() {};
    override bool CanPutInCargo( EntityAI parent ) return false;
    override bool CanPutIntoHands( EntityAI parent ) return false;
    override bool CanDisplayAttachmentSlot(string slot_name) return true;

	override void SetActions()
    {
		super.SetActions();
		
		RemoveAction(ActionRaiseFlag);
        RemoveAction(ActionLowerFlag);
        RemoveAction(ActionFoldBaseBuildingObject);

        AddAction(ActionFBWorkbenchOpen);    
        AddAction(ActionFBWorkbenchClaim);
        AddAction(ActionFBWorkbenchClearAccess);
        AddAction(ActionDismantleWorkbench);
    }

    override void EEDelete(EntityAI parent)
    {
        super.EEDelete(parent);

#ifdef SERVER
        if (m_WorkbenchTrigger && g_Game)
        {
            g_Game.ObjectDelete(m_WorkbenchTrigger);
            m_WorkbenchTrigger = null;
        }

        if (m_TrackedEntities)
            m_TrackedEntities.Clear();
#endif
    }

    override void OnStoreSave(ParamsWriteContext ctx)
    {
        super.OnStoreSave(ctx);
        WriteAuthorizedList(ctx);
        ctx.Write(m_WorkbenchOwnerId);

        // Persist craft queue
        int queueVersion = CRAFT_STORE_VERSION;
        ctx.Write(queueVersion);

        int queueCount = 0;
        if (m_CraftQueue)
            queueCount = m_CraftQueue.Count();
        ctx.Write(queueCount);

        if (queueCount > 0)
        {
            for (int i = 0; i < m_CraftQueue.Count(); i++)
            {
                FB_CraftJob job = m_CraftQueue[i];
                SerializeJob(ctx, job);
            }
        }
    }

    override bool OnStoreLoad(ParamsReadContext ctx, int version)
    {
        if (!super.OnStoreLoad(ctx, version))
            return false;

        if (!ReadAuthorizedList(ctx))
        {
            EnsureAuthorizedList();
            m_AuthorizedIds.Clear();
            return false;
        }

        if (!ctx.Read(m_WorkbenchOwnerId))
            m_WorkbenchOwnerId = "";
        // Flag ownership if an ID was stored; strings do not coerce to bool so compare explicitly.
        m_HasOwnerFlag = (m_WorkbenchOwnerId != "");

        int queueVersion;
        if (!ctx.Read(queueVersion))
            queueVersion = 0;

        int queueCount;
        if (!ctx.Read(queueCount))
            queueCount = 0;

        if (queueCount > 0)
        {
            if (!m_CraftQueue)
                m_CraftQueue = new array<ref FB_CraftJob>();

            m_CraftQueue.Clear();
            for (int i = 0; i < queueCount; i++)
            {
                FB_CraftJob loadedJob;
                if (DeserializeJob(ctx, loadedJob))
                    m_CraftQueue.Insert(loadedJob);
            }
        }

        // Restore active state and schedule completion for the head of the queue.
        if (m_CraftQueue && m_CraftQueue.Count() > 0)
        {
            FB_CraftJob head = m_CraftQueue[0];
            if (head)
            {
                head.Active = true;
                m_IsWorkbenchBusy = true;
                m_ActiveJobSequence++;
                float remaining = Math.Max(head.TimeRemaining, 0.001);
                ScheduleJobCompletion(remaining);
            }
        }
        else
        {
            m_IsWorkbenchBusy = false;
        }
#ifdef SERVER
        UpdateUpgradeEffects();
        RefreshBaseQueueLimit();
        SetSynchDirty();
#endif

        return true;
    }

    override void OnDebugSpawn()
    {
        GetInventory().CreateAttachment("U_Battery_1");
        GetInventory().CreateAttachment("U_Battery_2");
        GetInventory().CreateAttachment("U_Battery_3");
#ifdef SERVER
        UpdateUpgradeEffects();
#endif
    }

#ifdef SERVER
    protected void UpdateUpgradeEffects()
    {
        float previousPower = m_PowerDrainMultiplier;
        float previousSpeed = m_CraftSpeedMultiplier;
        int previousQueueBonus = m_QueueBonus;
        int previousTier = m_WorkbenchTier;
        int previousRequirement = m_RequiredCraftTier;
        bool previousUnlock = m_CraftingUnlocked;

            m_PowerDrainMultiplier = 1.0;
            m_CraftSpeedMultiplier = 1.0;
            m_QueueBonus = 0;
            m_TerritoryRadiusBonus = 0;
        ModuleFrontierUpgradeConfig upgradeConfig;
        if (WORKBENCH_UPGRADES_ENABLED)
            upgradeConfig = ModuleFrontierUpgradeConfig.Get();

        if (WORKBENCH_UPGRADES_ENABLED && GetInventory() && upgradeConfig)
        {
            int attachCount = GetInventory().AttachmentCount();
            for (int i = 0; i < attachCount; i++)
            {
                ItemBase moduleItem = ItemBase.Cast(GetInventory().GetAttachmentFromIndex(i));
                if (!moduleItem)
                    continue;

                FrontierUpgradeEffect effect = upgradeConfig.GetEffectForType(moduleItem.GetType());
                if (!effect)
                    continue;

                m_PowerDrainMultiplier *= effect.powerMultiplier;
                m_CraftSpeedMultiplier *= effect.craftSpeedMultiplier;
                m_QueueBonus += effect.queueBonus;
                m_TerritoryRadiusBonus += effect.territoryRadiusBonus;
            }
        }

        if (m_PowerDrainMultiplier <= 0)
            m_PowerDrainMultiplier = 0.1;

        if (m_CraftSpeedMultiplier <= 0)
            m_CraftSpeedMultiplier = 0.1;

        if (m_QueueBonus < 0)
            m_QueueBonus = 0;

        if (m_TerritoryRadiusBonus < 0)
            m_TerritoryRadiusBonus = 0;

        m_WorkbenchTier = 1;
        m_RequiredCraftTier = 1;
        m_CraftingUnlocked = true;

        bool stateChanged = previousTier != m_WorkbenchTier || previousRequirement != m_RequiredCraftTier || previousUnlock != m_CraftingUnlocked;
        bool queueChanged = previousQueueBonus != m_QueueBonus;

        if (stateChanged || queueChanged || previousPower != m_PowerDrainMultiplier || previousSpeed != m_CraftSpeedMultiplier)
        {
#ifdef SERVER
            if (g_CraftServerManager)
                g_CraftServerManager.NotifyQueueUpdate(this);
#endif
            SetSynchDirty();
        }

        UpdateRefresherTimeRemaining();

    #ifdef SERVER
        UpdateTriggerRadius();
        SeedTrackedEntities();
    #endif
    }

    protected void RefreshBaseQueueLimit()
    {
        ModuleFrontierConfigHub hub = ModuleFrontierConfigHub.Get();
        if (hub)
            m_BaseQueueLimit = hub.GetBaseQueueLimit();
        else
            m_BaseQueueLimit = 1;
    }
#endif

    float GetCraftSpeedMultiplier()
    {
        return Math.Max(m_CraftSpeedMultiplier, 0.1);
    }

    float GetPowerDrainMultiplier()
    {
        return Math.Max(m_PowerDrainMultiplier, 0.1);
    }

    float GetTerritoryRadius()
    {
        ModuleFrontierConfigHub hub = ModuleFrontierConfigHub.Get();
        float baseRadius = GameConstants.REFRESHER_RADIUS;
        if (hub)
            baseRadius = hub.GetTerritoryRadius();

        float radius = baseRadius + m_TerritoryRadiusBonus;
        return Math.Max(radius, 1);
    }

    int GetQueueLimit()
    {
        int baseLimit = Math.Max(m_BaseQueueLimit, 1);
        return baseLimit + m_QueueBonus;
    }

    bool CanAcceptNewJob()
    {
        return GetCraftQueue().Count() < GetQueueLimit();
    }
};
// test




