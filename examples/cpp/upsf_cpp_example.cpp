/* upsf_cpp_example
 *
 * BSD 3-Clause License
 *
 * Copyright (c) 2022, bisdn GmbH
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "upsf_cpp_example.hpp"

#include <gflags/gflags.h>

#include <string>

using namespace upsf;

DEFINE_string(upsfhost, "localhost", "UPSF host");
DEFINE_int32(upsfport, 50051, "UPSF port");
DEFINE_string(sgname, "my-sg", "service gateway name");
DEFINE_string(tsfname, "my-tsf", "traffic steering function name");
DEFINE_string(sgrpname, "my-sgrp", "subscriber group name");
DEFINE_string(sctxname, "my-sctx", "session context name");
DEFINE_string(ncname, "my-nc", "network connection name");

/*
 * Get list of service gateways
 */
void UpsfCppExample::sg_list()
{
    /* list service gateways */
    std::cout << "=== ServiceGateway list" << std::endl;

    std::vector<wt474_messages::v1::ServiceGateway> items;
    if (!client.ReadV1(items)) {
        LOG(ERROR) << "failure: list service gateways" << std::endl;
        return;
    }

    for (int i = 0; i < items.size(); i++) {
        std::cout << ">>> " << i << ". " << ServiceGatewayStream(items[i])
                  << std::endl;
    }
}

/*
 * Read details for service gateway
 */
void UpsfCppExample::sg_read(const std::string& sg_name)
{
    /* read service gateway */
    std::cout << "=== ServiceGateway(" << sg_name << ") read" << std::endl;
    wt474_messages::v1::ServiceGateway r_reply;
    if (!client.ReadV1(sg_name, r_reply)) {
        LOG(ERROR) << "failure: read service gateway" << std::endl;
        return;
    }
    std::cout << ">>> reply: " << ServiceGatewayStream(r_reply) << std::endl;
}

/*
 * Return true if service gateway exists
 */
bool UpsfCppExample::sg_exists(const std::string& sg_name)
{
    wt474_messages::v1::ServiceGateway r_reply;
    if (!client.ReadV1(sg_name, r_reply)) {
        return false;
    }
    return true;
}

/*
 * Create and update service gateway
 */
void UpsfCppExample::sg_create(const std::string& sg_name)
{
    if (!sg_exists(sg_name)) {
        /* create service gateway */
        std::cout << "=== ServiceGateway(" << sg_name << ") create" << std::endl;

        wt474_messages::v1::ServiceGateway c_request;
        wt474_messages::v1::ServiceGateway c_reply;

        /* set service gateway name */
        c_request.set_name(sg_name);

        /* use the associated stream helper from upsf_stream.hpp */
        std::cout << "<<< request: " << ServiceGatewayStream(c_request)
                  << std::endl;

        /* create SG, returns 0 in case of failure */
        if (!client.CreateV1(c_request, c_reply)) {
            LOG(ERROR) << "failure: create service gateway" << std::endl;
            return;
        }

        /* use the associated stream helper from upsf_stream.hpp */
        std::cout << ">>> reply: " << ServiceGatewayStream(c_reply)
                  << std::endl;
    }

    /* update service gateway */
    std::cout << "=== ServiceGateway(" << sg_name << ") update" << std::endl;

    wt474_messages::v1::ServiceGateway u_request;
    wt474_messages::v1::ServiceGateway u_reply;
    wt474_upsf_service::v1::UpdateReq::UpdateOptions options;

    /* set list_merge_strategy for UpdateOptions */
    options.set_list_merge_strategy(
        wt474_upsf_service::v1::UpdateReq::UpdateOptions::replace);
    /* set service gateway name */
    u_request.set_name(sg_name);
    /* set service gateway description */
    u_request.mutable_metadata()->set_description(sg_desc);
    /* set service gateway derived state */
    u_request.mutable_metadata()->set_derived_state(
        wt474_messages::v1::DerivedState::active);

    /* use the associated stream helper from upsf_stream.hpp */
    std::cout << "<<< request: " << ServiceGatewayStream(u_request)
              << std::endl;

    /* update SG, returns 0 in case of failure */
    if (!client.UpdateV1(u_request, u_reply, options)) {
        LOG(ERROR) << "failure: update service gateway" << std::endl;
        return;
    }

    /* use the associated stream helper from upsf_stream.hpp */
    std::cout << ">>> reply: " << ServiceGatewayStream(u_reply) << std::endl;
}

