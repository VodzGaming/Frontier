modded class ActionConstructor
{
    override void RegisterActions(TTypenameArray actions)
    {
        super.RegisterActions(actions);

        actions.Insert(ActionDismantleWorkbench);
		//actions.Insert(ActionInteractWithTable);
        actions.Insert(ActionFBWorkbenchOpen);
        actions.Insert(ActionFBWorkbenchClaim);
        actions.Insert(ActionFBWorkbenchClearAccess);
    }
};
