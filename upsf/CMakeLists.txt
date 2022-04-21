# dependencies
#
find_library(LIBPROTOBUF protobuf REQUIRED)
find_library(LIBGRPCPP grpc++ REQUIRED)
find_library(LIBGRPC grpc REQUIRED)
find_library(LIBGFLAGS gflags REQUIRED)
find_library(LIBGLOG glog REQUIRED)

# Proto compiler for cpp
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

# libupsf
#
add_library (upsf++ SHARED
  upsf.cpp
  upsf_c_wrapper.cpp
  ${upsf_grpc_srcs}
  ${upsf_grpc_hdrs}
  ${upsf_proto_srcs}
  ${upsf_proto_hdrs}
  )
set_target_properties(upsf++ PROPERTIES
  PUBLIC_HEADER "upsf.h;upsf.hpp"
  )
target_include_directories (upsf++ PUBLIC
  ${CMAKE_CURRENT_BINARY_DIR}
  )
target_link_libraries(upsf++
  PUBLIC ${LIBPROTOBUF}
  PUBLIC ${LIBGRPC}
  PUBLIC ${LIBGRPCPP}
  PUBLIC ${LIBGFLAGS}
  PUBLIC ${LIBGLOG}
  )
configure_file(upsf++.pc.in upsf++.pc @ONLY) 
 
# install
#
install(TARGETS upsf++)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/upsf++.pc"
  DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig"
  )
