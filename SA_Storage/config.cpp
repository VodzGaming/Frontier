class CfgPatches
{
	class SA_Storage
	{
		requiredAddons[] = 
		{
			"DZ_Data",
			"DZ_Characters",
			"DZ_Gear_Tools",
			"DZ_Scripts",
			"A6_Base_Storage",
			"vsm_addon_a6storage"
		};
		units[] = {};
		weapons[] = {};
	};
};
class CfgMods
{
	class SA_Storage
	{
		dir = "SA_Storage";
		picture = "";
		action = "";
		hideName = 1;
		hidePicture = 1;
		name = "SA_Storage";
		credits = "Potato";
		author = "Potato";
		authorID = "0";
		version = "1.0";
		extra = 0;
		inputs = "";
		type = "mod";
		dependencies[] = {"World"};
		class defs
		{			
			class imageSets
			{
				files[] = {"/Spud_Stuff/SA_Storage/gui/imagesets/SA_Storage_Slots.imageset"};
			};		
			class worldScriptModule
			{
				value = "";
				files[] = {"Spud_Stuff/SA_Storage/Scripts/4_World"};
			};
		};
	};	
};
