#include <vector>
#include <string>
#include <glslang/Public/ShaderLang.h>  // For EShLanguage

namespace VkShaderCompiler {
    // Initializes glslang library.
    void initializeGlslang();

    // Finalizes and cleans up glslang library.
    void finalizeGlslang();

    // Compiles GLSL shader code to SPIR-V.
    std::vector<uint32_t> compileGLSLtoSPIRV(const std::string& shaderCode, EShLanguage shaderType);

    // Initializes shader resource limits.
    TBuiltInResource InitResources();
}