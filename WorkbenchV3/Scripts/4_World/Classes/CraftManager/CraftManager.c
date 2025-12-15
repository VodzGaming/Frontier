class FB_Workbench;
class CraftHud;

class CraftManager
{
    FB_Workbench m_WB;
    ref CraftHud  m_CraftHud;

    void CraftManager() {}

    // Primary setter for the current workbench reference
    void SetWB(FB_Workbench wb) { m_WB = wb; }

    // Legacy alias so legacy action code that calls SetHPC keeps compiling
    void SetHPC(Object obj)
    {
        m_WB = FB_Workbench.Cast(obj);
    }

    FB_Workbench GetWB() { return m_WB; }
    FB_Workbench GetHPC() { return m_WB; }  // optional alias

    bool HasWorkbench()
    {
        return m_WB != NULL;
    }

    void OnKeyPress(int key)
    {
        switch (key)
        {
            case KeyCode.KC_ESCAPE:
                CloseCraft();
                break;
        }
    }

    void OpenCraft()
    {
        if (!m_CraftHud)
            m_CraftHud = new CraftHud(this);

        GetGame().GetUIManager().ShowScriptedMenu(m_CraftHud, NULL);
        if (m_CraftHud)
            m_CraftHud.RefreshData();
    }

    void CloseCraft()
    {
        if (m_CraftHud && GetGame().GetUIManager().GetMenu() == m_CraftHud)
        {
            GetGame().GetUIManager().Back();
        }

        if (m_CraftHud)
        {
            GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(ResetHud, 0, false);
        }
    }

    protected void ResetHud()
    {
        m_CraftHud = null;
    }

    void RequestCraft()
    {
        if (!m_WB)
            return;

        if (GetGame().IsServer() || !GetGame().IsMultiplayer())
        {
            FB_ClientState.Ensure();
            OpenCraft();
            return;
        }

        GetGame().RPCSingleParam(m_WB, FBRpc.CLIENT_REQUEST_WORKBENCH_DATA, NULL, true);
    }

    void RequestCraft(CraftItem item)
    {
        if (!item || item.RecipeId == "")
            return;

        if (!m_WB)
            return;

        if (GetGame().IsServer() || !GetGame().IsMultiplayer())
        {
#ifdef SERVER
            ExecuteCraft(item.RecipeId);
#endif
            return;
        }

        Param1<string> payload = new Param1<string>(item.RecipeId);
        GetGame().RPCSingleParam(m_WB, FBRpc.CLIENT_REQUEST_CRAFT_RECIPE, payload, true);
    }

    void OnWorkbenchDataReceived()
    {
        Print("[Frontier][CraftManager] OnWorkbenchDataReceived");
        if (!HasWorkbench())
            return;

        OpenCraft();
        if (m_CraftHud)
            m_CraftHud.RefreshData();
    }

    void OnCraftResultReceived(FBCraftResult result)
    {
        if (!result)
            return;

        PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
        if (player && result.Message != "")
        {
            if (result.Success)
                player.MessageStatus(result.Message);
            else
                player.MessageImportant(result.Message);
        }
    }

    void OnQueueUpdate(FB_QueueState state)
    {
        if (m_CraftHud)
            m_CraftHud.OnQueueUpdate(state);
    }

#ifdef SERVER
    protected void ExecuteCraft(string recipeId)
    {
        if (!m_WB)
            return;

        if (g_CraftServerManager)
        {
            string resultMessage;
            PlayerBase crafter = PlayerBase.Cast(GetGame().GetPlayer());
            bool success = g_CraftServerManager.TryCraft(crafter, m_WB, recipeId, resultMessage);
            if (!success)
            {
                FBCraftResult fail = new FBCraftResult();
                fail.Success = false;
                fail.Message = resultMessage;
                OnCraftResultReceived(fail);
            }
            else
            {
                FBCraftResult ok = new FBCraftResult();
                ok.Success = true;
                ok.Message = resultMessage;
                OnCraftResultReceived(ok);
            }
        }
    }
#endif
}
ref CraftManager g_CraftManager;  // make sure this global exists
