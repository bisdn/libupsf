add_executable (upsf_test_c upsf_test_c.c)

target_include_directories(upsf_test_c
  PRIVATE "${CMAKE_SOURCE_DIR}/upsf"
  )

target_link_libraries (upsf_test_c PUBLIC
  upsf++
  )
