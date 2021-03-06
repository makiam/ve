Pipeline = {}

function FindFBX()
	
end

project "Pipeline"

	kind "StaticLib"
	flags { common_flags }

	builddeps { "Core", "Resources" }
	
	pchheader "Pipeline/API.h"
	pchsource "../src/Pipeline/Pipeline.cpp"
	
	files
	{
		"Pipeline.lua",
		"../inc/Pipeline/**.h",
		"../src/Pipeline/**.cpp",
	}
	
	vpaths
	{
		[""] = { "**src/Pipeline*", "**inc/Pipeline*" },
		["Importers"] = { "**/*Importer*" },
		["Importers/Milkshape"] = { "**/*Milkshape*" },
		["Importers/FBX"] = { "**/*FBX*" },
		["Processors"] = "**/*Processor*",
		["Processors/Image"] = "**/*ImageProcessor*",
		["Processors/Mesh"] = "**/*MeshProcessor*",
	}

	includedirs
	{
		"../src/",
		"../inc/",
		"../dep/nvtt/include",
		"../dep/FbxSdk/2012.1/include",
	}

	Pipeline.libdirs =
	{
		"../dep/FbxSdk/2012.1/lib/vs2010/x86",
	}
	
	Pipeline.links =
	{
	}

	Pipeline.links.Debug =
	{
		--"fbxsdk-2012.1d",
	}

	Pipeline.links.Release =
	{
		--"fbxsdk-2012.1",
	}