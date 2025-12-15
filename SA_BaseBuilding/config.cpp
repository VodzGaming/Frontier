class CfgPatches
{
	class SA_BaseBuilding
	{
		units[] = {"SA_T1_Wall_Hologram","SA_T1_Wall_Kit","SA_T1_Wall","SA_BuildPermit"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data","RPC_Scripts","DZ_Gear_Tools","DZ_Gear_Camping"};
	};
};
class CfgMods
{
	class SA_BaseBuilding
	{
		type = "mod";
		author = "Potato";
		name = "SA_BaseBuilding";
		dir = "SA_BaseBuilding";
		defines[] = {"SAB"};
		dependencies[] = {"Game","World","Mission"};
		class defs
		{
			class gameScriptModule
			{
				value = "";
				files[] = {"SA_BaseBuilding/scripts/Common","SA_BaseBuilding/scripts/3_Game"};
			};
			class worldScriptModule
			{
				value = "";
				files[] = {"SA_BaseBuilding/scripts/Common","SA_BaseBuilding/scripts/4_World"};
			};
			class missionScriptModule
			{
				value = "";
				files[] = {"SA_BaseBuilding/scripts/Common","SA_BaseBuilding/scripts/5_Mission"};
			};
		};
	};
};
class CfgSlots
{
	class Slot_Blueprint
	{
		name = "Blueprint";
		displayName = "Blueprint";
		ghostIcon = "set:bbp_icons image:bbp_blueprint";
	};
};
class CfgVehicles
{
	class Inventory_Base;
	class KitBase;
	class BaseBuildingBase;
	class Hammer;
	class SA_Admin_Hammer: Hammer
	{
		displayName = "ADMIN HAMMER";
		descriptionShort = "No materials required for building, only dirty cheaters use this item.";
		itemSize[] = {2,2};
	};
	class BarbedWire: Inventory_Base
	{
		inventorySlot[] += {"Material_SA_BarbedWire"};
	};
	class SA_KIT_BASE: KitBase
	{
		scope = 0;
		isMeleeWeapon = 0;
		weight = 300;
		absorbency = 0;
		rotationFlags = 90;
		itemSize[] = {4,2};
		lootCategory = "Materials";
		reversed = 0;
		itemBehaviour = 2;
	};
	class SA_WALL_BASE: BaseBuildingBase
	{
		scope = 0;
		hiddenSelections[] = {"xlights_glass_r","xlights_glass_g","xlights_glass_b","xlights_glass_y"};
		overrideDrawArea = "3.0";
		forceFarBubble = "true";
		lootCategory = "Crafted";
		carveNavmesh = 1;
		weight = 60000;
		itemSize[] = {6,6};
		physLayer = "item_large";
		createProxyPhysicsOnInit = "false";
		createdProxiesOnInit[] = {"Deployed","Hologram"};
		rotationFlags = 2;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 100;
				};
			};
			class GlobalArmor
			{
				class Projectile
				{
					class Health
					{
						damage = 0;
					};
					class Blood
					{
						damage = 0;
					};
					class Shock
					{
						damage = 0;
					};
				};
				class FragGrenade
				{
					class Health
					{
						damage = 0;
					};
					class Blood
					{
						damage = 0;
					};
					class Shock
					{
						damage = 0;
					};
				};
			};
		};
		class EnergyManager
		{
			hasIcon = 1;
			energyUsagePerSecond = 0;
			plugType = 1;
			attachmentAction = 1;
			updateInterval = 5;
		};
		class PointLights
		{
			class PointLight
			{
				color[] = {1,1,1,0.05};
				ambient[] = {0.01,0.01,0.01,0.01};
				position = "light";
				hitpoint = "bulb";
				selection = "bulb";
				size = 0;
				radius = 10;
				brightness = 5;
				dayLight = 1;
				heatHazeRadius = 0;
				heatHazePower = 0;
				fireEffect = 0;
				fireEffectOctaves = 0;
				fireEffectPersistence = 0;
				fireEffectFract = 0;
			};
		};
	};
	class SA_GATE_BASE: SA_WALL_BASE{};
	class SA_DOOR_BASE: SA_WALL_BASE{};
	class SA_T1_Wall_Hologram: SA_KIT_BASE
	{
		scope = 2;
		displayName = "SA T1 Wall (Hologram)";
		descriptionShort = "Placement preview for an SA wall frame.";
		model = "\SA_BaseBuilding\models\T1_Wall\T1_Wall_Frame.p3d";
		hiddenSelections[] = {"camo"};
		hiddenSelectionsTextures[] = {"#(argb,8,8,3)color(0.1,0.85,1,0.25,ca)"};
		overrideDrawArea = "3.0";
		carveNavmesh = 1;
		lootCategory = "Tents";
		placement = "ForceSlopeOnTerrain";
		physLayer = "item_large";
		rotationFlags = 2;
		reversed = 1;
	};
	class SA_T1_Wall_Kit: SA_KIT_BASE
	{
		scope = 2;
		displayName = "SA T1 Wall Kit";
		descriptionShort = "A compact kit containing the components for an SA wall frame.";
		model = "\dz\gear\camping\fence_kit.p3d";
		projectionTypename = "SA_T1_Wall_Hologram";
	};
	class SA_T1_Wall: SA_WALL_BASE
	{
		scope = 2;
		displayName = "SA T1 Wall";
		descriptionShort = "A reinforced vanilla-style wall frame.";
		model = "\SA_BaseBuilding\models\T1_Wall\T1_Wall_Frame.p3d";
		attachments[] = {"Material_WoodenPlanks","Material_Nails"};
		class AnimationSources;
		class GUIInventoryAttachmentsProps
		{
			class Materials
			{
				name = "Materials";
				description = "";
				attachmentSlots[] = {"Material_WoodenPlanks","Material_Nails"};
				icon = "cat_bb_material";
				selection = "wall_build";
			};
		};
		class Construction
		{
			class wall_build
			{
				class frame
				{
					name = "Frame";
					is_base = 1;
					id = 1;
					required_parts[] = {};
					conflicted_parts[] = {};
					build_action_type = 2;
					dismantle_action_type = 2;
					material_type = 2;
					class Materials
					{
						class Material1
						{
							type = "WoodenPlank";
							slot_name = "Material_WoodenPlanks";
							quantity = 10;
						};
						class Material2
						{
							type = "Nail";
							slot_name = "Material_Nails";
							quantity = 20;
						};
					};
				};
			};
		};
	};
	class SA_BuildPermit: Inventory_Base
	{
		scope = 2;
		displayName = "Construction Permit";
		descriptionShort = "Temporary paper permit while the custom model is WIP.";
		model = "\dz\gear\consumables\paper.p3d";
		physLayer = "item_small";
		weight = 1;
		itemSize[] = {1,1};
		rotationFlags = 1;
	};
};
class CfgNonAIVehicles
{
	class ProxyBaseBuilding;
	class ProxyBlueprint: ProxyBaseBuilding
	{
		model = "\dz\gear\consumables\paper.p3d";
		inventorySlot[] = {"Blueprint"};
	};
};
