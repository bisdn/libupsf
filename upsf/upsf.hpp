/* upsf.hpp
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

#ifndef UPSF_HPP
#define UPSF_HPP

#include <mutex>
#include <shared_mutex>
#include <thread>

#include <glog/logging.h>
#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>

#include "wt474_upsf_messages/v1/messages_v1.grpc.pb.h"
#include "wt474_upsf_messages/v1/messages_v1.pb.h"
#include "wt474_upsf_service/v1/service_v1.grpc.pb.h"
#include "wt474_upsf_service/v1/service_v1.pb.h"

namespace upsf {

class UpsfSubscriber {

public:
    /**
   * constructor
   */
    UpsfSubscriber(
        bool watch = false)
        : watch(watch)
    {
        /* get all items */
        itemtypes.emplace(itemtypes.end(), wt474_upsf_service::v1::ItemType::service_gateway);
        itemtypes.emplace(itemtypes.end(), wt474_upsf_service::v1::ItemType::service_gateway_user_plane);
        itemtypes.emplace(itemtypes.end(), wt474_upsf_service::v1::ItemType::traffic_steering_function);
        itemtypes.emplace(itemtypes.end(), wt474_upsf_service::v1::ItemType::network_connection);
        itemtypes.emplace(itemtypes.end(), wt474_upsf_service::v1::ItemType::shard);
        itemtypes.emplace(itemtypes.end(), wt474_upsf_service::v1::ItemType::session_context);
        /* in all states */
        derivedstates.emplace(derivedstates.end(), wt474_messages::v1::DerivedState::unknown);
        derivedstates.emplace(derivedstates.end(), wt474_messages::v1::DerivedState::inactive);
        derivedstates.emplace(derivedstates.end(), wt474_messages::v1::DerivedState::active);
        derivedstates.emplace(derivedstates.end(), wt474_messages::v1::DerivedState::updating);
    };

    /**
   * constructor
   */
    UpsfSubscriber(
        const std::vector<wt474_upsf_service::v1::ItemType>& itemtypes,
        bool watch = false)
        : itemtypes(itemtypes)
        , watch(watch) {};

    /**
   * constructor
   */
    UpsfSubscriber(
        const std::vector<wt474_messages::v1::DerivedState>& derivedstates,
        bool watch = false)
        : derivedstates(derivedstates)
        , watch(watch) {};

    /**
   * constructor
   */
    UpsfSubscriber(
        const std::vector<wt474_upsf_service::v1::ItemType>& itemtypes,
        const std::vector<wt474_messages::v1::DerivedState>& derivedstates,
        const std::vector<std::string>& parents,
        const std::vector<std::string>& names,
        bool watch = false)
        : itemtypes(itemtypes)
        , derivedstates(derivedstates)
        , parents(parents)
        , names(names)
        , watch(watch) {};

    /**
   * destructor
   */
    virtual ~UpsfSubscriber() {};

public:
    virtual void notify(
        const wt474_messages::v1::Shard& shard) {};

    virtual void notify(
        const wt474_messages::v1::SessionContext& session_context) {};

    virtual void notify(
        const wt474_messages::v1::NetworkConnection& network_connection) {};

    virtual void notify(
        const wt474_messages::v1::ServiceGatewayUserPlane& service_gateway_user_plane) {};

    virtual void notify(
        const wt474_messages::v1::TrafficSteeringFunction& traffic_steering_function) {};

    virtual void notify(
        const wt474_messages::v1::ServiceGateway& service_gateway) {};

public:
    /**
   *
   */
    bool get_watch() const
    {
        return watch;
    }

    /**
   *
   */
    UpsfSubscriber&
    set_watch(bool watch)
    {
        this->watch = watch;
        return *this;
    };

public:
    // item types
    std::vector<wt474_upsf_service::v1::ItemType> itemtypes;
    // derived states
    std::vector<wt474_messages::v1::DerivedState> derivedstates;
    // item parents
    std::vector<std::string> parents;
    // item names
    std::vector<std::string> names;
    // watch
    bool watch;
};

class UpsfClient {

public:
    /**
   * constructor
   */
    UpsfClient(
        std::shared_ptr<grpc::Channel> channel)
        : channel(channel)
        , stub_(wt474_upsf_service::v1::upsf::NewStub(channel)) {};

