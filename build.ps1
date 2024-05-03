Write-Host "Cleaning and regenerating project files..."
Start-Sleep -Seconds 1
premake5 clean
Write-Host "Building for Visual Studio 2022..."
premake5 vs2022
Write-Host "Build completed."