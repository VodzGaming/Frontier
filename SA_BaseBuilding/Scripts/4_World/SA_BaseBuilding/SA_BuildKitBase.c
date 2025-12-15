class SA_BuildKitBase extends SA_KIT_BASE
{
	// Every concrete kit just needs to specify the entity it should spawn when deployed.
	protected string GetResultEntityType()
	{
		return string.Empty;
	}

	// Optional post-spawn hook in case derived kits need to tweak the placed entity.
	protected void AfterPlacementSpawn(Man player, EntityAI placedEntity)
	{
	}

	override void OnPlacementComplete(Man player, vector position = "0 0 0", vector orientation = "0 0 0")
	{
		super.OnPlacementComplete(player, position, orientation);

		if (!GetGame() || !GetGame().IsServer())
		{
			return;
		}

		string resultType = GetResultEntityType();
		if (resultType == string.Empty)
		{
			return;
		}

		EntityAI placedObject = EntityAI.Cast(GetGame().CreateObjectEx(resultType, position, ECE_PLACE_ON_SURFACE));
		if (!placedObject)
		{
			return;
		}

		placedObject.SetPosition(position);
		placedObject.SetOrientation(orientation);
		AfterPlacementSpawn(player, placedObject);
	}

	override string GetDeploySoundset()
	{
		return "putDown_FenceKit_SoundSet";
	}

	override string GetLoopDeploySoundset()
	{
		return "Shelter_Site_Build_Loop_SoundSet";
	}
}
