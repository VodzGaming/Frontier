class CfgPatches
{
	class SA_shippingcontainer
	{
		units[] = {"SA_shippingcontainer_Kit","SA_shippingcontainer"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data","A6_BaseClasses","A6_BaseKitBoxes"};
	};
};
class CfgVehicles
{
	class A6_Item;
	class A6_Kit;
	class SA_SoloLocker_Red_Kit: A6_Kit
	{
		scope=2;
		weight=6500;
		displayName="A.L.S. Solo Locker Kit";        
        model = "Spud_Stuff\SA_Storage\BaseObjects\SA_BaseKitBox\kitbox.p3d";
		descriptionShort = "Personal gear locker by Abyss Logistical Solutions. Holds full body kit slots and 250 inventory. Ideal for quick loadout swaps or compact loadouts. Can be disassembled with a screwdriver.";   
        hiddenSelections[]={"zbytek","camo"};   
        hiddenSelectionsTextures[] = {"Spud_Stuff\SA_Storage\BaseObjects\SA_BaseKitBox\data\Storage_kit_Red_Locker.paa","Spud_Stuff\SA_Storage\BaseObjects\SA_BaseKitBox\data\Storage_kit_Red_Locker.paa"};
		itemSize[] = {6,4};
	};    
	class SA_SoloLocker_RedRusty_Kit: A6_Kit
	{
		scope=2;
		weight=6500;
		displayName="A.L.S. Solo Locker Kit";        
        model = "Spud_Stuff\SA_Storage\BaseObjects\SA_BaseKitBox\kitbox.p3d";
		descriptionShort = "Personal gear locker by Abyss Logistical Solutions. Holds full body kit slots and 250 inventory. Ideal for quick loadout swaps or compact loadouts. Can be disassembled with a screwdriver.";   
        hiddenSelections[]={"zbytek","camo"};   
        hiddenSelectionsTextures[] = {"Spud_Stuff\SA_Storage\BaseObjects\SA_BaseKitBox\data\Storage_kit_Red_Rusty.paa","Spud_Stuff\SA_Storage\BaseObjects\SA_BaseKitBox\data\Storage_kit_Red_Rusty.paa"};
		itemSize[] = {6,4};
	};  
	class SA_SoloLocker_YellowRusty_Kit: A6_Kit
	{
		scope=2;
		weight=6500;
		displayName="A.L.S. Solo Locker Kit";        
        model = "Spud_Stuff\SA_Storage\BaseObjects\SA_BaseKitBox\kitbox.p3d";
		descriptionShort = "Personal gear locker by Abyss Logistical Solutions. Holds full body kit slots and 250 inventory. Ideal for quick loadout swaps or compact loadouts. Can be disassembled with a screwdriver.";   
        hiddenSelections[]={"zbytek","camo"};   
        hiddenSelectionsTextures[] = {"Spud_Stuff\SA_Storage\BaseObjects\SA_BaseKitBox\data\Storage_kit_YellowRusty.paa","Spud_Stuff\SA_Storage\BaseObjects\SA_BaseKitBox\data\Storage_kit_YellowRusty.paa"};
		itemSize[] = {6,4};
	};  
	class SA_SoloLocker_Yellow_Kit: A6_Kit
	{
		scope=2;
		weight=6500;
		displayName="A.L.S. Solo Locker Kit";        
        model = "Spud_Stuff\SA_Storage\BaseObjects\SA_BaseKitBox\kitbox.p3d";
		descriptionShort = "Personal gear locker by Abyss Logistical Solutions. Holds full body kit slots and 250 inventory. Ideal for quick loadout swaps or compact loadouts. Can be disassembled with a screwdriver.";   
        hiddenSelections[]={"zbytek","camo"};   
        hiddenSelectionsTextures[] = {"Spud_Stuff\SA_Storage\BaseObjects\SA_BaseKitBox\data\Storage_kit_Yellow_Locker.paa","Spud_Stuff\SA_Storage\BaseObjects\SA_BaseKitBox\data\Storage_kit_Yellow_Locker.paa"};
		itemSize[] = {6,4};
	};  
	class SA_SoloLocker_White_Kit: A6_Kit
	{
		scope=2;
		weight=6500;
		displayName="A.L.S. Solo Locker Kit";        
        model = "Spud_Stuff\SA_Storage\BaseObjects\SA_BaseKitBox\kitbox.p3d";
		descriptionShort = "Personal gear locker by Abyss Logistical Solutions. Holds full body kit slots and 250 inventory. Ideal for quick loadout swaps or compact loadouts. Can be disassembled with a screwdriver.";   
        hiddenSelections[]={"zbytek","camo"};   
        hiddenSelectionsTextures[] = {"Spud_Stuff\SA_Storage\BaseObjects\SA_BaseKitBox\data\Storage_kit_White_Locker.paa","Spud_Stuff\SA_Storage\BaseObjects\SA_BaseKitBox\data\Storage_kit_White_Locker.paa"};
		itemSize[] = {6,4};
	};   
	class SA_SoloLocker_Green_Kit: A6_Kit
	{
		scope=2;
		weight=6500;
		displayName="A.L.S. Solo Locker Kit";        
        model = "Spud_Stuff\SA_Storage\BaseObjects\SA_BaseKitBox\kitbox.p3d";
		descriptionShort = "Personal gear locker by Abyss Logistical Solutions. Holds full body kit slots and 250 inventory. Ideal for quick loadout swaps or compact loadouts. Can be disassembled with a screwdriver.";   
        hiddenSelections[]={"zbytek","camo"};   
        hiddenSelectionsTextures[] = {"Spud_Stuff\SA_Storage\BaseObjects\SA_BaseKitBox\data\Storage_kit_Green_Locker.paa","Spud_Stuff\SA_Storage\BaseObjects\SA_BaseKitBox\data\Storage_kit_Green_Locker.paa"};
		itemSize[] = {6,4};
	};    
	class SA_SoloLocker_Black_Kit: A6_Kit
	{
		scope=2;
		weight=6500;
		displayName="A.L.S. Solo Locker Kit";        
        model = "Spud_Stuff\SA_Storage\BaseObjects\SA_BaseKitBox\kitbox.p3d";
		descriptionShort = "Personal gear locker by Abyss Logistical Solutions. Holds full body kit slots and 250 inventory. Ideal for quick loadout swaps or compact loadouts. Can be disassembled with a screwdriver.";   
        hiddenSelections[]={"zbytek","camo"};   
        hiddenSelectionsTextures[] = {"Spud_Stuff\SA_Storage\BaseObjects\SA_BaseKitBox\data\Storage_kit_Black_Locker.paa","Spud_Stuff\SA_Storage\BaseObjects\SA_BaseKitBox\data\Storage_kit_Black_Locker.paa"};
		itemSize[] = {6,4};
	};      
    class A6_Storage_Base;
    class A6_Openable_Base : A6_Storage_Base {};
	class SA_SoloLocker_Base: A6_Openable_Base
    {
        scope = 0;
        displayName = "A.L.S. Solo Locker";
        descriptionShort = "Personal gear locker by Abyss Logistical Solutions. Holds full body kit slots and 250 inventory. Ideal for quick loadout swaps or compact loadouts. Can be disassembled with a screwdriver.";
        model = "Spud_Stuff\SA_Storage\BaseObjects\SA_Singlelocker\SA_SoloLocker.p3d";
        rotationFlags = 2;
        itemBehaviour = 0;
        hiddenSelections[]={"abyss"};
        hiddenSelectionsTextures[] = {"Spud_Stuff\SA_Storage\BaseObjects\SA_Singlelocker\data\Redlocker\SA_Red.paa"};
        attachments[]=
        {
            "CodeLock",
            "Att_CombinationLock",
            "Headgear",
            "Eyewear",
            "Mask",
            "Armband",
            "Gloves",
            "Body",
            "Vest",
            "Back",
            "Hips",
            "Legs",
            "Feet"
        };
        class DamageSystem
        {
            class GlobalHealth
            {
                class Health
                {
                    hitpoints = 2500;
                    healthLevels[] = 
                    {
                        {1.0,{"Spud_Stuff\SA_Storage\BaseObjects\SA_Singlelocker\data\SA_Locker.rvmat"}},
                        {0.7,{"Spud_Stuff\SA_Storage\BaseObjects\SA_Singlelocker\data\SA_Locker.rvmat"}},
                        {0.5,{"Spud_Stuff\SA_Storage\BaseObjects\SA_Singlelocker\data\SA_Locker_damage.rvmat"}},
                        {0.3,{"Spud_Stuff\SA_Storage\BaseObjects\SA_Singlelocker\data\SA_Locker_damage.rvmat"}},
                        {0.0,{"Spud_Stuff\SA_Storage\BaseObjects\SA_Singlelocker\data\SA_Locker_destruct.rvmat"}}
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
			class Clothes
			{
				name = "Clothes";
				description = "";
				attachmentSlots[] = {"Headgear","Eyewear","Mask","Armband","Gloves","Body","Vest","Back","Hips","Legs","Feet"};
				icon = "set:dayz_inventory image:body";
			};
        };     
		class Cargo
		{
			itemsCargoSize[] = {10,25};
			openable = 1;
			allowOwnedCargoManipulation = 1;
		};        
		class AnimationSources
		{
			class Doors1
			{
				source="user";
				initPhase=0;
				animPeriod=1.5;
			};
		};
    };    
	class SA_SoloLocker_Red: SA_SoloLocker_Base
	{	
        scope = 2;
        displayName = "A.L.S. Red Solo Locker";
        hiddenSelections[]={"abyss"};
        hiddenSelectionsTextures[] = {"Spud_Stuff\SA_Storage\BaseObjects\SA_Singlelocker\data\Redlocker\SA_Red.paa"};
	};   
	class SA_SoloLocker_RedRusty: SA_SoloLocker_Base
	{	
        scope = 2;
        displayName = "A.L.S. RedRusty Solo Locker";
        hiddenSelections[]={"abyss"};
        hiddenSelectionsTextures[] = {"Spud_Stuff\SA_Storage\BaseObjects\SA_Singlelocker\data\Redlocker_Rusty\SA_Red_Rusty.paa"};
	};      
	class SA_SoloLocker_Yellow: SA_SoloLocker_Base
	{	
        scope = 2;
        displayName = "A.L.S. Yellow Solo Locker";
        hiddenSelections[]={"abyss"};
        hiddenSelectionsTextures[] = {"Spud_Stuff\SA_Storage\BaseObjects\SA_Singlelocker\data\Yellowlocker\SA_Yellow.paa"};
	};   
	class SA_SoloLocker_YellowRusty: SA_SoloLocker_Base
	{	
        scope = 2;
        displayName = "A.L.S. YellowRusty Solo Locker";
        hiddenSelections[]={"abyss"};
        hiddenSelectionsTextures[] = {"Spud_Stuff\SA_Storage\BaseObjects\SA_Singlelocker\data\Yellowlocker_Rusty\SA_Yellow_Rusty.paa"};
	};      
	class SA_SoloLocker_White: SA_SoloLocker_Base
	{	
        scope = 2;
        displayName = "A.L.S. White Solo Locker";
        hiddenSelections[]={"abyss"};
        hiddenSelectionsTextures[] = {"Spud_Stuff\SA_Storage\BaseObjects\SA_Singlelocker\data\Whitelocker\SA_White.paa"};
	};       
	class SA_SoloLocker_Green: SA_SoloLocker_Base
	{	
        scope = 2;
        displayName = "A.L.S. Green Solo Locker";
        hiddenSelections[]={"abyss"};
        hiddenSelectionsTextures[] = {"Spud_Stuff\SA_Storage\BaseObjects\SA_Singlelocker\data\Greenlocker\SA_Green.paa"};
	};       
	class SA_SoloLocker_Black: SA_SoloLocker_Base
	{	
        scope = 2;
        displayName = "A.L.S. Black Solo Locker";
        hiddenSelections[]={"abyss"};
        hiddenSelectionsTextures[] = {"Spud_Stuff\SA_Storage\BaseObjects\SA_Singlelocker\data\Blacklocker\SA_Blacklocker.paa"};
	};    
};