    /**
   * destructor
   */
    virtual ~UpsfClient() {};

public:
    /****************************************
   * CreateV1
   ****************************************/

    /**
   * rpc CreateV1 (wt474_upsf_messages/v1/Item) returns (wt474_messages/v1/Item) {}
   */
    bool CreateV1(
        const wt474_messages::v1::Item& request,
        wt474_messages::v1::Item& reply)
    {
        VLOG(2) << "func: " << __PRETTY_FUNCTION__ << std::endl;
        grpc::ClientContext context;
        const std::lock_guard<std::mutex> lock(stub_mutex);
        grpc::Status status = stub_->CreateV1(&context, request, &reply);
        if (!status.ok()) {
            LOG(ERROR) << "failure: " << __FUNCTION__ << " code:" << status.error_code() << " reason:" << status.error_message() << std::endl;
            return false;
        }
        return true;
    };

    /**
   * rpc CreateV1 (wt474_upsf_messages/v1/Shard) returns (wt474_upsf_messages/v1/Shard) {}
   */
    bool CreateV1(
        const wt474_messages::v1::Shard& request,
        wt474_messages::v1::Shard& reply)
    {
        VLOG(2) << "func: " << __PRETTY_FUNCTION__ << std::endl;
        wt474_messages::v1::Item item_q;
        wt474_messages::v1::Item item_p;
        *item_q.mutable_shard() = request;
        bool result = CreateV1(item_q, item_p);
        reply = item_p.shard();
        return result;
    };

    /**
   * rpc CreateV1 (wt474_upsf_messages/v1/SessionContext) returns (wt474_upsf_messages/v1/SessionContext) {}
   */
    bool CreateV1(
        const wt474_messages::v1::SessionContext& request,
        wt474_messages::v1::SessionContext& reply)
    {
        VLOG(2) << "func: " << __PRETTY_FUNCTION__ << std::endl;
        wt474_messages::v1::Item item_q;
        wt474_messages::v1::Item item_p;
        *item_q.mutable_session_context() = request;
        bool result = CreateV1(item_q, item_p);
        reply = item_p.session_context();
        return result;
    };

    /**
   * rpc CreateV1 (wt474_upsf_messages/v1/NetworkConnection) returns (wt474_upsf_messages/v1/NetworkConnection) {}
   */
    bool CreateV1(
        const wt474_messages::v1::NetworkConnection& request,
        wt474_messages::v1::NetworkConnection& reply)
    {
        VLOG(2) << "func: " << __PRETTY_FUNCTION__ << std::endl;
        wt474_messages::v1::Item item_q;
        wt474_messages::v1::Item item_p;
        *item_q.mutable_network_connection() = request;
        bool result = CreateV1(item_q, item_p);
        reply = item_p.network_connection();
        return result;
    };

    /**
   * rpc CreateV1 (wt474_upsf_messages/v1/ServiceGatewayUserPlane) returns (wt474_upsf_messages/v1/ServiceGatewayUserPlane) {}
   */
    bool CreateV1(
        const wt474_messages::v1::ServiceGatewayUserPlane& request,
        wt474_messages::v1::ServiceGatewayUserPlane& reply)
    {
        VLOG(2) << "func: " << __PRETTY_FUNCTION__ << std::endl;
        wt474_messages::v1::Item item_q;
        wt474_messages::v1::Item item_p;
        *item_q.mutable_service_gateway_user_plane() = request;
        bool result = CreateV1(item_q, item_p);
        reply = item_p.service_gateway_user_plane();
        return result;
    };

    /**
   * rpc CreateV1 (wt474_upsf_messages/v1/ServiceGatewayUserPlane) returns (wt474_upsf_messages/v1/ServiceGatewayUserPlane) {}
   */
    bool CreateV1(
        const wt474_messages::v1::TrafficSteeringFunction& request,
        wt474_messages::v1::TrafficSteeringFunction& reply)
    {
        VLOG(2) << "func: " << __PRETTY_FUNCTION__ << std::endl;
        wt474_messages::v1::Item item_q;
        wt474_messages::v1::Item item_p;
        *item_q.mutable_traffic_steering_function() = request;
        bool result = CreateV1(item_q, item_p);
        reply = item_p.traffic_steering_function();
        return result;
    };

