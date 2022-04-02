# Create a library called "Hello" which includes the source file "hello.cxx".
# The extension is already found. Any number of sources could be listed here.
add_library (upsf SHARED
  upsf.cpp
  )

find_library(LIBABSLSYNC absl_synchronization)
find_library(LIBPROTOBUF protobuf)
find_library(LIBGRPCPP grpc++)
find_library(LIBGRPC grpc)
find_program(PROTOBUF_PROTOC protoc)
find_program(GRPC_CPP_PLUGIN_EXECUTABLE grpc_cpp_plugin)

# Proto file
get_filename_component(upsf_proto "../upsf/protos/sssupsf.proto" ABSOLUTE)
get_filename_component(upsf_proto_path "${upsf_proto}" PATH)

# Generated sources
set(upsf_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/sssupsf.pb.cc")
set(upsf_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/sssupsf.pb.h")
set(upsf_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/sssupsf.grpc.pb.cc")
set(upsf_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/sssupsf.grpc.pb.h")
add_custom_command(
      OUTPUT "${upsf_proto_srcs}" "${upsf_proto_hdrs}" "${upsf_grpc_srcs}" "${upsf_grpc_hdrs}"
      COMMAND ${PROTOBUF_PROTOC}
      ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${upsf_proto_path}"
        --plugin=protoc-gen-grpc="${GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${upsf_proto}"
      DEPENDS "${upsf_proto}")

# Include generated *.pb.h files
include_directories("${CMAKE_CURRENT_BINARY_DIR}")

# libupsf_grpc_proto
add_library(upsf_grpc_proto SHARED
  ${upsf_grpc_srcs}
  ${upsf_grpc_hdrs}
  ${upsf_proto_srcs}
  ${upsf_proto_hdrs}
  )
target_link_libraries(upsf_grpc_proto
  ${LIBABSLSYNC}
  ${LIBPROTOBUF}
  ${LIBGRPC}
  ${LIBGRPCPP}
  )

# libupsf
target_link_libraries(upsf
  upsf_grpc_proto
  )


# Make sure the compiler can find include files for our Hello library
# when other libraries or executables link to Hello
target_include_directories (upsf PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

