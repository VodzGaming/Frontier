	class FB_Workbench;

	class ActionFBWorkbenchOpen : ActionInteractBase
	{
		void ActionFBWorkbenchOpen()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
		m_HUDCursorIcon = CursorIcons.CloseHood;
		m_Text = "Open Workbench";
	}
	
	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTNone;
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		FB_Workbench bench = FB_Workbench.Cast(target.GetObject());
		return bench != NULL;
	}
	
	override void OnExecute( ActionData action_data )
	{
        super.OnExecute( action_data );
        Print("[Frontier][ActionFBWorkbenchOpen] OnExecute");

		if ( GetGame().IsClient() || !GetGame().IsMultiplayer() )
		{
			FB_Workbench ntarget = FB_Workbench.Cast( action_data.m_Target.GetObject() );
			if ( ntarget)
			{
				if (!ntarget.IsCraftingUnlocked() && action_data && action_data.m_Player)
					action_data.m_Player.MessageStatus(ntarget.GetCraftingLockMessage());

				if (!g_CraftManager)
					g_CraftManager = new CraftManager();
				g_CraftManager.SetHPC(ntarget);
				g_CraftManager.RequestCraft();
			}
		}
	}

	#ifdef SERVER
		override void OnExecuteServer(ActionData action_data)
		{
			super.OnExecuteServer(action_data);

			FB_Workbench bench = FB_Workbench.Cast(action_data.m_Target.GetObject());
			if (bench)
				bench.ForceStatusSync();
		}
	#endif

}
