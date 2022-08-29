add_executable (example_cpp example.cpp)

find_library(LIBGFLAGS gflags REQUIRED)

target_include_directories(example_cpp
  PRIVATE "${CMAKE_SOURCE_DIR}/upsf"
  )

target_link_libraries (example_cpp PRIVATE
  upsf++
  ${LIBGFLAGS}
  )
