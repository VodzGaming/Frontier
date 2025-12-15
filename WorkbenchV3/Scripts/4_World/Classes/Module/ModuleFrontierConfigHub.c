class ModuleFrontierConfigHub
{
    protected static ref ModuleFrontierConfigHub s_Instance;
    protected ref FrontierWorkbenchConfigEnvelope m_Config;
    protected ref ModuleFrontierAdminBypassConfig m_AdminConfig;
    protected ref FB_RecipeDB m_CraftingDB;

    static ModuleFrontierConfigHub Get()
    {
        if (!s_Instance)
            s_Instance = new ModuleFrontierConfigHub();
        return s_Instance;
    }

    void ModuleFrontierConfigHub()
    {
        LoadConfig();
    }

    protected string GetConfigDirectory()
    {
        return "$profile:/Frontier";
    }

    protected string GetConfigPath()
    {
        return string.Format("%1/WorkbenchPowerUsage.json", GetConfigDirectory());
    }

    protected string GetAdminPath()
    {
        return string.Format("%1/WorkbenchAdmins.json", GetConfigDirectory());
    }

    protected string GetLegacyPowerPath()
    {
        return string.Format("%1/WorkbenchPower.json", GetConfigDirectory());
    }

    protected string GetLegacyAdminPath()
    {
        return string.Format("%1/WorkbenchAdminBypass.json", GetConfigDirectory());
    }

    protected string GetLegacyRecipePath()
    {
        return string.Format("%1/WorkbenchRecipes.json", GetConfigDirectory());
    }

    void LoadConfig()
    {
        if (!m_Config)
            m_Config = new FrontierWorkbenchConfigEnvelope();
        if (!m_AdminConfig)
            m_AdminConfig = new ModuleFrontierAdminBypassConfig();

        bool changed = false;
        string directory = GetConfigDirectory();
        if (!FileExist(directory))
            MakeDirectory(directory);

        string configPath = GetConfigPath();
        if (FileExist(configPath))
        {
            JsonFileLoader<FrontierWorkbenchConfigEnvelope>.JsonLoadFile(configPath, m_Config);
        }
        else
        {
            LoadLegacySections();
            changed = true;
        }

        string recipeRoot = GetRecipeCategoryRoot();
        MakeDirectory(recipeRoot);
        m_CraftingDB = FB_RecipeDB.Load(recipeRoot, GetLegacyRecipePath());
        if (m_CraftingDB)
        {
            Print(string.Format("[Frontier][ConfigHub] Crafting DB loaded: %1 recipes from %2", m_CraftingDB.Recipes.Count(), recipeRoot));
            m_CraftingDB.SaveCategoryFiles(recipeRoot);
        }
        else
            Print("[Frontier][ConfigHub][WARN] Crafting DB failed to load");

        if (!m_Config.power)
        {
            m_Config.power = new ModuleFrontierPowerConfig();
            changed = true;
        }
        m_Config.power.TypesToLower();
        if (m_Config.power.ApplyFrontierDefaults())
            changed = true;

        if (!m_Config.refresh)
        {
            m_Config.refresh = new FrontierRefreshConfig();
            changed = true;
        }
        if (m_Config.refresh.ApplyDefaultsIfInvalid())
            changed = true;

        bool adminChanged = false;
        string adminPath = GetAdminPath();
        if (FileExist(adminPath))
        {
            JsonFileLoader<ModuleFrontierAdminBypassConfig>.JsonLoadFile(adminPath, m_AdminConfig);
        }
        else
        {
            LoadLegacyAdmin();
            adminChanged = true;
        }

        if (!m_AdminConfig)
        {
            m_AdminConfig = new ModuleFrontierAdminBypassConfig();
            adminChanged = true;
        }
        if (m_AdminConfig.Sanitize())
            adminChanged = true;
        if (m_AdminConfig.ApplySampleIfEmpty())
            adminChanged = true;

        if (changed)
            SaveConfig();
        if (adminChanged)
            SaveAdminConfig();
    }

    protected void LoadLegacySections()
    {
        string powerPath = GetLegacyPowerPath();
        if (FileExist(powerPath))
            JsonFileLoader<ModuleFrontierPowerConfig>.JsonLoadFile(powerPath, m_Config.power);

        LoadLegacyAdmin();

        // Removed loading crafting data from legacy path
    }

    protected void LoadLegacyAdmin()
    {
        string adminPath = GetLegacyAdminPath();
        if (FileExist(adminPath))
            JsonFileLoader<ModuleFrontierAdminBypassConfig>.JsonLoadFile(adminPath, m_AdminConfig);
    }

    void SaveConfig()
    {
        if (!m_Config)
            return;

        FrontierWorkbenchConfigEnvelope ordered = new FrontierWorkbenchConfigEnvelope();
        ordered.power = m_Config.power;
        ordered.refresh = m_Config.refresh;

        JsonFileLoader<FrontierWorkbenchConfigEnvelope>.JsonSaveFile(GetConfigPath(), ordered);
    }

    void SaveAdminConfig()
    {
        if (!m_AdminConfig)
            return;

        JsonFileLoader<ModuleFrontierAdminBypassConfig>.JsonSaveFile(GetAdminPath(), m_AdminConfig);
    }

    float GetTerritoryRadius()
    {
        ModuleFrontierPlacementConfig placement = ModuleFrontierPlacementConfig.Get();
        FrontierPlacementConfig cfg = placement.GetConfig();
        if (cfg && cfg.territoryRadius > 0)
            return cfg.territoryRadius;
        return GameConstants.REFRESHER_RADIUS;
    }

    ModuleFrontierPowerConfig GetPowerConfig()
    {
        if (!m_Config)
            LoadConfig();
        return m_Config.power;
    }

    FrontierRefreshConfig GetRefreshConfig()
    {
        if (!m_Config)
            LoadConfig();
        return m_Config.refresh;
    }

    ModuleFrontierAdminBypassConfig GetAdminConfig()
    {
        if (!m_AdminConfig)
            LoadConfig();
        return m_AdminConfig;
    }

    FB_RecipeDB GetRecipeDatabase()
    {
        if (!m_CraftingDB)
            LoadConfig();
        return m_CraftingDB;
    }

    protected string GetRecipeCategoryRoot()
    {
        string fixedPath = string.Format("%1/Recipes", GetConfigDirectory());
        return NormalizeRecipeRootPath(fixedPath);
    }

    protected string NormalizeRecipeRootPath(string path)
    {
        string normalized = path;
        normalized.Replace("\\", "/");
        if (normalized == "")
            return normalized;

        if (normalized.Length() > 0)
        {
            string last = normalized.Substring(normalized.Length() - 1, 1);
            if (last != "/")
                normalized = normalized + "/";
        }
        return normalized;
    }

    FrontierWorkbenchConfigEnvelope GetRawEnvelope()
    {
        if (!m_Config)
            LoadConfig();
        return m_Config;
    }

    int GetBaseQueueLimit()
    {
        // Base queue limit is now fixed; additional slots come strictly from upgrades.
        return 1;
    }
}
