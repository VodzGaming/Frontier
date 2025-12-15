const float SA_SNAP_MAX_DISTANCE = 2.5;

class SA_SnapResult
{
	vector Position;
	vector Orientation;
}

class SA_SnapSystem
{
	static bool TrySnapToNearbyStructure(SA_KIT_BASE kit, vector desiredPos, out SA_SnapResult result)
	{
		if (!kit)
		{
			return false;
		}

		// For now we only support wall kits, but this can expand as new kits come online.
		SA_T1_Wall_Kit wallKit = SA_T1_Wall_Kit.Cast(kit);
		if (wallKit)
		{
			return SnapWallKit(wallKit, desiredPos, result);
		}

		return false;
	}

	protected static bool SnapWallKit(SA_T1_Wall_Kit kit, vector desiredPos, out SA_SnapResult result)
	{
		array<Object> objects = new array<Object>();
		array<CargoBase> proxy = new array<CargoBase>();
		GetGame().GetObjectsAtPosition(desiredPos, SA_SNAP_MAX_DISTANCE, objects, proxy);

		float bestDistance = SA_SNAP_MAX_DISTANCE;
		SA_T1_Wall bestWall;
		int bestAnchor = -1;
		vector bestPoint = "0 0 0";

		foreach (Object obj : objects)
		{
			SA_T1_Wall candidate = SA_T1_Wall.Cast(obj);
			if (!candidate)
			{
				continue;
			}

			vector anchorPoints[4];
			anchorPoints[0] = candidate.GetSnappingPointWall_150();
			anchorPoints[1] = candidate.GetSnappingPointWall_600();
			anchorPoints[2] = candidate.GetSnappingPointFloor_150();
			anchorPoints[3] = candidate.GetSnappingPointFloor_600();

			for (int i = 0; i < 4; i++)
			{
				vector anchorPos = anchorPoints[i];
				float dist = vector.Distance(anchorPos, desiredPos);
				if (dist < bestDistance)
				{
					bestDistance = dist;
					bestPoint = anchorPos;
					bestAnchor = i;
					bestWall = candidate;
				}
			}
		}

		if (!bestWall || bestAnchor == -1)
		{
			return false;
		}

		result = new SA_SnapResult();
		result.Position = bestPoint;
		result.Orientation = GetOrientationForAnchor(bestWall.GetOrientation(), bestAnchor);
		return true;
	}

	protected static vector GetOrientationForAnchor(vector baseOrientation, int anchorIndex)
	{
		vector orientation = baseOrientation;

		// Anchors [1] and [3] represent the "right" side on our T1 wall memory layout.
		if (anchorIndex == 1 || anchorIndex == 3)
		{
			orientation[0] = orientation[0];
			orientation[1] = orientation[1] + 180;
			orientation[2] = orientation[2];
		}

		orientation[1] = Math.NormalizeAngle(orientation[1]);
		return orientation;
	}
}
