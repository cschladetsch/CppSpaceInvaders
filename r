#!/bin/bash
# Build and run Space Invaders

# Go to build directory and run cmake
cd build && cmake .. && make

# Check if build was successful
if [ $? -eq 0 ]; then
    echo "Build successful! Running game..."
    
    # Try to find and run the executable
    if [ -f "./bin/CppSpaceInvaders" ]; then
        ./bin/CppSpaceInvaders
    elif [ -f "./CppSpaceInvaders" ]; then
        ./CppSpaceInvaders
    elif [ -f "./bin/Debug/CppSpaceInvaders" ]; then
        ./bin/Debug/CppSpaceInvaders
    elif [ -f "./bin/Release/CppSpaceInvaders" ]; then
        ./bin/Release/CppSpaceInvaders
    else
        echo "Executable not found! Searching for it..."
        find .. -name "CppSpaceInvaders" -type f -executable
    fi
else
    echo "Build failed!"
fi
