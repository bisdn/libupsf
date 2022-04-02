# Add executable called "helloDemo" that is built from the source files
# "demo.cxx" and "demo_b.cxx". The extensions are automatically found.
add_executable (upsfcli upsf_client.cpp)

target_include_directories(upsfcli
  PRIVATE "${CMAKE_SOURCE_DIR}/upsf"
  )

# Link the executable to the Hello library. Since the Hello library has
# public include directories we will use those link directories when building
# helloDemo
target_link_libraries (upsfcli PRIVATE
  upsf
  upsf_grpc_proto
  )
