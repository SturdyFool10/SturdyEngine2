project "Core"
    kind "StaticLib"
    language "C++"
    cppdialect "C++latest"

    targetdir "../Binaries/%{cfg.buildcfg}/%{prj.name}"
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
    libdirs 
    {
        "../libs/Core/PreCompiled",
        vk_sdk_path .. "Lib"  -- Library directory from VK_SDK_PATH the vksdk path has a / at the end
    }
    links { 
        "glfw3",
        "dxcompiler", 
        "dxcompilerd", 
        "GenericCodeGen", 
        "GenericCodeGend", 
        "glslang-default-resource-limits", 
        "glslang-default-resource-limitsd", 
        "glslang", 
        "glslangd", 
        "MachineIndependent", 
        "MachineIndependentd", 
        "OSDependent", 
        "OSDependentd", 
        "shaderc", 
        "shadercd", 
        "shaderc_combined", 
        "shaderc_combinedd", 
        "shaderc_shared", 
        "shaderc_sharedd", 
        "shaderc_util", 
        "shaderc_utild", 
        "spirv-cross-c-shared", 
        "spirv-cross-c-sharedd", 
        "spirv-cross-c", 
        "spirv-cross-cd", 
        "spirv-cross-core", 
        "spirv-cross-cored", 
        "spirv-cross-cpp", 
        "spirv-cross-cppd", 
        "spirv-cross-glsl", 
        "spirv-cross-glsld", 
        "spirv-cross-hlsl", 
        "spirv-cross-hlsld", 
        "spirv-cross-msl", 
        "spirv-cross-msld", 
        "spirv-cross-reflect", 
        "spirv-cross-reflectd", 
        "spirv-cross-util", 
        "spirv-cross-utild", 
        "SPIRV-Tools-diff", 
        "SPIRV-Tools-diffd", 
        "SPIRV-Tools-link", 
        "SPIRV-Tools-linkd", 
        "SPIRV-Tools-lint", 
        "SPIRV-Tools-lintd", 
        "SPIRV-Tools-opt", 
        "SPIRV-Tools-optd", 
        "SPIRV-Tools-reduce", 
        "SPIRV-Tools-reduced", 
        "SPIRV-Tools-shared", 
        "SPIRV-Tools-sharedd", 
        "SPIRV-Tools", 
        "SPIRV-Toolsd", 
        "SPIRV", 
        "SPIRVd", 
        "SPVRemapper", 
        "SPVRemapperd", 
        "vulkan-1"
    }
    targetname "SturdyEngine2"
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