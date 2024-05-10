#include <glslang/Public/ShaderLang.h>
#include <glslang/Public/ResourceLimits.h>
#include <glslang/SPIRV/GlslangToSpv.h>
#include <iostream>
TBuiltInResource InitResources() {
    TBuiltInResource Resources;
    Resources.maxLights = 32;
    Resources.maxClipPlanes = 6;
    Resources.maxTextureUnits = 32;
    // Continue initializing other fields according to your shader needs...
    return Resources;
}
void initializeGlslang() {
    glslang::InitializeProcess();
}

void finalizeGlslang() {
    glslang::FinalizeProcess();
}

std::vector<uint32_t> compileGLSLtoSPIRV(const std::string& shaderCode, EShLanguage shaderType) {
    const char* shaderSource = shaderCode.c_str();

    glslang::TShader shader(shaderType);
    shader.setStrings(&shaderSource, 1);

    // Set the language version and compilation target
    int clientInputSemanticsVersion = 100; // replace with your target Vulkan version
    glslang::EShTargetClientVersion vulkanVersion = glslang::EShTargetVulkan_1_0;
    glslang::EShTargetLanguageVersion targetLanguageVersion = glslang::EShTargetSpv_1_0;

    shader.setEnvInput(glslang::EShSourceGlsl, shaderType, glslang::EShClientVulkan, clientInputSemanticsVersion);
    shader.setEnvClient(glslang::EShClientVulkan, vulkanVersion);
    shader.setEnvTarget(glslang::EShTargetSpv, targetLanguageVersion);

    TBuiltInResource resources = InitResources();
    EShMessages messages = (EShMessages)(EShMsgSpvRules | EShMsgVulkanRules);

    // Compile the shader to SPIR-V
    if (!shader.parse(&resources, 100, false, messages)) {
        std::cerr << "GLSL Parsing Failed for: " << std::endl << shader.getInfoLog() << std::endl;
        return {};
    }

    glslang::TProgram program;
    program.addShader(&shader);

    if (!program.link(messages)) {
        std::cerr << "GLSL Linking Failed for: " << std::endl << shader.getInfoLog() << std::endl;
        return {};
    }

    std::vector<uint32_t> spirv;
    glslang::GlslangToSpv(*program.getIntermediate(shaderType), spirv);
    return spirv;
}