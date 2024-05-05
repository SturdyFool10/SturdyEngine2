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