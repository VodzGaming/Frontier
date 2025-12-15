class VirtualStorageManager {
    static ref VirtualStorageManager g_Instance;
    ref map<string, ref array<string>> m_StorageData;
    ref map<string, bool> m_StorageVisibility; // Tracks visibility state of each storage

    static VirtualStorageManager GetInstance() {
        if (!g_Instance) g_Instance = new VirtualStorageManager();
        return g_Instance;
    }

    void VirtualStorageManager() {
        m_StorageData = new map<string, ref array<string>>();
        m_StorageVisibility = new map<string, bool>();
    }

    void SaveStorage(string id, array<EntityAI> items) {
        array<string> itemTypes = new array<string>();
        foreach (EntityAI item : items) {
            itemTypes.Insert(item.GetType());
        }
        m_StorageData.Set(id, itemTypes);
        m_StorageVisibility.Set(id, true); // Default to visible when saved
    }

    array<string> LoadStorage(string id) {
        if (m_StorageData.Contains(id)) {
            return m_StorageData.Get(id);
        }
        return new array<string>();
    }

    void ToggleStorageVisibility(string id, bool isVisible) {
        if (m_StorageData.Contains(id)) {
            m_StorageVisibility.Set(id, isVisible);

            // Logic to hide or show proxies
            if (isVisible) {
                ShowProxies(id);
            } else {
                HideProxies(id);
            }
        }
    }

    private void HideProxies(string id) {
        // Logic to hide proxies for the given storage ID
        Print("[VirtualStorageManager] Hiding proxies for storage ID: " + id);
        // Add your proxy hiding implementation here
    }

    private void ShowProxies(string id) {
        // Logic to show proxies for the given storage ID
        Print("[VirtualStorageManager] Showing proxies for storage ID: " + id);
        // Add your proxy showing implementation here
    }
}