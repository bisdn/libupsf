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
get_filename_component(upsf_messages_proto "../upsf/protos/wt474_upsf_messages/v1/messages_v1.proto" ABSOLUTE)
get_filename_component(upsf_service_proto "../upsf/protos/wt474_upsf_service/v1/service_v1.proto" ABSOLUTE)
get_filename_component(upsf_messages_proto_path "../upsf/protos" ABSOLUTE)
get_filename_component(upsf_service_proto_path "../upsf/protos" ABSOLUTE)

# Generated sources
set(upsf_messages_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/wt474_upsf_messages/v1/messages_v1.pb.cc")
set(upsf_messages_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/wt474_upsf_messages/v1/messages_v1.pb.h")
set(upsf_messages_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/wt474_upsf_messages/v1/messages_v1.grpc.pb.cc")
set(upsf_messages_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/wt474_upsf_messages/v1/messages_v1.grpc.pb.h")
add_custom_command(
      OUTPUT "${upsf_messages_proto_srcs}" "${upsf_messages_proto_hdrs}" "${upsf_messages_grpc_srcs}" "${upsf_messages_grpc_hdrs}"
      COMMAND ${PROTOBUF_PROTOC}
      ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${upsf_messages_proto_path}"
        --plugin=protoc-gen-grpc="${GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${upsf_messages_proto}"
      DEPENDS "${upsf_messages_proto}")

set(upsf_service_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/wt474_upsf_service/v1/service_v1.pb.cc")
set(upsf_service_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/wt474_upsf_service/v1/service_v1.pb.h")
set(upsf_service_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/wt474_upsf_service/v1/service_v1.grpc.pb.cc")
set(upsf_service_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/wt474_upsf_service/v1/service_v1.grpc.pb.h")
add_custom_command(
      OUTPUT "${upsf_service_proto_srcs}" "${upsf_service_proto_hdrs}" "${upsf_service_grpc_srcs}" "${upsf_service_grpc_hdrs}"
      COMMAND ${PROTOBUF_PROTOC}
      ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${upsf_service_proto_path}"
        -I "${upsf_messages_proto_path}"
        --plugin=protoc-gen-grpc="${GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${upsf_service_proto}"
      DEPENDS "${upsf_service_proto}")


# libupsf
#
add_library (upsf++ SHARED
  upsf.cpp
  upsf_c_wrapper.cpp
  upsf_c_mapping.cpp
  ${upsf_messages_grpc_srcs}
  ${upsf_messages_grpc_hdrs}
  ${upsf_messages_proto_srcs}
  ${upsf_messages_proto_hdrs}
  ${upsf_service_grpc_srcs}
  ${upsf_service_grpc_hdrs}
  ${upsf_service_proto_srcs}
  ${upsf_service_proto_hdrs}
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
