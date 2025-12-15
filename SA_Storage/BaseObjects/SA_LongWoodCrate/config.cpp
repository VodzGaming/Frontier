class CfgPatches
{
	class Abyss_Weapon_Crate
	{
		units[] = {"Abyss_LongWoodCrate_Kit","Abyss_LongWoodCrate"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data","A6_BaseClasses","A6_BaseKitBoxes"};
	};
};
class CfgVehicles
{
	class A6_Item;
	class A6_Kit;
	class SA_LongWoodCrate_Kit: A6_Kit
	{
		scope=2;
		weight=6500;
		displayName="A.L.S. Weapon Crate Kit";        
        model = "Spud_Stuff\SA_Storage\BaseObjects\SA_BaseKitBox\kitbox.p3d";
        hiddenSelections[]={"zbytek","camo"};   
        hiddenSelectionsTextures[] = {"Spud_Stuff\SA_Storage\BaseObjects\SA_BaseKitBox\data\Storage_kit_LongWoodCrate.paa","Spud_Stuff\SA_Storage\BaseObjects\SA_BaseKitBox\data\Storage_kit_LongWoodCrate.paa"};
		descriptionShort = "Extended-length wooden crate made for long-form gear or organized storage. Holds 500 inventory items. Built tough for extended field use. Can be disassembled with a screwdriver.";
		itemSize[] = {6,4};
	};    
    class A6_Storage_Base;
    class A6_Openable_Base : A6_Storage_Base {};
	class SA_LongWoodCrate_Base: A6_Openable_Base
    {
        scope = 0;
        displayName = "A.L.S. Weapon Crate";
        descriptionShort = "Extended-length wooden crate made for long-form gear or organized storage. Holds 500 inventory items. Built tough for extended field use. Can be disassembled with a screwdriver.";
        model = "Spud_Stuff\SA_Storage\BaseObjects\SA_LongWoodCrate\SA_LongWoodCrate.p3d";
        itemSize[] = {10,5};
        rotationFlags = 2;
        itemBehaviour = 0;
        hiddenSelections[]={"abyss"};
        hiddenSelectionsTextures[] = {"Spud_Stuff\SA_Storage\BaseObjects\SA_LongWoodCrate\Data\Longwoodcrate_co.paa"};
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
                        {1.0,{"Spud_Stuff\SA_Storage\BaseObjects\SA_LongWoodCrate\Data\LongWoodCrate.rvmat"}},
                        {0.7,{"Spud_Stuff\SA_Storage\BaseObjects\SA_LongWoodCrate\Data\LongWoodCrate.rvmat"}},
                        {0.5,{"Spud_Stuff\SA_Storage\BaseObjects\SA_LongWoodCrate\Data\LongWoodCrate_damage.rvmat"}},
                        {0.3,{"Spud_Stuff\SA_Storage\BaseObjects\SA_LongWoodCrate\Data\LongWoodCrate_damage.rvmat"}},
                        {0.0,{"Spud_Stuff\SA_Storage\BaseObjects\SA_LongWoodCrate\Data\LongWoodCrate_destruct.rvmat"}}
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
		class Cargo
		{
			itemsCargoSize[] = {10,50};
			openable = 1;
			allowOwnedCargoManipulation = 1;
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
	class SA_LongWoodCrate: SA_LongWoodCrate_Base
	{	
        scope = 2;
        hiddenSelections[]={"abyss"};
        hiddenSelectionsTextures[] = {"Spud_Stuff\SA_Storage\BaseObjects\SA_LongWoodCrate\Data\Longwoodcrate_co.paa"};
	};    
};