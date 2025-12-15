class FBResultCard
{
    protected Widget m_Root;
    protected ItemPreviewWidget m_Preview;
    protected TextWidget m_CountText;
    protected MultilineTextWidget m_NameWidget;
    protected Widget m_NamePanel;
    protected EntityAI m_ItemPreview;
    protected string m_BaseName;

    Widget Init(Widget parent, string typeName, int count)
    {
        m_Root = GetGame().GetWorkspace().CreateWidgets("WorkbenchV3/UI/OneResultItem.layout", parent);
        m_Preview = ItemPreviewWidget.Cast(m_Root.FindAnyWidget("CraftItemPreview"));
        m_CountText = TextWidget.Cast(m_Root.FindAnyWidget("ResultCountLabel"));
        m_NameWidget = MultilineTextWidget.Cast(m_Root.FindAnyWidget("CraftItemName"));
        m_NamePanel = m_Root.FindAnyWidget("CraftItemPanel");

        if (m_NamePanel)
            m_NamePanel.Show(true);

        if (m_CountText)
        {
            m_CountText.Show(true);
            m_CountText.SetText("");
        }

        SetupPreview(typeName);
        UpdateCount(count);
        return m_Root;
    }

    protected void SetupPreview(string typeName)
    {
        if (!m_Preview)
            return;

        CleanupPreview();

        if (!typeName || typeName == "")
            return;

        m_ItemPreview = EntityAI.Cast(GetGame().CreateObject(typeName, vector.Zero, true));
        if (m_ItemPreview)
        {
            m_Preview.SetItem(m_ItemPreview);
            if (m_NameWidget)
            {
                m_BaseName = m_ItemPreview.GetDisplayName();
                m_NameWidget.SetText(m_BaseName);
            }
        }
        else if (m_NameWidget)
        {
            m_BaseName = typeName;
            m_NameWidget.SetText(m_BaseName);
        }
    }

    protected void UpdateCount(int count)
    {
        int safeCount = Math.Max(1, count);

        if (m_NameWidget)
        {
            string baseText = m_BaseName;
            if (baseText == "")
                baseText = "Result";
            m_NameWidget.SetText(baseText);
        }

        if (m_CountText)
        {
            m_CountText.Show(true);
            m_CountText.SetText(string.Format("x%1", safeCount));
        }
    }

    void Cleanup()
    {
        CleanupPreview();
    }

    protected void CleanupPreview()
    {
        if (m_ItemPreview)
        {
            GetGame().ObjectDeleteOnClient(m_ItemPreview);
            m_ItemPreview = null;
        }
    }
}
