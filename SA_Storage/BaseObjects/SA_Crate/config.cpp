class CfgPatches
{
	class SA_MilitaryStorageCrate
	{
		units[] = {"SA_Crate_Kit","SA_Storage_Crate"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data","DZ_Characters","DZ_Characters_Backpacks","DZ_Characters_Tops","DZ_Characters_Pants","DZ_Scripts","DZ_Gear_Containers","DZ_Characters_Headgear","DZ_characters_glasses","DZ_Weapons_Melee_Blade","DZ_Gear_Tools","DZ_Gear_Camping","DZ_Weapons_Melee","A6_BaseClasses","A6_BaseKitBoxes"};
	};
};
class CfgVehicles
{
	class A6_Item;
	class A6_Kit;
	class SA_Crate_Kit: A6_Kit
	{
		scope=2;
		weight=6500;
		displayName="Crate Kit";        
        model = "Spud_Stuff\SA_Storage\BaseObjects\SA_BaseKitBox\kitbox.p3d";
        hiddenSelections[]={"zbytek","camo"};   
        hiddenSelectionsTextures[] = {"Spud_Stuff\SA_Storage\BaseObjects\SA_BaseKitBox\data\Storage_kit_MilitaryCrate.paa","Spud_Stuff\SA_Storage\BaseObjects\SA_BaseKitBox\data\Storage_kit_MilitaryCrate.paa"};
		descriptionShort = "General-purpose storage crate with 300 inventory capacity. Best suited for small stashes or personal kits. Can be disassembled with a screwdriver.";
		itemSize[] = {6,4};
	};    
    class A6_Storage_Base;
    class A6_Openable_Base : A6_Storage_Base {};
	class SA_Crate_Base: A6_Openable_Base
    {
        scope = 0;
        displayName = "A.L.S. Military Crate";
        descriptionShort = "General-purpose storage crate with 300 inventory capacity. Best suited for small stashes or personal kits. Designed for extreme durability and efficiency, it's the perfect solution for transporting and safeguarding your most critical supplies.";
        model = "Spud_Stuff\SA_Storage\BaseObjects\SA_Crate\SA_Crate.p3d";
        itemSize[] = {10,5};
        rotationFlags = 2;
        itemBehaviour = 0;
        hiddenSelections[]={"abyss"};
        hiddenSelectionsTextures[] = {"Spud_Stuff\SA_Storage\BaseObjects\SA_Crate\data\ALSCrate_co.paa"};
        attachments[]={"CodeLock","Att_CombinationLock"};
        class DamageSystem
        {
            class GlobalHealth
            {
                class Health
                {
                    hitpoints = 2500;
                    healthLevels[] = 
                    {
                        {1.0,{"Spud_Stuff\SA_Storage\BaseObjects\SA_Crate\data\MilitaryCrate.rvmat"}},
                        {0.7,{"Spud_Stuff\SA_Storage\BaseObjects\SA_Crate\data\MilitaryCrate.rvmat"}},
                        {0.5,{"Spud_Stuff\SA_Storage\BaseObjects\SA_Crate\data\MilitaryCrate_damage.rvmat"}},
                        {0.3,{"Spud_Stuff\SA_Storage\BaseObjects\SA_Crate\data\MilitaryCrate_damage.rvmat"}},
                        {0.0,{"Spud_Stuff\SA_Storage\BaseObjects\SA_Crate\data\MilitaryCrate_destruct.rvmat"}}
                    };
                };
            };
            class GlobalArmor
            {
                class Projectile
                {
                    class Health
                    {
                        damage=0.1;
                    };
                };
                class Melee
                {
                    class Health
                    {
                        damage=0.3;
                    };
                };
                class FragGrenade
                {
                    class Health
                    {
                        damage=8;
                    };
                };
            };
        };          
		class Cargo
		{
			itemsCargoSize[] = {10,30};
			openable = 1;
			allowOwnedCargoManipulation = 1;
		}; 
        class GUIInventoryAttachmentsProps
        {
           class CodeLock
            {
                name="CodeLock";
                description="";
                attachmentSlots[]=
                {
                    "CodeLock"
                };
                icon="cat_bb_attachments";
            };
            class Lock
            {
                name="Lock";
                description="";
                attachmentSlots[]=
                {
                    "Att_CombinationLock"
                };
                icon="cat_bb_attachments";
            };
        };
        class AnimationSources
        {
            class Doors1
            {
                source = "user";
                initPhase = 0;
                animPeriod = 1;
            };
        };
    };   
	class SA_Crate: SA_Crate_Base
	{	
        scope = 2;
        hiddenSelections[]={"abyss"};
        hiddenSelectionsTextures[] = {"Spud_Stuff\SA_Storage\BaseObjects\SA_Crate\data\ALSCrate_co.paa"};
	};  
	class SA_Crate_Keyroom: SA_Crate_Base
	{	
        scope = 2;
        hiddenSelections[]={"abyss"};
        hiddenSelectionsTextures[] = {"Spud_Stuff\SA_Storage\BaseObjects\SA_Crate\data\ALSCrate_co.paa"};
	}; 
};