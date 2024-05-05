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

   filter "configurations:Debug"
       defines { "DEBUG" }
       runtime "Debug"
       symbols "On"

   filter "configurations:Release"
       defines { "RELEASE" }
       runtime "Release"
       optimize "On"
       symbols "On"

   filter "configurations:Dist"
       defines { "DIST" }
       runtime "Release"
       optimize "On"
       symbols "Off"