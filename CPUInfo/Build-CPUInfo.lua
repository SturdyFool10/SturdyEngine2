project "CPUInfo"
   kind "StaticLib"
   language "C++"
   cppdialect "C++latest"

   targetdir "binaries/%{prj.name}/%{cfg.buildcfg}"

   files { "src/**.cpp", "src/**.h" }

   includedirs
   {
      "src"
   }

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"
      symbols "On"

   filter "configurations:Dist"
      defines { "NDEBUG" }
      optimize "On"
      symbols "Off"
