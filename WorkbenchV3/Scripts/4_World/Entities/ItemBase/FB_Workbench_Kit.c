class FB_Workbench_Kit;

modded class TerritoryFlagKit
{
    override void OnPlacementComplete( Man player, vector position = "0 0 0", vector orientation = "0 0 0" )
    {
        FB_Workbench_Kit fbKit = FB_Workbench_Kit.Cast(this);
        if (fbKit)
        {
            fbKit.HandleWorkbenchPlacement(player, position, orientation);
            return;
        }

        super.OnPlacementComplete(player, position, orientation);
    }
}

class FB_Workbench_Kit: TerritoryFlagKit
{
    void HandleWorkbenchPlacement( Man player, vector position, vector orientation )
    {
        if (!GetGame().IsServer())
            return;

        FB_Workbench totem = FB_Workbench.Cast( GetGame().CreateObjectEx( "FB_Workbench", GetPosition(), ECE_PLACE_ON_SURFACE ) );
        if (!totem)
            return;

        totem.SetPosition( position );
        totem.SetOrientation( orientation );

        PlayerBase placer = PlayerBase.Cast(player);
        if (placer)
            totem.FrontierAssignOwner(placer);

        string failureReason;
        if (!ValidatePlacement(position, totem, failureReason))
        {
            NotifyPlacementBlocked(placer, failureReason);
            RefundKit(placer, position);
            GetGame().ObjectDelete(totem);
            return;
        }

        HideAllSelections();
        SetIsDeploySound(true);
    }

    override bool IsBasebuildingKit() return true;
    override bool DisassembleOnLastDetach() return false;
    override void EEItemDetached(EntityAI item, string slot_name) {};
    override void OnEndPlacement() {};
    override void OnPlacementCancelled(Man player) {};
    override bool IsDeployable() return true;
    override bool CanAssignAttachmentsToQuickbar() return false;
    override string GetDeploySoundset() return "putDown_FenceKit_SoundSet";
    override string GetLoopDeploySoundset() return "Shelter_Site_Build_Loop_SoundSet";
    override string GetDeployFinishSoundset() return "";
    override void RefreshPhysics() {};

    override bool PlacementCanBeRotated() return true;
    override bool DoPlacingHeightCheck() return false;
    override bool CanReceiveAttachment(EntityAI attachment, int slotId) return false;
    override void OnDebugSpawn() {};

    protected bool ValidatePlacement(vector position, FB_Workbench placedBench, out string reason)
    {
        reason = "";
        ModuleFrontierPlacementConfig placementModule = ModuleFrontierPlacementConfig.Get();
        FrontierPlacementConfig cfg = placementModule.GetConfig();
        if (!cfg)
            return true;

        if (cfg.minDistance > 0 && IsPlacementTooClose(position, cfg.minDistance, placedBench))
        {
            reason = string.Format("Workbenches must be at least %1 meters apart.", Math.Round(cfg.minDistance).ToString());
            return false;
        }

        if (cfg.maxSlopeDegrees > 0 && !IsSlopeAllowed(position, cfg.maxSlopeDegrees))
        {
            reason = string.Format("Terrain slope must be under %1°.", Math.Round(cfg.maxSlopeDegrees).ToString());
            return false;
        }

        if (cfg.minElevation != 0 && position[1] < cfg.minElevation)
        {
            reason = string.Format("Elevation must be above %1m.", cfg.minElevation.ToString());
            return false;
        }

        if (cfg.maxElevation > 0 && position[1] > cfg.maxElevation)
        {
            reason = string.Format("Elevation must be below %1m.", cfg.maxElevation.ToString());
            return false;
        }

        if (!IsSurfaceAllowed(position, cfg))
        {
            if (cfg.allowedSurfaces && cfg.allowedSurfaces.Count() > 0)
                reason = "Workbench must be placed on an approved surface.";
            else
                reason = "Workbench cannot be placed on this surface.";
            return false;
        }

        if (IsInsideBlockedZone(position, cfg))
        {
            reason = "Workbench cannot be placed in this restricted area.";
            return false;
        }

        if (IsInsideTerritory(position, cfg, placedBench))
        {
            reason = "Workbench cannot be placed inside a territory.";
            return false;
        }

        return true;
    }

