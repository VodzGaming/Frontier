class FB_Workbench;

class ActionFBWorkbenchClearAccess : ActionInteractBase
{
    void ActionFBWorkbenchClearAccess()
    {
        m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
        m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
        m_HUDCursorIcon = CursorIcons.CloseHood;
        m_Text = "Unclaim / Clear Access";
    }

    override void CreateConditionComponents()
    {
        m_ConditionItem = new CCINone;
        m_ConditionTarget = new CCTNone;
    }

    override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
    {
        FB_Workbench bench = FB_Workbench.Cast(target.GetObject());
        if (!bench || !player)
            return false;

        return bench.HasOwner();
    }

    override void OnStartServer(ActionData action_data)
    {
        super.OnStartServer(action_data);

        FB_Workbench bench = FB_Workbench.Cast(action_data.m_Target.GetObject());
        PlayerBase player = action_data.m_Player;
        if (!bench || !player)
            return;

        if (!bench.IsOwner(player))
            return;

        bench.ClearAuthorizedPlayers();
    }

    override string GetText()
    {
        return "Unclaim / Clear Access";
    }
}