    /**
   * rpc CreateV1 (wt474_upsf_messages/v1/ServiceGateway) returns (wt474_upsf_messages/v1/ServiceGateway) {}
   */
    bool CreateV1(
        const wt474_messages::v1::ServiceGateway& request,
        wt474_messages::v1::ServiceGateway& reply)
    {
        VLOG(2) << "func: " << __PRETTY_FUNCTION__ << std::endl;
        wt474_messages::v1::Item item_q;
        wt474_messages::v1::Item item_p;
        *item_q.mutable_service_gateway() = request;
        bool result = CreateV1(item_q, item_p);
        reply = item_p.service_gateway();
        return result;
    };

    /****************************************
   * UpdateV1
   ****************************************/

    /**
   * rpc UpdateV1 (wt474_upsf_messages/v1/Item) returns (wt474_messages/v1/Item) {}
   */
    bool UpdateV1(
        const wt474_upsf_service::v1::UpdateReq& req,
        wt474_messages::v1::Item& resp)
    {
        VLOG(2) << "func: " << __PRETTY_FUNCTION__ << std::endl;
        grpc::ClientContext context;
        const std::lock_guard<std::mutex> lock(stub_mutex);
        grpc::Status status = stub_->UpdateV1(&context, req, &resp);
        if (!status.ok()) {
            LOG(ERROR) << "failure: " << __FUNCTION__ << " code:" << status.error_code() << " reason:" << status.error_message() << std::endl;
            return false;
        }
        return true;
    };

    /**
   * rpc UpdateV1 (wt474_upsf_messages/v1/Shard) returns (wt474_upsf_messages/v1/Shard) {}
   */
    bool UpdateV1(
        wt474_messages::v1::Shard& shard,
        wt474_messages::v1::Shard& reply,
        wt474_upsf_service::v1::UpdateReq::UpdateOptions& options)
    {
        VLOG(2) << "func: " << __PRETTY_FUNCTION__ << std::endl;
        wt474_upsf_service::v1::UpdateReq req;
        wt474_messages::v1::Item item;
        *req.mutable_item()->mutable_shard() = shard;
        *req.mutable_update_options() = options;
        bool success = UpdateV1(req, item);
        if (!success) {
            return false;
        }
        reply = item.shard();
        return success;
    };

    /**
   * rpc UpdateV1 (wt474_upsf_messages/v1/SessionContext) returns (wt474_upsf_messages/v1/SessionContext) {}
   */
    bool UpdateV1(
        wt474_messages::v1::SessionContext& session_context,
        wt474_messages::v1::SessionContext& reply,
        wt474_upsf_service::v1::UpdateReq::UpdateOptions& options)
    {
        VLOG(2) << "func: " << __PRETTY_FUNCTION__ << std::endl;
        wt474_upsf_service::v1::UpdateReq req;
        wt474_messages::v1::Item item;
        *req.mutable_item()->mutable_session_context() = session_context;
        *req.mutable_update_options() = options;
        bool success = UpdateV1(req, item);
        if (!success) {
            return false;
        }
        reply = item.session_context();
        return success;
    };

    /**
   * rpc UpdateV1 (wt474_upsf_messages/v1/NetworkConnection) returns (wt474_upsf_messages/v1/NetworkConnection) {}
   */
    bool UpdateV1(
        wt474_messages::v1::NetworkConnection& network_connection,
        wt474_messages::v1::NetworkConnection& reply,
        wt474_upsf_service::v1::UpdateReq::UpdateOptions& options)
    {
        VLOG(2) << "func: " << __PRETTY_FUNCTION__ << std::endl;
        wt474_upsf_service::v1::UpdateReq req;
        wt474_messages::v1::Item item;
        *req.mutable_item()->mutable_network_connection() = network_connection;
        *req.mutable_update_options() = options;
        bool success = UpdateV1(req, item);
        if (!success) {
            return false;
        }
        reply = item.network_connection();
        return success;
    };

    /**
   * rpc UpdateV1 (wt474_upsf_messages/v1/ServiceGatewayUserPlane) returns (wt474_upsf_messages/v1/ServiceGatewayUserPlane) {}
   */
    bool UpdateV1(
        wt474_messages::v1::ServiceGatewayUserPlane& service_gateway_user_plane,
        wt474_messages::v1::ServiceGatewayUserPlane& reply,
        wt474_upsf_service::v1::UpdateReq::UpdateOptions& options)
    {
        VLOG(2) << "func: " << __PRETTY_FUNCTION__ << std::endl;
        wt474_upsf_service::v1::UpdateReq req;
        wt474_messages::v1::Item item;
        *req.mutable_item()->mutable_service_gateway_user_plane() = service_gateway_user_plane;
        *req.mutable_update_options() = options;
        bool success = UpdateV1(req, item);
        if (!success) {
            return false;
        }
        reply = item.service_gateway_user_plane();
        return success;
    };

