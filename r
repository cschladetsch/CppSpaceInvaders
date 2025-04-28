#!/bin/bash
mkdir -p build && cd build && cmake .. && make && ./bin/CppSpaceInvaders $@
cd ..
