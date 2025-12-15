class ModuleFrontierPlacementConfig
{
    protected static ref ModuleFrontierPlacementConfig s_Instance;
    protected ref FrontierPlacementConfig m_Config;

    static ModuleFrontierPlacementConfig Get()
    {
        if (!s_Instance)
            s_Instance = new ModuleFrontierPlacementConfig();
        return s_Instance;
    }

    void ModuleFrontierPlacementConfig()
    {
        LoadConfig();
    }

    protected string GetConfigDirectory()
    {
        return "$profile:/Frontier";
    }

    protected string GetConfigPath()
    {
        return string.Format("%1/WorkbenchPlacement.json", GetConfigDirectory());
    }

    void LoadConfig()
    {
        if (!m_Config)
            m_Config = new FrontierPlacementConfig();

        string directory = GetConfigDirectory();
        if (!FileExist(directory))
            MakeDirectory(directory);

        string path = GetConfigPath();
        bool changed = false;
        if (FileExist(path))
        {
            JsonFileLoader<FrontierPlacementConfig>.JsonLoadFile(path, m_Config);
        }
        else
        {
            m_Config = FrontierPlacementConfig.BuildDefaults();
            changed = true;
        }

        if (!m_Config)
        {
            m_Config = FrontierPlacementConfig.BuildDefaults();
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

        JsonFileLoader<FrontierPlacementConfig>.JsonSaveFile(GetConfigPath(), m_Config);
    }

    FrontierPlacementConfig GetConfig()
    {
        if (!m_Config)
            LoadConfig();
        return m_Config;
    }
}
