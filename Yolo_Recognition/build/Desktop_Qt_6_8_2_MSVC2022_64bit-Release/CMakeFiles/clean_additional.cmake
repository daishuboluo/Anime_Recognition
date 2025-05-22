# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Release")
  file(REMOVE_RECURSE
  "CMakeFiles\\Yolo_Recognition_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\Yolo_Recognition_autogen.dir\\ParseCache.txt"
  "Yolo_Recognition_autogen"
  )
endif()