/*
 * Delete service gateway
 */
void UpsfCppExample::sg_delete(const std::string& sg_name)
{
    /* delete service gateway */
    std::cout << "=== ServiceGateway(" << sg_name << ") delete" << std::endl;

    std::string d_reply;
    if (!client.DeleteV1(sg_name, d_reply)) {
        LOG(ERROR) << "failure: delete service gateway" << std::endl;
        return;
    }

    /* use the associated stream helper from upsf_stream.hpp */
    std::cout << ">>> reply: " << d_reply << std::endl;
}

/* *****************************************************************************
 */

/*
 * Get list of service gateway user planes
 */
void UpsfCppExample::up_list()
{
    /* list service gateway user planes */
    std::cout << "=== ServiceGatewayUserPlane list" << std::endl;

    std::vector<wt474_messages::v1::ServiceGatewayUserPlane> items;
    if (!client.ReadV1(items)) {
        LOG(ERROR) << "failure: list service gateway user planes" << std::endl;
        return;
    }

    for (int i = 0; i < items.size(); i++) {
        std::cout << ">>> " << i << ". "
                  << ServiceGatewayUserPlaneStream(items[i]) << std::endl;
    }
}

/*
 * Read details for service gateway user plane
 */
void UpsfCppExample::up_read(const std::string& up_name)
{
    /* read service gateway user plane */
    std::cout << "=== ServiceGatewayUserPlane(" << up_name << ") read"
              << std::endl;

    wt474_messages::v1::ServiceGatewayUserPlane r_reply;
    if (!client.ReadV1(up_name, r_reply)) {
        LOG(ERROR) << "failure: read service gateway user plane" << std::endl;
        return;
    }

    /* use the associated stream helper from upsf_stream.hpp */
    std::cout << ">>> reply: " << ServiceGatewayUserPlaneStream(r_reply)
              << std::endl;
}

/*
 * Return true if service gateway user plane exists
 */
bool UpsfCppExample::up_exists(const std::string& up_name)
{
    wt474_messages::v1::ServiceGatewayUserPlane r_reply;
    if (!client.ReadV1(up_name, r_reply)) {
        return false;
    }
    return true;
}

/*
 * Create and update service gateway user plane
 */
void UpsfCppExample::up_create(const std::string& up_name,
    const std::string& up_desc)
{
    if (!up_exists(up_name)) {
        /* create service gateway user plane */
        std::cout << "=== ServiceGatewayUserPlane(" << up_name << ") create"
                  << std::endl;

        wt474_messages::v1::ServiceGatewayUserPlane c_request;
        wt474_messages::v1::ServiceGatewayUserPlane c_reply;

        /* set service gateway user plane name */
        c_request.set_name(up_name);

        /* use the associated stream helper from upsf_stream.hpp */
        std::cout << "<<< request: " << ServiceGatewayUserPlaneStream(c_request)
                  << std::endl;

        /* create SGUP, returns 0 in case of failure */
        if (!client.CreateV1(c_request, c_reply)) {
            LOG(ERROR) << "failure: create service gateway user plane"
                       << std::endl;
            return;
        }

        /* use the associated stream helper from upsf_stream.hpp */
        std::cout << ">>> reply: " << ServiceGatewayUserPlaneStream(c_reply)
                  << std::endl;
    }

    /* update service gateway user plane */
    std::cout << "=== ServiceGatewayUserPlane(" << up_name << ") update"
              << std::endl;

    wt474_messages::v1::ServiceGatewayUserPlane u_request;
    wt474_messages::v1::ServiceGatewayUserPlane u_reply;
    wt474_upsf_service::v1::UpdateReq::UpdateOptions options;

    u_request.set_name(up_name);
    u_request.set_service_gateway_name(sg_name);
    u_request.mutable_metadata()->set_description(up_desc);
    u_request.mutable_metadata()->set_derived_state(
        wt474_messages::v1::DerivedState::updating);
    u_request.mutable_spec()->set_max_session_count(128);
    u_request.mutable_spec()->set_max_shards(16);
    u_request.mutable_spec()->add_supported_service_group("ssg-1");
    u_request.mutable_spec()->add_supported_service_group("ssg-2");
    u_request.mutable_spec()->mutable_default_endpoint()->set_endpoint_name(
        "ep-1");
    u_request.mutable_spec()
        ->mutable_default_endpoint()
        ->mutable_vtep()
        ->set_ip_address("192.168.56.11");
    u_request.mutable_spec()
        ->mutable_default_endpoint()
        ->mutable_vtep()
        ->set_udp_port(4789);
    u_request.mutable_spec()
        ->mutable_default_endpoint()
        ->mutable_vtep()
        ->set_vni(12345);
    u_request.mutable_status()->set_allocated_session_count(7);
    u_request.mutable_status()->set_allocated_shards(1);

    /* use the associated stream helper from upsf_stream.hpp */
    std::cout << "<<< request: " << ServiceGatewayUserPlaneStream(u_request)
              << std::endl;

    if (!client.UpdateV1(u_request, u_reply, options)) {
        LOG(ERROR) << "failure: update service gateway user plane" << std::endl;
        return;
    }

    /* use the associated stream helper from upsf_stream.hpp */
    std::cout << ">>> reply: " << ServiceGatewayUserPlaneStream(u_reply)
              << std::endl;
}

