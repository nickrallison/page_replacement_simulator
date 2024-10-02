 # Almost all CMake files should start with this
# You should always specify a range with the newest
# and oldest tested versions of CMake. This will ensure
# you pick up the best policies.
cmake_minimum_required(VERSION 3.15...3.30)

# This is your project statement. You should always list languages;
# Listing the version is nice here since it sets lots of useful variables
project(
  CPSC457A2
  VERSION 1.0
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Subdirectories for cmake
add_subdirectory(src/scheduler)
add_subdirectory(src/page)
add_subdirectory(src/simulator)
add_subdirectory(src/executable)

# add_subdirectory(src/test)
# add_subdirectory(src/graphing)