class FB_Workbench;

class ActionDismantleWorkbenchCB : ActionContinuousBaseCB
{
    override void CreateActionComponent()
    {
        m_ActionData.m_ActionComponent = new CAContinuousTime(5);
    }
}

class ActionDismantleWorkbench: ActionContinuousBase
{
	protected const string REQUIRED_TOOL = "Screwdriver";

	void ActionDismantleWorkbench()
	{
		m_CallbackClass 	= ActionDismantleWorkbenchCB;
		m_SpecialtyWeight 	= UASoftSkillsWeight.PRECISE_LOW;
		m_CommandUID 		= DayZPlayerConstants.CMD_ACTIONFB_INTERACT;
		m_FullBody			= true;
		m_StanceMask 		= DayZPlayerConstants.STANCEMASK_CROUCH;
	}
	
	override void CreateConditionComponents()
	{
        m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNonRuined( UAMaxDistances.DEFAULT );
	}

	override string GetText()
	{
		return "Dismantle Workbench";
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		if (!player || !item || item.GetType() != REQUIRED_TOOL)
			return false;

		FB_Workbench workbench = FB_Workbench.Cast(target.GetObject());
		if (!workbench)
			return false;

		return workbench.IsEmpty();
	}

	override void OnFinishProgressServer(ActionData action_data)
	{		
        if (!action_data)
            return;

        PlayerBase player = action_data.m_Player;
        if (!player)
            return;

        FB_Workbench workbench = FB_Workbench.Cast(action_data.m_Target.GetObject());
        if (!workbench)
            return;

        ItemBase kit = ItemBase.Cast(player.GetInventory().CreateInInventory("FB_Workbench_Kit"));
        if (!kit)
        {
            player.MessageStatus("You need a free 1x4 slot to pack this workbench.");
            return;
        }

        ItemBase tool = action_data.m_MainItem;
        if (tool)
            tool.AddHealth(-tool.GetMaxHealth() / 3);

        workbench.Delete();
	}
};