    /**
   * rpc UpdateV1 (wt474_upsf_messages/v1/ServiceGatewayUserPlane) returns (wt474_upsf_messages/v1/ServiceGatewayUserPlane) {}
   */
    bool UpdateV1(
        wt474_messages::v1::TrafficSteeringFunction& traffic_steering_function,
        wt474_messages::v1::TrafficSteeringFunction& reply,
        wt474_upsf_service::v1::UpdateReq::UpdateOptions& options)
    {
        VLOG(2) << "func: " << __PRETTY_FUNCTION__ << std::endl;
        wt474_upsf_service::v1::UpdateReq req;
        wt474_messages::v1::Item item;
        *req.mutable_item()->mutable_traffic_steering_function() = traffic_steering_function;
        *req.mutable_update_options() = options;
        bool success = UpdateV1(req, item);
        if (!success) {
            return false;
        }
        reply = item.traffic_steering_function();
        return success;
    };

    /**
   * rpc UpdateV1 (wt474_upsf_messages/v1/ServiceGateway) returns (wt474_upsf_messages/v1/ServiceGateway) {}
   */
    bool UpdateV1(
        wt474_messages::v1::ServiceGateway& service_gateway,
        wt474_messages::v1::ServiceGateway& reply,
        wt474_upsf_service::v1::UpdateReq::UpdateOptions& options)
    {
        VLOG(2) << "func: " << __PRETTY_FUNCTION__ << std::endl;
        wt474_upsf_service::v1::UpdateReq req;
        wt474_messages::v1::Item item;
        *req.mutable_item()->mutable_service_gateway() = service_gateway;
        *req.mutable_update_options() = options;
        bool success = UpdateV1(req, item);
        if (!success) {
            return false;
        }
        reply = item.service_gateway();
        return success;
    };

    /****************************************
   * DeleteV1
   ****************************************/

    /**
   * rpc DeleteV1 (wt474_upsf_service/v1/Item) returns (wt474_messages/v1/Item) {}
   */
    bool DeleteV1(
        const google::protobuf::StringValue& req,
        google::protobuf::StringValue& resp)
    {
        VLOG(2) << "func: " << __PRETTY_FUNCTION__ << std::endl;
        grpc::ClientContext context;
        const std::lock_guard<std::mutex> lock(stub_mutex);
        grpc::Status status = stub_->DeleteV1(&context, req, &resp);
        if (!status.ok()) {
            LOG(ERROR) << "failure: " << __FUNCTION__ << " code:" << status.error_code() << " reason:" << status.error_message() << std::endl;
            return false;
        }
        return true;
    };

    /**
   * rpc DeleteV1 (wt474_upsf_service/v1/Item) returns (wt474_messages/v1/Item) {}
   */
    bool DeleteV1(
        const std::string& request,
        std::string& reply)
    {
        VLOG(2) << "func: " << __PRETTY_FUNCTION__ << std::endl;
        grpc::ClientContext context;
        google::protobuf::StringValue str_q;
        google::protobuf::StringValue str_p;
        str_q.set_value(request);
        const std::lock_guard<std::mutex> lock(stub_mutex);
        grpc::Status status = stub_->DeleteV1(&context, str_q, &str_p);
        if (!status.ok()) {
            LOG(ERROR) << "failure: " << __FUNCTION__ << " code:" << status.error_code() << " reason:" << status.error_message() << std::endl;
            return false;
        }
        reply.assign(str_p.value());
        return true;
    };

    /****************************************
   * LookupV1
   ****************************************/

