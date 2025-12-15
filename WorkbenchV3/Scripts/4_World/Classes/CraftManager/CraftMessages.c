enum FBCraftCommand
{
    REQUEST_DATA = 0,
    EXECUTE_RECIPE
}

class CraftItem
{
    string RecipeId;
}

class FBCraftResult
{
    bool Success;
    string Message;
}
