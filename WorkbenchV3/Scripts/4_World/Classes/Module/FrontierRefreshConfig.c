class FrontierRefreshConfig
{
    int autoRefreshIntervalSeconds;
    int ceRefreshIntervalSeconds;

    void FrontierRefreshConfig()
    {
        autoRefreshIntervalSeconds = 600; // default ~10 minutes
        ceRefreshIntervalSeconds = 1800;  // default CE flag refresh if CE global is missing
    }

    bool ApplyDefaultsIfInvalid()
    {
        bool changed = false;

        if (autoRefreshIntervalSeconds <= 0)
        {
            autoRefreshIntervalSeconds = 600;
            changed = true;
        }

        if (ceRefreshIntervalSeconds < 0)
        {
            ceRefreshIntervalSeconds = 0; // 0 means use CE global
            changed = true;
        }

        return changed;
    }
}
