class FB_Workbench;

class FB_IngredientConsumption
{
    ItemBase Item;
    float Quantity;
    bool DestroyWhole;
}

class CraftServerManager
{
    protected ref FB_RecipeDB m_Database;

    static CraftServerManager LoadData(string path = "")
    {
        CraftServerManager manager = new CraftServerManager();
        manager.Load(path);
        return manager;
    }

    void Load(string path)
    {
        ModuleFrontierConfigHub hub = ModuleFrontierConfigHub.Get();
        if (hub)
            m_Database = hub.GetRecipeDatabase();
        else
            m_Database = FB_RecipeDB.BuildSampleData();
    }

    void SendWorkbenchData(PlayerBase player, FB_Workbench bench = NULL)
    {
        if (!player || !player.GetIdentity())
            return;

        ref FB_Data clientData = BuildClientData();
        Param1<ref FB_Data> payload = new Param1<ref FB_Data>(clientData);
        GetGame().RPCSingleParam(player, FBRpc.SERVER_SEND_MAIN_DATA, payload, true, player.GetIdentity());

        if (bench)
        {
            RegisterWatcher(bench, player);
            SendQueueState(player, bench);
        }
    }

    bool TryCraft(PlayerBase player, FB_Workbench bench, string recipeId, out string resultMessage)
    {
        resultMessage = "";

        if (!player || !bench)
        {
            resultMessage = "Missing workbench context";
            SendCraftResult(player, false, resultMessage);
            return false;
        }

        if (!m_Database)
        {
            resultMessage = "Craft database not loaded";
            SendCraftResult(player, false, resultMessage);
            return false;
        }

        FB_Recipe recipe = m_Database.FindById(recipeId);
        if (!recipe)
        {
            resultMessage = "Recipe is not available";
            SendCraftResult(player, false, resultMessage);
            return false;
        }

        if (recipe.RequireBench && !bench)
        {
            resultMessage = "Workbench is required";
            SendCraftResult(player, false, resultMessage);
            return false;
        }

        if (recipe.RequireTerritory && !FB_IsNearTerritory(bench.GetPosition(), recipe.BenchRadius))
        {
            resultMessage = "Territory uplink not detected";
            SendCraftResult(player, false, resultMessage);
            return false;
        }

        if (!bench.HasPower())
        {
            resultMessage = "Workbench batteries are depleted";
            SendCraftResult(player, false, resultMessage);
            return false;
        }

        if (!bench.IsCraftingUnlocked())
        {
            resultMessage = bench.GetCraftingLockMessage();
            SendCraftResult(player, false, resultMessage);
            return false;
        }

        if (!bench.CanAcceptNewJob())
        {
            resultMessage = "Workbench queue is full";
            SendCraftResult(player, false, resultMessage);
            return false;
        }

        if (recipe.RequiredAttachments && recipe.RequiredAttachments.Count() > 0)
        {
            array<string> missing = new array<string>();
            if (!HasRequiredToolsInCargo(bench, recipe.RequiredAttachments, missing))
            {
                if (missing && missing.Count() > 0)
                    resultMessage = string.Format("Workbench is missing tools: %1", JoinStrings(missing, ", "));
                else
                    resultMessage = "Workbench is missing required tools";
                SendCraftResult(player, false, resultMessage);
                return false;
            }
        }

        ref array<ref FB_IngredientConsumption> consumption;
        if (!CollectIngredients(bench, recipe, consumption))
        {
            resultMessage = "Missing required ingredients";
            SendCraftResult(player, false, resultMessage);
            return false;
        }

        ConsumeIngredients(consumption);
        FB_CraftJob job = CreateCraftJob(recipe, player, bench);
        bench.EnqueueCraftJob(job);
        NotifyQueueUpdate(bench);

        resultMessage = string.Format("Queued %1", recipe.Name);
        SendCraftResult(player, true, resultMessage);
        return true;
    }

    protected FB_Data BuildClientData()
    {
        FB_Data data = new FB_Data();
        if (!m_Database || !m_Database.Recipes)
            return data;

        int total = 0;
        map<string, int> categoryCounts = new map<string, int>();

        foreach (FB_Recipe recipe : m_Database.Recipes)
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

            data.Recipes.Insert(copy);
            Print(string.Format("[Frontier][CraftServerManager] Recipe sent id=%1 name=%2 category=%3", copy.Id, copy.Name, copy.Category));

            total++;
            string cat = copy.Category;
            if (cat == "")
                cat = "General";
            int existing;
            if (categoryCounts.Find(cat, existing))
                categoryCounts.Set(cat, existing + 1);
            else
                categoryCounts.Insert(cat, 1);
        }

