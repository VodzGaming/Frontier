modded class ActionTargetsCursor
{
    override protected void CheckRefresherFlagVisibilityEx(ActionTarget target)
    {
        // Let base do its normal work first.
        super.CheckRefresherFlagVisibilityEx(target);

        Widget flagWidget = m_Root.FindAnyWidget("item_flag_icon");
        if (!flagWidget)
            return;

        // Default hide; we'll opt-in per target.
        flagWidget.Show(false);

        Object obj = target.GetObject();
        if (!obj)
            obj = target.GetParent();
        if (!obj)
            return;

        // Walk up to the topmost parent so we color the root entity (e.g., holograms/attachments).
        while (obj.GetParent())
            obj = obj.GetParent();

        // Workbench: keep powered/decay coloring.
        FB_Workbench bench = FB_Workbench.Cast(obj);
        if (bench)
        {
            int color = 0xFF64C864;
            if (bench.BaseLifeTimeLeft() <= 0)
                color = 0xFFD84C4C;

            flagWidget.SetColor(color);
            flagWidget.Show(true);
            return;
        }

        // Other flags (e.g., BBP/LB): show the flag icon using default color.
        if (obj.IsInherited(TerritoryFlag))
        {
            flagWidget.SetColor(0xFFFFFFFF);
            flagWidget.Show(true);
        }
    }
}