/*
 * Delete service gateway user plane
 */
void UpsfCppExample::up_delete(const std::string& up_name)
{
    /* delete service gateway */
    std::cout << "=== ServiceGatewayUserPlane(" << up_name << ") delete"
              << std::endl;

    std::string d_reply;
    if (!client.DeleteV1(up_name, d_reply)) {
        LOG(ERROR) << "failure: delete service gateway user plane" << std::endl;
        return;
    }
    std::cout << ">>> reply: " << d_reply << std::endl;
}

/* *****************************************************************************
 */

/*
 * Get list of traffic steering functions
 */
void UpsfCppExample::tsf_list()
{
    /* list traffic steering functions */
    std::cout << "=== TrafficSteeringFunctions list" << std::endl;

    std::vector<wt474_messages::v1::TrafficSteeringFunction> items;
    if (!client.ReadV1(items)) {
        LOG(ERROR) << "failure: list traffic steering functions" << std::endl;
        return;
    }

    for (int i = 0; i < items.size(); i++) {
        std::cout << ">>> " << i << ". "
                  << TrafficSteeringFunctionStream(items[i]) << std::endl;
    }
}

/*
 * Read details for traffic steering function
 */
void UpsfCppExample::tsf_read(const std::string& tsf_name)
{
    /* read traffic steering function */
    std::cout << "=== TrafficSteeringFunction(" << tsf_name << ") read"
              << std::endl;

    wt474_messages::v1::TrafficSteeringFunction r_reply;
    if (!client.ReadV1(tsf_name, r_reply)) {
        LOG(ERROR) << "failure: read traffic steering function" << std::endl;
        return;
    }

    /* use the associated stream helper from upsf_stream.hpp */
    std::cout << ">>> reply: " << TrafficSteeringFunctionStream(r_reply)
              << std::endl;
}

/*
 * Return true if traffic steering function exists
 */
bool UpsfCppExample::tsf_exists(const std::string& tsf_name)
{
    wt474_messages::v1::TrafficSteeringFunction r_reply;
    if (!client.ReadV1(tsf_name, r_reply)) {
        return false;
    }
    return true;
}

/*
 * Create and update traffic steering function
 */
void UpsfCppExample::tsf_create(const std::string& tsf_name)
{
    if (!tsf_exists(tsf_name)) {
        /* create traffic steering function */
        std::cout << "=== TrafficSteeringFunction(" << tsf_name << ") create"
                  << std::endl;

        wt474_messages::v1::TrafficSteeringFunction c_request;
        wt474_messages::v1::TrafficSteeringFunction c_reply;

        /* set traffic steering function name */
        c_request.set_name(tsf_name);

        /* use the associated stream helper from upsf_stream.hpp */
        std::cout << "<<< request: " << TrafficSteeringFunctionStream(c_request)
                  << std::endl;

        if (!client.CreateV1(c_request, c_reply)) {
            LOG(ERROR) << "failure: create traffic steering function"
                       << std::endl;
            return;
        }

        /* use the associated stream helper from upsf_stream.hpp */
        std::cout << ">>> reply: " << TrafficSteeringFunctionStream(c_reply)
                  << std::endl;
    }

    /* update traffic steering function */
    std::cout << "=== TrafficSteeringFunction(" << tsf_name << ") update"
              << std::endl;

    wt474_messages::v1::TrafficSteeringFunction u_request;
    wt474_messages::v1::TrafficSteeringFunction u_reply;
    wt474_upsf_service::v1::UpdateReq::UpdateOptions options;

    u_request.set_name(tsf_name);
    u_request.mutable_metadata()->set_description(tsf_desc);
    u_request.mutable_metadata()->set_derived_state(
        wt474_messages::v1::DerivedState::updating);
    u_request.mutable_spec()->mutable_default_endpoint()->set_endpoint_name(
        "ep-0");
    u_request.mutable_spec()
        ->mutable_default_endpoint()
        ->mutable_vtep()
        ->set_ip_address("192.168.30.11");
    u_request.mutable_spec()
        ->mutable_default_endpoint()
        ->mutable_vtep()
        ->set_udp_port(4789);
    u_request.mutable_spec()
        ->mutable_default_endpoint()
        ->mutable_vtep()
        ->set_vni(12345);

    /* use the associated stream helper from upsf_stream.hpp */
    std::cout << "<<< request: " << TrafficSteeringFunctionStream(u_request)
              << std::endl;

    if (!client.UpdateV1(u_request, u_reply, options)) {
        LOG(ERROR) << "failure: update traffic steering function" << std::endl;
        return;
    }

    /* use the associated stream helper from upsf_stream.hpp */
    std::cout << ">>> reply: " << TrafficSteeringFunctionStream(u_reply)
              << std::endl;
}

