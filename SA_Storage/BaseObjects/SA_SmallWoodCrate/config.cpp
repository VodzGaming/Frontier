class CfgPatches
{
	class SA_SmallWoodCrate
	{
		units[] = {"SA_SmallWoodCrate_Crate_Kit","SA_SmallWoodCrate_Crate"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data","A6_BaseClasses","A6_BaseKitBoxes"};
	};
};
class CfgVehicles
{
	class A6_Item;
	class A6_Kit;
	class SA_SmallWoodCrate_Kit: A6_Kit
	{
		scope=2;
		weight=6500;
		displayName="A.L.S. Small Wood Kit";        
        model = "Spud_Stuff\SA_Storage\BaseObjects\SA_BaseKitBox\kitbox.p3d";
		descriptionShort = "Classic wooden storage crate. Holds 600 items. Popular for its blend of rustic appearance and rugged utility. Can be disassembled with a screwdriver.";
		hiddenSelections[]={"zbytek","camo"};   
        hiddenSelectionsTextures[] = {"Spud_Stuff\SA_Storage\BaseObjects\SA_BaseKitBox\data\Storage_kit_SmallWoodCrate.paa","Spud_Stuff\SA_Storage\BaseObjects\SA_BaseKitBox\data\Storage_kit_SmallWoodCrate.paa"};
		itemSize[] = {6,4};
	};    
    class A6_Storage_Base;
	class SA_SmallWoodCrate_Base: A6_Storage_Base
    {
        scope = 0;
        displayName = "A.L.S. Small Wood Crate";
        descriptionShort = "Classic wooden storage crate. Holds 600 items. Popular for its blend of rustic appearance and rugged utility. Can be disassembled with a screwdriver.";
        model = "Spud_Stuff\SA_Storage\BaseObjects\SA_SmallWoodCrate\SA_SmallWoodCrate.p3d";
        rotationFlags = 2;
        itemBehaviour = 0;
        hiddenSelections[]={"abyss"};
        hiddenSelectionsTextures[] = {"Spud_Stuff\SA_Storage\BaseObjects\SA_SmallWoodCrate\data\Abyss_Smalllogocrate_co.paa"};
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
                        {1.0,{"Spud_Stuff\SA_Storage\BaseObjects\SA_SmallWoodCrate\data\Abyss_Smalllogocrate.rvmat"}},
                        {0.7,{"Spud_Stuff\SA_Storage\BaseObjects\SA_SmallWoodCrate\data\Abyss_Smalllogocrate.rvmat"}},
                        {0.5,{"Spud_Stuff\SA_Storage\BaseObjects\SA_SmallWoodCrate\data\Abyss_Smalllogocrate.rvmat"}},
                        {0.3,{"Spud_Stuff\SA_Storage\BaseObjects\SA_SmallWoodCrate\data\Abyss_Smalllogocrate.rvmat"}},
                        {0.0,{"Spud_Stuff\SA_Storage\BaseObjects\SA_SmallWoodCrate\data\Abyss_Smalllogocrate.rvmat"}}
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
			itemsCargoSize[] = {10,60};
			openable = 1;
			allowOwnedCargoManipulation = 1;
		};
        class AnimationSources
        {
			class hideproxy
            {
                source = "user";
				initPhase=0;
				animPeriod=1.5;
            };
        };
    };
	class SA_SmallWoodCrate: SA_SmallWoodCrate_Base
	{	
        scope = 2;
        hiddenSelections[]={"abyss"};
        hiddenSelectionsTextures[] = {"Spud_Stuff\SA_Storage\BaseObjects\SA_SmallWoodCrate\data\Abyss_Smalllogocrate_co.paa"};
	};     
};