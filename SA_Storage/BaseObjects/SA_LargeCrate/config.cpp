class CfgPatches
{
	class SA_CoveredMediumCrate
	{
		units[] = {"SACoveredMediumCrate","SA_CoveredMediumCrate_Crate"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data","A6_BaseClasses","A6_BaseKitBoxes"};
	};
};
class CfgVehicles
{
	class A6_Item;
	class A6_Kit;
	class SA_LargeCrate_Kit: A6_Kit
	{
		scope=2;
		weight=6500;
		displayName="A.L.S. Medium Crate Kit";        
        model = "Spud_Stuff\SA_Storage\BaseObjects\SA_BaseKitBox\kitbox.p3d";
        hiddenSelections[]={"zbytek","camo"};   
        hiddenSelectionsTextures[] = {"Spud_Stuff\SA_Storage\BaseObjects\SA_BaseKitBox\data\Storage_kit_LargeCrate.paa","Spud_Stuff\SA_Storage\BaseObjects\SA_BaseKitBox\data\Storage_kit_LargeCrate.paa"};
		descriptionShort = "High-capacity cargo crate designed for bulk storage. Used in major supply hubs. With 900 inventory slots, it's ideal for storing building supplies, gear, or trade goods. Can be disassembled with a screwdriver.";
		itemSize[] = {6,4};
	};    
    class A6_Storage_Base;
	class SA_LargeCrate_Base: A6_Storage_Base
    {
        scope = 0;
        displayName = "A.L.S. Large Crate";
        descriptionShort = "High-capacity cargo crate designed for bulk storage. Used in major supply hubs. With 900 inventory slots, it's ideal for storing building supplies, gear, or trade goods. Can be disassembled with a screwdriver.";
        model = "Spud_Stuff\SA_Storage\BaseObjects\SA_LargeCrate\SA_LargeCrate.p3d";
        rotationFlags = 2;
        itemBehaviour = 0;
        hiddenSelections[]={"abyss","abyss1","abyss2"};
        hiddenSelectionsTextures[] = {"Spud_Stuff\SA_Storage\BaseObjects\SA_smallwoodcrate\data\smallcrate_co.paa","Spud_Stuff\SA_Storage\BaseObjects\SA_MediumCrateCovered\data\TarpSheet_co.paa","Spud_Stuff\SA_Storage\BaseObjects\SA_MediumCrateCovered\data\woodboxes_co.paa"};
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
                        {1.0,{"Spud_Stuff\SA_Storage\BaseObjects\SA_smallwoodcrate\data\smallcrate_co.paa","Spud_Stuff\SA_Storage\BaseObjects\SA_MediumCrateCovered\data\TarpSheet.rvmat","Spud_Stuff\SA_Storage\BaseObjects\SA_MediumCrateCovered\data\woodboxes.rvmat"}},
                        {0.7,{"Spud_Stuff\SA_Storage\BaseObjects\SA_smallwoodcrate\data\smallcrate_co.paa","Spud_Stuff\SA_Storage\BaseObjects\SA_MediumCrateCovered\data\TarpSheet.rvmat","Spud_Stuff\SA_Storage\BaseObjects\SA_MediumCrateCovered\data\woodboxes.rvmat"}},
                        {0.5,{"Spud_Stuff\SA_Storage\BaseObjects\SA_smallwoodcrate\data\smallcrate_co.paa","Spud_Stuff\SA_Storage\BaseObjects\SA_MediumCrateCovered\data\TarpSheet.rvmat","Spud_Stuff\SA_Storage\BaseObjects\SA_MediumCrateCovered\data\woodboxes.rvmat"}},
                        {0.3,{"Spud_Stuff\SA_Storage\BaseObjects\SA_smallwoodcrate\data\smallcrate_co.paa","Spud_Stuff\SA_Storage\BaseObjects\SA_MediumCrateCovered\data\TarpSheet.rvmat","Spud_Stuff\SA_Storage\BaseObjects\SA_MediumCrateCovered\data\woodboxes.rvmat"}},
                        {0.0,{"Spud_Stuff\SA_Storage\BaseObjects\SA_smallwoodcrate\data\smallcrate_co.paa","Spud_Stuff\SA_Storage\BaseObjects\SA_MediumCrateCovered\data\TarpSheet.rvmat","Spud_Stuff\SA_Storage\BaseObjects\SA_MediumCrateCovered\data\woodboxes.rvmat"}}
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
			itemsCargoSize[] = {10,90};
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
	class SA_LargeCrate: SA_LargeCrate_Base
	{	
        scope = 2;
        hiddenSelections[]={"abyss","abyss1","abyss2"};
        hiddenSelectionsTextures[] = {"Spud_Stuff\SA_Storage\BaseObjects\SA_smallwoodcrate\data\smallcrate_co.paa","Spud_Stuff\SA_Storage\BaseObjects\SA_MediumCrateCovered\data\TarpSheet_co.paa","Spud_Stuff\SA_Storage\BaseObjects\SA_MediumCrateCovered\data\woodboxes_co.paa"};
	};    
};