/*
 * Delete traffic steering function
 */
void UpsfCppExample::tsf_delete(const std::string& tsf_name)
{
    /* delete traffic steering function */
    std::cout << "=== TrafficSteeringFunction(" << tsf_name << ") delete"
              << std::endl;

    std::string d_reply;
    if (!client.DeleteV1(tsf_name, d_reply)) {
        LOG(ERROR) << "failure: delete traffic steering function" << std::endl;
        return;
    }

    /* use the associated stream helper from upsf_stream.hpp */
    std::cout << ">>> reply: " << d_reply << std::endl;
}

/* *****************************************************************************
 */

/*
 * Get list of subscriber groups
 */
void UpsfCppExample::sgrp_list()
{
    /* list service gateways */
    std::cout << "=== SubscriberGroup list" << std::endl;

    std::vector<wt474_messages::v1::Shard> items;
    if (!client.ReadV1(items)) {
        LOG(ERROR) << "failure: list shards" << std::endl;
        return;
    }

    for (int i = 0; i < items.size(); i++) {
        std::cout << ">>> " << i << ". " << ShardStream(items[i]) << std::endl;
    }
}

/*
 * Read details for subscriber group
 */
void UpsfCppExample::sgrp_read(const std::string& sgrp_name)
{
    /* read shard */
    std::cout << "=== SubscriberGroup(" << sgrp_name << ") read" << std::endl;

    wt474_messages::v1::Shard r_reply;
    if (!client.ReadV1(sgrp_name, r_reply)) {
        LOG(ERROR) << "failure: read shard" << std::endl;
        return;
    }

    /* use the associated stream helper from upsf_stream.hpp */
    std::cout << ">>> reply: " << ShardStream(r_reply) << std::endl;
}

/*
 * Return true if subscriber group exists
 */
bool UpsfCppExample::sgrp_exists(const std::string& sgrp_name)
{
    wt474_messages::v1::Shard r_reply;
    if (!client.ReadV1(sgrp_name, r_reply)) {
        return false;
    }
    return true;
}

/*
 * Create and update subscriber group
 */
