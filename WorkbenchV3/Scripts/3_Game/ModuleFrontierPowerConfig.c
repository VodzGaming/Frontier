class ModuleFrontierPowerConfig
{
    ref map<string, float> powerConsumptionOfItemPerHour;
    bool exactMatchOnly; // when true, disable inheritance lookup
    bool enablePeriodicFullScan; // when true, benches periodically force a full rescan
    int periodicFullScanSeconds; // interval between forced rescans; 0/negative disables

    void ModuleFrontierPowerConfig()
    {
        powerConsumptionOfItemPerHour = new map<string, float>();
        // Default to exact-match lookups and periodic rescans for a safer, deterministic behavior.
        exactMatchOnly = true;
        enablePeriodicFullScan = true;
        periodicFullScanSeconds = 300; // 5 minutes
    }

    protected bool ContainsKeyCaseInsensitive(string className)
    {
        if (!powerConsumptionOfItemPerHour)
            return false;

        string lowered = className;
        lowered.ToLower();

        foreach (string key, float value : powerConsumptionOfItemPerHour)
        {
            string loweredKey = key;
            loweredKey.ToLower();
            if (loweredKey == lowered)
                return true;
        }

        return false;
    }

    float GetItemPowerConsumptionPerHour(string item)
    {
        if (!powerConsumptionOfItemPerHour)
            return 0.0;

        string loweredItem = item;
        loweredItem.ToLower();

        // Exact match (case-insensitive).
        foreach (string kExact, float vExact : powerConsumptionOfItemPerHour)
        {
            string lk = kExact;
            lk.ToLower();
            if (lk == loweredItem)
                return vExact;
        }

        if (!exactMatchOnly)
        {
            // Inheritance match: allow config to specify a base class (e.g., BBP_WALL_BASE).
            if (GetGame())
            {
                foreach (string kKind, float vKind : powerConsumptionOfItemPerHour)
                {
                    if (GetGame().IsKindOf(item, kKind))
                        return vKind;
                }
            }
        }

        return 0.0;
    }

    void TypesToLower()
    {
        // Preserve original key casing to keep IsKindOf working on class names.
        // We handle case-insensitive exact matches in GetItemPowerConsumptionPerHour.
    }

    bool ApplyFrontierDefaults()
    {
        if (!powerConsumptionOfItemPerHour)
            powerConsumptionOfItemPerHour = new map<string, float>();

        if (periodicFullScanSeconds < 0)
            periodicFullScanSeconds = 0;
        if (periodicFullScanSeconds == 0 && enablePeriodicFullScan)
            periodicFullScanSeconds = 300;

        // assumes ~13.5x accelerated time (12x day / 15x night) and 72k-unit batteries.
        // values below keep a single item powered for roughly 5 real days.
        bool changed = false;
        changed |= EnsureDefault("DryBag_Black", 30);
        changed |= EnsureDefault("WoodenCrate", 35);
        changed |= EnsureDefault("SeaChest", 40);
        changed |= EnsureDefault("barrel_green", 45);
        changed |= EnsureDefault("barrel_blue", 45);
        changed |= EnsureDefault("barrel_red", 45);
        changed |= EnsureDefault("barrel_yellow", 45);
        changed |= EnsureDefault("mediumtent", 65);
        changed |= EnsureDefault("civiltent", 80);
        changed |= EnsureDefault("partytent", 140);
        changed |= EnsureDefault("CarTent", 160);
        changed |= EnsureDefault("cartent", 160);
        changed |= EnsureDefault("Frontier_Workbench_Tools", 70);
        changed |= EnsureDefault("Frontier_Workbench_Press", 110);
        return changed;
    }

    protected bool EnsureDefault(string className, float value)
    {
        if (ContainsKeyCaseInsensitive(className))
            return false;

        powerConsumptionOfItemPerHour.Insert(className, value);
        return true;
    }
}
