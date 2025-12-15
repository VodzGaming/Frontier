
class SA_T1_Wall_Kit extends SA_BuildKitBase
{
	override string GetResultEntityType()
	{
		return "SA_T1_Wall";
	}
}

class SA_T1_Wall extends SA_WALL_BASE
{
	override string GetConstructionKitType()
	{
		return "SA_T1_Wall_Kit";
	}

	protected vector GetSnapPoint( string pointName )
	{
		vector worldPos = GetPosition();

		if ( MemoryPointExists( pointName ) )
		{
			vector localPos = GetMemoryPointPos( pointName );
			worldPos = ModelToWorld( localPos );
		}

		return worldPos;
	}

	vector GetSnappingPointWall_150()
	{
		return GetSnapPoint( "snap_left_outer" );
	}

	vector GetSnappingPointWall_600()
	{
		return GetSnapPoint( "snap_right_outer" );
	}

	vector GetSnappingPointFloor_150()
	{
		return GetSnapPoint( "snap_bottom_center" );
	}

	vector GetSnappingPointFloor_600()
	{
		return GetSnapPoint( "snap_floor_left" );
	}

	override void SetPartsAfterStoreLoad()
	{
		Construction construction = GetConstruction();
		if (!construction)
		{
			return;
		}

		ConstructionPart basePart = construction.GetBaseConstructionPart();
		if (!basePart)
		{
			SynchronizeBaseState();
			return;
		}

		super.SetPartsAfterStoreLoad();
	}
}