void UpsfCppExample::sgrp_create(const std::string& sgrp_name)
{
    if (!sgrp_exists(sgrp_name)) {
        /* create shard */
        std::cout << "=== SubscriberGroup(" << tsf_name << ") create"
                  << std::endl;

        wt474_messages::v1::Shard c_request;
        wt474_messages::v1::Shard c_reply;

        /* set subscriber group name */
        c_request.set_name(sgrp_name);

        /* use the associated stream helper from upsf_stream.hpp */
        std::cout << "<<< request: " << ShardStream(c_request) << std::endl;

        if (!client.CreateV1(c_request, c_reply)) {
            LOG(ERROR) << "failure: create shard" << std::endl;
            return;
        }

        /* use the associated stream helper from upsf_stream.hpp */
        std::cout << ">>> reply: " << ShardStream(c_reply) << std::endl;
    }

    /* update shard */
    std::cout << "=== SubscriberGroup(" << tsf_name << ") update" << std::endl;

    wt474_messages::v1::Shard u_request;
    wt474_messages::v1::Shard u_reply;
    wt474_upsf_service::v1::UpdateReq::UpdateOptions options;

    u_request.set_name(sgrp_name);
    u_request.mutable_metadata()->set_description(sgrp_desc);
    u_request.mutable_metadata()->set_derived_state(
        wt474_messages::v1::DerivedState::updating);
    u_request.mutable_spec()->set_max_session_count(4096);
    u_request.mutable_spec()->set_virtual_mac("22:22:22:22:22:22");
    u_request.mutable_spec()
        ->mutable_desired_state()
        ->set_service_gateway_user_plane("up-a");
    u_request.mutable_spec()->mutable_desired_state()->add_network_connection(
        "net-conn-1");
    u_request.mutable_spec()->mutable_desired_state()->add_network_connection(
        "net-conn-2");
    u_request.mutable_spec()->add_prefix("192.168.2.0/24");
    u_request.mutable_spec()->add_prefix("192.168.3.0/24");
    u_request.mutable_status()->set_allocated_session_count(64);
    u_request.mutable_status()
        ->mutable_current_state()
        ->set_service_gateway_user_plane("up-b");
    (*u_request.mutable_status()
            ->mutable_current_state()
            ->mutable_tsf_network_connection())["key1"]
        = "value1";
    (*u_request.mutable_status()
            ->mutable_current_state()
            ->mutable_tsf_network_connection())["key2"]
        = "value2";
    u_request.mutable_mbb()->set_mbb_state(
        wt474_messages::v1::Shard_Mbb_MbbState::
            Shard_Mbb_MbbState_upstream_switchover_required);

    /* use the associated stream helper from upsf_stream.hpp */
    std::cout << "<<< request: " << ShardStream(u_request) << std::endl;

    if (!client.UpdateV1(u_request, u_reply, options)) {
        LOG(ERROR) << "failure: update shard" << std::endl;
        return;
    }

    /* use the associated stream helper from upsf_stream.hpp */
    std::cout << ">>> reply: " << ShardStream(u_reply) << std::endl;
}

/*
 * Delete subscriber group
 */
void UpsfCppExample::sgrp_delete(const std::string& sgrp_name)
{
    /* delete shard */
    std::cout << "=== SubscriberGroup(" << sgrp_name << ") delete" << std::endl;

    std::string d_reply;
    if (!client.DeleteV1(sgrp_name, d_reply)) {
        LOG(ERROR) << "failure: delete shard" << std::endl;
        return;
    }

    /* use the associated stream helper from upsf_stream.hpp */
    std::cout << ">>> reply: " << d_reply << std::endl;
}

/* *****************************************************************************
 */

/*
 * Get list of session contexts
 */
void UpsfCppExample::sctx_list()
{
    /* list session contexts */
    std::cout << "=== SessionContext list" << std::endl;

    std::vector<wt474_messages::v1::SessionContext> items;
    if (!client.ReadV1(items)) {
        LOG(ERROR) << "failure: read service gateways" << std::endl;
        return;
    }

    for (int i = 0; i < items.size(); i++) {
        std::cout << ">>> " << i << ". " << SessionContextStream(items[i]) << std::endl;
    }
}

/*
 * Read details for session context
 */
void UpsfCppExample::sctx_read(const std::string& sctx_name)
{
    /* read session_context */
    std::cout << "=== SessionContext(" << sctx_name << ") read" << std::endl;

    wt474_messages::v1::SessionContext r_reply;
    if (!client.ReadV1(sctx_name, r_reply)) {
        LOG(ERROR) << "failure: read session_context" << std::endl;
        return;
    }

    /* use the associated stream helper from upsf_stream.hpp */
    std::cout << ">>> reply: " << SessionContextStream(r_reply) << std::endl;
}

/*
 * Return true if session context exists
 */
bool UpsfCppExample::sctx_exists(const std::string& sctx_name)
{
    wt474_messages::v1::SessionContext r_reply;
    if (!client.ReadV1(sctx_name, r_reply)) {
        return false;
    }
    return true;
}

/*
 * Create and update session context
 */
