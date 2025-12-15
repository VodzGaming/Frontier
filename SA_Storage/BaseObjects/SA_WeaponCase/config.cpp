class CfgPatches
{
	class Abyss_Weapon_Crate
	{
		units[] = {"SA_Weapon_Crate_Kit","SA_Weapon_Crate"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data","A6_BaseClasses","A6_BaseKitBoxes"};
	};
};
class CfgVehicles
{
	class A6_Item;
	class A6_Kit;
	class SA_Weapon_Crate_Kit: A6_Kit
	{
		scope=2;
		weight=6500;
		displayName="A.L.S. Weapon Crate Kit";        
        model = "Spud_Stuff\SA_Storage\BaseObjects\SA_BaseKitBox\kitbox.p3d";
        hiddenSelections[]={"zbytek","camo"};   
        hiddenSelectionsTextures[] = {"Spud_Stuff\SA_Storage\BaseObjects\SA_BaseKitBox\data\Storage_kit_WeaponCrate.paa","Spud_Stuff\SA_Storage\BaseObjects\SA_BaseKitBox\data\Storage_kit_WeaponCrate.paa"};
		descriptionShort = "Field-grade weapon crate by Abyss LS. Stores 4 rifles and additional equipment. Rugged, stackable, and designed for quick deployment. Can be disassembled with a screwdriver.";
		itemSize[] = {6,4};
	};    
    class A6_Storage_Base;
    class A6_Openable_Base : A6_Storage_Base {};
	class SA_Weapon_Crate_Base: A6_Openable_Base
    {
        scope = 0;
        displayName = "A.L.S. Weapon Crate";
        descriptionShort = "Field-grade weapon crate by Abyss LS. Stores 4 rifles and additional equipment. Rugged, stackable, and designed for quick deployment. Can be disassembled with a screwdriver.";
        model = "Spud_Stuff\SA_Storage\BaseObjects\SA_WeaponCase\SA_Weapon_Crate.p3d";
        itemSize[] = {10,5};
        rotationFlags = 2;
        itemBehaviour = 0;
        hiddenSelections[]={"abyss"};
        hiddenSelectionsTextures[] = {"Spud_Stuff\SA_Storage\BaseObjects\SA_WeaponCase\data\WeaponCase_co.paa"};
        attachments[]={"Shoulder1","Shoulder2","Shoulder3","Shoulder4","CodeLock","Att_CombinationLock"};
        class DamageSystem
        {
            class GlobalHealth
            {
                class Health
                {
                    hitpoints = 2500;
                    healthLevels[] = 
                    {
                        {1.0,{"Spud_Stuff\SA_Storage\BaseObjects\SA_WeaponCase\data\WeaponCrate.rvmat"}},
                        {0.7,{"Spud_Stuff\SA_Storage\BaseObjects\SA_WeaponCase\data\WeaponCrate.rvmat"}},
                        {0.5,{"Spud_Stuff\SA_Storage\BaseObjects\SA_WeaponCase\data\WeaponCrate_damage.rvmat"}},
                        {0.3,{"Spud_Stuff\SA_Storage\BaseObjects\SA_WeaponCase\data\WeaponCrate_damage.rvmat"}},
                        {0.0,{"Spud_Stuff\SA_Storage\BaseObjects\SA_WeaponCase\data\WeaponCrate_destruct.rvmat"}}
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
					"Shoulder4"
				};
				icon = "shoulderleft";
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
			itemsCargoSize[] = {};
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
	class SA_Weapon_Crate: SA_Weapon_Crate_Base
	{	
        scope = 2;
        hiddenSelections[]={"abyss"};
        hiddenSelectionsTextures[] = {"Spud_Stuff\SA_Storage\BaseObjects\SA_WeaponCase\data\WeaponCase_co.paa"};
	};     
	class SA_Weapon_Crate_keyroom: SA_Weapon_Crate_Base
	{	
        scope = 2;
        hiddenSelections[]={"abyss"};
        hiddenSelectionsTextures[] = {"Spud_Stuff\SA_Storage\BaseObjects\SA_WeaponCase\data\WeaponCase_co.paa"};
	}; 
};