# CMakeList.txt : CMake project for c++playgroun, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (aoc2025 "aoc2025.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET aoc2025 PROPERTY CXX_STANDARD 23)
endif()

# TODO: Add tests and install targets if needed.
