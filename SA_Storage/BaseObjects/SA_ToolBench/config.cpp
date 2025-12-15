class CfgPatches
{
	class Abyss_Tool_Bench
	{
		units[] = {"SA_ToolBench_Black","SA_ToolBench_Black_Kit"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data","A6_BaseClasses","A6_BaseKitBoxes"};
	};
};
class CfgVehicles
{
	class A6_Item;
	class A6_Kit;
	class SA_ToolBench_Black_Kit: A6_Kit
	{
		scope = 2;
		weight = 6500;
		displayName = "(Black) Tool Bench Kit";
		model = "Spud_Stuff\SA_Storage\BaseObjects\SA_BaseKitBox\kitbox.p3d";
		descriptionShort = "A Kit that containts a Crate. Can be disassembled with a screwdriver.";
		itemSize[] = {6,4};
	};
	class SA_ToolBench_Blue_Kit: A6_Kit
	{
		scope = 2;
		weight = 6500;
		displayName = "(Blue) Tool Bench Kit";
		model = "Spud_Stuff\SA_Storage\BaseObjects\SA_BaseKitBox\kitbox.p3d";
		descriptionShort = "A Kit that containts a Crate. Can be disassembled with a screwdriver.";
		itemSize[] = {6,4};
	};
	class SA_ToolBench_Red_Kit: A6_Kit
	{
		scope = 2;
		weight = 6500;
		displayName = "(Red) Tool Bench Kit";
		model = "Spud_Stuff\SA_Storage\BaseObjects\SA_BaseKitBox\kitbox.p3d";
		descriptionShort = "A Kit that containts a Crate. Can be disassembled with a screwdriver.";
		itemSize[] = {6,4};
	};
	class SA_ToolBench_White_Kit: A6_Kit
	{
		scope = 2;
		weight = 6500;
		displayName = "(White) Tool Bench Kit";
		model = "Spud_Stuff\SA_Storage\BaseObjects\SA_BaseKitBox\kitbox.p3d";
		descriptionShort = "A Kit that containts a Crate. Can be disassembled with a screwdriver.";
		itemSize[] = {6,4};
	};
	class SA_ToolBench_Yellow_Kit: A6_Kit
	{
		scope = 2;
		weight = 6500;
		displayName = "(Yellow) Tool Bench Kit";
		model = "Spud_Stuff\SA_Storage\BaseObjects\SA_BaseKitBox\kitbox.p3d";
		descriptionShort = "A Kit that containts a Crate. Can be disassembled with a screwdriver.";
		itemSize[] = {6,4};
	};
	class A6_Storage_Base;
	class SA_ToolBench_Base: A6_Storage_Base
	{
		scope = 0;
		displayName = "Tool Bench (Black)";
		descriptionShort = "The Tool Bench holds a bunch of tools and has 40 slots of storage, can be dismatled by pliers.";
		model = "Spud_Stuff\SA_Storage\BaseObjects\SA_ToolBench\SA_ToolBench.p3d";
		rotationFlags = 2;
		physLayer = "item_large";
		itemBehaviour = 0;
		attachments[] = {"HandSaw1","HandSaw2","HandSaw3","HandSaw4","HandSaw5","HandSaw6","Hatchet1","Hatchet2","Hatchet3","Pliers1","Pliers2","Screwdriver1","Screwdriver2","Axe","Pickaxe","Melee"};
		simpleHiddenSelections[] = {"slot_melee_melee","slot_melee_rifle"};
		hiddenSelectionsTextures[] = {"Spud_Stuff\SA_Storage\BaseObjects\SA_ToolBench\data\SA_ToolBench_Black.paa"};
		hiddenSelections[] = {"abyss"};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 100000;
					healthLevels[] = {{1.0,{"Spud_Stuff\SA_Storage\BaseObjects\SA_ToolBench\data\Abyss_Tool_Bench.rvmat"}},{0.7,{"Spud_Stuff\SA_Storage\BaseObjects\SA_ToolBench\data\Abyss_Tool_Bench.rvmat"}},{0.5,{"Spud_Stuff\SA_Storage\BaseObjects\SA_ToolBench\data\Abyss_Tool_Bench_damage.rvmat"}},{0.3,{"Spud_Stuff\SA_Storage\BaseObjects\SA_ToolBench\data\Abyss_Tool_Bench_damage.rvmat"}},{0.0,{"Spud_Stuff\SA_Storage\BaseObjects\SA_ToolBench\data\Abyss_Tool_Bench_destruct.rvmat"}}};
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
		class GUIInventoryAttachmentsProps
		{
			class CodeLock
			{
				name = "CodeLock";
				description = "";
				attachmentSlots[] = {"CodeLock"};
				icon = "cat_bb_attachments";
			};
			class Lock
			{
				name = "Lock";
				description = "";
				attachmentSlots[] = {"Att_CombinationLock"};
				icon = "cat_bb_attachments";
			};
			class BigTools
			{
				name = "Big Tools";
				description = "Axe, Pickaxe, Shovel";
				attachmentSlots[] = {"Shovel1","Pickaxe1","Axe1","SledgeHammer"};
				icon = "set:rag_baseitems image:SledgeHammer";
			};
			class SmallTools
			{
				name = "Small Tools";
				description = "2x Screwdrivers, 2x Pliers, 5x Hatchets, 3x Saws, 3x Metal Wire";
				attachmentSlots[] = {"Pliers1","Pliers2","Screwdriver1","Screwdriver2","Saw1","Saw2","Saw3","Hatchet2","Hatchet3","Hatchet1"};
				icon = "set:rag_baseitems image:Pliers";
			};
		};
		class Cargo
		{
			itemsCargoSize[] = {5,8};
			openable = 1;
			allowOwnedCargoManipulation = 1;
		};
		class AnimEvents
		{
			class SoundWeapon
			{
				class movement
				{
					soundSet = "seachest_movement_SoundSet";
					id = 1;
				};
				class pickUpItem_Light
				{
					soundSet = "pickUpSeaChest_SoundSet";
					id = 796;
				};
				class pickUpItem
				{
					soundSet = "pickUpSeaChest_SoundSet";
					id = 797;
				};
				class drop
				{
					soundset = "seachest_drop_SoundSet";
					id = 898;
				};
			};
		};
	};
	class SA_ToolBench_Black: SA_ToolBench_Base
	{	
		scope = 2;
		displayName = "Tool Bench (Black)";
		hiddenSelections[]={"abyss"};
		hiddenSelectionsTextures[] = {"Spud_Stuff\SA_Storage\BaseObjects\SA_ToolBench\data\SA_ToolBench_Black.paa"};
	};
	class SA_ToolBench_Blue: SA_ToolBench_Base
	{	
		scope = 2;
		displayName = "Tool Bench (Blue)";
		hiddenSelections[]={"abyss"};
		hiddenSelectionsTextures[] = {"Spud_Stuff\SA_Storage\BaseObjects\SA_ToolBench\data\SA_ToolBench_Blue.paa"};
	};
	class SA_ToolBench_Red: SA_ToolBench_Base
	{	
		scope = 2;
		displayName = "Tool Bench (Red)";
		hiddenSelections[]={"abyss"};
		hiddenSelectionsTextures[] = {"Spud_Stuff\SA_Storage\BaseObjects\SA_ToolBench\data\SA_ToolBench_Red.paa"};
	};
	class SA_ToolBench_White: SA_ToolBench_Base
	{	
		scope = 2;
		displayName = "Tool Bench (White)";
		hiddenSelections[]={"abyss"};
		hiddenSelectionsTextures[] = {"Spud_Stuff\SA_Storage\BaseObjects\SA_ToolBench\data\SA_ToolBench_White.paa"};
	};
	class SA_ToolBench_Yellow: SA_ToolBench_Base
	{	
		scope = 2;
		displayName = "Tool Bench (Yellow)";
		hiddenSelections[]={"abyss"};
		hiddenSelectionsTextures[] = {"Spud_Stuff\SA_Storage\BaseObjects\SA_ToolBench\data\SA_ToolBench_Yellow.paa"};
	};
};