void UpsfCppExample::sctx_create(const std::string& sctx_name)
{
    if (!sctx_exists(sctx_name)) {
        /* create session context */
        std::cout << "=== SessionContext(" << sctx_name << ") create"
                  << std::endl;

        wt474_messages::v1::SessionContext c_request;
        wt474_messages::v1::SessionContext c_reply;

        /* set session context name */
        c_request.set_name(sctx_name);

        /* use the associated stream helper from upsf_stream.hpp */
        std::cout << "<<< request: " << SessionContextStream(c_request) << std::endl;

        if (!client.CreateV1(c_request, c_reply)) {
            LOG(ERROR) << "failure: create session_context" << std::endl;
            return;
        }

        /* use the associated stream helper from upsf_stream.hpp */
        std::cout << ">>> reply: " << SessionContextStream(c_reply) << std::endl;
    }

    /* update session_context */
    std::cout << "=== SessionContext(" << sctx_name << ") update"
              << std::endl;

    wt474_messages::v1::SessionContext u_request;
    wt474_messages::v1::SessionContext u_reply;
    wt474_upsf_service::v1::UpdateReq::UpdateOptions options;

    u_request.set_name(sctx_name);
    u_request.mutable_metadata()->set_description(sctx_desc);
    u_request.mutable_metadata()->set_derived_state(
        wt474_messages::v1::DerivedState::updating);
    u_request.mutable_spec()->set_traffic_steering_function("tsf-a");
    u_request.mutable_spec()->add_required_service_group("rsg-1");
    u_request.mutable_spec()->add_required_service_group("rsg-2");
    u_request.mutable_spec()->set_required_quality(3);
    u_request.mutable_spec()->set_circuit_id("circuit-id");
    u_request.mutable_spec()->set_remote_id("remote-id");
    u_request.mutable_spec()
        ->mutable_session_filter()
        ->set_source_mac_address("00:11:22:33:44:55");
    u_request.mutable_spec()->mutable_session_filter()->set_svlan(50);
    u_request.mutable_spec()->mutable_session_filter()->set_cvlan(100);
    u_request.mutable_spec()->mutable_desired_state()->set_shard(sgrp_name);
    u_request.mutable_status()->mutable_current_state()->set_user_plane_shard(
        "up-a");
    u_request.mutable_status()->mutable_current_state()->set_tsf_shard(
        "tsf-a");

    /* use the associated stream helper from upsf_stream.hpp */
    std::cout << "<<< request: " << SessionContextStream(u_request) << std::endl;

    if (!client.UpdateV1(u_request, u_reply, options)) {
        LOG(ERROR) << "failure: update session_context" << std::endl;
        return;
    }

    /* use the associated stream helper from upsf_stream.hpp */
    std::cout << ">>> reply: " << SessionContextStream(u_reply) << std::endl;
}

/*
 * Delete session context
 */
void UpsfCppExample::sctx_delete(const std::string& sctx_name)
{
    /* delete session_context */
    std::cout << "=== SessionContext(" << sctx_name << ") delete" << std::endl;

    std::string d_reply;
    if (!client.DeleteV1(sctx_name, d_reply)) {
        LOG(ERROR) << "failure: delete session_context" << std::endl;
        return;
    }

    /* use the associated stream helper from upsf_stream.hpp */
    std::cout << ">>> reply: " << d_reply << std::endl;
}

/* *****************************************************************************
 */

/*
 * Get list of network connections
 */
void UpsfCppExample::nc_list()
{
    /* list network connections */
    std::cout << "=== NetworkConnection list" << std::endl;

    std::vector<wt474_messages::v1::NetworkConnection> items;
    if (!client.ReadV1(items)) {
        LOG(ERROR) << "failure: list network connections" << std::endl;
        return;
    }

    for (int i = 0; i < items.size(); i++) {
        std::cout << ">>> " << i << ". " << NetworkConnectionStream(items[i]) << std::endl;
    }
}

/*
 * Read details for network connection
 */
void UpsfCppExample::nc_read(const std::string& nc_name)
{
    /* read traffic steering function */
    std::cout << "=== NetworkConnection(" << nc_name << ") read" << std::endl;

    wt474_messages::v1::NetworkConnection r_reply;
    if (!client.ReadV1(nc_name, r_reply)) {
        LOG(ERROR) << "failure: read network connection" << std::endl;
        return;
    }

    /* use the associated stream helper from upsf_stream.hpp */
    std::cout << ">>> reply: " << NetworkConnectionStream(r_reply) << std::endl;
}

/*
 * Return true if network connection exists
 */
bool UpsfCppExample::nc_exists(const std::string& nc_name)
{
    wt474_messages::v1::SessionContext r_reply;
    if (!client.ReadV1(nc_name, r_reply)) {
        return false;
    }
    return true;
}

/*
 * Create and update network connection
 */
