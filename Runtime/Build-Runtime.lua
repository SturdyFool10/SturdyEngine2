project "App"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++latest"
   targetdir "Binaries/%{cfg.buildcfg}"
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
      "../libs/Runtime/include"
   }

   libdirs { "../libs/Core/PreCompiled", "../libs/Runtime/PreCompiled" }

   links
   {
      "Core"
   }

   targetdir("../Binaries/%{cfg.buildcfg}/%{wks.name}")
   targetname "%{wks.name}"
   objdir ("../Binaries/Intermediates/%{cfg.buildcfg}/%{prj.name}")

    filter "system:windows"
       systemversion "latest"
       defines { "WINDOWS" }

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