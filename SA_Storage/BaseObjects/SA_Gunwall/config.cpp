class CfgPatches
{
	class SA_Gunwall
	{
		units[] = {"SA_Gunwall"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data","A6_BaseClasses","A6_BaseKitBoxes"};
	};
};
class CfgVehicles
{   
	class A6_Item;
	class A6_Kit;
	class SA_Gunwall_Kit: A6_Kit
	{
		scope=2;
		weight=6500;
		displayName="A.L.S. Abyss Gunwall Kit";        
        model = "Spud_Stuff\SA_Storage\BaseObjects\SA_BaseKitBox\kitbox.p3d";
        hiddenSelections[]={"zbytek","camo"};   
        hiddenSelectionsTextures[] = {"Spud_Stuff\SA_Storage\BaseObjects\SA_BaseKitBox\data\Storage_kit_gunwall.paa","Spud_Stuff\SA_Storage\BaseObjects\SA_BaseKitBox\data\Storage_kit_gunwall.paa"};
		descriptionShort = "Wall-mounted weapons rack by Abyss LS. Holds up to 8 rifles and 2 pistols, with added storage for ammo and attachments. Designed for rapid access in secure locations. Can be disassembled with a screwdriver.";
		itemSize[] = {6,4};
	};
	class A6_Coverable_Base;
	class SA_Gunwall_Base: A6_Coverable_Base
	{
		scope = 0;
		displayName="A.L.S. Gun Wall";
		descriptionShort = "Wall-mounted weapons rack by Abyss LS. Holds up to 8 rifles and 2 pistols, with added storage for ammo and attachments. Designed for rapid access in secure locations. Can be disassembled with a screwdriver.";  
		model = "Spud_Stuff\SA_Storage\BaseObjects\SA_Gunwall\SA_Gunwall.p3d";
		attachments[] = 
        {
			"CodeLock",
			"Att_CombinationLock",
			"Shoulder1",
			"Shoulder2",
			"Shoulder3",
			"Shoulder4",
			"Shoulder5",
			"Shoulder6",
			"Shoulder7",
			"Shoulder8"		
		};
		slopeTolerance = 0.4;
		yawPitchRollLimit[] = {45,45,45};
		weight = 5000;
		itemBehaviour = 0;
		itemSize[] = {6,6};
		carveNavmesh = 1;
		canBeDigged = 0;
		physLayer = "item_large";
		rotationFlags = 0;
		hiddenSelectionsTextures[] = {"Spud_Stuff\SA_Storage\BaseObjects\SA_Gunwall\data\GunWall_co.paa","Spud_Stuff\SA_Storage\BaseObjects\SA_Gunwall\data\Gun Lockers_Base_c.paa"};
		hiddenSelections[] = {"abyss","abyss1"};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 100000;
                    healthLevels[] = 
                    {
                        {1.0,{"Spud_Stuff\SA_Storage\BaseObjects\SA_Gunwall\data\GunWall.rvmat","Spud_Stuff\SA_Storage\BaseObjects\SA_Gunwall\data\barrel_yellow.rvmat"}},
                        {0.7,{"Spud_Stuff\SA_Storage\BaseObjects\SA_Gunwall\data\GunWall.rvmat","Spud_Stuff\SA_Storage\BaseObjects\SA_Gunwall\data\barrel_yellow.rvmat"}},
                        {0.5,{"Spud_Stuff\SA_Storage\BaseObjects\SA_Gunwall\data\GunWall.rvmat","Spud_Stuff\SA_Storage\BaseObjects\SA_Gunwall\data\barrel_yellow.rvmat"}},
                        {0.3,{"Spud_Stuff\SA_Storage\BaseObjects\SA_Gunwall\data\GunWall.rvmat","Spud_Stuff\SA_Storage\BaseObjects\SA_Gunwall\data\barrel_yellow.rvmat"}},
                        {0.0,{"Spud_Stuff\SA_Storage\BaseObjects\SA_Gunwall\data\GunWall.rvmat","Spud_Stuff\SA_Storage\BaseObjects\SA_Gunwall\data\barrel_yellow.rvmat"}}
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
			class Rifle_Storage
			{
				name = "Rifle Storage";
				description = "";
				attachmentSlots[] = 
				{
					"Shoulder1",
					"Shoulder2",
					"Shoulder3",
					"Shoulder4",
					"Shoulder5",
					"Shoulder6",
					"Shoulder7",
					"Shoulder8"	
				};
				icon = "shoulderleft";
			};
			class Pistols
			{
				name = "Pistols";
				description = "";
				attachmentSlots[] = {"Pistol1","Pistol2"};
				icon = "pistol";
			};
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
			itemsCargoSize[] = {10,5};
			openable = 0;
			allowOwnedCargoManipulation = 1;
		};
    }; 	
	class SA_Gunwall: SA_Gunwall_Base
	{	
        scope = 2;
		hiddenSelections[] = {"abyss","abyss1"};
		hiddenSelectionsTextures[] = {"Spud_Stuff\SA_Storage\BaseObjects\SA_Gunwall\data\GunWall_co.paa","Spud_Stuff\SA_Storage\BaseObjects\SA_Gunwall\data\Gun Lockers_Base_c.paa"};
	};    
};