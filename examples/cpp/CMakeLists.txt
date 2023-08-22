add_executable (example_cpp example.cpp)

find_library(LIBGFLAGS gflags REQUIRED)
find_library(LIBGLOG glog REQUIRED)
find_library(LIBABSL_LOG_INTERNAL_MESSAGE absl_log_internal_message REQUIRED)
find_library(LIBABSL_HASH absl_hash REQUIRED)
find_library(LIBABSL_CORD absl_cord REQUIRED)
find_library(LIBABSL_CORDZ_INFO absl_cordz_info REQUIRED)
find_library(LIBABSL_CORDZ_FUNCTIONS absl_cordz_functions REQUIRED)
find_library(LIBABSL_SYNCHRONIZATION absl_synchronization REQUIRED)
find_library(LIBGPR gpr REQUIRED)

target_include_directories(example_cpp
  PRIVATE "${CMAKE_SOURCE_DIR}/upsf"
  )

target_link_libraries (example_cpp PRIVATE
  upsf++
  ${LIBGFLAGS}
  ${LIBGLOG}
  ${LIBABSL_LOG_INTERNAL_MESSAGE}
  ${LIBABSL_HASH}
  ${LIBABSL_CORD}
  ${LIBABSL_CORDZ_INFO}
  ${LIBABSL_CORDZ_FUNCTIONS}
  ${LIBABSL_SYNCHRONIZATION}
  ${LIBGPR}
  )
