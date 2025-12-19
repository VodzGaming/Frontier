modded class ActionTogglePlaceObject : ActionSingleUseBase {
	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item) {	
		if (!super.ActionCondition(player, target, item)) return false;
		if (item.IsInherited(DeployableContainer_Base)) return true;
		return item.CanFindFBWorkBench();
	}
}
