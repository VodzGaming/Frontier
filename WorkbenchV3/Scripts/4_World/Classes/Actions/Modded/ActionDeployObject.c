modded class ActionDeployObject : ActionDeployBase {
	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item) {	
		if (!player||!item) return false;
		if (item.IsInherited(BaseBuildingBase) && !item.CanFindFBWorkBench()) return false;
		return super.ActionCondition(player, target, item);
	}
}