    /**
   * rpc LookupV1 (wt474_messages/v1/SessionContext_Spec) returns (wt474_messages/v1/SessionContext) {}
   */
    bool LookupV1(
        wt474_messages::v1::SessionContext::Spec& session_context_spec,
        wt474_messages::v1::SessionContext& resp)
    {
        VLOG(2) << "func: " << __PRETTY_FUNCTION__ << std::endl;
        grpc::ClientContext context;
        const std::lock_guard<std::mutex> lock(stub_mutex);
        grpc::Status status = stub_->LookupV1(&context, session_context_spec, &resp);
        if (!status.ok()) {
            LOG(ERROR) << "failure: " << __FUNCTION__ << " code:" << status.error_code() << " reason:" << status.error_message() << std::endl;
            return false;
        }
        return true;
    };

    /****************************************
   * ReadV1
   ****************************************/

    bool ReadV1(
        const std::string& name,
        wt474_messages::v1::ServiceGateway& service_gateway)
    {
        VLOG(2) << "func: " << __PRETTY_FUNCTION__ << std::endl;
        wt474_upsf_service::v1::ReadReq req;

        /* set item type */
        req.add_itemtype(wt474_upsf_service::v1::ItemType::service_gateway);

        /* set parent name */
        //req.add_parent()->set_value(name);
        req.add_name()->set_value(name);

        /* set watch */
        req.set_watch(false);

        grpc::ClientContext context;
        wt474_messages::v1::Item item;
        const std::lock_guard<std::mutex> lock(stub_mutex);
        std::unique_ptr<grpc::ClientReader<wt474_messages::v1::Item>> reader(stub_->ReadV1(&context, req));
        while (reader->Read(&item)) {
            if (item.has_service_gateway()) {
                service_gateway = item.service_gateway();
                break;
            }
        }
        grpc::Status status = reader->Finish();
        if (!status.ok()) {
            LOG(ERROR) << "failure: " << __FUNCTION__ << " code:" << status.error_code() << " reason:" << status.error_message() << std::endl;
            return false;
        }
        if (item.service_gateway().name() != name) {
            return false;
        }

        return true;
    };

    bool ReadV1(
        const std::string& name,
        wt474_messages::v1::ServiceGatewayUserPlane& service_gateway_user_plane)
    {
        VLOG(2) << "func: " << __PRETTY_FUNCTION__ << std::endl;
        wt474_upsf_service::v1::ReadReq req;

        /* set item type */
        req.add_itemtype(wt474_upsf_service::v1::ItemType::service_gateway_user_plane);

        /* set parent name */
        //req.add_parent()->set_value(name);
        req.add_name()->set_value(name);

        /* set watch */
        req.set_watch(false);

        grpc::ClientContext context;
        wt474_messages::v1::Item item;
        const std::lock_guard<std::mutex> lock(stub_mutex);
        std::unique_ptr<grpc::ClientReader<wt474_messages::v1::Item>> reader(stub_->ReadV1(&context, req));
        while (reader->Read(&item)) {
            if (item.has_service_gateway_user_plane()) {
                service_gateway_user_plane = item.service_gateway_user_plane();
                break;
            }
        }
        grpc::Status status = reader->Finish();
        if (!status.ok()) {
            LOG(ERROR) << "failure: " << __FUNCTION__ << " code:" << status.error_code() << " reason:" << status.error_message() << std::endl;
            return false;
        }
        if (item.service_gateway_user_plane().name() != name) {
            return false;
        }

        return true;
    };

    bool ReadV1(
        const std::string& name,
        wt474_messages::v1::TrafficSteeringFunction& traffic_steering_function)
    {
        VLOG(2) << "func: " << __PRETTY_FUNCTION__ << std::endl;
        wt474_upsf_service::v1::ReadReq req;

        /* set item type */
        req.add_itemtype(wt474_upsf_service::v1::ItemType::traffic_steering_function);

        /* set parent name */
        //req.add_parent()->set_value(name);
        req.add_name()->set_value(name);

        /* set watch */
        req.set_watch(false);

        grpc::ClientContext context;
        wt474_messages::v1::Item item;
        const std::lock_guard<std::mutex> lock(stub_mutex);
        std::unique_ptr<grpc::ClientReader<wt474_messages::v1::Item>> reader(stub_->ReadV1(&context, req));
        while (reader->Read(&item)) {
            if (item.has_traffic_steering_function()) {
                traffic_steering_function = item.traffic_steering_function();
                break;
            }
        }
        grpc::Status status = reader->Finish();
        if (!status.ok()) {
            LOG(ERROR) << "failure: " << __FUNCTION__ << " code:" << status.error_code() << " reason:" << status.error_message() << std::endl;
            return false;
        }
        if (item.traffic_steering_function().name() != name) {
            return false;
        }

        return true;
    };

