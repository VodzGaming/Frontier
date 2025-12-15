class FBRecipeCard
{
    protected Widget m_Root;
    protected ItemPreviewWidget m_Preview;
    protected EntityAI m_PreviewEntity;
    protected string m_PreviewType;
    protected ButtonWidget m_Button;
    protected TextWidget m_Label;
    protected TextWidget m_SubLabel;
    protected TextWidget m_TimeLabel;
    protected TextWidget m_HaveNeedLabel;
    protected FB_Recipe m_Data;
    protected static ref map<string, EntityAI> s_PreviewCache = new map<string, EntityAI>();
    protected static const string RECIPE_ICON_DIR = "WorkbenchV3/UI/Icons";
    protected static const string RECIPE_ICON_FALLBACK = "WorkbenchV3/UI/Icons/Weapon.edds";

    Widget Init(Widget parent, FB_Recipe recipe)
    {
        m_Data = recipe;
        m_Root = GetGame().GetWorkspace().CreateWidgets("WorkbenchV3/UI/Recipe_card.layout", parent);
        m_Button = ButtonWidget.Cast(m_Root.FindAnyWidget("CardButton"));
        m_Preview = ItemPreviewWidget.Cast(m_Root.FindAnyWidget("CardPreview"));
        m_Label = TextWidget.Cast(m_Root.FindAnyWidget("CardName"));
        m_SubLabel = TextWidget.Cast(m_Root.FindAnyWidget("CardDesc"));
        m_TimeLabel = TextWidget.Cast(m_Root.FindAnyWidget("CardTime"));
        m_HaveNeedLabel = TextWidget.Cast(m_Root.FindAnyWidget("CardHaveNeed"));

        if (m_Label)
            m_Label.SetText(recipe.Name);

        if (m_SubLabel)
            m_SubLabel.SetText(recipe.Category);

        if (m_TimeLabel)
        {
            string timeLabel = "Craft: --";
            if (recipe.CraftTime > 0)
                timeLabel = string.Format("Craft: %1s", recipe.CraftTime.ToString());
            m_TimeLabel.SetText(timeLabel);
        }

        if (m_HaveNeedLabel)
        {
            m_HaveNeedLabel.SetText("");
            m_HaveNeedLabel.Show(false); // hide legacy "Needs" label
        }

        BuildPreview(recipe);
        return m_Root;
    }

    protected void BuildPreview(FB_Recipe recipe)
    {
        if (!m_Preview || !recipe)
            return;

        string typeName = recipe.Result;
        if (recipe.Results && recipe.Results.Count() > 0)
        {
            FB_ResultItem primary = recipe.Results[0];
            if (primary)
                typeName = primary.GetSpawnType();
        }

        if (typeName == "")
            typeName = recipe.Result;
        if (typeName == "")
            return;

        m_PreviewType = typeName;

        // Try cache first
        if (s_PreviewCache.Find(typeName, m_PreviewEntity) && m_PreviewEntity)
        {
            m_Preview.SetItem(m_PreviewEntity);
            return;
        }

        // Clean up any previous preview entity
        CleanupPreview();

        EntityAI spawned = EntityAI.Cast(GetGame().CreateObjectEx(typeName, vector.Zero, ECE_LOCAL | ECE_NOLIFETIME | ECE_DYNAMIC_PERSISTENCY));
        if (!spawned)
            return;

        // Cache for reuse across cards
        s_PreviewCache.Insert(typeName, spawned);
        m_PreviewEntity = spawned;
        m_Preview.SetItem(m_PreviewEntity);
    }

    bool IsRecipeControl(Widget w)
    {
        Widget target = w;
        while (target)
        {
            if (target == m_Button || target == m_Root || target == m_Preview)
                return true;
            target = target.GetParent();
        }
        return false;
    }

    FB_Recipe GetRecipe()
    {
        return m_Data;
    }

    Widget GetRoot()
    {
        return m_Root;
    }

    void Cleanup()
    {
        CleanupPreview();
    }

    protected void CleanupPreview()
    {
        if (m_Preview)
            m_Preview.SetItem(NULL);
        if (m_PreviewEntity)
        {
            // If this entity is the cached instance for this type, keep it alive for reuse.
            EntityAI cached;
            if (!m_PreviewType || !s_PreviewCache.Find(m_PreviewType, cached) || cached != m_PreviewEntity)
            {
                GetGame().ObjectDelete(m_PreviewEntity);
            }
            m_PreviewEntity = null;
        }
        m_PreviewType = "";
    }
}
