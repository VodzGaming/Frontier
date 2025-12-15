class ModuleFrontierAdminBypass
{
    protected static ref ModuleFrontierAdminBypass s_Instance;

    static ModuleFrontierAdminBypass Get()
    {
        if (!s_Instance)
            s_Instance = new ModuleFrontierAdminBypass();
        return s_Instance;
    }

    bool IsSteamIdBypassed(string steamId)
    {
        ModuleFrontierAdminBypassConfig cfg = ModuleFrontierConfigHub.Get().GetAdminConfig();
        if (!cfg)
            return false;
        return cfg.Contains(steamId);
    }

    ref array<string> GetBypassList()
    {
        ModuleFrontierAdminBypassConfig cfg = ModuleFrontierConfigHub.Get().GetAdminConfig();
        if (!cfg)
            return new array<string>();
        return cfg.adminSteamIds;
    }
}
