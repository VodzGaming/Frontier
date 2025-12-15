class CfgPatches
{
	class Frontier_Workbench
	{
		units[] = {"FB_Workbench","FB_Workbench_Kit","FB_TerritoryBeacon"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data","DZ_Scripts","JM_CF_Scripts","DZ_Gear_Tools","DZ_Gear_Consumables","DZ_Weapons_Melee","DZ_Gear_Camping"};
	};
};

class CfgMods
{
	class Frontier_Workbench
	{
		dir = "WorkbenchV3";
		picture = "";
		action = "";
		hideName = 1;
		hidePicture = 1;
		name = "Frontier Workbench";
		author = "Veldaric";
		version = "1.1";
		type = "mod";
		dependencies[] = {"Game","World","Mission"};
		class defs
		{
			class widgetStyles
			{
				files[] = {"WorkbenchV3/datasets/gui/LionCraftStyles.styles"};
			};
			class imageSets
			{
				files[] = {"WorkbenchV3/datasets/gui/lion_ui_components.imageset"};
			};
			class gameScriptModule  
			{
				files[] = {"WorkbenchV3/Scripts/3_Game"}; 
			};
			class worldScriptModule 
			{ 
				files[] = {"WorkbenchV3/Scripts/4_World"};
			};
            class missionScriptModule
            {
				files[]={"WorkbenchV3/Scripts/5_Mission/mission"}; 
            }; 
		};
	};
};
class CfgVehicles
{
	class Inventory_Base;
	class Container_Base;
	class FB_Workbench: Inventory_Base
	{
		scope=2;
		displayName="Frontier Workbench";
		descriptionShort="A crafting workbench used with the Frontier system.";
		model = "\WorkbenchV3\models\workbench\workbench.p3d";
		forceFarBubble = "true";
		physLayer = "item_large";
        hiddenSelections[]={"camo"};
		 hiddenSelectionsTextures[] = {"WorkbenchV3\models\workbench\data\Camo_co.paa"};
		weight = 1000;
		itemSize[] = {10,10};
		itemBehaviour = 0;
		class Cargo
		{
			itemsCargoSize[] = {10,50};
			openable = 0;
			allowOwnedCargoManipulation = 1;
		};
		attachments[] = {"FB_Battery_1","FB_Battery_2","FB_Battery_3","FB_ModuleSlot_Power","FB_ModuleSlot_Utility"};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 5000;
					healthLevels[] = {
						{1,{"WorkbenchV3\models\workbench\data\Camo.rvmat"}},
						{0.7,{"WorkbenchV3\models\workbench\data\Camo.rvmat"}},
						{0.5,{"WorkbenchV3\models\workbench\data\Camo_damage.rvmat"}},
						{0.3,{"WorkbenchV3\models\workbench\data\Camo_damage.rvmat"}},
						{0,{"WorkbenchV3\models\workbench\data\Camo_destruct.rvmat"}}
					};
				};
			};
		};
		soundImpactType = "metal";
	};
	class FB_WorkbenchHologram: Inventory_Base
	{
		scope = 2;
		displayName = "Hologram";
		descriptionShort = "hologram";
		model = "\WorkbenchV3\models\workbench\workbench.p3d";
		hiddenSelections[] = {"camo"};
		hiddenSelectionsTextures[] = {"#(argb,8,8,3)color(0.1,0.85,1,0.25,ca)"};
		overrideDrawArea = "3.0";
		itemBehaviour = 2;
		attachments[] = {};
		carveNavmesh = 1;
		lootCategory = "Tents";
		placement = "ForceSlopeOnTerrain";
		physLayer = "item_large";
		rotationFlags = 2;
	};
	class TerritoryFlagKit;
	class FB_Workbench_KitBase: TerritoryFlagKit
	{
		scope = 0;
		model = "\DZ\gear\camping\watchtower_kit_folded.p3d";
		itemSize[] = {1,4};
		carveNavmesh = 1;
		canBeDigged = 0;
		itemsCargoSize[] = {0,0};
		simulation = "inventoryItem";
		physLayer = "item_large";
		rotationFlags = 2;
		weight = 5000;
		itemBehaviour = 2;
		attachments[] = {};
		projectionTypename = "FB_WorkbenchHologram";
		class Cargo
		{
			itemsCargoSize[] = {0,0};
			openable = 0;
			allowOwnedCargoManipulation = 1;
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 5000;
				};
			};
			class GlobalArmor
			{
				class FragGrenade
				{
					class Health
					{
						damage = 8;
					};
					class Blood
					{
						damage = 8;
					};
					class Shock
					{
						damage = 8;
					};
				};
			};
		};
	};
	class FB_Workbench_Kit: FB_Workbench_KitBase
	{
		scope = 2;
		displayName = "Workbench Kit";
		descriptionShort = "A cardboard box used to transport small items. Place to deploy item inside. The items can be dismantled with a screwdriver after deployment.";
		projectionTypename = "FB_WorkbenchHologram";
	};
	class FB_Battery_1: Inventory_Base
	{
		scope = 2;
		displayName = "Territory Maintenance Battery 72V";
		descriptionShort = "These high-capacity batteries are crucial for keeping your territory operational. They power your structures and defenses, preventing decay and ensuring your base stays secure. Regularly check and replace them to maintain your territory’s integrity and defense.";
		quantityBar = 1;
		varQuantityInit = 72000;
		varQuantityMin = 0;
		varQuantityMax = 72000;
		varQuantityDestroyOnMin = 1;
		destroyOnEmpty = 1;
		model = "\DZ\vehicles\parts\battery_car.p3d";
		inventorySlot[] = {"FB_Battery_1","FB_Battery_2","FB_Battery_3"};
		itemSize[] = {2,1};
		weight = 300;
		rotationFlags = 16;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 500;
					healthLevels[] = {{1.0,{}},{0.7,{}},{0.5,{}},{0.3,{}},{0.0,{}}};
				};
			};
		};
	};
	class FB_Battery_2: Inventory_Base
	{
		scope = 2;
		displayName = "Territory Maintenance Battery 144V";
		descriptionShort = "These high-capacity batteries are crucial for keeping your territory operational. They power your structures and defenses, preventing decay and ensuring your base stays secure. Regularly check and replace them to maintain your territory’s integrity and defense.";
		quantityBar = 1;
		varQuantityInit = 144000;
		varQuantityMin = 0;
		varQuantityMax = 144000;
		varQuantityDestroyOnMin = 1;
		destroyOnEmpty = 1;
		model = "\DZ\vehicles\parts\battery_car.p3d";
		inventorySlot[] = {"FB_Battery_1","FB_Battery_2","FB_Battery_3"};
		itemSize[] = {2,1};
		weight = 300;
		rotationFlags = 16;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 500;
					healthLevels[] = {{1.0,{}},{0.7,{}},{0.5,{}},{0.3,{}},{0.0,{}}};
				};
			};
		};
	};
	class FB_Battery_3: Inventory_Base
	{
		scope = 2;
		displayName = "Territory Maintenance Battery 216V";
		descriptionShort = "These high-capacity batteries are crucial for keeping your territory operational. They power your structures and defenses, preventing decay and ensuring your base stays secure. Regularly check and replace them to maintain your territory’s integrity and defense.";
		quantityBar = 1;
		varQuantityInit = 216000;
		varQuantityMin = 0;
		varQuantityMax = 216000;
		varQuantityDestroyOnMin = 1;
		destroyOnEmpty = 1;
		model = "\DZ\vehicles\parts\battery_car.p3d";
		inventorySlot[] = {"FB_Battery_1","FB_Battery_2","FB_Battery_3"};
		itemSize[] = {2,1};
		weight = 300;
		rotationFlags = 16;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 500;
					healthLevels[] = {{1.0,{}},{0.7,{}},{0.5,{}},{0.3,{}},{0.0,{}}};
				};
			};
		};
	};
	class FB_WorkbenchModule_Power: Inventory_Base
	{
		scope = 2;
		displayName = "Workbench Power Optimizer";
		descriptionShort = "Reduces overall power consumption when attached to a workbench.";
		model = "\dz\vehicles\parts\battery_truck.p3d";
		inventorySlot[] = {"FB_ModuleSlot_Power"};
		weight = 1500;
		itemSize[] = {2,2};
	};
	class FB_WorkbenchModule_Radius: Inventory_Base
	{
		scope = 2;
		displayName = "Territory Uplink Amplifier";
		descriptionShort = "Extends protected radius while attached to a workbench.";
		model = "\dz\gear\tools\sewing_kit.p3d";
		inventorySlot[] = {"FB_ModuleSlot_Utility"};
		weight = 1200;
		itemSize[] = {2,2};
	};
	class FB_WorkbenchModule_Queue: Inventory_Base
	{
		scope = 2;
		displayName = "Workbench Scheduler";
		descriptionShort = "Adds extra job buffer capacity.";
		model = "\dz\gear\consumables\paper.p3d";
		inventorySlot[] = {"FB_ModuleSlot_Utility"};
		weight = 1000;
		itemSize[] = {2,1};
	};
};
class CfgSlots
{
	class Slot_FB_Battery_1
	{ 
		name="FB_Battery_1";
		displayName="Battery";
		ghostIcon="batterycharger";
	};
	class Slot_FB_Battery_2
	{
		name="FB_Battery_2";
		displayName="Battery";
		ghostIcon="batterycharger";
	};
	class Slot_FB_Battery_3
	{
		name="FB_Battery_3";
		displayName="Battery";
		ghostIcon="batterycharger";
	};
	class Slot_FB_ModuleSlot_Power
	{
		name="FB_ModuleSlot_Power";
		displayName="Power Module";
		ghostIcon="powersupply";
	};
	class Slot_FB_ModuleSlot_Utility
	{
		name="FB_ModuleSlot_Utility";
		displayName="Utility Module";
		ghostIcon="plug";
	};
};
