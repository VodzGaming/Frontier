class FrontierUpgradeEffect
{
    float powerMultiplier;
    float craftSpeedMultiplier;
    int queueBonus;
    string slot;
    int tierBonus;
    float territoryRadiusBonus;

    void FrontierUpgradeEffect()
    {
        powerMultiplier = 1.0;
        craftSpeedMultiplier = 1.0;
        queueBonus = 0;
        slot = "";
        tierBonus = 0;
        territoryRadiusBonus = 0;
    }
}

class FrontierUpgradeConfig
{
    ref map<string, ref FrontierUpgradeEffect> modules;
    int baseTier;
    int craftUnlockTier;

    void FrontierUpgradeConfig()
    {
        modules = new map<string, ref FrontierUpgradeEffect>();
        baseTier = 1;
        craftUnlockTier = 2;
    }

    bool Normalize()
    {
        bool changed = false;
        if (!modules)
        {
            modules = new map<string, ref FrontierUpgradeEffect>();
            changed = true;
        }

        if (baseTier < 0)
        {
            baseTier = 0;
            changed = true;
        }

        if (craftUnlockTier < 1)
        {
            craftUnlockTier = 1;
            changed = true;
        }

        foreach (string key, FrontierUpgradeEffect effect : modules)
        {
            if (!effect)
            {
                modules.Remove(key);
                changed = true;
                continue;
            }

            if (effect.powerMultiplier <= 0)
            {
                effect.powerMultiplier = 1.0;
                changed = true;
            }

            if (effect.craftSpeedMultiplier <= 0)
            {
                effect.craftSpeedMultiplier = 1.0;
                changed = true;
            }

            if (effect.territoryRadiusBonus < 0)
            {
                effect.territoryRadiusBonus = 0;
                changed = true;
            }

            if (!effect.slot)
            {
                effect.slot = "";
                changed = true;
            }

            if (effect.tierBonus < 0)
            {
                effect.tierBonus = 0;
                changed = true;
            }
        }

        return changed;
    }

    static FrontierUpgradeConfig BuildDefaults()
    {
        FrontierUpgradeConfig cfg = new FrontierUpgradeConfig();
        cfg.baseTier = 2;
        cfg.craftUnlockTier = 2;

        FrontierUpgradeEffect powerModule = new FrontierUpgradeEffect();
        powerModule.slot = "FB_ModuleSlot_Power";
        powerModule.powerMultiplier = 0.75;
        cfg.modules.Insert("FB_WorkbenchModule_Power", powerModule);

        FrontierUpgradeEffect radiusModule = new FrontierUpgradeEffect();
        radiusModule.slot = "FB_ModuleSlot_Utility";
        radiusModule.craftSpeedMultiplier = 0.8;
        radiusModule.territoryRadiusBonus = 50; // sample: boost territory coverage while attached
        cfg.modules.Insert("FB_WorkbenchModule_Radius", radiusModule);

        FrontierUpgradeEffect queueModule = new FrontierUpgradeEffect();
        queueModule.slot = "FB_ModuleSlot_Utility";
        queueModule.queueBonus = 2;
        cfg.modules.Insert("FB_WorkbenchModule_Queue", queueModule);

        return cfg;
    }
}
