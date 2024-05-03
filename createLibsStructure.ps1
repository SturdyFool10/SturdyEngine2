# Define the base path
$basePath = "libs"

# Define the list of directories to be created under the base path
$directories = @(
    "Core/PreCompiled",
    "Core/include",
    "Runtime/PreCompiled",
    "Runtime/include",
    "Editor/PreCompiled",
    "Editor/include"
)

# Iterate over each directory path
foreach ($dir in $directories) {
    # Combine the base path with the current directory path
    $fullPath = Join-Path -Path $basePath -ChildPath $dir

    # Check if the directory exists
    if (-Not (Test-Path $fullPath)) {
        # Create the directory since it does not exist
        New-Item -Path $fullPath -ItemType Directory | Out-Null
        Write-Host "Created directory: $fullPath"
    } else {
        Write-Host "Directory already exists: $fullPath"
    }
}
