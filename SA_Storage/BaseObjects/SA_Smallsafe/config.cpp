class CfgPatches
{
	class SA_Small_Safe
	{
		units[] = {"SA_Small_Safe_Kit","SA_Small_Safe"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data","A6_BaseClasses","A6_BaseKitBoxes"};
	};
};
class CfgVehicles
{
	class A6_Item;
	class A6_Kit;
	class SA_Small_Safe_Kit: A6_Kit
	{
		scope=2;
		weight=6500;
		displayName="A.L.S. Small Safe Kit";        
        model = "Spud_Stuff\SA_Storage\BaseObjects\SA_BaseKitBox\kitbox.p3d";
		descriptionShort = "A Kit that containts a A.L.S. Small Safe. Can be disassembled with a screwdriver.";  
        hiddenSelections[]={"zbytek","camo"};   
        hiddenSelectionsTextures[] = {"Spud_Stuff\SA_Storage\BaseObjects\SA_BaseKitBox\data\Storage_kit_SmallSafe.paa","Spud_Stuff\SA_Storage\BaseObjects\SA_BaseKitBox\data\Storage_kit_SmallSafe.paa"};
		itemSize[] = {6,4};
	};    
    class A6_Storage_Base;
    class A6_Openable_Base : A6_Storage_Base {};
	class SA_Small_Safe_Base: A6_Openable_Base
    {
        scope = 2;
        displayName = "A.L.S. Small Safe";
        descriptionShort = "Secure your valuables with this sturdy safe from Abyss Logistical Solutions. Provides 60 slots for essential items. Can be disassembled with a screwdriver.";
        model = "Spud_Stuff\SA_Storage\BaseObjects\SA_smallsafe\SA_safe.p3d";
        itemSize[] = {10,5};
        rotationFlags = 2;
        itemBehaviour = 0;
        hiddenSelections[]={"abyss"};
        hiddenSelectionsTextures[] = {"Spud_Stuff\SA_Storage\BaseObjects\SA_Smallsafe\data\smallsafe_co.paa"};
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
                        {1.0,{"Spud_Stuff\SA_Storage\BaseObjects\SA_Smallsafe\data\smallsafe.rvmat"}},
                        {0.7,{"Spud_Stuff\SA_Storage\BaseObjects\SA_Smallsafe\data\smallsafe.rvmat"}},
                        {0.5,{"Spud_Stuff\SA_Storage\BaseObjects\SA_Smallsafe\data\smallsafe_damage.rvmat"}},
                        {0.3,{"Spud_Stuff\SA_Storage\BaseObjects\SA_Smallsafe\data\smallsafe_damage.rvmat"}},
                        {0.0,{"Spud_Stuff\SA_Storage\BaseObjects\SA_Smallsafe\data\smallsafe_destruct.rvmat"}}
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
			itemsCargoSize[] = {10,30};
			openable = 1;
			allowOwnedCargoManipulation = 1;
		};
        class AnimationSources
        {
            class Doors1
            {
                source = "user";
				initPhase=0;
				animPeriod=1.5;
            };
        };
    };    
	class SA_Small_Safe: SA_Small_Safe_Base
	{	
        scope = 2;
        hiddenSelections[]={"abyss"};
        hiddenSelectionsTextures[] = {"Spud_Stuff\SA_Storage\BaseObjects\SA_Smallsafe\data\smallsafe_co.paa"};
	};  
};