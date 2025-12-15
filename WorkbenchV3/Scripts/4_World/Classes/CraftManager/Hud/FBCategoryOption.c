class FBCategoryOption
{
    protected Widget m_Root;
    protected ButtonWidget m_Button;
    protected ImageWidget m_Icon;
    protected TextWidget m_Label;
    protected Widget m_Highlight;
    protected Widget m_HighlightOutline;
    protected Widget m_Base;
    protected string m_Category;
    protected static const string CATEGORY_ICON_DIR = "WorkbenchV3/UI/Icons";
    protected static const string CATEGORY_ICON_DIR_ALT = "UI/Icons";
    protected static const string CATEGORY_ICON_FALLBACK = "Menu.edds";

    Widget Init(Widget parent, string category)
    {
        m_Category = category;
        m_Root = GetGame().GetWorkspace().CreateWidgets("WorkbenchV3/UI/CategoryOption.layout", parent);
        m_Button = ButtonWidget.Cast(m_Root.FindAnyWidget("BtnCategoryOption"));
        if (!m_Button)
            m_Button = ButtonWidget.Cast(m_Root);
        m_Icon = ImageWidget.Cast(m_Root.FindAnyWidget("CategoryIcon"));
        m_Label = TextWidget.Cast(m_Root.FindAnyWidget("CategoryName"));
        m_Highlight = m_Root.FindAnyWidget("CategoryHighlight");
        m_HighlightOutline = m_Root.FindAnyWidget("CategoryHighlightOutline");
        m_Base = m_Root.FindAnyWidget("CategoryBase");

        LoadIcon(category);
        // Hide label entirely (icon-only rail)
        if (m_Label)
            m_Label.Show(false);

        SetSelected(false);
        return m_Root;
    }

    string GetCategory()
    {
        return m_Category;
    }

    void SetSelected(bool selected)
    {
        if (!m_Button)
            return;

        if (m_Button)
        {
            // keep button transparent so highlight layers remain visible
            m_Button.SetColor(ARGB(0, 0, 0, 0));
        }

        if (m_Label)
        {
            if (selected)
                m_Label.SetColor(ARGB(255, 250, 250, 230));
            else
                m_Label.SetColor(ARGB(220, 210, 210, 210));
        }

        if (m_Icon)
        {
            if (selected)
                m_Icon.SetColor(ARGB(255, 255, 255, 255));
            else
                m_Icon.SetColor(ARGB(220, 220, 220, 220));
        }

        if (m_Highlight)
            m_Highlight.Show(selected);

        if (m_HighlightOutline)
            m_HighlightOutline.Show(selected);

        if (m_Base)
        {
            if (selected)
                m_Base.SetColor(ARGB(255, 40, 70, 110));
            else
                m_Base.SetColor(ARGB(255, 15, 20, 30));
        }
    }

    bool OwnsWidget(Widget w)
    {
        if (!w)
            return false;

        Widget target = w;
        while (target)
        {
            if (target == m_Root)
                return true;
            target = target.GetParent();
        }
        return false;
    }

    protected void LoadIcon(string category)
    {
        if (!m_Icon)
            return;

        string iconName = GetIconForCategory(category);
        string iconLower = iconName;
        iconLower.ToLower();

        // Try a few path variants because some packing tools keep or strip the root folder.
        array<string> candidates = {
            iconName,                                           // raw filename in current folder
            string.Format("%1", iconName),                      // same, explicit format
            iconLower,
            string.Format("%1/%2", CATEGORY_ICON_DIR, iconName),
            string.Format("\\%1\\%2", CATEGORY_ICON_DIR, iconName),
            string.Format("%1/%2", CATEGORY_ICON_DIR_ALT, iconName),
            string.Format("\\%1\\%2", CATEGORY_ICON_DIR_ALT, iconName),
            string.Format("%1/%2", CATEGORY_ICON_DIR, iconLower),
            string.Format("\\%1\\%2", CATEGORY_ICON_DIR, iconLower),
            string.Format("%1/%2", CATEGORY_ICON_DIR_ALT, iconLower),
            string.Format("\\%1\\%2", CATEGORY_ICON_DIR_ALT, iconLower),
            string.Format("%1/%2", CATEGORY_ICON_DIR, CATEGORY_ICON_FALLBACK),
            string.Format("\\%1\\%2", CATEGORY_ICON_DIR, CATEGORY_ICON_FALLBACK),
            string.Format("%1/%2", CATEGORY_ICON_DIR_ALT, CATEGORY_ICON_FALLBACK),
            string.Format("\\%1\\%2", CATEGORY_ICON_DIR_ALT, CATEGORY_ICON_FALLBACK),
            string.Format("%1/%2", CATEGORY_ICON_DIR, "Weapons.edds"),
            string.Format("%1/%2", CATEGORY_ICON_DIR_ALT, "Weapons.edds")
        };

        bool loaded = false;
        for (int i = 0; i < candidates.Count(); i++)
        {
            string path = candidates[i];
            if (m_Icon.LoadImageFile(0, path))
            {
                loaded = true;
                Print(string.Format("[Frontier][CategoryOption] Loaded icon=%1 for category=%2", path, category));
                break;
            }
            else
            {
                Print(string.Format("[Frontier][CategoryOption] Failed to load icon=%1 for category=%2", path, category));
            }
        }
        if (!loaded)
            Print(string.Format("[Frontier][CategoryOption][WARN] No icon could be loaded for category=%1", category));
        else
            m_Icon.SetImage(0);
    }

    protected void UpdateLabel(string category)
    {
        if (!m_Label)
            return;

        string label = category;
        label = label.Trim();
        if (label == "")
            label = "General";
        m_Label.SetText(label);
    }

    protected string GetIconForCategory(string category)
    {
        string sanitized = category;
        sanitized = sanitized.Trim();
        if (sanitized == "")
            sanitized = "Defense";

        sanitized.Replace(" ", "");
        sanitized.Replace("-", "");
        sanitized.Replace("_", "");

        string key = sanitized;
        key.ToLower();

        switch (key)
        {
            case "ammo": return "Ammo.edds";
            case "attachments": return "Weapons.edds";
            case "clothing": return "Clothing.edds";
            case "medical": return "Medical.edds";
            case "storage": return "Storage.edds";
            case "survival": return "Survival.edds";
            case "weapon": return "Weapons.edds";
            case "weapons": return "Weapons.edds";
            case "weaponsupgrade": return "WeaponsUpgrade.edds";
            case "all": return "Menu.edds";
        }

        return sanitized + ".edds";
    }

    Widget GetRootWidget()
    {
        return m_Root;
    }
}
