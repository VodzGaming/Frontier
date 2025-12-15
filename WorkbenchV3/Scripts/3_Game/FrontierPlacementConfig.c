class FrontierPlacementZone
{
    ref array<float> position;
    float radius;

    void FrontierPlacementZone()
    {
        position = {0, 0, 0};
        radius = 0;
    }

    vector GetPosVector()
    {
        if (!position || position.Count() < 3)
            return "0 0 0";
        return Vector(position[0], position[1], position[2]);
    }
}

class FrontierPlacementConfig
{
    float minDistance;
    float maxSlopeDegrees;
    float minElevation;
    float maxElevation;
    bool blockInsideTerritory;
    float territoryRadius;
    ref array<string> territoryFlagTypes;
    ref array<string> noBuildClasses;
    ref array<string> allowedSurfaces;
    ref array<string> blockedSurfaces;
    ref array<ref FrontierPlacementZone> blockedZones;

    void FrontierPlacementConfig()
    {
        minDistance = 0;
        maxSlopeDegrees = 0;
        minElevation = 0;
        maxElevation = 0;
        blockInsideTerritory = true;
        territoryRadius = GameConstants.REFRESHER_RADIUS;
        territoryFlagTypes = new array<string>();
        noBuildClasses = new array<string>();
        allowedSurfaces = new array<string>();
        blockedSurfaces = new array<string>();
        blockedZones = new array<ref FrontierPlacementZone>();
    }

    bool Normalize()
    {
        bool changed = false;

        if (!allowedSurfaces)
        {
            allowedSurfaces = new array<string>();
            changed = true;
        }

        if (!blockedSurfaces)
        {
            blockedSurfaces = new array<string>();
            changed = true;
        }

        if (!blockedZones)
        {
            blockedZones = new array<ref FrontierPlacementZone>();
            changed = true;
        }

        if (!territoryFlagTypes)
        {
            territoryFlagTypes = new array<string>();
            changed = true;
        }

        if (!noBuildClasses)
        {
            noBuildClasses = new array<string>();
            changed = true;
        }

        for (int i = 0; i < allowedSurfaces.Count(); i++)
        {
            string surf = allowedSurfaces[i];
            string lowered = surf;
            lowered.ToLower();
            if (lowered != surf)
            {
                allowedSurfaces.Set(i, lowered);
                changed = true;
            }
        }

        for (int j = 0; j < blockedSurfaces.Count(); j++)
        {
            string bsurf = blockedSurfaces[j];
            string bLowered = bsurf;
            bLowered.ToLower();
            if (bLowered != bsurf)
            {
                blockedSurfaces.Set(j, bLowered);
                changed = true;
            }
        }

        if (minDistance < 0)
        {
            minDistance = 0;
            changed = true;
        }

        if (territoryRadius <= 0)
        {
            territoryRadius = GameConstants.REFRESHER_RADIUS;
            changed = true;
        }

        if (maxSlopeDegrees < 0)
        {
            maxSlopeDegrees = 0;
            changed = true;
        }

        for (int z = blockedZones.Count() - 1; z >= 0; z--)
        {
            FrontierPlacementZone zone = blockedZones[z];
            if (!zone)
            {
                blockedZones.RemoveOrdered(z);
                changed = true;
                continue;
            }

            if (!zone.position || zone.position.Count() < 3)
            {
                zone.position = {0, 0, 0};
                changed = true;
            }

            if (zone.radius < 0)
            {
                zone.radius = 0;
                changed = true;
            }
        }

        return changed;
    }

    static FrontierPlacementConfig BuildDefaults()
    {
        FrontierPlacementConfig cfg = new FrontierPlacementConfig();
        cfg.minDistance = 0;
        cfg.maxSlopeDegrees = 20;
        cfg.minElevation = 0;
        cfg.maxElevation = 0;
        cfg.blockInsideTerritory = true;
        cfg.territoryRadius = GameConstants.REFRESHER_RADIUS;
        cfg.territoryFlagTypes.Insert("TerritoryFlag");
        cfg.territoryFlagTypes.Insert("FB_TerritoryBeacon");
        cfg.noBuildClasses.Insert("NoBuildZone");

        FrontierPlacementZone sample = new FrontierPlacementZone();
        sample.position = {0, 0, 0};
        sample.radius = 0;
        cfg.blockedZones.Insert(sample);

        return cfg;
    }
}
