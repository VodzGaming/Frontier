class CfgPatches
{
	class SA_Single_Crate
	{
		units[] = {"SA_SingleCrate_Kit","SA_SingleCrate"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data","A6_BaseClasses","A6_BaseKitBoxes"};
	};
};
class CfgVehicles
{
	class A6_Item;
	class A6_Kit;
	class SA_SingleCrate_Kit: A6_Kit
	{
		scope=2;
		weight=6500;
		displayName="A.L.S. Single Crate Kit";        
        model = "Spud_Stuff\SA_Storage\BaseObjects\SA_BaseKitBox\kitbox.p3d";
        hiddenSelections[]={"zbytek","camo"};   
        hiddenSelectionsTextures[] = {"Spud_Stuff\SA_Storage\BaseObjects\SA_BaseKitBox\data\Storage_kit_SingelCrate.paa","Spud_Stuff\SA_Storage\BaseObjects\SA_BaseKitBox\data\Storage_kit_SingelCrate.paa"};
		descriptionShort = "All-purpose crate with 700 inventory slots. Built for base setups requiring quick, flexible storage. Standard issue for Abyss LS camps. Can be disassembled with a screwdriver.";
		itemSize[] = {6,4};
	};    
	class A6_Storage_Base;
	class A6_Openable_Base: A6_Storage_Base{};
	class SA_SingleCrate_Base: A6_Openable_Base
    {
        scope = 0;
        displayName = "A.L.S. Single Crate";
        descriptionShort = "All-purpose crate with 700 inventory slots. Built for base setups requiring quick, flexible storage. Standard issue for Abyss LS camps. Can be disassembled with a screwdriver.";
        model = "Spud_Stuff\SA_Storage\BaseObjects\SA_SingleCrate\SA_SingleCrate.p3d";
        rotationFlags = 2;
        itemBehaviour = 0;
        hiddenSelections[]={"abyss","abyss1"};
        hiddenSelectionsTextures[] = {"Spud_Stuff\SA_Storage\BaseObjects\SA_MediumCrateCovered\data\TarpSheet_co.paa","Spud_Stuff\SA_Storage\BaseObjects\SA_mediumcratecovered\data\woodboxes_co.paa"};
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
                        {1.0,{"Spud_Stuff\SA_Storage\BaseObjects\SA_MediumCrateCovered\data\TarpSheet.rvmat","Spud_Stuff\SA_Storage\BaseObjects\SA_mediumcratecovered\data\woodboxes.rvmat"}},
                        {0.7,{"Spud_Stuff\SA_Storage\BaseObjects\SA_MediumCrateCovered\data\TarpSheet.rvmat","Spud_Stuff\SA_Storage\BaseObjects\SA_mediumcratecovered\data\woodboxes.rvmat"}},
                        {0.5,{"Spud_Stuff\SA_Storage\BaseObjects\SA_MediumCrateCovered\data\TarpSheet.rvmat","Spud_Stuff\SA_Storage\BaseObjects\SA_mediumcratecovered\data\woodboxes.rvmat"}},
                        {0.3,{"Spud_Stuff\SA_Storage\BaseObjects\SA_MediumCrateCovered\data\TarpSheet.rvmat","Spud_Stuff\SA_Storage\BaseObjects\SA_mediumcratecovered\data\woodboxes.rvmat"}},
                        {0.0,{"Spud_Stuff\SA_Storage\BaseObjects\SA_MediumCrateCovered\data\TarpSheet.rvmat","Spud_Stuff\SA_Storage\BaseObjects\SA_mediumcratecovered\data\woodboxes.rvmat"}}
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
			itemsCargoSize[] = {10,70};
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
	class SA_SingleCrate: SA_SingleCrate_Base
	{	
        scope = 2;
        hiddenSelections[]={"abyss","abyss1"};
        hiddenSelectionsTextures[] = {"Spud_Stuff\SA_Storage\BaseObjects\SA_MediumCrateCovered\data\TarpSheet_co.paa","Spud_Stuff\SA_Storage\BaseObjects\SA_mediumcratecovered\data\woodboxes_co.paa"};
	};     
};