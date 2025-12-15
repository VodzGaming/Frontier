class CfgPatches
{
	class SA_Cupboard
	{
		units[] = {"SA_Cupboard_Kit","SA_Cupboard"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data","DZ_Characters","DZ_Characters_Backpacks","DZ_Characters_Tops","DZ_Characters_Pants","DZ_Scripts","DZ_Gear_Containers","DZ_Characters_Headgear","DZ_characters_glasses","DZ_Weapons_Melee_Blade","DZ_Gear_Tools","DZ_Gear_Camping","DZ_Weapons_Melee","A6_BaseClasses","A6_BaseKitBoxes"};
	};
};
class CfgVehicles
{
	class A6_Item;
	class A6_Kit;
	class SA_Cupboard_Kit: A6_Kit
	{
		scope = 2;
		weight = 6500;
		displayName = "Cupboard Kit";
		model = "Spud_Stuff\SA_Storage\BaseObjects\SA_BaseKitBox\kitbox.p3d";
		hiddenSelections[] = {"zbytek","camo"};
		hiddenSelectionsTextures[] = {"Spud_Stuff\SA_Storage\BaseObjects\SA_BaseKitBox\data\Storage_kit_MilitaryCrate.paa","Spud_Stuff\SA_Storage\BaseObjects\SA_BaseKitBox\data\Storage_kit_MilitaryCrate.paa"};
		descriptionShort = "High-capacity wardrobe unit from Abyss LS. Includes full gear slots and 500 inventory. Ideal for base interiors. Can be disassembled with a screwdriver.";
		itemSize[] = {6,4};
	};
	class A6_Storage_Base;
	class A6_Openable_Base: A6_Storage_Base{};
	class SA_Cupboard_Base: A6_Openable_Base
	{
		scope = 0;
		displayName = "A.L.S. Cupboard";
		descriptionShort = "High-capacity wardrobe unit from Abyss LS. Includes full gear slots and 500 inventory. Ideal for base interiors. Can be disassembled with a screwdriver.";
		model = "Spud_Stuff\SA_Storage\BaseObjects\SA_Cupboard\SA_Cupboard.p3d";
		itemSize[] = {10,5};
		rotationFlags = 2;
		itemBehaviour = 0;
		hiddenSelections[] = {"camo"};
		hiddenSelectionsTextures[] = {"Spud_Stuff\SA_Storage\BaseObjects\SA_Cupboard\data\OldCupboard_CO.paa"};
		attachments[] = {"CodeLock","Att_CombinationLock","Vest","Back","Headgear","Legs","Body","Gloves","Feet","Armband","Eyewear","Mask","Hips"};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 2500;
					healthLevels[] = {{1.0,{"Spud_Stuff\SA_Storage\BaseObjects\SA_Cupboard\data\Wood.rvmat"}},{0.7,{"Spud_Stuff\SA_Storage\BaseObjects\SA_Cupboard\data\Wood.rvmat"}},{0.5,{"Spud_Stuff\SA_Storage\BaseObjects\SA_Cupboard\data\Wood.rvmat"}},{0.3,{"Spud_Stuff\SA_Storage\BaseObjects\SA_Cupboard\data\Wood.rvmat"}},{0.0,{"Spud_Stuff\SA_Storage\BaseObjects\SA_Cupboard\data\Wood.rvmat"}}};
				};
			};
			class GlobalArmor
			{
				class Projectile
				{
					class Health
					{
						damage = 0.1;
					};
				};
				class Melee
				{
					class Health
					{
						damage = 0.3;
					};
				};
				class FragGrenade
				{
					class Health
					{
						damage = 8;
					};
				};
			};
		};
		class Cargo
		{
			itemsCargoSize[] = {10,50};
			openable = 1;
			allowOwnedCargoManipulation = 1;
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
			class Accessories
			{
				name = "Accessories";
				description = "";
				attachmentSlots[] = {"Mask","Eyewear","Gloves","Feet","Armband"};
				icon = "eyewear";
			};
			class Clothing
			{
				name = "Clothing";
				description = "";
				attachmentSlots[] = {"Vest","Body","Hips","Legs","Back","Headgear"};
				icon = "body";
			};
		};
	};
	class SA_Cupboard: SA_Cupboard_Base
	{
		scope = 2;
		hiddenSelections[] = {"abyss"};
		hiddenSelectionsTextures[] = {"Spud_Stuff\SA_Storage\BaseObjects\SA_Cupboard\data\OldCupboard_CO.paa"};
	};
};
