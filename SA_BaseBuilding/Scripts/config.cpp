class CfgPatches
{
	class MT_Scripts
	{
		requiredAddons[] = { "DZ_Scripts" };
	};
};

class CfgAddons
{
    class PreloadAddons
    {
        class SA_BaseBuilding
        {
            list[]={};
        };
    };
};

class CfgMods
{
    class SA_BaseBuilding
    {
        name="";
        dir="SA_BaseBuilding";
        picture="";
        action="";
        author="";
        overview = "";
		inputs = "SA_BaseBuilding/Scripts/Inputs.xml";
		type = "mod";
        defines[] = {};
		dependencies[] =
		{
			"Game", "World", "Mission"
		};

        class defs
		{
			class imageSets
			{
				files[]= {};
			};
			class widgetStyles
			{
				files[]= {};
			};

			class engineScriptModule 
			{ 
				files[] = { "SA_BaseBuilding/Scripts/1_Core"};
			};

			class gameScriptModule
			{
				files[] = { "SA_BaseBuilding/Scripts/3_Game" };
			};
			class worldScriptModule
			{
				files[] = { "SA_BaseBuilding/Scripts/4_World" };
			};

			class missionScriptModule 
			{
				files[] = { "SA_BaseBuilding/Scripts/5_Mission" };
			};
		};
    };
};
