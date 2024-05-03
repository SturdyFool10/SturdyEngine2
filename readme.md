
# SturdyEngine2

**SturdyEngine2** is a cross-platform C++ engine designed for creating games for Linux and Windows. It leverages modern graphics technologies such as Vulkan to provide a powerful environment for game developers.

## Getting Started

To get up and running with SturdyEngine2, follow these steps. Please note that SturdyEngine2 does not ship with its dependencies, so you'll need to set these up manually.

### Step 1: Create Library Structure

Run the PowerShell script `createLibsStructure.ps1` to create the necessary folder structure for the libraries

### Step 2: Download Required Libraries

You will need to manually download and/or install the following libraries:
- **Vulkan SDK**
- **GLFW3**
- **GLM**

### Step 3: Organize Include Files

Navigate to `/libs/Core/includes`. Here, create two new folders(Case Sensitive):
- **GLM**
- **GLFW** 

Copy the contents of the `include` folders from both GLM and GLFW into the respective folders you just created.

### Step 4: Add Static Libraries

Add the static library files for GLFW(glfw3.lib) and any of the Vulkan standard libraries to the `/libs/Core/PreCompiled` folder. Simply placing them there is sufficient. note that GLM does not have a precompiled library and as such does not need this step

### Step 5: Build the Project

Run the script `build.ps1` to generate a Visual Studio solution file located in the root directory of the project

You can modify the project settings including the project name in `premake5.lua` as needed.

After following these steps, you should have a functioning development environment for SturdyEngine2. Happy coding!
