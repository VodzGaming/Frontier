class SA_SoloLocker_Base : A6_Openable_Base {
	override void SetActions() {
        super.SetActions();
		#ifdef CodeLock
			AddAction(ActionInteractLockOnA6);
			AddAction(ActionManageLockOnA6);
			AddAction(ActionLockAdminOnA6);
		#endif
    }
    override bool CanBeDismantled() {
		return true;
	}
}
class SA_SoloLocker_Red extends SA_SoloLocker_Base {}
class SA_SoloLocker_RedRusty extends SA_SoloLocker_Base {}
class SA_SoloLocker_YellowRusty extends SA_SoloLocker_Base {}
class SA_SoloLocker_White extends SA_SoloLocker_Base {}
class SA_SoloLocker_Green extends SA_SoloLocker_Base {}
class SA_SoloLocker_Black extends SA_SoloLocker_Base {}