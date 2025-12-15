modded class PlayerBase
{
    void OpenFrontierWorkbench()
    {
        if (!g_CraftManager) g_CraftManager = new CraftManager();
        g_CraftManager.OpenCraft();
    }

    override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
    {
        super.OnRPC(sender, rpc_type, ctx);

        switch (rpc_type)
        {
            case FBRpc.SERVER_SEND_MAIN_DATA:
            {
                Param1<ref FB_Data> payload;
                if (!ctx.Read(payload))
                    return;

                if (!g_CraftManager)
                    g_CraftManager = new CraftManager();

                FB_ClientState.UpdateData(payload.param1);
                if (g_CraftManager)
                    g_CraftManager.OnWorkbenchDataReceived();
                break;
            }
            case FBRpc.SERVER_SEND_CRAFT_RESULT:
            {
                Param1<ref FBCraftResult> resultData;
                if (!ctx.Read(resultData))
                    return;

                if (!g_CraftManager)
                    g_CraftManager = new CraftManager();

                if (g_CraftManager)
                    g_CraftManager.OnCraftResultReceived(resultData.param1);
                break;
            }
            case FBRpc.SERVER_SEND_QUEUE_STATE:
            {
                Param1<ref FB_QueueState> queuePayload;
                if (!ctx.Read(queuePayload))
                    return;

                FB_ClientState.UpdateQueue(queuePayload.param1);

                if (!g_CraftManager)
                    g_CraftManager = new CraftManager();

                if (g_CraftManager)
                    g_CraftManager.OnQueueUpdate(queuePayload.param1);
                break;
            }
        }
    }
}