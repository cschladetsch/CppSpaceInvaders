#!/bin/bash
mkdir -p build && cd build && cmake .. && make && ./CppSpaceInvaders $@
cd ~/local/CppSpaceInvaders
