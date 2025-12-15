modded class MissionBase
{
    override void OnInit()
    {
        super.OnInit();

        if (GetGame().IsServer() || !GetGame().IsMultiplayer())
        {
            // Ensure a profiles dir and load your data
            MakeDirectory("$profile:/Frontier");
            // If your CraftServerManager.LoadData() already handles path creation, you can use your no-arg version.
            // Otherwise, pass a path:
            // g_CraftServerManager = CraftServerManager.LoadData("$profile:/Frontier/Workbench.json");
            ModuleFrontierConfigHub.Get();
            ModuleFrontierPlacementConfig.Get();
            ModuleFrontierUpgradeConfig.Get();
            g_CraftServerManager = CraftServerManager.LoadData();
            ModuleFrontierWorkbenchPower.Get();
            ModuleFrontierAdminBypass.Get();
        }
    }
}