    bool ReadV1(
        const std::string& name,
        wt474_messages::v1::NetworkConnection& network_connection)
    {
        VLOG(2) << "func: " << __PRETTY_FUNCTION__ << std::endl;
        wt474_upsf_service::v1::ReadReq req;

        /* set item type */
        req.add_itemtype(wt474_upsf_service::v1::ItemType::network_connection);

        /* set parent name */
        //req.add_parent()->set_value(name);
        req.add_name()->set_value(name);

        /* set watch */
        req.set_watch(false);

        grpc::ClientContext context;
        wt474_messages::v1::Item item;
        const std::lock_guard<std::mutex> lock(stub_mutex);
        std::unique_ptr<grpc::ClientReader<wt474_messages::v1::Item>> reader(stub_->ReadV1(&context, req));
        while (reader->Read(&item)) {
            if (item.has_network_connection()) {
                network_connection = item.network_connection();
                break;
            }
        }
        grpc::Status status = reader->Finish();
        if (!status.ok()) {
            LOG(ERROR) << "failure: " << __FUNCTION__ << " code:" << status.error_code() << " reason:" << status.error_message() << std::endl;
            return false;
        }
        if (item.network_connection().name() != name) {
            return false;
        }

        return true;
    };

    bool ReadV1(
        const std::string& name,
        wt474_messages::v1::Shard& shard)
    {
        VLOG(2) << "func: " << __PRETTY_FUNCTION__ << std::endl;
        wt474_upsf_service::v1::ReadReq req;

        /* set item type */
        req.add_itemtype(wt474_upsf_service::v1::ItemType::shard);

        /* set parent name */
        //req.add_parent()->set_value(name);
        req.add_name()->set_value(name);

        /* set watch */
        req.set_watch(false);

        grpc::ClientContext context;
        wt474_messages::v1::Item item;
        const std::lock_guard<std::mutex> lock(stub_mutex);
        std::unique_ptr<grpc::ClientReader<wt474_messages::v1::Item>> reader(stub_->ReadV1(&context, req));
        while (reader->Read(&item)) {
            if (item.has_shard()) {
                shard = item.shard();
                break;
            }
        }
        grpc::Status status = reader->Finish();
        if (!status.ok()) {
            LOG(ERROR) << "failure: " << __FUNCTION__ << " code:" << status.error_code() << " reason:" << status.error_message() << std::endl;
            return false;
        }
        if (item.shard().name() != name) {
            return false;
        }

        return true;
    };

    bool ReadV1(
        const std::string& name,
        wt474_messages::v1::SessionContext& session_context)
    {
        VLOG(2) << "func: " << __PRETTY_FUNCTION__ << std::endl;
        wt474_upsf_service::v1::ReadReq req;

        /* set item type */
        req.add_itemtype(wt474_upsf_service::v1::ItemType::session_context);

        /* set parent name */
        //req.add_parent()->set_value(name);
        req.add_name()->set_value(name);

        /* set watch */
        req.set_watch(false);

        grpc::ClientContext context;
        wt474_messages::v1::Item item;
        const std::lock_guard<std::mutex> lock(stub_mutex);
        std::unique_ptr<grpc::ClientReader<wt474_messages::v1::Item>> reader(stub_->ReadV1(&context, req));
        while (reader->Read(&item)) {
            if (item.has_session_context()) {
                session_context = item.session_context();
                break;
            }
        }
        grpc::Status status = reader->Finish();
        if (!status.ok()) {
            LOG(ERROR) << "failure: " << __FUNCTION__ << " code:" << status.error_code() << " reason:" << status.error_message() << std::endl;
            return false;
        }
        if (item.session_context().name() != name) {
            return false;
        }

        return true;
    };