void UpsfCppExample::nc_create(const std::string& nc_name)
{
    if (!nc_exists(nc_name)) {
        /* create network connection */
        std::cout << "=== NetworkConnection(" << nc_name << ") create"
                  << std::endl;

        wt474_messages::v1::NetworkConnection c_request;
        wt474_messages::v1::NetworkConnection c_reply;

        c_request.set_name(nc_name);

        /* use the associated stream helper from upsf_stream.hpp */
        std::cout << "<<< request: " << NetworkConnectionStream(c_request) << std::endl;

        if (!client.CreateV1(c_request, c_reply)) {
            LOG(ERROR) << "failure: create network connection" << std::endl;
            return;
        }

        /* use the associated stream helper from upsf_stream.hpp */
        std::cout << ">>> reply: " << NetworkConnectionStream(c_reply) << std::endl;
    }

    /* update network connection */
    std::cout << "=== NetworkConnection(" << nc_name << ") update"
              << std::endl;

    wt474_messages::v1::NetworkConnection u_request;
    wt474_messages::v1::NetworkConnection u_reply;
    wt474_upsf_service::v1::UpdateReq::UpdateOptions options;

    u_request.set_name(nc_name);
    u_request.mutable_metadata()->set_description(nc_desc);
    u_request.mutable_metadata()->set_derived_state(
        wt474_messages::v1::DerivedState::updating);
    u_request.mutable_spec()->set_maximum_supported_quality(7);
    u_request.mutable_spec()
        ->mutable_ss_ptp()
        ->add_sgup_endpoint()
        ->set_endpoint_name("vxlan0");
    u_request.mutable_spec()
        ->mutable_ss_ptp()
        ->mutable_sgup_endpoint(0)
        ->mutable_vtep()
        ->set_ip_address("1.1.1.1");
    u_request.mutable_spec()
        ->mutable_ss_ptp()
        ->mutable_sgup_endpoint(0)
        ->mutable_vtep()
        ->set_udp_port(4789);
    u_request.mutable_spec()
        ->mutable_ss_ptp()
        ->mutable_sgup_endpoint(0)
        ->mutable_vtep()
        ->set_vni(12345);
    u_request.mutable_spec()
        ->mutable_ss_ptp()
        ->add_sgup_endpoint()
        ->set_endpoint_name("l2vpn0");
    u_request.mutable_spec()
        ->mutable_ss_ptp()
        ->mutable_sgup_endpoint(1)
        ->mutable_l2vpn()
        ->set_vpn_id(3333);
    u_request.mutable_spec()
        ->mutable_ss_ptp()
        ->add_sgup_endpoint()
        ->set_endpoint_name("port0");
    u_request.mutable_spec()
        ->mutable_ss_ptp()
        ->mutable_sgup_endpoint(2)
        ->mutable_port_vlan()
        ->set_logical_port("eth0");
    u_request.mutable_spec()
        ->mutable_ss_ptp()
        ->mutable_sgup_endpoint(2)
        ->mutable_port_vlan()
        ->set_logical_port("eth0");
    u_request.mutable_spec()
        ->mutable_ss_ptp()
        ->mutable_sgup_endpoint(2)
        ->mutable_port_vlan()
        ->set_svlan(50);
    u_request.mutable_spec()
        ->mutable_ss_ptp()
        ->mutable_sgup_endpoint(2)
        ->mutable_port_vlan()
        ->set_cvlan(0);
    u_request.mutable_spec()
        ->mutable_ss_ptp()
        ->mutable_tsf_endpoint()
        ->set_endpoint_name("vxl100");
    u_request.mutable_spec()
        ->mutable_ss_ptp()
        ->mutable_tsf_endpoint()
        ->mutable_vtep()
        ->set_ip_address("192.168.30.11");
    u_request.mutable_spec()
        ->mutable_ss_ptp()
        ->mutable_tsf_endpoint()
        ->mutable_vtep()
        ->set_udp_port(4789);
    u_request.mutable_spec()
        ->mutable_ss_ptp()
        ->mutable_tsf_endpoint()
        ->mutable_vtep()
        ->set_vni(12345);
    (*u_request.mutable_status()->mutable_nc_active())["net-conn-1"] = true;
    (*u_request.mutable_status()->mutable_nc_active())["net-conn-2"] = true;
    (*u_request.mutable_status()->mutable_nc_active())["net-conn-3"] = true;
    u_request.mutable_status()->set_allocated_shards(97);

    /* use the associated stream helper from upsf_stream.hpp */
    std::cout << "<<< request: " << NetworkConnectionStream(u_request) << std::endl;

    std::cout << NetworkConnectionStream(u_request) << std::endl;
    if (!client.UpdateV1(u_request, u_reply, options)) {
        LOG(ERROR) << "failure: update traffic steering function" << std::endl;
        return;
    }

    /* use the associated stream helper from upsf_stream.hpp */
    std::cout << ">>> reply: " << NetworkConnectionStream(u_reply) << std::endl;
}

