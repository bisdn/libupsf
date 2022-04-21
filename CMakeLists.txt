cmake_minimum_required (VERSION 3.8.0)
project (upsf
  LANGUAGES C CXX
  HOMEPAGE_URL https://gitlab.bisdn.de/DBNG/libupsf
  DESCRIPTION "a UPSF gRPC client implementation in C++"
  VERSION 0.1.0
  )

add_subdirectory (upsf)
add_subdirectory (upsf_test_c)
add_subdirectory (upsf_test_cpp)
