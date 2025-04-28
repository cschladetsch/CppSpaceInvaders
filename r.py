#!/usr/bin/env python3
"""
Quick script to fix relative include paths in C++ source files
"""

import os
import re
import subprocess

def fix_includes_in_file(file_path):
    # Read the file
    with open(file_path, 'r') as file:
        content = file.read()
    
    # Replace relative includes with proper include paths
    updated_content = re.sub(
        r'#include\s+"\.\.\/([A-Za-z]+\.h)"', 
        r'#include "Entity/\1"', 
        content
    )
    
    # Write back if changes were made
    if content != updated_content:
        with open(file_path, 'w') as file:
            file.write(updated_content)
        print(f"Fixed includes in: {file_path}")
        return True
    return False

# Create necessary directories
os.makedirs("include/Entity", exist_ok=True)
os.makedirs("include/Core", exist_ok=True)

# Find all cpp files and fix includes
count = 0
for root, dirs, files in os.walk("src"):
    for file in files:
        if file.endswith(".cpp"):
            file_path = os.path.join(root, file)
            if fix_includes_in_file(file_path):
                count += 1

print(f"Fixed includes in {count} files")
print("Done! Now try building your project again.")
