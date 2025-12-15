class ModuleFrontierAdminBypassConfig
{
    ref array<string> adminSteamIds;

    void ModuleFrontierAdminBypassConfig()
    {
        adminSteamIds = new array<string>();
    }

    protected void EnsureLists()
    {
        if (!adminSteamIds)
            adminSteamIds = new array<string>();
    }

    bool ApplySampleIfEmpty()
    {
        EnsureLists();
        if (adminSteamIds.Count() > 0)
            return false;

        // Example entry so server owners instantly see the expected format.
        adminSteamIds.Insert("76561198000000000");
        return true;
    }

    bool Sanitize()
    {
        EnsureLists();

        bool changed = false;
        for (int i = adminSteamIds.Count() - 1; i >= 0; i--)
        {
            string current = adminSteamIds[i];
            string trimmed = current;
            trimmed.Trim();

            if (trimmed != current)
                changed = true;

            if (!trimmed || trimmed == "")
            {
                adminSteamIds.RemoveOrdered(i);
                changed = true;
                continue;
            }

            adminSteamIds[i] = trimmed;
        }

        return changed;
    }

    bool Contains(string steamId)
    {
        if (!steamId || steamId == "")
            return false;

        EnsureLists();
        return adminSteamIds.Find(steamId) > -1;
    }
}
