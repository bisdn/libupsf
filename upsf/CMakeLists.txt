# BSD 3-Clause License
# 
# Copyright (c) 2022, bisdn GmbH
# All rights reserved.
# 
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
# 
# 1. Redistributions of source code must retain the above copyright notice, this
#    list of conditions and the following disclaimer.
# 
# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
# 
# 3. Neither the name of the copyright holder nor the names of its
#    contributors may be used to endorse or promote products derived from
#    this software without specific prior written permission.
# 
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

# dependencies
#
find_package(PkgConfig)
pkg_check_modules(PROTOBUF REQUIRED IMPORTED_TARGET protobuf)
pkg_check_modules(GRPCPP REQUIRED IMPORTED_TARGET grpc++)
pkg_check_modules(GRPC REQUIRED IMPORTED_TARGET grpc)
pkg_check_modules(GLOG REQUIRED IMPORTED_TARGET libglog>=0.7)

# Proto compiler for cpp
find_program(PROTOBUF_PROTOC protoc)
find_program(GRPC_CPP_PLUGIN_EXECUTABLE grpc_cpp_plugin HINTS /usr/bin REQUIRED)

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
  PkgConfig::PROTOBUF
  PkgConfig::GRPC
  PkgConfig::GRPCPP
  PkgConfig::GLOG
  )
target_link_libraries(upsf++ PUBLIC
  PkgConfig::PROTOBUF
  PkgConfig::GRPC
  PkgConfig::GRPCPP
  PkgConfig::GLOG
  )
configure_file(upsf++.pc.in upsf++.pc @ONLY) 
 
# install
#
install(TARGETS upsf++)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/upsf++.pc"
  DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig"
  )