        array<string> cats = categoryCounts.GetKeyArray();
        string summary = "[Frontier][CraftServerManager] BuildClientData recipes=" + total.ToString() + " categories=";
        for (int i = 0; i < cats.Count(); i++)
        {
            string catName = cats[i];
            int count = categoryCounts.Get(catName);
            string spacer = " ";
            if (i == 0)
                spacer = "";
            summary += string.Format("%1%2:%3", spacer, catName, count.ToString());
        }
        Print(summary);

        return data;
    }

    protected void RegisterWatcher(FB_Workbench bench, PlayerBase player)
    {
        if (!bench || !player || !player.GetIdentity())
            return;

        string playerId = player.GetIdentity().GetId();
        if (playerId == "")
            return;

        bench.RegisterWatcher(playerId);
    }

    void NotifyQueueUpdate(FB_Workbench bench)
    {
        if (!bench)
            return;

        ref array<string> watchers = bench.GetWatchers();
        if (!watchers || watchers.Count() == 0)
            return;

        FB_QueueState state = BuildQueueState(bench);
        for (int i = watchers.Count() - 1; i >= 0; i--)
        {
            string watcherId = watchers[i];
            PlayerBase target = FindPlayerById(watcherId);
            if (!target || !target.GetIdentity())
            {
                watchers.RemoveOrdered(i);
                continue;
            }
            SendQueueState(target, bench, state);
        }
    }

    protected void SendQueueState(PlayerBase player, FB_Workbench bench, FB_QueueState state = NULL)
    {
        if (!player || !player.GetIdentity())
            return;

        if (!state)
            state = BuildQueueState(bench);

        Param1<ref FB_QueueState> payload = new Param1<ref FB_QueueState>(state);
        GetGame().RPCSingleParam(player, FBRpc.SERVER_SEND_QUEUE_STATE, payload, true, player.GetIdentity());
    }

    protected FB_QueueState BuildQueueState(FB_Workbench bench)
    {
        FB_QueueState state = new FB_QueueState();
        if (!bench)
            return state;

        state.QueueLimit = bench.GetQueueLimit();
        state.WorkbenchTier = bench.GetWorkbenchTier();
        state.RequiredTier = bench.GetCraftingTierRequirement();
        state.CraftingUnlocked = bench.IsCraftingUnlocked();

        array<ref FB_CraftJob> queue = bench.GetCraftQueue();
        if (!queue)
            return state;

        for (int i = 0; i < queue.Count(); i++)
        {
            FB_CraftJob job = queue[i];
            if (!job)
                continue;

            FB_QueueEntry entry = new FB_QueueEntry();
            entry.RecipeId = job.RecipeId;
            entry.RecipeName = job.RecipeName;
            entry.RemainingTime = job.TimeRemaining;
            entry.TotalTime = job.TotalTime;
            entry.Active = job.Active;
            entry.Position = i;
            entry.RequestedBy = job.RequestedByName;
            state.Entries.Insert(entry);
        }

        return state;
    }

    protected PlayerBase FindPlayerById(string id)
    {
        if (!id || id == "")
            return NULL;

        array<Man> players = new array<Man>();
        GetGame().GetPlayers(players);
        for (int i = 0; i < players.Count(); i++)
        {
            PlayerBase player = PlayerBase.Cast(players[i]);
            if (!player || !player.GetIdentity())
                continue;

            if (player.GetIdentity().GetId() == id)
                return player;
        }
        return NULL;
    }

    protected FB_CraftJob CreateCraftJob(FB_Recipe recipe, PlayerBase player, FB_Workbench bench)
    {
        FB_CraftJob job = new FB_CraftJob();
        job.RecipeId = recipe.Id;
        job.RecipeName = recipe.Name;
        float baseTime = Math.Max(recipe.CraftTime, 1.0);
        float speedMultiplier = 1.0;
        if (bench)
            speedMultiplier = bench.GetCraftSpeedMultiplier();
        job.TotalTime = baseTime * speedMultiplier;
        job.TimeRemaining = job.TotalTime;
        job.Active = false;

        if (player && player.GetIdentity())
        {
            job.RequestedById = player.GetIdentity().GetId();
            job.RequestedByName = player.GetIdentity().GetName();
        }

        if (recipe.Results && recipe.Results.Count() > 0)
        {
            for (int i = 0; i < recipe.Results.Count(); i++)
            {
                FB_ResultItem res = recipe.Results[i];
                if (res)
                    job.Results.Insert(res.Clone());
            }
        }
        else if (recipe.Result != "")
        {
            job.Results.Insert(new FB_ResultItem(recipe.Result, Math.Max(1, recipe.ResultCount)));
        }

        return job;
    }

    void OnJobCompleted(FB_Workbench bench, FB_CraftJob job)
    {
        if (!bench || !job)
            return;

        GrantJobResults(bench, job);
        bench.UpdateRefresherTimeRemaining();
        NotifyQueueUpdate(bench);

        PlayerBase player = FindPlayerById(job.RequestedById);
        if (player)
        {
            SendCraftResult(player, true, string.Format("Crafted %1", job.RecipeName));
            SendWorkbenchData(player, bench);
        }
    }

    void OnJobProgress(FB_Workbench bench)
    {
        NotifyQueueUpdate(bench);
    }

    protected bool CollectIngredients(FB_Workbench bench, FB_Recipe recipe, out array<ref FB_IngredientConsumption> outConsumption)
    {
        outConsumption = new array<ref FB_IngredientConsumption>();
        if (!recipe.Ingredients || recipe.Ingredients.Count() == 0)
            return true;

        ref map<string, int> remaining = new map<string, int>();
        foreach (FB_Ingredient ingredient : recipe.Ingredients)
        {
            string key = ingredient.Type;
            key.ToLower();

            int current;
            if (remaining.Find(key, current))
                remaining.Set(key, current + ingredient.Count);
            else
                remaining.Insert(key, ingredient.Count);
        }

        TraverseInventory(bench, remaining, outConsumption, false);

        foreach (string typeKey, int qtyLeft : remaining)
        {
            if (qtyLeft > 0)
                return false;
        }

        return true;
    }

    protected bool HasRequiredToolsInCargo(FB_Workbench bench, array<string> required, out array<string> missing)
    {
        missing = new array<string>();

        if (!bench || !required || required.Count() == 0)
            return true;

        map<string, int> cargoCounts = new map<string, int>();
        GatherCargoTools(bench, cargoCounts);

        for (int i = 0; i < required.Count(); i++)
        {
            string requiredType = required[i];
            string lookup = requiredType;
            lookup.ToLower();

            int available;
            if (!cargoCounts.Find(lookup, available) || available <= 0)
                missing.Insert(requiredType);
        }

        return missing.Count() == 0;
    }

    protected void GatherCargoTools(EntityAI entity, map<string, int> counts)
    {
        if (!entity || !counts)
            return;

        ItemBase item = ItemBase.Cast(entity);
        if (item && !item.IsRuined())
        {
            string typeName = item.GetType();
            string key = typeName;
            key.ToLower();

            int quantity = 1;
            if (item.HasQuantity())
                quantity = Math.Max(1, Math.Floor(item.GetQuantity()));

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
            int cargoCount = cargo.GetItemCount();
            for (int i = 0; i < cargoCount; i++)
            {
                EntityAI cargoEntity = cargo.GetItem(i);
                if (cargoEntity)
                    GatherCargoTools(cargoEntity, counts);
            }
        }
    }

    protected void TraverseInventory(EntityAI entity, map<string, int> remaining, array<ref FB_IngredientConsumption> consumption, bool includeEntity = true)
    {
        if (!entity || !remaining)
            return;

        if (includeEntity)
        {
            ItemBase asItem = ItemBase.Cast(entity);
            if (asItem)
                ConsiderEntityForConsumption(asItem, remaining, consumption);
        }

        if (!entity.GetInventory())
            return;

        CargoBase cargo = entity.GetInventory().GetCargo();
        if (cargo)
        {
            int cargoCount = cargo.GetItemCount();
            for (int i = 0; i < cargoCount; i++)
            {
                EntityAI cargoEntity = cargo.GetItem(i);
                if (cargoEntity)
                    TraverseInventory(cargoEntity, remaining, consumption, true);
            }
        }

        int attachments = entity.GetInventory().AttachmentCount();
        for (int j = 0; j < attachments; j++)
        {
            EntityAI attachment = entity.GetInventory().GetAttachmentFromIndex(j);
            if (attachment)
                TraverseInventory(attachment, remaining, consumption, true);
        }
    }

    protected string JoinStrings(array<string> values, string separator)
    {
        if (!values || values.Count() == 0)
            return "";

        string text = values[0];
        for (int i = 1; i < values.Count(); i++)
        {
            text += separator;
            text += values[i];
        }
        return text;
    }

    protected void ConsiderEntityForConsumption(ItemBase item, map<string, int> remaining, array<ref FB_IngredientConsumption> consumption)
    {
        if (!item || item.IsRuined())
            return;

        string typeName = item.GetType();
        typeName.ToLower();

        int needCount;
        if (!remaining || !remaining.Find(typeName, needCount))
            return;

        if (needCount <= 0)
            return;

        FB_IngredientConsumption use = new FB_IngredientConsumption();
        use.Item = item;

        if (item.HasQuantity())
        {
            int available = Math.Floor(item.GetQuantity());
            int take = Math.Min(available, needCount);
            if (take <= 0)
                return;

            use.Quantity = take;
            use.DestroyWhole = take >= available;
            remaining.Set(typeName, needCount - take);
        }
        else
        {
            use.Quantity = 1;
            use.DestroyWhole = true;
            remaining.Set(typeName, needCount - 1);
        }

        consumption.Insert(use);
    }

    protected void ConsumeIngredients(array<ref FB_IngredientConsumption> consumption)
    {
        if (!consumption)
            return;

        foreach (FB_IngredientConsumption entry : consumption)
        {
            if (!entry || !entry.Item)
                continue;

            if (entry.DestroyWhole)
            {
                GetGame().ObjectDelete(entry.Item);
            }
            else
            {
                entry.Item.AddQuantity(-entry.Quantity);
            }
        }
    }

    protected void GrantResults(FB_Workbench bench, FB_Recipe recipe)
    {
        if (!bench || !recipe)
            return;

        if (recipe.Results && recipe.Results.Count() > 0)
        {
            foreach (FB_ResultItem res : recipe.Results)
            {
                if (res)
                    GrantResultItem(bench, res);
            }
        }
        else
        {
            GrantResultItem(bench, new FB_ResultItem(recipe.Result, recipe.ResultCount));
        }
    }

    protected void GrantJobResults(FB_Workbench bench, FB_CraftJob job)
    {
        if (!bench || !job)
            return;

        if (job.Results && job.Results.Count() > 0)
        {
            for (int i = 0; i < job.Results.Count(); i++)
            {
                FB_ResultItem res = job.Results[i];
                if (res)
                    GrantResultItem(bench, res);
            }
        }
        else
        {
            if (m_Database)
            {
                FB_Recipe recipe = m_Database.FindById(job.RecipeId);
                if (recipe)
                    GrantResults(bench, recipe);
            }
        }
    }

    protected void GrantResultItem(FB_Workbench bench, FB_ResultItem result)
    {
        if (!bench || !result)
            return;

        string typeName = result.GetSpawnType();
        if (!typeName || typeName == "")
            typeName = result.Type;
        if (!typeName || typeName == "")
            return;

        int remaining = result.GetSafeCount();
        ItemBase preview = ItemBase.Cast(GetGame().CreateObject(typeName, vector.Zero, true));
        float q_cur, q_min, q_max;
        if (preview)
        {
            QuantityConversions.GetItemQuantity(preview, q_cur, q_min, q_max);
            GetGame().ObjectDelete(preview);
        }

        if (q_max <= 0)
            q_max = 1;

        float targetHealth = result.GetClampedHealth();

        while (remaining > 0)
        {
            ItemBase newItem = ItemBase.Cast(bench.GetInventory().CreateInInventory(typeName));
            if (!newItem)
                newItem = ItemBase.Cast(GetGame().CreateObject(typeName, bench.GetPosition(), false));

            if (!newItem)
                break;

            ApplyResultHealth(newItem, targetHealth);

            if (newItem.IsInherited(Magazine))
            {
                int give = Math.Min(remaining, q_max);
                Magazine.Cast(newItem).ServerSetAmmoCount(give);
                remaining -= give;
            }
            else if (newItem.HasQuantity())
            {
                int giveQty = Math.Min(remaining, q_max);
                newItem.SetQuantity(giveQty);
                remaining -= giveQty;
            }
            else
            {
                remaining -= 1;
            }
        }
    }

    protected void SendCraftResult(PlayerBase player, bool success, string message)
    {
        if (!player || !player.GetIdentity())
            return;

        FBCraftResult result = new FBCraftResult();
        result.Success = success;
        result.Message = message;
        Param1<ref FBCraftResult> payload = new Param1<ref FBCraftResult>(result);
        GetGame().RPCSingleParam(player, FBRpc.SERVER_SEND_CRAFT_RESULT, payload, true, player.GetIdentity());
    }

    protected void ApplyResultHealth(ItemBase item, float healthPercent)
    {
        if (!item)
            return;

        float normalized = Math.Clamp(healthPercent, 1, 100) / 100;
        item.SetHealth01("", "", normalized);
    }
}

ref CraftServerManager g_CraftServerManager;
