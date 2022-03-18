# CMakeList.txt : CMake project for HuobiDomTest, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (HuobiDomTest "HuobiDomTest.cpp" "HuobiDomTest.h" "common.h" "CompareDoms.cpp" "DateTime.cpp" "DateTime.h" "DomMapImplementation.cpp" "DomMapImplementation.h" "DomVectorImplementation.cpp" "DomVectorImplementation.h" "HighResClock.cpp" "HighResClock.h" "HuobiDomTest.cpp" "HuobiDomTest.h" "stdafx.cpp" "stdafx.h" "StringParser.cpp" "StringParser.h" "targetver.h")

# TODO: Add tests and install targets if needed.
