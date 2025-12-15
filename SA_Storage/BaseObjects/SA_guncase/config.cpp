class CfgPatches
{
	class Abyss_guncase
	{
		units[] = {"Abyss_guncase_Kit","Abyss_guncase"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data","A6_BaseClasses","A6_BaseKitBoxes"};
	};
};
class CfgVehicles
{
	class A6_Item;
	class A6_Kit;
	class SA_Guncase_Kit: A6_Kit
	{
		scope=2;
		weight=6500;
		displayName="A.L.S. Gun Case Kit";        
        model = "Spud_Stuff\SA_Storage\BaseObjects\SA_BaseKitBox\kitbox.p3d";
        hiddenSelections[]={"zbytek","camo"};   
        hiddenSelectionsTextures[] = {"Spud_Stuff\SA_Storage\BaseObjects\SA_BaseKitBox\data\Storage_kit_guncase.paa","Spud_Stuff\SA_Storage\BaseObjects\SA_BaseKitBox\data\Storage_kit_guncase.paa"};
        descriptionShort = "Compact tactical gun case built for Abyss LS operatives. Stores up to 5 rifles and 2 pistols with additional gear space. Durable and secure, ideal for mobile armories. Can be disassembled with a screwdriver.";
		itemSize[] = {6,4};
	};    
    class A6_Storage_Base;
    class A6_Openable_Base : A6_Storage_Base {};
	class SA_Guncase_Base: A6_Openable_Base
    {
        scope = 0;
        displayName = "A.L.S. Gun Case";
        descriptionShort = "Compact tactical gun case built for Abyss LS operatives. Stores up to 5 rifles and 2 pistols with additional gear space. Durable and secure, ideal for mobile armories. Can be disassembled with a screwdriver.";
        model = "Spud_Stuff\SA_Storage\BaseObjects\SA_guncase\SA_GunCase.p3d";
        itemSize[] = {10,5};
        rotationFlags = 2;
        itemBehaviour = 0;
        hiddenSelections[]={"abyss","glass"};
        hiddenSelectionsTextures[] = {"Spud_Stuff\SA_Storage\BaseObjects\SA_guncase\data\Abyss_Gun_Case_co.paa","Spud_Stuff\SA_Storage\BaseObjects\SA_guncase\data\Abyss_Glass.paa"};
        attachments[] = 
		{
			"Shoulder1",
			"Shoulder2",
			"Shoulder3",
			"Shoulder4",
			"Shoulder5",
            "Pistol1",
            "Pistol2"				
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
                        {1.0,{"Spud_Stuff\SA_Storage\BaseObjects\SA_guncase\data\Abyss_Gun_Case.rvmat","Spud_Stuff\SA_Storage\BaseObjects\SA_guncase\data\glass_i.rvmat"}},
                        {0.7,{"Spud_Stuff\SA_Storage\BaseObjects\SA_guncase\data\Abyss_Gun_Case.rvmat","Spud_Stuff\SA_Storage\BaseObjects\SA_guncase\data\glass_i.rvmat"}},
                        {0.5,{"Spud_Stuff\SA_Storage\BaseObjects\SA_guncase\data\Abyss_Gun_Case_damage.rvmat","Spud_Stuff\SA_Storage\BaseObjects\SA_guncase\data\glass_i_damage.rvmat"}},
                        {0.3,{"Spud_Stuff\SA_Storage\BaseObjects\SA_guncase\data\Abyss_Gun_Case_damage.rvmat","Spud_Stuff\SA_Storage\BaseObjects\SA_guncase\data\glass_i_damage.rvmat"}},
                        {0.0,{"Spud_Stuff\SA_Storage\BaseObjects\SA_guncase\data\Abyss_Gun_Case_destruct.rvmat","Spud_Stuff\SA_Storage\BaseObjects\SA_guncase\data\glass_i_destruct.rvmat"}}
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
                    "Shoulder5"
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
                    "Pistol2"
                };
				icon = "pistol";
			};
        };            
		class Cargo
		{
			itemsCargoSize[] = {10,5};
			openable = 0;
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
			class hideproxy
            {
                source = "user";
				initPhase=0;
				animPeriod=1.5;
            };
        };
    };  
	class SA_Guncase: SA_Guncase_Base
	{	
        scope = 2;
        hiddenSelections[]={"abyss","glass"};
        hiddenSelectionsTextures[] = {"Spud_Stuff\SA_Storage\BaseObjects\SA_guncase\data\Abyss_Gun_Case_co.paa","Spud_Stuff\SA_Storage\BaseObjects\SA_guncase\data\Abyss_Glass.paa"};
	};    
};