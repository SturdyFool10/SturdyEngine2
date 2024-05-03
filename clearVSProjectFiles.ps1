# Define the root directory to start the search
$rootDirectory = Get-Location

# Define the file patterns to search for
$filePatterns = @("*.vcxproj", "*.sln")

# Get all files matching the patterns in the directory and its subdirectories
$filesToDelete = Get-ChildItem -Path $rootDirectory -Include $filePatterns -Recurse -File

# Loop through the files and delete them
foreach ($file in $filesToDelete) {
    Write-Host "Deleting file: $($file.FullName)"
    Remove-Item $file.FullName -Force
}

Write-Host "Deletion complete."
