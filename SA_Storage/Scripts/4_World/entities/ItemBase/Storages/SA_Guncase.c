class SA_Guncase_Base : A6_Openable_Base {
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
class SA_Guncase extends SA_Guncase_Base {}