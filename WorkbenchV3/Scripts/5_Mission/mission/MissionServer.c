modded class MissionServer
{
    void InvokeOnConnect(PlayerBase player, PlayerIdentity identity)
    {
        super.InvokeOnConnect(player, identity);
            if (player && g_CraftServerManager)
        {
            g_CraftServerManager.SendWorkbenchData(player);
        }
    }
}
