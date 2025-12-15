class CfgPatches
{
	class SA_SciFiBox
	{
		units[] = {"sA_SciFiBox","SA_SciFiBox_Crate"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data","A6_BaseClasses","A6_BaseKitBoxes"};
	};
};
class CfgVehicles
{
	class A6_Item;
	class A6_Kit;
	class SA_SciFiBox_Kit: A6_Kit
	{
		scope=2;
		weight=6500;
		displayName="A.L.S. SciFi Box Kit";        
        model = "Spud_Stuff\SA_Storage\BaseObjects\SA_BaseKitBox\kitbox.p3d";
        hiddenSelections[]={"zbytek","camo"};   
        hiddenSelectionsTextures[] = {"Spud_Stuff\SA_Storage\BaseObjects\SA_BaseKitBox\data\Storage_kit_SciFIBox.paa","Spud_Stuff\SA_Storage\BaseObjects\SA_BaseKitBox\data\Storage_kit_SciFIBox.paa"};
		descriptionShort = "Experimental containment crate designed with advanced polymers by Abyss LS. Holds 600 items. Secure and sleek. Often found in classified storage zones. Can be disassembled with a screwdriver.";
		itemSize[] = {6,4};
	};    
    class A6_Storage_Base;
	class SA_SciFiBox_Base: A6_Storage_Base
    {
        scope = 0;
        displayName = "A.L.S. SciFi Box";
        descriptionShort = "Experimental containment crate designed with advanced polymers by Abyss LS. Holds 600 items. Secure and sleek. Often found in classified storage zones. Can be disassembled with a screwdriver.";
        model = "Spud_Stuff\SA_Storage\BaseObjects\SA_SciFIBox\SA_SciFIBox.p3d";
        rotationFlags = 2;
        itemBehaviour = 0;
        hiddenSelections[]={"abyss"};
        hiddenSelectionsTextures[] = {"Spud_Stuff\SA_Storage\BaseObjects\SA_SciFIBox\data\abyss_SciFIBox_co.paa"};
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
                        {1.0,{}},
                        {0.7,{}},
                        {0.5,{}},
                        {0.3,{}},
                        {0.0,{}}
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
		};;
    };    
	class SA_SciFiBox: SA_SciFiBox_Base
	{	
        scope = 2;
        hiddenSelections[]={"abyss"};
        hiddenSelectionsTextures[] = {"Spud_Stuff\SA_Storage\BaseObjects\SA_SciFIBox\data\abyss_SciFIBox_co.paa"};
	};   
};