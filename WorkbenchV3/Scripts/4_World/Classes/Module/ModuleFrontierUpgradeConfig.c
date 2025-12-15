class ModuleFrontierUpgradeConfig
{
    protected static ref ModuleFrontierUpgradeConfig s_Instance;
    protected ref FrontierUpgradeConfig m_Config;

    static ModuleFrontierUpgradeConfig Get()
    {
        if (!s_Instance)
            s_Instance = new ModuleFrontierUpgradeConfig();
        return s_Instance;
    }

    void ModuleFrontierUpgradeConfig()
    {
        LoadConfig();
    }

    protected string GetConfigDirectory()
    {
        return "$profile:/Frontier";
    }

    protected string GetConfigPath()
    {
        return string.Format("%1/WorkbenchUpgrades.json", GetConfigDirectory());
    }

    void LoadConfig()
    {
        if (!m_Config)
            m_Config = new FrontierUpgradeConfig();

        string directory = GetConfigDirectory();
        if (!FileExist(directory))
            MakeDirectory(directory);

        string path = GetConfigPath();
        bool changed = false;
        if (FileExist(path))
        {
            JsonFileLoader<FrontierUpgradeConfig>.JsonLoadFile(path, m_Config);
        }
        else
        {
            m_Config = FrontierUpgradeConfig.BuildDefaults();
            changed = true;
        }

        if (!m_Config)
        {
            m_Config = FrontierUpgradeConfig.BuildDefaults();
            changed = true;
        }

        if (m_Config.Normalize())
            changed = true;

        if (changed)
            SaveConfig();
    }

    void SaveConfig()
    {
        if (!m_Config)
            return;

        JsonFileLoader<FrontierUpgradeConfig>.JsonSaveFile(GetConfigPath(), m_Config);
    }

    FrontierUpgradeEffect GetEffectForType(string typeName)
    {
        if (!m_Config)
            LoadConfig();

        if (!m_Config || !m_Config.modules)
            return NULL;

        FrontierUpgradeEffect effect;
        if (m_Config.modules.Find(typeName, effect))
            return effect;

        return NULL;
    }

    bool IsTypeSupported(string typeName)
    {
        return GetEffectForType(typeName) != NULL;
    }

    int GetBaseTier()
    {
        if (!m_Config)
            LoadConfig();

        if (!m_Config)
            return 1;

        return Math.Max(m_Config.baseTier, 0);
    }

    int GetCraftUnlockTier()
    {
        if (!m_Config)
            LoadConfig();

        if (!m_Config)
            return 1;

        return Math.Max(m_Config.craftUnlockTier, 1);
    }
}
