# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/SymulatorWypozyczalni_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/SymulatorWypozyczalni_autogen.dir/ParseCache.txt"
  "SymulatorWypozyczalni_autogen"
  )
endif()
