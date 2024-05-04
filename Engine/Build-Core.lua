project "Core"
    kind "StaticLib"
    language "C++"
    cppdialect "C++latest"

    targetdir "binaries/%{prj.name}/%{cfg.buildcfg}"
    files { "src/**.cpp", "src/**.h" }

    -- Retrieve VK_SDK_PATH environment variable
    local vk_sdk_path = os.getenv("VK_SDK_PATH"):gsub("\\", "/")
    if vk_sdk_path == nil then
        print("ERROR: VK_SDK_PATH environment variable is not set.\nEnsure the Vulkan SDK has been installed.")
        os.exit(1) -- Exit if VK_SDK_PATH is not set
    end
    print("Vulkan SDK Found in folder: " .. vk_sdk_path)
    includedirs
    {
        "src",
        vk_sdk_path .. "Include/",  -- Include directory from VK_SDK_PATH, the vksdk path has a / at the end
        "../libs/Core/include"
    }
    
    links
    {
        "CPUInfo"
    }
    
    libdirs 
    {
        "../libs/Core/PreCompiled",
        vk_sdk_path .. "Lib"  -- Library directory from VK_SDK_PATH the vksdk path has a / at the end
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
