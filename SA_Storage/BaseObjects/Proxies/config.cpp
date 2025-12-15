
class CfgPatches
{
	class SA_Storage_Proxies
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data","DZ_Characters_Tops","DZ_Characters_Backpacks","DZ_Scripts","DZ_Weapons_Firearms","DZ_Characters_Pants","DZ_Gear_Containers","DZ_Characters_Shoes","DZ_Gear_Consumables","DZ_Weapons_Explosives","DZ_Characters_Gloves"};
	};
};
class CfgVehicles
{
	class Inventory_Base;
	class HandSaw: Inventory_Base
	{
		inventorySlot[] = {"HandSaw1","HandSaw2","HandSaw3","HandSaw4","HandSaw5","HandSaw6"};
	};
	class Pliers: Inventory_Base
	{
		inventorySlot[] = {"Pliers1","Pliers2"};
	};
	class Screwdriver: Inventory_Base
	{
		inventorySlot[] = {"Screwdriver1","Screwdriver2"};
	};
	class Hatchet: Inventory_Base
	{
		inventorySlot[] = {"Hatchet1","Hatchet2","Hatchet3"};
	};
	class WoodAxe: Inventory_Base
	{
		inventorySlot[] = {"Axe","Shoulder","Melee"};
	};
	class FirefighterAxe: Inventory_Base
	{
		inventorySlot[] = {"Axe","Shoulder","Melee"};
	};
	class Pickaxe: Inventory_Base
	{
		inventorySlot[] = {"Pickaxe","Shoulder","Melee"};
	};
	class RA_CodeLock: Inventory_Base
	{
		inventorySlot[] = {"CodeLock"};
	};
};
class CfgNonAIVehicles
{
	class ProxyAttachment;
	class ProxyHandSaw1: ProxyAttachment
	{
		scope = 2;
		inventorySlot = "HandSaw1";
		model = "Spud_Stuff\SA_Storage\BaseObjects\Proxies\HandSaw1.p3d";
	};
	class ProxyHandSaw2: ProxyAttachment
	{
		scope = 2;
		inventorySlot = "HandSaw2";
		model = "Spud_Stuff\SA_Storage\BaseObjects\Proxies\HandSaw2.p3d";
	};
	class ProxyHandSaw3: ProxyAttachment
	{
		scope = 2;
		inventorySlot = "HandSaw3";
		model = "Spud_Stuff\SA_Storage\BaseObjects\Proxies\HandSaw3.p3d";
	};
	class ProxyHandSaw4: ProxyAttachment
	{
		scope = 2;
		inventorySlot = "HandSaw4";
		model = "Spud_Stuff\SA_Storage\BaseObjects\Proxies\HandSaw4.p3d";
	};
	class ProxyHandSaw5: ProxyAttachment
	{
		scope = 2;
		inventorySlot = "HandSaw5";
		model = "Spud_Stuff\SA_Storage\BaseObjects\Proxies\HandSaw5.p3d";
	};
	class ProxyHandSaw6: ProxyAttachment
	{
		scope = 2;
		inventorySlot = "HandSaw6";
		model = "Spud_Stuff\SA_Storage\BaseObjects\Proxies\HandSaw6.p3d";
	};
	class ProxyHatchet1: ProxyAttachment
	{
		scope = 2;
		inventorySlot = "Hatchet1";
		model = "Spud_Stuff\SA_Storage\BaseObjects\Proxies\Hatchet1.p3d";
	};
	class ProxyHatchet2: ProxyAttachment
	{
		scope = 2;
		inventorySlot = "Hatchet2";
		model = "Spud_Stuff\SA_Storage\BaseObjects\Proxies\Hatchet2.p3d";
	};
	class ProxyHatchet3: ProxyAttachment
	{
		scope = 2;
		inventorySlot = "Hatchet3";
		model = "Spud_Stuff\SA_Storage\BaseObjects\Proxies\Hatchet3.p3d";
	};
	class ProxyPliers1: ProxyAttachment
	{
		scope = 2;
		inventorySlot = "Pliers1";
		model = "Spud_Stuff\SA_Storage\BaseObjects\Proxies\Pliers1.p3d";
	};
	class ProxyPliers2: ProxyAttachment
	{
		scope = 2;
		inventorySlot = "Pliers2";
		model = "Spud_Stuff\SA_Storage\BaseObjects\Proxies\Pliers2.p3d";
	};
	class ProxyScrewdriver1: ProxyAttachment
	{
		scope = 2;
		inventorySlot = "Screwdriver1";
		model = "Spud_Stuff\SA_Storage\BaseObjects\Proxies\Screwdriver1.p3d";
	};
	class ProxyScrewdriver2: ProxyAttachment
	{
		scope = 2;
		inventorySlot = "Screwdriver2";
		model = "Spud_Stuff\SA_Storage\BaseObjects\Proxies\Screwdriver2.p3d";
	};
	class ProxyAxe: ProxyAttachment
	{
		scope = 2;
		inventorySlot = "Axe";
		model = "Spud_Stuff\SA_Storage\BaseObjects\Proxies\Axe.p3d";
	};
	class ProxyPickaxe: ProxyAttachment
	{
		scope = 2;
		inventorySlot = "Pickaxe";
		model = "Spud_Stuff\SA_Storage\BaseObjects\Proxies\Pickaxe.p3d";
	};
	class ProxyCodeLock: ProxyAttachment
	{
		scope = 2;
		inventorySlot = "CodeLock";
		model = "Spud_Stuff\SA_Storage\BaseObjects\Proxies\CodeLock.p3d";
	};
};
class CfgSlots
{
	class Slot_HandSaw1
	{
		name = "HandSaw1";
		displayName = "HandSaw";
		ghostIcon = "set:SA_Storage_Slots image:SA_Storage_HackSaw";
	};
	class Slot_HandSaw2
	{
		name = "HandSaw2";
		displayName = "HandSaw";
		ghostIcon = "set:SA_Storage_Slots image:SA_Storage_HackSaw";
	};
	class Slot_HandSaw3
	{
		name = "HandSaw3";
		displayName = "HandSaw";
		ghostIcon = "set:SA_Storage_Slots image:SA_Storage_HackSaw";
	};
	class Slot_HandSaw4
	{
		name = "HandSaw4";
		displayName = "HandSaw";
		ghostIcon = "set:SA_Storage_Slots image:SA_Storage_HackSaw";
	};
	class Slot_HandSaw5
	{
		name = "HandSaw5";
		displayName = "HandSaw";
		ghostIcon = "set:SA_Storage_Slots image:SA_Storage_HackSaw";
	};
	class Slot_HandSaw6
	{
		name = "HandSaw6";
		displayName = "HandSaw";
		ghostIcon = "set:SA_Storage_Slots image:SA_Storage_HackSaw";
	};
	class Slot_Hatchet1
	{
		name = "Hatchet1";
		displayName = "Hatchet";
		ghostIcon = "set:SA_Storage_Slots image:SA_Storage_Hatchet";
	};
	class Slot_Hatchet2
	{
		name = "Hatchet2";
		displayName = "Hatchet";
		ghostIcon = "set:SA_Storage_Slots image:SA_Storage_Hatchet";
	};
	class Slot_Hatchet3
	{
		name = "Hatchet3";
		displayName = "Hatchet";
		ghostIcon = "set:SA_Storage_Slots image:SA_Storage_Hatchet";
	};
	class Slot_Pliers1
	{
		name = "Pliers1";
		displayName = "Pliers";
		ghostIcon = "set:SA_Storage_Slots image:SA_Storage_Pliers";
	};
	class Slot_Pliers2
	{
		name = "Pliers2";
		displayName = "Pliers";
		ghostIcon = "set:SA_Storage_Slots image:SA_Storage_Pliers";
	};
	class Slot_Screwdriver1
	{
		name = "Screwdriver1";
		displayName = "Screwdriver";
		ghostIcon = "set:SA_Storage_Slots image:SA_Storage_ScrewDriver";
	};
	class Slot_Screwdriver2
	{
		name = "Screwdriver2";
		displayName = "Screwdriver";
		ghostIcon = "set:SA_Storage_Slots image:SA_Storage_ScrewDriver";
	};
	class Slot_Axe
	{
		name = "Axe";
		displayName = "Axe";
		ghostIcon = "set:SA_Storage_Slots image:SA_Storage_Axe";
	};
	class Slot_Pickaxe
	{
		name = "Pickaxe";
		displayName = "Pickaxe";
		ghostIcon = "set:SA_Storage_Slots image:SA_Storage_PickAxe";
	};
};