    bool ReadV1(
        std::vector<wt474_messages::v1::ServiceGateway>& service_gateways)
    {
        VLOG(2) << "func: " << __PRETTY_FUNCTION__ << std::endl;
        wt474_upsf_service::v1::ReadReq req;

        /* set item type */
        req.add_itemtype(wt474_upsf_service::v1::ItemType::service_gateway);

        /* set watch */
        req.set_watch(false);

        grpc::ClientContext context;
        wt474_messages::v1::Item item;
        const std::lock_guard<std::mutex> lock(stub_mutex);
        std::unique_ptr<grpc::ClientReader<wt474_messages::v1::Item>> reader(stub_->ReadV1(&context, req));
        while (reader->Read(&item)) {
            if (not item.has_service_gateway())
                continue;
            service_gateways.push_back(item.service_gateway());
        }
        grpc::Status status = reader->Finish();
        if (!status.ok()) {
            LOG(ERROR) << "failure: " << __FUNCTION__ << " code:" << status.error_code() << " reason:" << status.error_message() << std::endl;
            return false;
        }

        return true;
    };

    bool ReadV1(
        std::vector<wt474_messages::v1::ServiceGatewayUserPlane>& service_gateway_user_planes)
    {
        VLOG(2) << "func: " << __PRETTY_FUNCTION__ << std::endl;
        wt474_upsf_service::v1::ReadReq req;

        /* set item type */
        req.add_itemtype(wt474_upsf_service::v1::ItemType::service_gateway_user_plane);

        /* set watch */
        req.set_watch(false);

        grpc::ClientContext context;
        wt474_messages::v1::Item item;
        const std::lock_guard<std::mutex> lock(stub_mutex);
        std::unique_ptr<grpc::ClientReader<wt474_messages::v1::Item>> reader(stub_->ReadV1(&context, req));
        while (reader->Read(&item)) {
            if (not item.has_service_gateway_user_plane())
                continue;
            service_gateway_user_planes.push_back(item.service_gateway_user_plane());
        }
        grpc::Status status = reader->Finish();
        if (!status.ok()) {
            LOG(ERROR) << "failure: " << __FUNCTION__ << " code:" << status.error_code() << " reason:" << status.error_message() << std::endl;
            return false;
        }

        return true;
    };

    bool ReadV1(
        std::vector<wt474_messages::v1::TrafficSteeringFunction>& traffic_steering_functions)
    {
        VLOG(2) << "func: " << __PRETTY_FUNCTION__ << std::endl;
        wt474_upsf_service::v1::ReadReq req;

        /* set item type */
        req.add_itemtype(wt474_upsf_service::v1::ItemType::traffic_steering_function);

        /* set watch */
        req.set_watch(false);

        grpc::ClientContext context;
        wt474_messages::v1::Item item;
        const std::lock_guard<std::mutex> lock(stub_mutex);
        std::unique_ptr<grpc::ClientReader<wt474_messages::v1::Item>> reader(stub_->ReadV1(&context, req));
        while (reader->Read(&item)) {
            if (not item.has_traffic_steering_function())
                continue;
            traffic_steering_functions.push_back(item.traffic_steering_function());
        }
        grpc::Status status = reader->Finish();
        if (!status.ok()) {
            LOG(ERROR) << "failure: " << __FUNCTION__ << " code:" << status.error_code() << " reason:" << status.error_message() << std::endl;
            return false;
        }

        return true;
    };

    bool ReadV1(
        std::vector<wt474_messages::v1::NetworkConnection>& network_connections)
    {
        VLOG(2) << "func: " << __PRETTY_FUNCTION__ << std::endl;
        wt474_upsf_service::v1::ReadReq req;

        /* set item type */
        req.add_itemtype(wt474_upsf_service::v1::ItemType::network_connection);

        /* set watch */
        req.set_watch(false);

        grpc::ClientContext context;
        wt474_messages::v1::Item item;
        const std::lock_guard<std::mutex> lock(stub_mutex);
        std::unique_ptr<grpc::ClientReader<wt474_messages::v1::Item>> reader(stub_->ReadV1(&context, req));
        while (reader->Read(&item)) {
            if (not item.has_network_connection())
                continue;
            network_connections.push_back(item.network_connection());
        }
        grpc::Status status = reader->Finish();
        if (!status.ok()) {
            LOG(ERROR) << "failure: " << __FUNCTION__ << " code:" << status.error_code() << " reason:" << status.error_message() << std::endl;
            return false;
        }

        return true;
    };

