class ModuleFrontierWorkbenchPower
{
    protected static ref ModuleFrontierWorkbenchPower s_Instance;

    static ModuleFrontierWorkbenchPower Get()
    {
        if (!s_Instance)
            s_Instance = new ModuleFrontierWorkbenchPower();
        return s_Instance;
    }

    float GetItemPowerConsumptionPerHour(string itemType)
    {
        ModuleFrontierConfigHub hub = ModuleFrontierConfigHub.Get();
        ModuleFrontierPowerConfig cfg = hub.GetPowerConfig();
        if (!cfg)
            return 0.0;
        return cfg.GetItemPowerConsumptionPerHour(itemType);
    }

    ModuleFrontierPowerConfig GetRawConfig()
    {
        return ModuleFrontierConfigHub.Get().GetPowerConfig();
    }
}
