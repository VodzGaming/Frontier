class CfgPatches
{
	class SA_Storage_Box
	{
		units[] = {"SA_Storage_Box","SA_Storage_Box_Crate"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data","A6_BaseClasses","A6_BaseKitBoxes"};
	};
};
class CfgVehicles
{
	class A6_Item;
	class A6_Kit;
	class SA_Storage_Box_Kit: A6_Kit
	{
		scope=2;
		weight=6500;
		displayName="A.L.S. Storage Box Kit";        
        model = "Spud_Stuff\SA_Storage\BaseObjects\SA_BaseKitBox\kitbox.p3d";
        hiddenSelections[]={"zbytek","camo"};   
        hiddenSelectionsTextures[] = {"Spud_Stuff\SA_Storage\BaseObjects\SA_BaseKitBox\data\Storage_kit_storagebox.paa","Spud_Stuff\SA_Storage\BaseObjects\SA_BaseKitBox\data\Storage_kit_storagebox.paa"};
		descriptionShort = "Basic utility box with 600 item capacity. Versatile and easy to stack or stash. Used for everything from ammo to canned goods. Can be disassembled with a screwdriver.";
		itemSize[] = {6,4};
	};    
    class A6_Storage_Base;
	class SA_Storage_Box_Base: A6_Storage_Base
    {
        scope = 0;
        displayName = "A.L.S. Storage Box";
        descriptionShort = "Basic utility box with 600 item capacity. Versatile and easy to stack or stash. Used for everything from ammo to canned goods. Can be disassembled with a screwdriver.";
        model = "Spud_Stuff\SA_Storage\BaseObjects\SA_storagebox\SA_StorageBox.p3d";
        rotationFlags = 2;
        itemBehaviour = 0;
        hiddenSelections[]={"abyss","abyss1","abyss2"};
        hiddenSelectionsTextures[] = {"Spud_Stuff\SA_Storage\BaseObjects\SA_storagebox\data\Abyss_storagebox_co.paa"};
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
                        {1.0,{"Spud_Stuff\SA_Storage\BaseObjects\SA_storagebox\data\Abyss_storagebox.rvmat"}},
                        {0.7,{"Spud_Stuff\SA_Storage\BaseObjects\SA_storagebox\data\Abyss_storagebox.rvmat"}},
                        {0.5,{"Spud_Stuff\SA_Storage\BaseObjects\SA_storagebox\data\Abyss_storagebox.rvmat"}},
                        {0.3,{"Spud_Stuff\SA_Storage\BaseObjects\SA_storagebox\data\Abyss_storagebox.rvmat"}},
                        {0.0,{"Spud_Stuff\SA_Storage\BaseObjects\SA_storagebox\data\Abyss_storagebox.rvmat"}}
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
	class SA_Storage_Box: SA_Storage_Box_Base
	{	
        scope = 2;
        hiddenSelections[]={"abyss","abyss1","abyss2"};
        hiddenSelectionsTextures[] = {"Spud_Stuff\SA_Storage\BaseObjects\SA_storagebox\data\Abyss_storagebox_co.paa"};
	}; 
};