    protected bool IsPlacementTooClose(vector position, float minDist, FB_Workbench justPlaced)
    {
        array<Object> objects = new array<Object>();
        array<CargoBase> proxy = new array<CargoBase>();
        GetGame().GetObjectsAtPosition(position, minDist, objects, proxy);
        foreach (Object obj : objects)
        {
            FB_Workbench bench = FB_Workbench.Cast(obj);
            if (!bench || bench == justPlaced)
                continue;
            return true;
        }
        return false;
    }

    protected bool IsSlopeAllowed(vector position, float maxSlopeDegrees)
    {
        if (maxSlopeDegrees <= 0)
            return true;

        vector normal = GetGame().SurfaceGetNormal(position[0], position[2]);
        normal.Normalize();
        float dot = Math.Clamp(normal[1], -1.0, 1.0);
        float slopeDeg = Math.Acos(dot) * Math.RAD2DEG;
        return slopeDeg <= maxSlopeDegrees;
    }

    protected bool IsSurfaceAllowed(vector position, FrontierPlacementConfig cfg)
    {
        if (!cfg)
            return true;

        string surfType;
        GetGame().SurfaceGetType(position[0], position[2], surfType);
        surfType.ToLower();

        if (cfg.allowedSurfaces && cfg.allowedSurfaces.Count() > 0)
        {
            if (cfg.allowedSurfaces.Find(surfType) == -1)
                return false;
        }

        if (cfg.blockedSurfaces && cfg.blockedSurfaces.Count() > 0)
        {
            if (cfg.blockedSurfaces.Find(surfType) > -1)
                return false;
        }

        return true;
    }

    protected bool IsInsideBlockedZone(vector position, FrontierPlacementConfig cfg)
    {
        if (!cfg || !cfg.blockedZones || cfg.blockedZones.Count() == 0)
            return false;

        foreach (FrontierPlacementZone zone : cfg.blockedZones)
        {
            if (!zone || zone.radius <= 0)
                continue;

            vector center = zone.GetPosVector();
            float distSq = vector.DistanceSq(position, center);
            if (distSq <= zone.radius * zone.radius)
                return true;
        }

        return false;
    }

    protected bool IsInsideTerritory(vector position, FrontierPlacementConfig cfg, FB_Workbench justPlaced)
    {
        if (!cfg || !cfg.blockInsideTerritory)
            return false;

        float radius = cfg.territoryRadius;
        if (radius <= 0)
            radius = GameConstants.REFRESHER_RADIUS;

        array<Object> objects = new array<Object>();
        array<CargoBase> proxy = new array<CargoBase>();
        GetGame().GetObjectsAtPosition(position, radius, objects, proxy);

        foreach (Object obj : objects)
        {
            // Check territory flags
            bool isFlag = false;
            if (cfg.territoryFlagTypes && cfg.territoryFlagTypes.Count() > 0)
            {
                foreach (string flagType : cfg.territoryFlagTypes)
                {
                    if (obj && obj.IsKindOf(flagType))
                    {
                        isFlag = true;
                        break;
                    }
                }
            }
            else if (obj && obj.IsKindOf("TerritoryFlag"))
                isFlag = true;

            // Check no-build classes
            bool isNoBuild = false;
            if (cfg.noBuildClasses && cfg.noBuildClasses.Count() > 0)
            {
                foreach (string cls : cfg.noBuildClasses)
                {
                    if (obj && obj.IsKindOf(cls))
                    {
                        isNoBuild = true;
                        break;
                    }
                }
            }

            if (!isFlag && !isNoBuild)
                continue;

            if (justPlaced && obj == justPlaced)
                continue;

            return true;
        }

        return false;
    }

    protected void NotifyPlacementBlocked(PlayerBase player, string message)
    {
        if (!player)
            return;

        if (message && message != "")
            player.MessageStatus(message);
        else
            player.MessageStatus("Cannot place the workbench here.");
    }

    protected void RefundKit(PlayerBase player, vector dropPosition)
    {
        if (!player)
            return;

        EntityAI returned = player.GetInventory().CreateInInventory("FB_Workbench_Kit");
        if (!returned)
        {
            HumanInventory hInv = player.GetHumanInventory();
            if (hInv)
                returned = hInv.CreateInHands("FB_Workbench_Kit");
        }

        if (!returned)
            returned = EntityAI.Cast(GetGame().CreateObject("FB_Workbench_Kit", dropPosition, false));

        if (returned)
            returned.SetPosition(dropPosition);
    }
};