    bool ReadV1(
        std::vector<wt474_messages::v1::Shard>& shards)
    {
        VLOG(2) << "func: " << __PRETTY_FUNCTION__ << std::endl;
        wt474_upsf_service::v1::ReadReq req;

        /* set item type */
        req.add_itemtype(wt474_upsf_service::v1::ItemType::shard);

        /* set watch */
        req.set_watch(false);

        grpc::ClientContext context;
        wt474_messages::v1::Item item;
        const std::lock_guard<std::mutex> lock(stub_mutex);
        std::unique_ptr<grpc::ClientReader<wt474_messages::v1::Item>> reader(stub_->ReadV1(&context, req));
        while (reader->Read(&item)) {
            if (not item.has_shard())
                continue;
            shards.push_back(item.shard());
        }
        grpc::Status status = reader->Finish();
        if (!status.ok()) {
            LOG(ERROR) << "failure: " << __FUNCTION__ << " code:" << status.error_code() << " reason:" << status.error_message() << std::endl;
            return false;
        }

        return true;
    };

    bool ReadV1(
        std::vector<wt474_messages::v1::SessionContext>& session_contexts)
    {
        VLOG(2) << "func: " << __PRETTY_FUNCTION__ << std::endl;
        wt474_upsf_service::v1::ReadReq req;

        /* set item type */
        req.add_itemtype(wt474_upsf_service::v1::ItemType::session_context);

        /* set watch */
        req.set_watch(false);

        grpc::ClientContext context;
        wt474_messages::v1::Item item;
        const std::lock_guard<std::mutex> lock(stub_mutex);
        std::unique_ptr<grpc::ClientReader<wt474_messages::v1::Item>> reader(stub_->ReadV1(&context, req));
        while (reader->Read(&item)) {
            if (not item.has_session_context())
                continue;
            session_contexts.push_back(item.session_context());
        }
        grpc::Status status = reader->Finish();
        if (!status.ok()) {
            LOG(ERROR) << "failure: " << __FUNCTION__ << " code:" << status.error_code() << " reason:" << status.error_message() << std::endl;
            return false;
        }

        return true;
    };

    bool ReadV1(
        UpsfSubscriber& subscriber)
    {
        VLOG(2) << "func: " << __PRETTY_FUNCTION__ << std::endl;
        wt474_upsf_service::v1::ReadReq req;

        /* set item_types */
        for (auto it : subscriber.itemtypes) {
            req.add_itemtype(it);
        }
        /* set item_states */
        for (auto it : subscriber.derivedstates) {
            req.add_itemstate(it);
        }
        /* set item parents */
        int index = 0;
        for (auto it : subscriber.parents) {
            req.mutable_parent(index++)->set_value(it);
        }
        /* set item names */
        int jndex = 0;
        for (auto it : subscriber.names) {
            req.mutable_name(jndex++)->set_value(it);
        }
        /* set watch */
        req.set_watch(subscriber.get_watch());

        std::unique_ptr<wt474_upsf_service::v1::upsf::Stub> subscriber_stub_(wt474_upsf_service::v1::upsf::NewStub(channel));
        grpc::ClientContext context;
        wt474_messages::v1::Item item;
        /* create a unique subscriber grpc stub for this long-lasting operation */
        std::unique_ptr<grpc::ClientReader<wt474_messages::v1::Item>> reader(subscriber_stub_->ReadV1(&context, req));
        do {
            while (reader->Read(&item)) {

                // shard
                if (item.has_shard()) {
                    subscriber.notify(item.shard());
                }
                // session_context
                else if (item.has_session_context()) {
                    subscriber.notify(item.session_context());
                }
                // network_connection
                else if (item.has_network_connection()) {
                    subscriber.notify(item.network_connection());
                }
                // service_gateway_user_plane
                else if (item.has_service_gateway_user_plane()) {
                    subscriber.notify(item.service_gateway_user_plane());
                }
                // traffic_steering_function
                else if (item.has_traffic_steering_function()) {
                    subscriber.notify(item.traffic_steering_function());
                }
                // service_gateway
                else if (item.has_service_gateway()) {
                    subscriber.notify(item.service_gateway());
                }
            }
            grpc::Status status = reader->Finish();
            if (!status.ok()) {
                LOG(ERROR) << "failure: " << __FUNCTION__ << " code:" << status.error_code() << " reason:" << status.error_message() << std::endl;
                return false;
            }

        } while (subscriber.get_watch()); // continue if watch == true

        return true;
    };

private:
    std::shared_ptr<grpc::Channel> channel;
    std::unique_ptr<wt474_upsf_service::v1::upsf::Stub> stub_;
    std::mutex stub_mutex;
};

} // namespace upsf

#endif
