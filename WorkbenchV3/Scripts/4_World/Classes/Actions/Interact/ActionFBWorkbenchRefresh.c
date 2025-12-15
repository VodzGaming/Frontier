class FB_Workbench;

class ActionFBWorkbenchRefresh : ActionInteractBase
{
	void ActionFBWorkbenchRefresh()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
		m_HUDCursorIcon = CursorIcons.CloseHood;
		m_Text = "Refresh";
	}
	
	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTNone;
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		FB_Workbench ntarget = FB_Workbench.Cast(target.GetObject());
		if (!ntarget)
			return false;

		return true;
	}

	override void OnExecuteServer( ActionData action_data )
	{
		Object target_object = action_data.m_Target.GetObject();
		FB_Workbench ntarget = FB_Workbench.Cast( target_object );
		
		if( ntarget )
			ntarget.UpdateRefresherTimeRemaining();
	}
};
