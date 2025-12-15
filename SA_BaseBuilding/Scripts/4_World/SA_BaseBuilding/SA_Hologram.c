modded class Hologram
{
	override protected vector GetProjectionEntityPosition(PlayerBase player)
	{
		vector projectedPos = super.GetProjectionEntityPosition(player);

		PlayerBase placer = player;
		if (!placer)
		{
			return projectedPos;
		}

		ItemBase itemInHands = ItemBase.Cast(placer.GetHumanInventory().GetEntityInHands());
		SA_KIT_BASE saKit = SA_KIT_BASE.Cast(itemInHands);
		if (!saKit)
		{
			return projectedPos;
		}

		SA_SnapResult result;
		if (SA_SnapSystem.TrySnapToNearbyStructure(saKit, projectedPos, result))
		{
			m_AlignToTerrain = false;
			SetIsFloating(false);

			if (m_Projection)
			{
				m_Projection.SetOrientation(result.Orientation);
			}

			return result.Position;
		}

		return projectedPos;
	}
}
