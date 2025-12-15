class CraftHud: UIScriptedMenu
{
    protected Widget m_Root;
    protected Widget m_RecipePanel;
    protected Widget m_RecipeDetailHost;
    protected Widget m_RecipeDetailPanel;
    protected WrapSpacerWidget m_RecipeWrap;
    protected ScrollWidget m_IngredientScroll;
    protected WrapSpacerWidget m_IngredientWrap;
    protected WrapSpacerWidget m_ResultWrap;
    protected WrapSpacerWidget m_QueueWrap;
    protected WrapSpacerWidget m_CategoryIconWrap;
    protected TextWidget m_CategoryLabel;
    protected EditBoxWidget m_RecipeSearchInput;
    protected ButtonWidget m_RecipeSearchButton;
    protected TextWidget m_ItemName;
    protected TextWidget m_CraftInfo;
    protected ItemPreviewWidget m_ItemPreview;
    protected MultilineTextWidget m_AttachmentInfo;
    protected TextWidget m_QueueHeader;
    protected ProgressBarWidget m_Progress;
    protected float m_ActiveTotalTime;
    protected float m_ActiveRemainingTime;
    protected bool m_ProgressActive;
    protected string m_QueueHeaderBase;
    protected ButtonWidget m_BtnCraft;
    protected ButtonWidget m_BtnCraftDetail;
    protected ButtonWidget m_BtnClose;
    protected Widget m_LockPanel;
    protected TextWidget m_LockTitle;
    protected TextWidget m_LockHint;
    protected TextWidget m_TierStatus;
    protected Widget m_ProgressPanel;
    protected ref CraftManager m_Manager;
    protected ref array<ref FBRecipeCard> m_RecipeCards;
    protected ref array<ref FBIngredientCard> m_IngredientCards;
    protected ref array<ref FBResultCard> m_ResultCards;
    protected ref array<ref FBCraftQueueCard> m_QueueCards;
    protected ref array<ref FBCategoryOption> m_CategoryOptions;
    protected ref array<string> m_Categories;
    protected ref map<string, ref array<ref FB_Recipe>> m_CategoryRecipes;
    protected FB_Recipe m_SelectedRecipe;
    protected EntityAI m_PreviewEntity;
    protected string m_SelectedRecipeId;
    protected int m_SelectedCategoryIndex;
    protected bool m_CraftingUnlocked;
    protected int m_CurrentQueueLimit;
    protected const string DEFAULT_CATEGORY = "General";
    protected const int MAX_RESULT_SLOTS = 4;
    protected const int MAX_INGREDIENT_SLOTS = 8;
    protected const int MAX_CATEGORY_BUTTONS = 7;
    void RefreshData()
    {
        PopulateRecipes();
        UpdateQueueDisplay();
    }

    void CraftHud(CraftManager mgr)
    {
        m_Manager = mgr;
        m_RecipeCards = new array<ref FBRecipeCard>();
        m_IngredientCards = new array<ref FBIngredientCard>();
        m_ResultCards = new array<ref FBResultCard>();
        m_QueueCards = new array<ref FBCraftQueueCard>();
        m_CategoryOptions = new array<ref FBCategoryOption>();
        m_Categories = new array<string>();
        m_CategoryRecipes = new map<string, ref array<ref FB_Recipe>>();
        m_SelectedCategoryIndex = 0;
        m_CraftingUnlocked = false;
        m_CurrentQueueLimit = 1;
        DisablePlayerControls();
        Init();
    }

    void ~CraftHud()
    {
        RestorePlayerControls();
        ClearIngredientCards();
        ClearResultCards();
        ClearQueueCards();
        ClearCategoryOptions();
    }

    override Widget Init()
    {
        Print("[Frontier][CraftHud] Init - start");
        if (!m_Root)
        {
            string layoutPath = "WorkbenchV3/UI/UniversalWorkbench.layout";
            layoutRoot = GetGame().GetWorkspace().CreateWidgets(layoutPath);
            if (!layoutRoot)
            {
                Print("[Frontier][CraftHud] Failed to create layout " + layoutPath);
                return NULL;
            }
            Print("[Frontier][CraftHud] Layout created");
            m_Root = layoutRoot;
            // Downscale HUD on lower resolutions (baseline 4K). At 1080p this lands near 0.5x.
            FBHudScaling.ApplyTo(m_Root, 3840.0, 2160.0, 0.5, 1.0);
            m_RecipePanel = m_Root.FindAnyWidget("PanelWrapperAllRecipes");

            m_RecipeWrap = WrapSpacerWidget.Cast(m_Root.FindAnyWidget("WrapSpacerWidgetRecipes"));
            m_QueueWrap = WrapSpacerWidget.Cast(m_Root.FindAnyWidget("WrapSpacerQueue"));
            m_CategoryIconWrap = WrapSpacerWidget.Cast(m_Root.FindAnyWidget("CategoryIconWrap"));
            m_CategoryLabel = TextWidget.Cast(m_Root.FindAnyWidget("CategoryLabel"));
            m_RecipeSearchInput = EditBoxWidget.Cast(m_Root.FindAnyWidget("RecipeSearchInput"));
            m_RecipeSearchButton = ButtonWidget.Cast(m_Root.FindAnyWidget("SearchSubmitButton"));
            m_QueueHeader = TextWidget.Cast(m_Root.FindAnyWidget("QueueHeader"));
            m_BtnClose = ButtonWidget.Cast(m_Root.FindAnyWidget("BtnClose"));
            m_BtnCraft = ButtonWidget.Cast(m_Root.FindAnyWidget("PrimaryCraftButton"));
            m_LockPanel = m_Root.FindAnyWidget("WorkbenchLockedPanel");
            m_LockTitle = TextWidget.Cast(m_Root.FindAnyWidget("LockedTitle"));
            m_LockHint = MultilineTextWidget.Cast(m_Root.FindAnyWidget("LockedHint"));
            m_TierStatus = TextWidget.Cast(m_Root.FindAnyWidget("TierStatusLabel"));
            m_RecipeDetailHost = m_Root.FindAnyWidget("RecipeDetailHost");
            m_ProgressPanel = m_Root.FindAnyWidget("ProgressPanel");
            if (m_ProgressPanel)
                m_ProgressPanel.Show(false);
            EnsureRecipeDetailPanel();
            Print("[Frontier][CraftHud] Widgets wired");

            if (m_BtnCraft)
                m_BtnCraft.Enable(false);
            if (m_BtnCraftDetail)
                m_BtnCraftDetail.Enable(false);

            PopulateRecipes();
            UpdateQueueDisplay();
            Print("[Frontier][CraftHud] Init - finished");
        }

        return m_Root;
    }


    protected void EnsureRecipeDetailPanel()
    {
        if (!m_RecipeDetailHost || m_RecipeDetailPanel)
            return;

        m_RecipeDetailPanel = GetGame().GetWorkspace().CreateWidgets("WorkbenchV3/UI/UniversalRecipeDetail.layout", m_RecipeDetailHost);
        if (!m_RecipeDetailPanel)
            return;

        m_ItemName = TextWidget.Cast(m_RecipeDetailPanel.FindAnyWidget("DetailRecipeName"));
        m_CraftInfo = TextWidget.Cast(m_RecipeDetailPanel.FindAnyWidget("DetailCraftInfo"));
        m_ItemPreview = ItemPreviewWidget.Cast(m_RecipeDetailPanel.FindAnyWidget("DetailPreviewWidget"));
        m_AttachmentInfo = MultilineTextWidget.Cast(m_RecipeDetailPanel.FindAnyWidget("DetailAttachmentInfo"));
        m_ResultWrap = WrapSpacerWidget.Cast(m_RecipeDetailPanel.FindAnyWidget("DetailResultWrap"));
        m_IngredientScroll = ScrollWidget.Cast(m_RecipeDetailPanel.FindAnyWidget("DetailIngredientScroll"));
        m_IngredientWrap = WrapSpacerWidget.Cast(m_RecipeDetailPanel.FindAnyWidget("DetailIngredientWrap"));
        m_BtnCraftDetail = ButtonWidget.Cast(m_RecipeDetailPanel.FindAnyWidget("DetailBtnCraft"));
        m_Progress = ProgressBarWidget.Cast(m_RecipeDetailPanel.FindAnyWidget("DetailProgress"));

        if (m_RecipeDetailHost)
        {
            m_RecipeDetailHost.Show(false);
            m_RecipeDetailHost.SetSort(1000);
        }

        if (m_BtnCraftDetail)
            m_BtnCraftDetail.Enable(false);
    }

    protected void ShowRecipeDetailPanel()
    {
        if (m_RecipeDetailHost)
            m_RecipeDetailHost.Show(true);
    }

    protected void HideRecipeDetailPanel()
    {
        if (m_RecipeDetailHost)
            m_RecipeDetailHost.Show(false);
    }

    override bool OnClick(Widget w, int x, int y, int button)
    {
        ButtonWidget buttonWidget = ButtonWidget.Cast(w);

        if (buttonWidget)
        {
            if (w == m_BtnCraft || w == m_BtnCraftDetail)
            {
                TryCraftSelected();
                return true;
            }
            if (w == m_BtnClose)
            {
                m_Manager.CloseCraft();
                return true;
            }
            if (w == m_RecipeSearchButton)
            {
                RefreshSearch();
                return true;
            }
        }

        FBRecipeCard card = FindRecipeCardForWidget(w);
        if (card)
        {
            SelectRecipe(card.GetRecipe());
            return true;
        }

        FBCategoryOption categoryOption = FindCategoryOptionForWidget(w);
        if (categoryOption)
        {
            Print(string.Format("[Frontier][CraftHud] Category icon clicked %1", categoryOption.GetCategory()));
            HandleCategorySelection(categoryOption.GetCategory());
            return true;
        }

        return false;
    }

    override bool OnMouseEnter(Widget w, int x, int y)
    {
        super.OnMouseEnter(w, x, y);
        FBIngredientCard ingredientCard = FindIngredientCardForWidget(w);
        if (ingredientCard)
            ingredientCard.OnEnter();
        return false;
    }

    override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
    {
        super.OnMouseLeave(w, enterW, x, y);
        FBIngredientCard ingredientCard = FindIngredientCardForWidget(w);
        if (ingredientCard)
            ingredientCard.OnLeave();
        return false;
    }

    void OnQueueUpdate(FB_QueueState state)
    {
        Print("[Frontier][CraftHud] OnQueueUpdate");
        if (!m_Root)
            return;

        UpdateLockOverlay(state);
        BuildQueueList(state);
        UpdateProgress(state);
    }

    protected void UpdateLockOverlay(FB_QueueState state)
    {
        Print("[Frontier][CraftHud] UpdateLockOverlay");
        if (state && state.QueueLimit > 0)
            m_CurrentQueueLimit = state.QueueLimit;

        m_CraftingUnlocked = true;

        if (m_LockPanel)
            m_LockPanel.Show(false);

        if (m_TierStatus)
            m_TierStatus.Show(false);

        if (m_LockTitle)
            m_LockTitle.SetText("");

        if (m_LockHint)
            m_LockHint.SetText("");

        UpdateCraftButtonState();
    }

    protected void PopulateRecipes()
    {
        if (!m_RecipeWrap)
            return;

        FB_Data data = FB_ClientState.GetData();
        if (!data || !data.Recipes || data.Recipes.Count() == 0)
        {
            ClearRecipeCards(true);
            m_Categories.Clear();
            m_CategoryRecipes.Clear();
            m_SelectedCategoryIndex = 0;
            UpdateCategoryControls();
            SelectRecipe(NULL);
            return;
        }

        string previousRecipeId = m_SelectedRecipeId;
        string previousCategory = GetCurrentCategoryName();

        BuildCategoryIndex(data);

        string desiredCategory = "";
        if (previousRecipeId != "")
            desiredCategory = FindCategoryForRecipe(previousRecipeId);
        if (desiredCategory == "" && previousCategory != "")
            desiredCategory = previousCategory;

        int index = m_Categories.Find(desiredCategory);
        if (index >= 0)
            m_SelectedCategoryIndex = index;
        else
            m_SelectedCategoryIndex = 0;

        m_SelectedRecipeId = previousRecipeId;
        BuildCategoryOptions();
        BuildRecipeCardsForCategory(GetCurrentCategoryName());
        ResetIngredientScroll();
        UpdateCategoryControls();
    }

    protected string GetSearchText()
    {
        if (!m_RecipeSearchInput)
            return "";

        string txt = m_RecipeSearchInput.GetText();
        txt.Trim();
        txt.ToLower();
        return txt;
    }

    protected bool RecipeMatchesSearch(FB_Recipe recipe, string searchLower)
    {
        if (searchLower == "" || !recipe)
            return true;

        string name = recipe.Name;
        name.ToLower();

        string result = recipe.Result;
        result.ToLower();

        if (name.Contains(searchLower) || result.Contains(searchLower))
            return true;

        if (recipe.Id)
        {
            string id = recipe.Id;
            id.ToLower();
            if (id.Contains(searchLower))
                return true;
        }

        if (recipe.Results && recipe.Results.Count() > 0)
        {
            for (int i = 0; i < recipe.Results.Count(); i++)
            {
                FB_ResultItem res = recipe.Results[i];
                if (!res)
                    continue;

                string spawn = res.GetSpawnType();
                spawn.ToLower();
                if (spawn.Contains(searchLower))
                    return true;
            }
        }

        return false;
    }

    protected void BuildCategoryIndex(FB_Data data)
    {
        m_Categories.Clear();
        m_CategoryRecipes.Clear();
        ref array<ref FB_Recipe> allRecipes = new array<ref FB_Recipe>();

        for (int i = 0; i < data.Recipes.Count(); i++)
        {
            FB_Recipe recipe = data.Recipes[i];
            if (!recipe)
                continue;

            string category = NormalizeCategory(recipe.Category);
            allRecipes.Insert(recipe);
            ref array<ref FB_Recipe> bucket;
            if (!m_CategoryRecipes.Find(category, bucket))
            {
                bucket = new array<ref FB_Recipe>();
                m_CategoryRecipes.Insert(category, bucket);
                m_Categories.Insert(category);
            }
            bucket.Insert(recipe);
        }

        m_Categories.Sort();

        // Insert "All" category at the top with every recipe
        if (allRecipes && allRecipes.Count() > 0)
        {
            m_CategoryRecipes.Insert("All", allRecipes);
            m_Categories.InsertAt("All", 0);
        }
    }

    protected string NormalizeCategory(string category)
    {
        if (!category)
            return DEFAULT_CATEGORY;

        category = category.Trim();
        if (category == "")
            return DEFAULT_CATEGORY;
        return category;
    }

    protected string GetCurrentCategoryName()
    {
        if (!m_Categories || m_Categories.Count() == 0)
            return "";

        if (m_SelectedCategoryIndex < 0 || m_SelectedCategoryIndex >= m_Categories.Count())
            return "";

        return m_Categories[m_SelectedCategoryIndex];
    }

    protected void BuildRecipeCardsForCategory(string categoryName)
    {
        ClearRecipeCards(false);

        if (categoryName == "")
        {
            SelectRecipe(NULL);
            return;
        }

        ref array<ref FB_Recipe> recipes;
        if (!m_CategoryRecipes.Find(categoryName, recipes) || !recipes)
        {
            SelectRecipe(NULL);
            return;
        }
        Print(string.Format("[Frontier][CraftHud] BuildRecipeCardsForCategory %1 recipes=%2", categoryName, recipes.Count()));

        string searchLower = GetSearchText();
        FB_Recipe retainedSelection = NULL;
        for (int i = 0; i < recipes.Count(); i++)
        {
            FB_Recipe recipe = recipes[i];
            if (!recipe)
                continue;

            if (!RecipeMatchesSearch(recipe, searchLower))
                continue;

            FBRecipeCard card = new FBRecipeCard();
            card.Init(m_RecipeWrap, recipe);
            m_RecipeCards.Insert(card);

            if (m_SelectedRecipeId != "" && recipe.Id == m_SelectedRecipeId)
                retainedSelection = recipe;
        }

        if (retainedSelection)
            SelectRecipe(retainedSelection);
        else
            SelectRecipe(NULL);
        Print("[Frontier][CraftHud] Category build complete");
    }

    protected void UpdateCategoryControls()
    {
        string label = "No Recipes";
        if (m_Categories && m_Categories.Count() > 0)
            label = GetCurrentCategoryName();

        if (m_CategoryLabel)
            m_CategoryLabel.SetText(label);
        UpdateCategorySelection();
    }

    protected void BuildCategoryOptions()
    {
        ClearCategoryOptions();

        if (!m_CategoryIconWrap)
            return;

        if (!m_CategoryOptions)
            m_CategoryOptions = new array<ref FBCategoryOption>();

        if (!m_Categories)
            return;

        string currentCategory = GetCurrentCategoryName();

        for (int i = 0; i < m_Categories.Count(); i++)
        {
            string category = m_Categories[i];
            FBCategoryOption option = new FBCategoryOption();
            option.Init(m_CategoryIconWrap, category);
            option.SetSelected(category == currentCategory);
            m_CategoryOptions.Insert(option);
        }

        Print(string.Format("[Frontier][CraftHud] Categories=%1 total=%2", m_Categories, m_Categories.Count()));
    }


    protected void UpdateCategorySelection()
    {
        if (!m_CategoryOptions)
            return;

        string currentCategory = GetCurrentCategoryName();

        for (int i = 0; i < m_CategoryOptions.Count(); i++)
        {
            FBCategoryOption option = m_CategoryOptions[i];
            if (option)
                option.SetSelected(option.GetCategory() == currentCategory);
        }
    }

    protected FBCategoryOption FindCategoryOptionForWidget(Widget w)
    {
        if (!m_CategoryOptions)
            return NULL;

        for (int i = 0; i < m_CategoryOptions.Count(); i++)
        {
            FBCategoryOption option = m_CategoryOptions[i];
            if (option && option.OwnsWidget(w))
                return option;
        }
        return NULL;
    }

    override bool OnKeyPress(Widget w, int x, int y, int key)
    {
        if (w == m_RecipeSearchInput)
        {
            RefreshSearch();
            return false;
        }

        return super.OnKeyPress(w, x, y, key);
    }

    protected void HandleCategorySelection(string category)
    {
        if (!category || category == "")
            return;

        int index = m_Categories.Find(category);
        if (index < 0)
            return;

        if (index == m_SelectedCategoryIndex)
            return;

        m_SelectedCategoryIndex = index;
        m_SelectedRecipeId = "";
        BuildRecipeCardsForCategory(category);
        ResetIngredientScroll();
        UpdateCategoryControls();
    }

    protected void RefreshSearch()
    {
        BuildRecipeCardsForCategory(GetCurrentCategoryName());
        ResetIngredientScroll();
        UpdateCategoryControls();
    }

    protected void ClearCategoryOptions()
    {
        ClearWidgetChildren(m_CategoryIconWrap);

        if (m_CategoryOptions)
            m_CategoryOptions.Clear();
    }

    protected void ClearWidgetChildren(Widget parent)
    {
        if (!parent)
            return;

        Widget child = parent.GetChildren();
        while (child)
        {
            Widget next = child.GetSibling();
            parent.RemoveChild(child);
            child = next;
        }
    }

    protected string FindCategoryForRecipe(string recipeId)
    {
        if (recipeId == "" || !m_CategoryRecipes)
            return "";

        foreach (string categoryName, ref array<ref FB_Recipe> recipes : m_CategoryRecipes)
        {
            if (!recipes)
                continue;

            for (int i = 0; i < recipes.Count(); i++)
            {
                FB_Recipe recipe = recipes[i];
                if (recipe && recipe.Id == recipeId)
                    return categoryName;
            }
        }

        return "";
    }

    protected void SelectRecipe(FB_Recipe recipe)
    {
        m_SelectedRecipe = recipe;
        if (m_SelectedRecipe)
            m_SelectedRecipeId = m_SelectedRecipe.Id;
        else
            m_SelectedRecipeId = "";
        UpdateRecipeDetails();
    }

    protected void UpdateRecipeDetails()
    {
        ClearIngredientCards();
        ClearResultCards();

        if (!m_SelectedRecipe)
        {
            HideRecipeDetailPanel();
            if (m_ItemName)
                m_ItemName.SetText("Select a Recipe");
            if (m_CraftInfo)
                m_CraftInfo.SetText("Craft time: --");
            if (m_ItemPreview)
            {
                m_ItemPreview.SetItem(NULL);
                DeletePreviewEntity();
            }
            if (m_AttachmentInfo)
                m_AttachmentInfo.SetText("Tools (bench storage): None");
            if (m_BtnCraft)
                m_BtnCraft.Enable(false);
            if (m_BtnCraftDetail)
                m_BtnCraftDetail.Enable(false);
            return;
        }

        ShowRecipeDetailPanel();

        if (m_ItemName)
        {
            string displayName = m_SelectedRecipe.Name;
            int displayCount = m_SelectedRecipe.ResultCount;
            string previewType = m_SelectedRecipe.Result;

            if (m_SelectedRecipe.Results && m_SelectedRecipe.Results.Count() > 0)
            {
                FB_ResultItem primary = m_SelectedRecipe.Results[0];
                if (primary)
                {
                    displayCount = primary.GetSafeCount();
                    previewType = primary.GetSpawnType();
                }
            }

            if (displayCount > 1)
            {
                displayName = string.Format("%1 x%2", m_SelectedRecipe.Name, displayCount);
            }
            m_ItemName.SetText(displayName);
        }

        if (m_CraftInfo)
        {
            string craftLabel = "Craft time: --";
            if (m_SelectedRecipe.CraftTime > 0)
            {
                float craftSeconds = m_SelectedRecipe.CraftTime;
                craftLabel = string.Format("Craft time: %1s", craftSeconds.ToString());
            }
            m_CraftInfo.SetText(craftLabel);
        }

        if (m_ItemPreview)
        {
            DeletePreviewEntity();
            string previewTypeName = m_SelectedRecipe.Result;
            if (m_SelectedRecipe.Results && m_SelectedRecipe.Results.Count() > 0)
            {
                FB_ResultItem previewItem = m_SelectedRecipe.Results[0];
                if (previewItem)
                {
                    string variantType = previewItem.GetSpawnType();
                    if (variantType != "")
                        previewTypeName = variantType;
                }
            }

            if (previewTypeName == "")
                previewTypeName = m_SelectedRecipe.Result;

            if (previewTypeName != "")
            {
                m_PreviewEntity = EntityAI.Cast(GetGame().CreateObject(previewTypeName, vector.Zero, true));
                if (!m_PreviewEntity)
                {
                    m_PreviewEntity = EntityAI.Cast(GetGame().CreateObjectEx(previewTypeName, vector.Zero, ECE_LOCAL | ECE_NOLIFETIME | ECE_DYNAMIC_PERSISTENCY));
                }
                if (m_PreviewEntity)
                {
                    Print(string.Format("[Frontier][CraftHud] Preview entity created for %1", previewTypeName));
                    m_ItemPreview.SetItem(m_PreviewEntity);
                    m_ItemPreview.Show(true);
                }
                else
                {
                    Print(string.Format("[Frontier][CraftHud][WARN] Failed to create preview entity for %1", previewTypeName));
                }
            }
            else
            {
                Print("[Frontier][CraftHud][WARN] No preview type available for selected recipe");
            }
        }

        UpdateAttachmentInfo();
        BuildResultList();
        BuildIngredientList();
        UpdateCraftButtonState();
    }

    protected void UpdateAttachmentInfo()
    {
        if (!m_AttachmentInfo)
            return;

        if (!m_SelectedRecipe || !m_SelectedRecipe.RequiredAttachments || m_SelectedRecipe.RequiredAttachments.Count() == 0)
        {
            m_AttachmentInfo.SetText("Tools (bench storage): None");
            m_AttachmentInfo.SetColor(ARGB(255, 180, 180, 180));
            return;
        }

        bool hasAllAttachments = HasRequiredAttachments();
        string text;
        if (hasAllAttachments)
            text = "Tools (store inside bench):\n";
        else
            text = "Missing tools (store inside bench):\n";

        for (int i = 0; i < m_SelectedRecipe.RequiredAttachments.Count(); i++)
        {
            text += m_SelectedRecipe.RequiredAttachments[i];
            if (i < m_SelectedRecipe.RequiredAttachments.Count() - 1)
                text += "\n";
        }
        m_AttachmentInfo.SetText(text);
        if (hasAllAttachments)
            m_AttachmentInfo.SetColor(ARGB(255, 150, 200, 160));
        else
            m_AttachmentInfo.SetColor(ARGB(255, 220, 80, 80));
    }

    protected void BuildResultList()
    {
        if (!m_ResultWrap || !m_SelectedRecipe)
            return;

        int displayed = 0;
        if (m_SelectedRecipe.Results && m_SelectedRecipe.Results.Count() > 0)
        {
            for (int i = 0; i < m_SelectedRecipe.Results.Count() && displayed < MAX_RESULT_SLOTS; i++)
            {
                FB_ResultItem result = m_SelectedRecipe.Results[i];
                if (!result)
                    continue;

                string typeName = result.GetSpawnType();
                if (typeName == "")
                    typeName = m_SelectedRecipe.Result;

                if (typeName == "")
                    continue;

                int count = result.GetSafeCount();
                if (count <= 0)
                    count = Math.Max(1, m_SelectedRecipe.ResultCount);

                AddResultSlot(typeName, count);
                displayed++;
            }
        }

        if (displayed == 0)
        {
            string fallbackType = m_SelectedRecipe.Result;
            if (fallbackType != "")
            {
                int fallbackCount = Math.Max(1, m_SelectedRecipe.ResultCount);
                AddResultSlot(fallbackType, fallbackCount);
                displayed++;
            }
        }

        if (m_ResultWrap)
            m_ResultWrap.Show(displayed > 0);
    }

    protected void AddResultSlot(string typeName, int count)
    {
        if (!m_ResultWrap)
            return;

        FBResultCard card = new FBResultCard();
        card.Init(m_ResultWrap, typeName, count);
        m_ResultCards.Insert(card);
    }

    protected void BuildIngredientList()
    {
        if (!m_IngredientWrap || !m_SelectedRecipe)
            return;

        Print(string.Format("[Frontier][CraftHud] BuildIngredientList recipe=%1 ingredients=%2", m_SelectedRecipe.Id, m_SelectedRecipe.Ingredients.Count()));

        map<string, int> cargoCounts = new map<string, int>();
        FB_Workbench bench = m_Manager.GetWB();
        if (bench)
            GatherWorkbenchCargoItems(bench, cargoCounts);

        int displayed = 0;
        for (int i = 0; i < m_SelectedRecipe.Ingredients.Count() && displayed < MAX_INGREDIENT_SLOTS; i++)
        {
            FB_Ingredient ingredient = m_SelectedRecipe.Ingredients[i];
            if (!ingredient)
                continue;

            string key = ingredient.Type;
            key.ToLower();
            int have = 0;
            cargoCounts.Find(key, have);

            FBIngredientCard card = new FBIngredientCard();
            card.Init(m_IngredientWrap, ingredient.Type, have, ingredient.Count);
            m_IngredientCards.Insert(card);
            displayed++;
            Print(string.Format("[Frontier][CraftHud] Ingredient card added type=%1 have=%2 need=%3", ingredient.Type, have, ingredient.Count));
        }

        Print(string.Format("[Frontier][CraftHud] BuildIngredientList completed recipe=%1 cards=%2 displayed=%3", m_SelectedRecipe.Id, m_IngredientCards.Count(), displayed));

        if (m_IngredientWrap)
            m_IngredientWrap.Show(displayed > 0);
        if (m_IngredientScroll)
            m_IngredientScroll.Show(displayed > 0);
        if (displayed == 0)
        {
            Print("[Frontier][CraftHud][WARN] No ingredients to display (check recipe data)");
        }
    }

    protected void GatherWorkbenchCargoItems(EntityAI entity, map<string, int> counts)
    {
        if (!entity || !counts)
            return;

        ItemBase item = ItemBase.Cast(entity);
        if (item && !item.IsRuined())
        {
            string typeName = item.GetType();
            string key = typeName;
            key.ToLower();

            int quantity = GetItemQuantity(item);
            int existing;
            if (counts.Find(key, existing))
                counts.Set(key, existing + quantity);
            else
                counts.Insert(key, quantity);
        }

        if (!entity.GetInventory())
            return;

        CargoBase cargo = entity.GetInventory().GetCargo();
        if (cargo)
        {
            for (int i = 0; i < cargo.GetItemCount(); i++)
            {
                EntityAI cargoItem = cargo.GetItem(i);
                GatherWorkbenchCargoItems(cargoItem, counts);
            }
        }
    }

    protected int GetItemQuantity(ItemBase item)
    {
        if (!item)
            return 0;

        Magazine mag = Magazine.Cast(item);
        if (mag)
            return mag.GetAmmoCount();

        if (item.HasQuantity())
            return Math.Floor(item.GetQuantity());

        return 1;
    }

    protected void UpdateCraftButtonState()
    {
        if (!m_BtnCraft && !m_BtnCraftDetail)
            return;

        if (!m_CraftingUnlocked || !m_SelectedRecipe)
        {
            if (m_BtnCraft)
                m_BtnCraft.Enable(false);
            if (m_BtnCraftDetail)
                m_BtnCraftDetail.Enable(false);
            return;
        }

        bool canCraft = true;
        for (int i = 0; i < m_IngredientCards.Count(); i++)
        {
            FBIngredientCard card = m_IngredientCards[i];
            if (!card.HasEnough())
            {
                canCraft = false;
                break;
            }
        }

        if (m_SelectedRecipe.RequiredAttachments && m_SelectedRecipe.RequiredAttachments.Count() > 0)
        {
            if (!HasRequiredAttachments())
                canCraft = false;
        }

        if (m_BtnCraft)
            m_BtnCraft.Enable(canCraft);
        if (m_BtnCraftDetail)
            m_BtnCraftDetail.Enable(canCraft);
    }

    protected bool HasRequiredAttachments()
    {
        FB_Workbench bench = m_Manager.GetWB();
        if (!bench)
            return false;

        map<string, int> cargoCounts = new map<string, int>();
        GatherWorkbenchCargoItems(bench, cargoCounts);

        for (int i = 0; i < m_SelectedRecipe.RequiredAttachments.Count(); i++)
        {
            string required = m_SelectedRecipe.RequiredAttachments[i];
            string requiredLower = required;
            requiredLower.ToLower();

            int available;
            if (!cargoCounts.Find(requiredLower, available) || available <= 0)
            {
                string exactKey = required;
                exactKey.ToLower();
                if (!cargoCounts.Find(exactKey, available) || available <= 0)
                    return false;
            }
        }
        return true;
    }

    protected void TryCraftSelected()
    {
        if (!m_SelectedRecipe)
            return;

        if (!m_CraftingUnlocked)
            return;

        CraftItem payload = new CraftItem();
        payload.RecipeId = m_SelectedRecipe.Id;
        m_Manager.RequestCraft(payload);
    }

    protected void ClearRecipeCards(bool resetSelection = false)
    {
        if (!m_RecipeWrap)
            return;

        Widget child = m_RecipeWrap.GetChildren();
        while (child)
        {
            Widget next = child.GetSibling();
            m_RecipeWrap.RemoveChild(child);
            child = next;
        }

        if (m_RecipeCards)
        {
            for (int i = 0; i < m_RecipeCards.Count(); i++)
            {
                if (m_RecipeCards[i])
                    m_RecipeCards[i].Cleanup();
            }
            m_RecipeCards.Clear();
        }

        m_SelectedRecipe = NULL;
        if (resetSelection)
            m_SelectedRecipeId = "";
    }

    protected void ClearIngredientCards()
    {
        if (m_IngredientWrap)
        {
            while (m_IngredientWrap.GetChildren())
            {
                Widget child = m_IngredientWrap.GetChildren();
                m_IngredientWrap.RemoveChild(child);
            }
        }

        if (m_IngredientCards)
        {
            for (int i = 0; i < m_IngredientCards.Count(); i++)
            {
                m_IngredientCards[i].Cleanup();
            }
            m_IngredientCards.Clear();
        }
        ResetIngredientScroll();
    }

    protected void ClearResultCards()
    {
        if (m_ResultWrap)
        {
            Widget child = m_ResultWrap.GetChildren();
            while (child)
            {
                Widget next = child.GetSibling();
                m_ResultWrap.RemoveChild(child);
                child = next;
            }
        }

        if (m_ResultCards)
        {
            for (int i = 0; i < m_ResultCards.Count(); i++)
            {
                m_ResultCards[i].Cleanup();
            }
            m_ResultCards.Clear();
        }
    }

    protected void ResetIngredientScroll()
    {
        if (m_IngredientScroll)
            m_IngredientScroll.VScrollToPos01(0);
    }

    protected void ClearQueueCards()
    {
        if (!m_QueueWrap)
            return;

        Widget child = m_QueueWrap.GetChildren();
        while (child)
        {
            Widget next = child.GetSibling();
            m_QueueWrap.RemoveChild(child);
            child = next;
        }

        if (m_QueueCards)
            m_QueueCards.Clear();
    }

    protected void UpdateQueueDisplay()
    {
        FB_QueueState state = FB_ClientState.GetQueue();
        BuildQueueList(state);
        UpdateProgress(state);
    }

    protected void BuildQueueList(FB_QueueState state)
    {
        ClearQueueCards();

        if (!state || !state.Entries || state.Entries.Count() == 0)
        {
            if (m_QueueHeader)
                m_QueueHeader.SetText("Queue (empty)");
            return;
        }

        if (m_QueueHeader)
            m_QueueHeader.SetText("Queue");

        if (!m_QueueCards)
            m_QueueCards = new array<ref FBCraftQueueCard>();

        float cumulativeWait = 0;
        for (int i = 0; i < state.Entries.Count(); i++)
        {
            FB_QueueEntry entry = state.Entries[i];
            if (!entry || !m_QueueWrap)
                continue;

            float waitTime = -1;
            if (entry.Active)
            {
                waitTime = entry.RemainingTime;
                cumulativeWait = entry.RemainingTime;
            }
            else
            {
                waitTime = cumulativeWait;
                cumulativeWait += entry.TotalTime;
            }

            FBCraftQueueCard card = new FBCraftQueueCard();
            card.Init(m_QueueWrap, entry, waitTime);
            card.SetActive(entry.Active);
            m_QueueCards.Insert(card);
        }
    }

    protected void UpdateProgress(FB_QueueState state)
    {
        if (!m_Progress)
            return;

        int queueCount = 0;
        if (state && state.Entries)
            queueCount = state.Entries.Count();

        int queueLimit = m_CurrentQueueLimit;
        if (state && state.QueueLimit > 0)
            queueLimit = state.QueueLimit;
        if (queueLimit <= 0)
            queueLimit = Math.Max(queueCount, 1);
        m_CurrentQueueLimit = queueLimit;

        m_QueueHeaderBase = string.Format("Queue %1/%2", queueCount.ToString(), queueLimit.ToString());

        if (!state || !state.Entries || state.Entries.Count() == 0)
        {
            m_ProgressActive = false;
            m_Progress.Show(false);
            m_Progress.SetCurrent(0);
            if (m_QueueHeader)
                m_QueueHeader.SetText(string.Format("%1 (waiting)", m_QueueHeaderBase));
            if (m_ProgressPanel)
                m_ProgressPanel.Show(false);
            return;
        }

        FB_QueueEntry active = NULL;
        for (int i = 0; i < state.Entries.Count(); i++)
        {
            FB_QueueEntry candidate = state.Entries[i];
            if (candidate && candidate.Active)
            {
                active = candidate;
                break;
            }
        }

        if (!active)
        {
            m_ProgressActive = false;
            m_Progress.Show(false);
            m_Progress.SetCurrent(0);
            if (m_QueueHeader)
                m_QueueHeader.SetText(string.Format("%1 (waiting)", m_QueueHeaderBase));
            if (m_ProgressPanel)
                m_ProgressPanel.Show(false);
            return;
        }

        m_ActiveTotalTime = Math.Max(active.TotalTime, 0.01);
        m_ActiveRemainingTime = Math.Max(active.RemainingTime, 0.0);
        m_ProgressActive = true;
        ApplyProgressUI();
    }

    protected string FormatTime(float seconds)
    {
        if (seconds < 0)
            seconds = 0;

        int total = Math.Floor(seconds);
        int mins = total / 60;
        int secs = total % 60;
        string secStr = secs.ToString();
        if (secs < 10)
            secStr = string.Format("0%1", secStr);
        return string.Format("%1:%2", mins.ToString(), secStr);
    }

    protected void ApplyProgressUI()
    {
        if (!m_Progress)
            return;

        float total = Math.Max(m_ActiveTotalTime, 0.01);
        float remaining = Math.Clamp(m_ActiveRemainingTime, 0.0, total);
        float ratio = Math.Clamp((total - remaining) / total, 0.0, 1.0);
        float percent = ratio * 100.0;
        m_Progress.SetCurrent(percent);
        m_Progress.Show(true);

        if (m_QueueHeader)
        {
            string timer = FormatTime(remaining);
            m_QueueHeader.SetText(string.Format("%1 - %2", m_QueueHeaderBase, timer));
        }

        if (m_ProgressPanel)
            m_ProgressPanel.Show(true);
    }

    protected FBRecipeCard FindRecipeCardForWidget(Widget w)
    {
        for (int i = 0; i < m_RecipeCards.Count(); i++)
        {
            FBRecipeCard card = m_RecipeCards[i];
            if (card && card.IsRecipeControl(w))
                return card;
        }
        return NULL;
    }

    protected FBIngredientCard FindIngredientCardForWidget(Widget w)
    {
        for (int i = 0; i < m_IngredientCards.Count(); i++)
        {
            FBIngredientCard card = m_IngredientCards[i];
            if (card && card.OwnsWidget(w))
                return card;
        }
        return NULL;
    }

    override bool OnChange(Widget w, int x, int y, bool finished)
    {
        if (w == m_RecipeSearchInput)
        {
            RefreshSearch();
            return false;
        }

        return super.OnChange(w, x, y, finished);
    }

    override void Update(float timeslice)
    {
        super.Update(timeslice);

        if (m_ProgressActive && m_ActiveRemainingTime > 0)
        {
            m_ActiveRemainingTime = Math.Max(0, m_ActiveRemainingTime - timeslice);
            ApplyProgressUI();
        }
    }

    protected void DisablePlayerControls()
    {
        Mission mission = GetGame().GetMission();
        if (mission)
            mission.PlayerControlDisable(INPUT_EXCLUDE_ALL);
    }

    protected void RestorePlayerControls()
    {
        Mission mission = GetGame().GetMission();
        if (mission)
            mission.PlayerControlEnable(true);
        DeletePreviewEntity();
    }

    protected void DeletePreviewEntity()
    {
        if (m_PreviewEntity)
        {
            GetGame().ObjectDeleteOnClient(m_PreviewEntity);
            m_PreviewEntity = null;
        }
        if (m_ItemPreview)
            m_ItemPreview.SetItem(NULL);
    }
}
