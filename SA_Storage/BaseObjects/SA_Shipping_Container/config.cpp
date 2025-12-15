class CfgPatches
{
	class SA_shippingcontainer
	{
		units[] = {"SA_shippingcontainer_Kit","SA_shippingcontainer"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data","A6_Base_Storage","A6_BaseClasses","A6_BaseKitBoxes"};
	};
};
class CfgVehicles
{
	class A6_Item;
	class A6_Kit;
	class SA_ShippingContainer_Kit: A6_Kit
	{
		scope=2;
		weight=6500;
		displayName="A.L.S. Shipping Container Kit";        
        model = "Spud_Stuff\SA_Storage\BaseObjects\SA_BaseKitBox\kitbox.p3d";
		descriptionShort = "High-security container engineered by Abyss Logistical Solutions. Equipped with 10 rifle slots, 10 pistol mounts, full gear storage, and 1000 inventory. Designed for elite base command and long-term deployment logistics. Can be disassembled with a screwdriver.";   
        hiddenSelections[]={"zbytek","camo"};   
        hiddenSelectionsTextures[] = {"Spud_Stuff\SA_Storage\BaseObjects\SA_BaseKitBox\data\Storage_kit_shippingcontainer.paa","\Spud_Stuff\SA_Storage\BaseObjects\SA_BaseKitBox\data\Storage_kit_shippingcontainer.paa"};
		itemSize[] = {6,4};
	};    
    class A6_Storage_Base;
    class A6_Openable_Base : A6_Storage_Base {};
	class SA_ShippingContainer_Base: A6_Openable_Base
    {
        scope = 0;
        displayName = "A.L.S. Shipping Container";
        descriptionShort = "High-security container engineered by Abyss Logistical Solutions. Equipped with 10 rifle slots, 10 pistol mounts, full gear storage, and 1000 inventory. Designed for elite base command and long-term deployment logistics. Can be disassembled with a screwdriver.";
        model = "Spud_Stuff\SA_Storage\BaseObjects\SA_Shipping_Container\SA_shipping_container.p3d";
        rotationFlags = 2;
        itemBehaviour = 0;
        hiddenSelections[]={"abyss"};
        hiddenSelectionsTextures[] = {"Spud_Stuff\SA_Storage\BaseObjects\SA_Shipping_Container\data\Container_co.paa"};
        attachments[]=
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
			"Shoulder8",
			"Shoulder9",
			"Shoulder10",
            "Pistol1",
            "Pistol2",
            "Pistol3",
            "Pistol4",
            "Pistol5",
            "Pistol6",
            "Pistol7",
            "Pistol8",
            "Pistol9",
            "Pistol10",
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
                        {1.0,{"Spud_Stuff\SA_Storage\BaseObjects\SA_Shipping_Container\data\Container.rvmat"}},
                        {0.7,{"Spud_Stuff\SA_Storage\BaseObjects\SA_Shipping_Container\data\Container.rvmat"}},
                        {0.5,{"Spud_Stuff\SA_Storage\BaseObjects\SA_Shipping_Container\data\Container.rvmat"}},
                        {0.3,{"Spud_Stuff\SA_Storage\BaseObjects\SA_Shipping_Container\data\Container.rvmat"}},
                        {0.0,{"Spud_Stuff\SA_Storage\BaseObjects\SA_Shipping_Container\data\Container.rvmat"}}
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
					"Shoulder8",
					"Shoulder9",
					"Shoulder10"		
				};
				icon = "shoulderleft";
			};
			class Pistols
			{
				name = "Pistols";
				description = "";
				attachmentSlots[] = 
                {
                    "Pistol1",
                    "Pistol2",
                    "Pistol3",
                    "Pistol4",
                    "Pistol5",
                    "Pistol6",
                    "Pistol7",
                    "Pistol8",
                    "Pistol9",
                    "Pistol10"
                };
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
			itemsCargoSize[] = {10,200};
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
	class SA_ShippingContainer: SA_ShippingContainer_Base
	{	
        scope = 2;
        hiddenSelections[]={"abyss"};
        hiddenSelectionsTextures[] = {"Spud_Stuff\SA_Storage\BaseObjects\SA_Shipping_Container\data\Container_co.paa"};
	};   
};