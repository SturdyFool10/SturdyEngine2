project "Editor"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++latest"
   targetdir "../Binaries/%{cfg.buildcfg}/%{prj.name}"
   staticruntime "off"

   files { "src/**.h", "src/**.cpp" }

   includedirs
   {
      "src",
	  -- Include Core and cpuinfo
      "../CPUInfo/src",
	  "../Engine/src",
      --dependancy includes
      "../libs/Core/include",
      "../libs/Runtime/include",
      "../libs/Editor/include"
   }

   libdirs { "../libs/Core/PreCompiled", "../libs/Runtime/PreCompiled", "../libs/Editor/PreCompiled" }

   links
   {
      "Core",
      "CPUInfo"
   }

   targetdir ("../Binaries/" .. OutputDir .. "/%{prj.name}")
   objdir ("../Binaries/Intermediates/%{cfg.buildcfg}/%{prj.name}")
    filter "configurations:DebugWin"
       defines { "DEBUG" }
       symbols "On"

    filter "configurations:ReleaseWin"
        defines { "NDEBUG" }
        optimize "On"
        symbols "On"

    filter "configurations:DistWin"
        defines { "NDEBUG" }
        optimize "On"
        symbols "Off"

    filter "configurations:DebugVulkanWin"
        defines { "DEBUG", "VULKAN" }
        symbols "On"

    filter "configurations:ReleaseVulkanWin"
        defines { "NDEBUG", "VULKAN" }
        optimize "On"
        symbols "On"

    filter "configurations:DistVulkanWin"
        defines { "NDEBUG", "VULKAN" }
        optimize "On"
        symbols "Off"