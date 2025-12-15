class FB_RecipeDB
{
    ref array<ref FB_Recipe> Recipes;
    ref map<string, string> m_CategoryIcons;

    void FB_RecipeDB()
    {
        Recipes = new array<ref FB_Recipe>();
        m_CategoryIcons = new map<string, string>();
    }

    static FB_RecipeDB Load(string categoryRoot, string legacyPath)
    {
        FB_RecipeDB db = new FB_RecipeDB();
        // Build candidate roots to support servers that name their profiles directory "config" or similar.
        ref array<string> roots = new array<string>();
        roots.Insert(NormalizeDirectory(categoryRoot)); // primary
        roots.Insert(NormalizeDirectory("$profile:/Frontier")); // explicit primary
        roots.Insert(NormalizeDirectory("$profile:/config/Frontier")); // common alt when -profiles=config

        string activeRoot = "";
        bool updated = false;
        for (int r = 0; r < roots.Count(); r++)
        {
            string root = roots[r];
            if (root == "" || (activeRoot != "" && root == activeRoot))
                continue;

            Print(string.Format("[Frontier][Recipes] Trying recipe root %1", root));
            if (!FileExist(root))
                continue;

            if (db.LoadCategoryFiles(root))
            {
                activeRoot = root;
                break;
            }
        }

        if (activeRoot == "" && roots.Count() > 0)
            activeRoot = roots[0];

        if (activeRoot != "" && !FileExist(activeRoot))
            MakeDirectory(activeRoot);

        if (activeRoot != "" && db.Recipes.Count() == 0 && db.LoadCategoryFiles(activeRoot))
            Print(string.Format("[Frontier][Recipes] Loaded recipe categories from %1", activeRoot));

        if (db.Recipes.Count() == 0)
        {
            if (legacyPath && legacyPath != "" && FileExist(legacyPath))
            {
                JsonFileLoader<FB_RecipeDB>.JsonLoadFile(legacyPath, db);
                string backupPath = legacyPath + ".bak";
                if (FileExist(backupPath))
                    DeleteFile(backupPath);
                if (CopyFile(legacyPath, backupPath))
                    DeleteFile(legacyPath);
                updated = true;
            }
            else
            {
                db = BuildSampleData();
                updated = true;
            }
        }

        if (db.EnsureConsistency())
            updated = true;

        if (updated)
        {
            Print("[Frontier][Recipes] Saving recipe categories (data changed during load)");
            if (activeRoot != "")
                db.SaveCategoryFiles(activeRoot);
        }
        Print(string.Format("[Frontier][Recipes] Loaded %1 recipes", db.Recipes.Count()));

        return db;
    }

    void SaveCategoryFiles(string rootDir)
    {
        string normalizedRoot = NormalizeDirectory(rootDir);
        MakeDirectory(normalizedRoot);

        map<string, ref FB_RecipeCategoryFile> grouped = new map<string, ref FB_RecipeCategoryFile>();
        for (int i = 0; i < Recipes.Count(); i++)
        {
            FB_Recipe recipe = Recipes[i];
            if (!recipe)
                continue;

            string category = recipe.Category;
            if (category == "")
                category = "General";

            FB_RecipeCategoryFile entry;
            if (!grouped.Find(category, entry))
            {
                entry = new FB_RecipeCategoryFile();
                entry.Category = category;
                entry.IconKey = GetCategoryIcon(category);
                grouped.Insert(category, entry);
            }
            entry.Recipes.Insert(CloneRecipe(recipe));
        }

        array<string> categories = grouped.GetKeyArray();
        for (int c = 0; c < categories.Count(); c++)
        {
            string targetCategory = categories[c];
            FB_RecipeCategoryFile categoryEntry = grouped.Get(targetCategory);
            string folderPath = normalizedRoot + targetCategory + "/";
            MakeDirectory(folderPath);
            string filePath = folderPath + GetCategoryFileName(targetCategory);
            JsonFileLoader<FB_RecipeCategoryFile>.JsonSaveFile(filePath, categoryEntry);
        }
    }

    bool EnsureConsistency()
    {
        bool changed = false;
        if (!Recipes)
        {
            Recipes = new array<ref FB_Recipe>();
            changed = true;
        }

        if (Recipes.Count() == 0)
        {
            FB_RecipeDB defaults = BuildSampleData();
            if (defaults && defaults.Recipes)
            {
                Recipes.Clear();
                for (int d = 0; d < defaults.Recipes.Count(); d++)
                {
                    FB_Recipe sample = defaults.Recipes[d];
                    if (sample)
                        Recipes.Insert(CloneRecipe(sample));
                }
                changed = true;
            }
        }

        if (NormalizeRecipes(this))
            changed = true;

        return changed;
    }

    string GetCategoryIcon(string category)
    {
        if (!category || category == "" || !m_CategoryIcons)
            return "";
        string icon;
        if (m_CategoryIcons.Find(category, icon))
            return icon;
        return "";
    }

    void SetCategoryIcon(string category, string icon)
    {
        if (!category || category == "")
            return;
        if (!m_CategoryIcons)
            m_CategoryIcons = new map<string, string>();
        m_CategoryIcons.Insert(category, icon);
    }

    protected static bool EnsureDefaultRecipes(FB_RecipeDB db)
    {
        bool added = false;
        FB_RecipeDB defaults = BuildSampleData();
        if (!defaults || !defaults.Recipes)
            return false;

        for (int i = 0; i < defaults.Recipes.Count(); i++)
        {
            FB_Recipe sample = defaults.Recipes[i];
            if (!sample)
                continue;

            if (db.FindById(sample.Id))
                continue;

            FB_Recipe clone = CloneRecipe(sample);
            db.Recipes.Insert(clone);
            added = true;
        }

        return added;
    }

    protected static FB_Recipe CloneRecipe(FB_Recipe source)
    {
        if (!source)
            return NULL;

        FB_Recipe copy = new FB_Recipe();
        copy.Id = source.Id;
        copy.Name = source.Name;
        copy.Result = source.Result;
        copy.ResultCount = source.ResultCount;
        copy.RequireBench = source.RequireBench;
        copy.RequireTerritory = source.RequireTerritory;
        copy.BenchRadius = source.BenchRadius;
        copy.Category = source.Category;
        copy.IconKey = source.IconKey;

        if (source.Ingredients)
        {
            for (int i = 0; i < source.Ingredients.Count(); i++)
            {
                FB_Ingredient ing = source.Ingredients[i];
                if (ing)
                    copy.Ingredients.Insert(new FB_Ingredient(ing.Type, ing.Count));
            }
        }

        if (source.RequiredAttachments)
        {
            for (int j = 0; j < source.RequiredAttachments.Count(); j++)
            {
                string attachment = source.RequiredAttachments[j];
                copy.RequiredAttachments.Insert(attachment);
            }
        }

        if (source.Results)
        {
            for (int k = 0; k < source.Results.Count(); k++)
            {
                FB_ResultItem res = source.Results[k];
                if (res)
                    copy.Results.Insert(res.Clone());
            }
        }
        copy.CraftTime = source.CraftTime;

        return copy;
    }

    protected static bool NormalizeRecipes(FB_RecipeDB db)
    {
        bool changed = false;
        if (!db || !db.Recipes)
            return changed;

        for (int i = 0; i < db.Recipes.Count(); i++)
        {
            FB_Recipe recipe = db.Recipes[i];
            if (!recipe)
                continue;

            if (!recipe.Ingredients)
            {
                recipe.Ingredients = new array<ref FB_Ingredient>();
                changed = true;
            }
            if (!recipe.RequiredAttachments)
            {
                recipe.RequiredAttachments = new array<string>();
                changed = true;
            }
            if (!recipe.Results)
            {
                recipe.Results = new array<ref FB_ResultItem>();
                changed = true;
            }

            if (recipe.Results.Count() == 0 && recipe.Result != "")
            {
                recipe.Results.Insert(new FB_ResultItem(recipe.Result, Math.Max(1, recipe.ResultCount)));
                changed = true;
            }

            for (int r = 0; r < recipe.Results.Count(); r++)
            {
                FB_ResultItem resultItem = recipe.Results[r];
                if (!resultItem)
                    continue;

                if (!resultItem.VariantTypes)
                    resultItem.VariantTypes = new array<string>();

                resultItem.HealthPercent = Math.Clamp(resultItem.HealthPercent, 1, 100);
            }

            if (recipe.CraftTime <= 0)
            {
                recipe.CraftTime = 5.0;
                changed = true;
            }
        }
        return changed;
    }

    static FB_RecipeDB BuildSampleData()
    {
        FB_RecipeDB defaults = new FB_RecipeDB();

        FB_Recipe knife = new FB_Recipe();
        knife.Id = "fb_sample_stoneknife";
        knife.Name = "Stone Knife";
        knife.Result = "StoneKnife";
        knife.ResultCount = 1;
        knife.Ingredients.Insert(new FB_Ingredient("SmallStone", 2));
        knife.Category = "Survival";
        knife.IconKey = "greenkcpic";
        knife.CraftTime = 4.0;
        defaults.Recipes.Insert(knife);

        FB_Recipe bandage = new FB_Recipe();
        bandage.Id = "fb_sample_bandage";
        bandage.Name = "Clean Bandage";
        bandage.Result = "BandageDressing";
        bandage.ResultCount = 1;
        bandage.Ingredients.Insert(new FB_Ingredient("Rag", 2));
        bandage.RequireBench = true;
        bandage.Category = "Medical";
        bandage.IconKey = "medicpic";
        bandage.CraftTime = 6.0;
        defaults.Recipes.Insert(bandage);

        FB_Recipe arrow = new FB_Recipe();
        arrow.Id = "fb_sample_bonearrow";
        arrow.Name = "Bone Arrow";
        arrow.Result = "Ammo_BoneArrow";
        arrow.ResultCount = 5;
        arrow.Ingredients.Insert(new FB_Ingredient("BoneHook", 5));
        arrow.Ingredients.Insert(new FB_Ingredient("LongWoodenStick", 1));
        arrow.RequireTerritory = true;
        arrow.BenchRadius = 10;
        arrow.Category = "Defense";
        arrow.IconKey = "weaponpic";
        arrow.CraftTime = 10.0;
        defaults.Recipes.Insert(arrow);

        FB_Recipe crate = new FB_Recipe();
        crate.Id = "fb_wooden_crate";
        crate.Name = "Wooden Crate";
        crate.Result = "WoodenCrate";
        crate.ResultCount = 1;
        crate.RequireBench = true;
        crate.Ingredients.Insert(new FB_Ingredient("WoodenPlank", 4));
        crate.Ingredients.Insert(new FB_Ingredient("Nail", 10));
        crate.RequiredAttachments.Insert("HandSaw");
        crate.Category = "Storage";
        crate.IconKey = "housepic";
        crate.CraftTime = 12.0;
        defaults.Recipes.Insert(crate);

        return defaults;
    }

    FB_Recipe FindById(string id)
    {
        for (int i = 0; i < Recipes.Count(); i++)
        {
            if (Recipes[i].Id == id)
                return Recipes[i];
        }
        return NULL;
    }

    protected bool LoadCategoryFiles(string rootDir)
    {
        string normalizedRoot = NormalizeDirectory(rootDir);
        string pattern = normalizedRoot + "*";
        string folderName;
        FileAttr attr;
        FindFileHandle handle = FindFile(pattern, folderName, attr, FindFileFlags.DIRECTORIES);
        if (!handle)
            return false;

        bool loaded = false;
        while (true)
        {
            if (folderName && folderName != "." && folderName != "..")
            {
                string filePath = normalizedRoot + folderName + "/" + GetCategoryFileName(folderName);
                if (FileExist(filePath))
                {
                    FB_RecipeCategoryFile categoryFile = new FB_RecipeCategoryFile();
                    JsonFileLoader<FB_RecipeCategoryFile>.JsonLoadFile(filePath, categoryFile);
                    ProcessCategoryFile(folderName, categoryFile);
                    loaded = true;
                }
            }

            if (!FindNextFile(handle, folderName, attr))
                break;
        }

        CloseFindFile(handle);
        return loaded;
    }

    protected void ProcessCategoryFile(string folderName, FB_RecipeCategoryFile data)
    {
        if (!data || !data.Recipes)
            return;

        string category = data.Category;
        if (category == "")
            category = folderName;
        if (category == "")
            category = "General";

        if (data.IconKey != "")
            m_CategoryIcons.Insert(category, data.IconKey);
        else if (!m_CategoryIcons.Contains(category))
            m_CategoryIcons.Insert(category, "");

        for (int i = 0; i < data.Recipes.Count(); i++)
        {
            FB_Recipe recipe = data.Recipes[i];
            if (!recipe)
                continue;

            FB_Recipe copy = CloneRecipe(recipe);
            if (copy.Category == "")
                copy.Category = category;
            Recipes.Insert(copy);
        }
    }

    protected static string NormalizeDirectory(string path)
    {
        string normalized = path;
        if (!normalized || normalized == "")
            normalized = "$profile:/Frontier";
        normalized.Replace("\\", "/");
        if (!StringEndsWith(normalized, "/"))
            normalized = normalized + "/";
        return normalized;
    }

    protected static string GetCategoryFileName(string category)
    {
        return category + "Recipes.json";
    }
}

static bool StringEndsWith(string source, string suffix)
{
    if (!source || !suffix)
        return false;
    int sourceLen = source.Length();
    int suffixLen = suffix.Length();
    if (suffixLen > sourceLen)
        return false;
    string end = source.Substring(sourceLen - suffixLen, suffixLen);
    return end == suffix;
}

class FB_RecipeCategoryFile
{
    string Category;
    string IconKey;
    ref array<ref FB_Recipe> Recipes;

    void FB_RecipeCategoryFile()
    {
        Recipes = new array<ref FB_Recipe>();
    }
}

static bool FB_IsNearTerritory(vector pos, float radius)
{
    array<Object> objects = new array<Object>();
    array<CargoBase> proxy = new array<CargoBase>();
    GetGame().GetObjectsAtPosition(pos, radius, objects, proxy);

    for (int i = 0; i < objects.Count(); i++)
    {
        if (objects[i].IsKindOf("Flag_Base") || objects[i].IsKindOf("FB_TerritoryBeacon"))
            return true;
    }

    return false;
}
