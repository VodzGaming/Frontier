modded class ItemBase {
	bool CanFindFBWorkBench() {
		float searchRange[] = { FindTerritoryRadius() };
		Object m_FB_Workbench = null;
		foreach (float range : searchRange) {
			m_FB_Workbench = FindClosestByType("FB_Workbench", player.GetPosition(), range);
			if (m_FB_Workbench) return true;
		} return false;
	}
	Object FindClosestByType(string typeName, vector aroundPos, float radius) {
		array<Object> arr = new array<Object>;
		GetGame().GetObjectsAtPosition(aroundPos, radius, arr, null);
		Object best = null; float bestD2 = 3.4e38;
		for (int i = 0; i < arr.Count(); i++) {
			Object o = arr[i]; if (!o) continue;
			if (!o.IsKindOf(typeName)) continue;
			float d2 = vector.DistanceSq(o.GetPosition(), aroundPos);
			if (d2 < bestD2) { bestD2 = d2; best = o; }
		} return best;
	}
    float FindTerritoryRadius() {
        ModuleFrontierConfigHub hub = ModuleFrontierConfigHub.Get();
        float t_Radius = GameConstants.REFRESHER_RADIUS;
        if (hub) t_Radius = hub.GetTerritoryRadius();
        float f_Radius = t_Radius;
        return Math.Max(f_Radius, 1);
    }
}