/*
 * Delete network connection
 */
void UpsfCppExample::nc_delete(const std::string& nc_name)
{
    /* delete traffic steering function */
    std::cout << "=== NetworkConnection(" << nc_name << ") delete" << std::endl;

    std::string d_reply;
    if (!client.DeleteV1(nc_name, d_reply)) {
        LOG(ERROR) << "failure: delete network connection" << std::endl;
        return;
    }

    /* use the associated stream helper from upsf_stream.hpp */
    std::cout << ">>> reply: " << d_reply << std::endl;
}

/* *****************************************************************************
 */

void UpsfCppExample::create_items()
{
    std::cout << std::endl
              << "<<< SERVICE GATEWAYS >>>" << std::endl
              << std::endl;

    sg_list();
    sg_create(sg_name);
    sg_read(sg_name);
    sg_list();

    std::cout << std::endl
              << "<<< SERVICE GATEWAY USER PLANES >>>" << std::endl
              << std::endl;

    up_list();
    for (auto up : up_names) {
        up_create(up.first, up.second);
        up_read(up.first);
        up_list();
    }

    std::cout << std::endl
              << "<<< TRAFFIC STEERING FUNCTIONS >>>" << std::endl
              << std::endl;

    tsf_list();
    tsf_create(tsf_name);
    tsf_read(tsf_name);
    tsf_list();

    std::cout << std::endl
              << "<<< SHARDS (SUBSCRIBER GROUPS) >>>" << std::endl
              << std::endl;

    sgrp_list();
    sgrp_create(sgrp_name);
    sgrp_read(sgrp_name);
    sgrp_list();

    std::cout << std::endl
              << "<<< SESSION CONTEXTS >>>" << std::endl
              << std::endl;

    sctx_list();
    sctx_create(sctx_name);
    sctx_read(sctx_name);
    sctx_list();

    std::cout << std::endl
              << "<<< NETWORK CONNECTIONS >>>" << std::endl
              << std::endl;

    nc_list();
    nc_create(nc_name);
    nc_read(nc_name);
    nc_list();
}

void UpsfCppExample::delete_items()
{
    std::cout << std::endl
              << "<<< NETWORK CONNECTIONS >>>" << std::endl
              << std::endl;

    nc_delete(nc_name);
    nc_list();

    std::cout << std::endl
              << "<<< SESSION CONTEXTS >>>" << std::endl
              << std::endl;

    sctx_delete(sctx_name);
    sctx_list();

    std::cout << std::endl
              << "<<< SHARDS (SUBSCRIBER GROUPS) >>>" << std::endl
              << std::endl;

    sgrp_delete(sgrp_name);
    sgrp_list();

    std::cout << std::endl
              << "<<< TRAFFIC STEERING FUNCTIONS >>>" << std::endl
              << std::endl;

    tsf_delete(tsf_name);
    tsf_list();

    std::cout << std::endl
              << "<<< SERVICE GATEWAY USER PLANES >>>" << std::endl
              << std::endl;

    for (auto up : up_names) {
        up_delete(up.first);
    }
    up_list();

    std::cout << std::endl
              << "<<< SERVICE GATEWAYS >>>" << std::endl
              << std::endl;

    sg_delete(sg_name);
    sg_list();
}

int main(int argc, char** argv)
{
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    google::InitGoogleLogging(argv[0]);

    VLOG(1) << "UPSF host: " << FLAGS_upsfhost << std::endl;
    VLOG(1) << "UPSF port: " << FLAGS_upsfport << std::endl;
    VLOG(1) << "logtostderr: " << FLAGS_logtostderr << std::endl;

    // upsf address
    std::stringstream upsfaddr;
    upsfaddr << FLAGS_upsfhost << ":" << FLAGS_upsfport;

    // example
    UpsfCppExample example(upsfaddr.str(), FLAGS_sgname,
        FLAGS_tsfname, FLAGS_sgrpname, FLAGS_sctxname, FLAGS_ncname);

    // create items
    example.create_items();

    // remove items
    example.delete_items();

    return 0;
};
