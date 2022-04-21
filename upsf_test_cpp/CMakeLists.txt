add_executable (upsf_test_cpp upsf_test_cpp.cpp)

find_library(LIBGFLAGS gflags REQUIRED)

target_include_directories(upsf_test_cpp
  PRIVATE "${CMAKE_SOURCE_DIR}/upsf"
  )

target_link_libraries (upsf_test_cpp PRIVATE
  upsf++
  ${LIBGFLAGS}
  )
