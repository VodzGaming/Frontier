class FrontierWorkbenchTrigger : Trigger
{
    protected FB_Workbench m_Workbench;

    override void EOnInit(IEntity other, int extra)
    {
        SetCollisionSphere(0);
    }

    void SetWorkbench(FB_Workbench wb)
    {
        m_Workbench = wb;
    }

    override void OnEnterServerEvent(TriggerInsider insider)
    {
        if (!g_Game || !g_Game.IsServer())
            return;

        super.OnEnterServerEvent(insider);

        if (!m_Workbench)
            return;

        ItemBase item = ItemBase.Cast(insider.GetObject());
        if (!item)
            return;

        m_Workbench.RegisterTrackedEntity(item);
    }

    override void OnLeaveServerEvent(TriggerInsider insider)
    {
        if (!g_Game || !g_Game.IsServer())
            return;

        super.OnLeaveServerEvent(insider);

        if (!m_Workbench)
            return;

        ItemBase item = ItemBase.Cast(insider.GetObject());
        if (!item)
            return;

        m_Workbench.UnregisterTrackedEntity(item);
    }

    override protected bool CanAddObjectAsInsider(Object object)
    {
        if (!object || !object.IsInherited(ItemBase))
            return false;

        if (object == m_Workbench)
            return false;

        return GetInsiderForObject(object) == null;
    }
}
