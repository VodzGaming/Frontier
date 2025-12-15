class FB_ResultItem
{
    string Type;
    int Count;
    float HealthPercent;
    ref array<string> VariantTypes;
    string VariantOverride;

    void FB_ResultItem(string t = "", int c = 1, float hp = 100)
    {
        Type = t;
        Count = c;
        HealthPercent = hp;
        VariantTypes = new array<string>();
        VariantOverride = "";
    }

    FB_ResultItem Clone()
    {
        FB_ResultItem copy = new FB_ResultItem(Type, Count, HealthPercent);
        copy.VariantOverride = VariantOverride;
        if (VariantTypes)
        {
            for (int i = 0; i < VariantTypes.Count(); i++)
            {
                string variant = VariantTypes[i];
                if (variant)
                    copy.VariantTypes.Insert(variant);
            }
        }
        return copy;
    }

    string GetSpawnType()
    {
        if (VariantOverride && VariantOverride != "")
            return VariantOverride;
        if (VariantTypes && VariantTypes.Count() > 0 && VariantTypes[0] != "")
            return VariantTypes[0];
        return Type;
    }

    int GetSafeCount()
    {
        return Math.Max(1, Count);
    }

    float GetClampedHealth()
    {
        float hp = HealthPercent;
        if (hp <= 0)
            hp = 100;
        return Math.Clamp(hp, 1, 100);
    }
}

class FB_Ingredient
{
    string Type;
    int Count;
    void FB_Ingredient(string t, int c){ Type = t; Count = c; }
}

class FB_Recipe
{
    string Id;
    string Name;
    string Result;
    int ResultCount;
    bool RequireBench;
    bool RequireTerritory;
    float BenchRadius;
    string Category;
    string IconKey;
    ref array<string> RequiredAttachments;
    ref array<ref FB_Ingredient> Ingredients;
    ref array<ref FB_ResultItem> Results;
    float CraftTime;

    void FB_Recipe()
    {
        Ingredients = new array<ref FB_Ingredient>();
        ResultCount = 1;
        BenchRadius = 3.0;
        RequiredAttachments = new array<string>();
        Results = new array<ref FB_ResultItem>();
        CraftTime = 5.0;
    }
}

class FB_Data
{
    ref array<ref FB_Recipe> Recipes;
    void FB_Data(){ Recipes = new array<ref FB_Recipe>(); }
}

class FB_QueueEntry
{
    string RecipeId;
    string RecipeName;
    float RemainingTime;
    float TotalTime;
    bool Active;
    int Position;
    string RequestedBy;
}

class FB_QueueState
{
    ref array<ref FB_QueueEntry> Entries;
    int QueueLimit;
    int WorkbenchTier;
    int RequiredTier;
    bool CraftingUnlocked;
    void FB_QueueState()
    {
        Entries = new array<ref FB_QueueEntry>();
        QueueLimit = 1;
        WorkbenchTier = 0;
        RequiredTier = 1;
        CraftingUnlocked = false;
    }
}

class FB_CraftJob
{
    string RecipeId;
    string RecipeName;
    ref array<ref FB_ResultItem> Results;
    float TotalTime;
    float TimeRemaining;
    string RequestedById;
    string RequestedByName;
    bool Active;

    void FB_CraftJob()
    {
        Results = new array<ref FB_ResultItem>();
        TotalTime = 5.0;
        TimeRemaining = TotalTime;
    }
}

class FB_ClientState
{
    static ref FB_Data Data;
    static ref FB_QueueState QueueState;
    static void Ensure()
    {
        if (!Data)
        {
            Data = new FB_Data();
            FB_RecipeDB defaults = FB_RecipeDB.BuildSampleData();
            if (defaults && defaults.Recipes)
            {
                foreach (FB_Recipe recipe : defaults.Recipes)
                {
                    FB_Recipe copy = new FB_Recipe();
                    copy.Id = recipe.Id;
                    copy.Name = recipe.Name;
                    copy.Result = recipe.Result;
                    copy.ResultCount = recipe.ResultCount;
                    copy.RequireBench = recipe.RequireBench;
                    copy.RequireTerritory = recipe.RequireTerritory;
                    copy.BenchRadius = recipe.BenchRadius;
                    copy.Category = recipe.Category;
                    copy.IconKey = recipe.IconKey;
                    copy.CraftTime = recipe.CraftTime;

                    if (recipe.Ingredients)
                    {
                        foreach (FB_Ingredient ingredient : recipe.Ingredients)
                        {
                            copy.Ingredients.Insert(new FB_Ingredient(ingredient.Type, ingredient.Count));
                        }
                    }

                    if (recipe.RequiredAttachments)
                    {
                        foreach (string attachment : recipe.RequiredAttachments)
                        {
                            copy.RequiredAttachments.Insert(attachment);
                        }
                    }

                    if (recipe.Results)
                    {
                        foreach (FB_ResultItem res : recipe.Results)
                        {
                            copy.Results.Insert(res.Clone());
                        }
                    }

                    Data.Recipes.Insert(copy);
                }
            }
        }

        if (!QueueState)
            QueueState = new FB_QueueState();
    }

    static void UpdateData(ref FB_Data newData)
    {
        Ensure();
        if (newData)
        {
            Data = newData;
            int count = 0;
            if (Data && Data.Recipes)
                count = Data.Recipes.Count();
            Print(string.Format("[Frontier][ClientState] UpdateData recipes=%1", count.ToString()));
            if (Data && Data.Recipes)
            {
                // Print per-category breakdown for debugging
                map<string, int> catCounts = new map<string, int>();
                for (int i = 0; i < Data.Recipes.Count(); i++)
                {
                    FB_Recipe r = Data.Recipes[i];
                    if (!r)
                        continue;
                    string cat = r.Category;
                    if (cat == "")
                        cat = "General";
                    int existing;
                    if (catCounts.Find(cat, existing))
                        catCounts.Set(cat, existing + 1);
                    else
                        catCounts.Insert(cat, 1);
                    Print(string.Format("[Frontier][ClientState] Recipe received id=%1 name=%2 category=%3", r.Id, r.Name, r.Category));
                }
                array<string> cats = catCounts.GetKeyArray();
                string summary = "[Frontier][ClientState] Categories=";
                for (int c = 0; c < cats.Count(); c++)
                {
                    string name = cats[c];
                    int cCount = catCounts.Get(name);
                    string spacer = " ";
                    if (c == 0)
                        spacer = "";
                    summary += string.Format("%1%2:%3", spacer, name, cCount.ToString());
                }
                Print(summary);
            }
        }
    }

    static void UpdateDataFromJson(string jsonPayload)
    {
        if (!jsonPayload || jsonPayload == "")
        {
            Print("[Frontier][ClientState][WARN] Received empty crafting payload");
            return;
        }

        ref FB_Data parsed;
        string errorMessage;
        if (!JsonFileLoader<FB_Data>.LoadData(jsonPayload, parsed, errorMessage))
        {
            Print(string.Format("[Frontier][ClientState][ERROR] Failed to parse crafting payload: %1", errorMessage));
            return;
        }

        UpdateData(parsed);
    }

    static FB_Data GetData()
    {
        Ensure();
        return Data;
    }

    static void UpdateQueue(ref FB_QueueState queueState)
    {
        Ensure();
        if (queueState)
            QueueState = queueState;
        else
            QueueState = new FB_QueueState();
    }

    static FB_QueueState GetQueue()
    {
        Ensure();
        return QueueState;
    }
}
