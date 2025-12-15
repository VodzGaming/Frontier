class FBIngredientCard
{
    protected Widget m_Root;
    protected ItemPreviewWidget m_Preview;
    protected TextWidget m_CountText;
    protected TextWidget m_NameWidget;
    protected Widget m_TooltipPanel;
    protected EntityAI m_ItemPreview;
    protected string m_TypeName;
    protected bool m_HasEnough;

    Widget Init(Widget parent, string typeName, int haveCount, int needCount)
    {
        m_TypeName = typeName;
        m_Root = GetGame().GetWorkspace().CreateWidgets("WorkbenchV3/UI/OneItemForCraft.layout", parent);
        m_Preview = ItemPreviewWidget.Cast(m_Root.FindAnyWidget("CraftItemPreview"));
        m_CountText = TextWidget.Cast(m_Root.FindAnyWidget("Craftitemnedeed"));
        m_TooltipPanel = m_Root.FindAnyWidget("CraftItemPanel");
        m_NameWidget = TextWidget.Cast(m_Root.FindAnyWidget("CraftItemName"));

        if (m_CountText)
        {
            m_CountText.Show(false);
            m_CountText.SetText(" ");
        }

        if (m_TooltipPanel)
            m_TooltipPanel.Show(true);

        SetupPreview(typeName);
        UpdateCount(haveCount, needCount);
        return m_Root;
    }

    void SetupPreview(string typeName)
    {
        if (!m_Preview)
            return;

        Cleanup();

        // create a local preview entity; fallback to client-only flags if normal spawn fails
        m_ItemPreview = EntityAI.Cast(GetGame().CreateObject(typeName, vector.Zero, true));
        if (!m_ItemPreview)
        {
            m_ItemPreview = EntityAI.Cast(GetGame().CreateObjectEx(typeName, vector.Zero, ECE_LOCAL | ECE_NOLIFETIME | ECE_DYNAMIC_PERSISTENCY));
        }
        if (m_ItemPreview)
        {
            if (m_NameWidget)
                m_NameWidget.SetText(m_ItemPreview.GetDisplayName());
            m_Preview.SetItem(m_ItemPreview);
        }
        else if (m_NameWidget)
        {
            m_NameWidget.SetText(typeName);
        }
    }

    void UpdateCount(int haveCount, int needCount)
    {
        if (m_CountText)
        {
            string label = string.Format("%1/%2", haveCount, needCount);
            m_CountText.SetText(label);
        }

        if (m_NameWidget)
        {
            string displayName = m_TypeName;
            if (m_ItemPreview && m_ItemPreview.GetDisplayName() != "")
                displayName = m_ItemPreview.GetDisplayName();
            m_NameWidget.SetText(string.Format("%1 (%2/%3)", displayName, haveCount, needCount));
        }

        m_HasEnough = haveCount >= needCount;
        int color;
        if (m_HasEnough)
            color = ARGB(255, 120, 255, 120);
        else
            color = ARGB(255, 220, 64, 64);

        if (m_NameWidget)
            m_NameWidget.SetColor(color);
    }

    void Cleanup()
    {
        if (m_ItemPreview)
        {
            GetGame().ObjectDeleteOnClient(m_ItemPreview);
            m_ItemPreview = null;
        }
    }

    bool HasEnough()
    {
        return m_HasEnough;
    }

    void OnEnter()
    {
        // Optional hover highlight could be added here
    }

    void OnLeave()
    {
        // Optional hover highlight could be added here
    }

    Widget GetRoot()
    {
        return m_Root;
    }

    bool OwnsWidget(Widget w)
    {
        Widget target = w;
        while (target)
        {
            if (target == m_Root)
                return true;
            target = target.GetParent();
        }
        return false;
    }
}
