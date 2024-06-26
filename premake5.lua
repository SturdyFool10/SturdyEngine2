-- premake5.lua
workspace "New Project"
   architecture "x64"
   configurations { "DebugWin", "ReleaseWin", "DistWin", "DebugVulkanWin", "ReleaseVulkanWin", "DistVulkanWin" }
   startproject "App"

   -- Workspace-wide build options for MSVC
   filter "system:windows"
      buildoptions { "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }

OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"
group "Engine"
	include "Engine/Build-Core.lua"
group "Editor"
    include "Editor/Build-Editor.lua"
group ""

include "Runtime/Build-Runtime.lua"
