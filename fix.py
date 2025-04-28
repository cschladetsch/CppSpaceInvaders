#!/usr/bin/env python3
"""
Robust script to fix the Space Invaders project structure:
1. Create the proper directory structure
2. Copy header files to the correct locations (avoiding duplicates)
3. Fix include paths in source files
"""

import os
import re
import shutil
import glob

def fix_includes_in_file(file_path):
    """Fix include statements in a source file"""
    try:
        with open(file_path, 'r') as file:
            content = file.read()
        
        # Replace relative includes with proper include paths
        updated_content = re.sub(
            r'#include\s+"\.\.\/([A-Za-z]+\.h)"', 
            r'#include "\1"',  # Change to direct includes without prefix
            content
        )
        
        # Also fix Entity/ includes if they exist
        updated_content = re.sub(
            r'#include\s+"Entity\/([A-Za-z]+\.h)"', 
            r'#include "\1"',  # Change to direct includes without prefix
            updated_content
        )
        
        # Write back if changes were made
        if content != updated_content:
            with open(file_path, 'w') as file:
                file.write(updated_content)
            print(f"Fixed includes in: {file_path}")
            return True
        return False
    except Exception as e:
        print(f"Error processing {file_path}: {e}")
        return False

# Create necessary directories
print("Creating directory structure...")
os.makedirs("include", exist_ok=True)

# Find and copy header files to include directory
print("Copying header files...")
copied_files = set()

# Check specific files that we know should exist
entity_headers = ["UFO.h", "Barrier.h", "Bullet.h", "Enemy.h", "Player.h", "Graphics.h", "Transform.h"]

# Look in common locations for headers
potential_locations = [".", "src", "src/Entity", "src/Core"]
for location in potential_locations:
    for header in entity_headers:
        header_path = os.path.join(location, header)
        if os.path.exists(header_path) and header not in copied_files:
            # Ensure we're not copying from include/ to include/
            if not header_path.startswith("include/"):
                try:
                    dest_path = os.path.join("include", header)
                    # Only copy if target doesn't exist or is different
                    if not os.path.exists(dest_path) or not os.path.samefile(header_path, dest_path):
                        shutil.copy(header_path, dest_path)
                        print(f"Copied {header_path} to include/")
                    copied_files.add(header)
                except Exception as e:
                    print(f"Warning: Could not copy {header_path}: {e}")

if not copied_files:
    print("Warning: No header files found to copy! You may need to manually copy your .h files to the include directory.")

# Find all cpp files and fix includes
print("Fixing include paths in source files...")
count = 0
for root, dirs, files in os.walk("src"):
    for file in files:
        if file.endswith(".cpp"):
            file_path = os.path.join(root, file)
            if fix_includes_in_file(file_path):
                count += 1

print(f"Fixed includes in {count} files")

# Update CMakeLists.txt to use the simple include structure
cmake_path = "CMakeLists.txt"
if os.path.exists(cmake_path):
    try:
        with open(cmake_path, 'r') as file:
            cmake_content = file.read()
        
        # Make sure we're using file globbing and the correct include directory
        if "target_include_directories" in cmake_content:
            # Use simpler include structure
            updated_cmake = re.sub(
                r'target_include_directories\([^)]+\)',
                'target_include_directories(${PROJECT_NAME} PRIVATE\n    ${CMAKE_CURRENT_SOURCE_DIR}/include\n    ${SDL3_INCLUDE_DIRS}\n)',
                cmake_content
            )
            
            if cmake_content != updated_cmake:
                with open(cmake_path, 'w') as file:
                    file.write(updated_cmake)
                print("Updated CMakeLists.txt with simpler include structure")
    except Exception as e:
        print(f"Error updating CMakeLists.txt: {e}")

print("\nDone! Now try building your project again with:")
print("cd build")
print("cmake ..")
print("make")
