project "App"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++latest"
   targetdir "Binaries/%{cfg.buildcfg}"
   staticruntime "off"

   files {
        "src/**.h",
        "src/**.cpp",
        "shaders/**.vert", 
        "shaders/**.frag", 
        "shaders/**.geom", 
        "shaders/**.comp",
        "shaders/**.tesc",
        "shaders/**.tese",
        "shaders/**.mesh",
        "shaders/**.task",
        "shaders/**.rgen",
        "shaders/**.rint",
        "shaders/**.rahit",
        "shaders/**.rchit",
        "shaders/**.rmiss",
        "shaders/**.rcall"
    }
   
   local vk_sdk_path = os.getenv("VK_SDK_PATH"):gsub("\\", "/")
   if vk_sdk_path == nil then
       print("[Runtime]: ERROR: VK_SDK_PATH environment variable is not set.\nEnsure the Vulkan SDK has been installed.")
       os.exit(1) -- Exit if VK_SDK_PATH is not set
   end
   print("[Runtime]: Vulkan SDK Found in folder: " .. vk_sdk_path)

   includedirs
   {
      "src",
	  -- Include Core and cpuinfo
      "../CPUInfo/src",
	  "../Engine/src",
      --dependancy includes
      "../libs/Core/include",
      "../libs/Runtime/include",
      vk_sdk_path .. "Include"
   }

   libdirs { "../libs/Core/PreCompiled", "../libs/Runtime/PreCompiled", vk_sdk_path .. "Lib" }

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
        