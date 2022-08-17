add_executable (example_c example.c)

target_include_directories(example_c
  PRIVATE "${CMAKE_SOURCE_DIR}/upsf"
  )

target_link_libraries (example_c PUBLIC
  upsf++
  )
