#ifndef UPSF_HPP
#define UPSF_HPP

#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>
#include <glog/logging.h>

#include "sssupsf.grpc.pb.h"

namespace upsf {

class UpsfSubscriber {

public:
  /**
   * constructor
   */
  UpsfSubscriber() {
      subscriptions.emplace(subscriptions.end(), bbf::sss::SubscribeReq::SubscriptionType::SubscribeReq_SubscriptionType_shards);
      subscriptions.emplace(subscriptions.end(), bbf::sss::SubscribeReq::SubscriptionType::SubscribeReq_SubscriptionType_session_contexts);
      subscriptions.emplace(subscriptions.end(), bbf::sss::SubscribeReq::SubscriptionType::SubscribeReq_SubscriptionType_network_connections);
      subscriptions.emplace(subscriptions.end(), bbf::sss::SubscribeReq::SubscriptionType::SubscribeReq_SubscriptionType_service_gateway_user_planes);
      subscriptions.emplace(subscriptions.end(), bbf::sss::SubscribeReq::SubscriptionType::SubscribeReq_SubscriptionType_traffic_steering_functions);
      subscriptions.emplace(subscriptions.end(), bbf::sss::SubscribeReq::SubscriptionType::SubscribeReq_SubscriptionType_service_gateways);
  }

  /**
   * constructor
   */
  UpsfSubscriber(const std::vector<bbf::sss::SubscribeReq::SubscriptionType> &subscriptions) 
      : subscriptions(subscriptions) {}

  /**
   * destructor
   */
  virtual ~UpsfSubscriber() {}

public:

  /**
   * returns false for terminating subscription loop
   */
  virtual bool update(const bbf::sss::Shard &shard) {
    return true;
  };

  /**
   * returns false for terminating subscription loop
   */
  virtual bool update(const bbf::sss::SessionContext &session_context) {
    return true;
  };

  /**
   * returns false for terminating subscription loop
   */
  virtual bool update(const bbf::sss::NetworkConnection &network_connection) {
    return true;
  };

  /**
   * returns false for terminating subscription loop
   */
  virtual bool update(const bbf::sss::ServiceGatewayUserPlane &service_gateway_user_plane) {
    return true;
  };

  /**
   * returns false for terminating subscription loop
   */
  virtual bool update(const bbf::sss::TrafficSteeringFunction &traffic_steering_function) {
    return true;
  };

  /**
   * returns false for terminating subscription loop
   */
  virtual bool update(const bbf::sss::ServiceGateway &service_gateway) {
    return true;
  };

public:

  // subscription types
  std::vector<bbf::sss::SubscribeReq::SubscriptionType> subscriptions;
};

class UpsfClient {

public:
  /**
   * constructor
   */
  UpsfClient(std::shared_ptr<grpc::Channel> channel)
      : channel(channel), stub_(bbf::sss::sssUpsf::NewStub(channel)) {}

  /**
   * destructor
   */
  virtual ~UpsfClient() {}

public:

  /****************************************
   * Shard
   ****************************************/

  /**
   * rpc UpdateShard (UpdateShardReq) returns (ShardResp) {}
   */
  bool UpdateShard(const bbf::sss::UpdateShardReq &req,
                   bbf::sss::ShardResp &resp) {
    VLOG(2) << "func: " << __PRETTY_FUNCTION__ << std::endl;
    grpc::ClientContext context;
    grpc::Status status = stub_->UpdateShard(&context, req, &resp);
    if (!status.ok()) {
      LOG(ERROR) << "failure: " << __FUNCTION__ << " code:" << status.error_code() << " reason:" << status.error_message() << std::endl;
      return false;
    }
    return true;
  };

  /**
   * rpc UpdateShard (UpdateShardReq) returns (ShardResp) {}
   */
  bool UpdateShard(const std::string &name,
                   const bbf::sss::ItemStatus &item_status,
                   int max_session_count,
                   const std::string &desired_service_gateway_user_plane,
                   const std::vector<std::string> &desired_network_connection,
                   const std::string &required_qos,
                   const std::string &current_service_gateway_user_plane,
                   const std::map<std::string, std::string> &tsf_network_connection,
                   const std::vector<std::string> &ip_prefixes,
                   bbf::sss::ShardResp &resp) {
    VLOG(2) << "func: " << __PRETTY_FUNCTION__ << std::endl;
    bbf::sss::UpdateShardReq req;
    req.mutable_shard()->mutable_metadata()->set_name(name);
    req.mutable_shard()->mutable_metadata()->set_item_status(item_status);
    req.mutable_shard()->mutable_spec()->set_desired_service_gateway_user_plane(desired_service_gateway_user_plane);
    for (auto it : desired_network_connection) {
      req.mutable_shard()->mutable_spec()->add_desired_network_connection(it);
    }
    req.mutable_shard()->mutable_spec()->set_required_qos(required_qos);
    req.mutable_shard()->mutable_spec()->set_max_session_count(max_session_count);
    req.mutable_shard()->mutable_status()->set_current_service_gateway_user_plane(current_service_gateway_user_plane);
    for (auto it : tsf_network_connection) {
      req.mutable_shard()->mutable_status()->mutable_tsf_network_connection()->insert(google::protobuf::MapPair<std::string, std::string>(it.first, it.second));
    }
    for (auto it : ip_prefixes) {
      req.mutable_shard()->mutable_status()->add_ip_prefixes(it);
    }
    return UpdateShard(req, resp);
  };

  /**
   * rpc DeleteShard (DeleteShardReq) returns (ShardResp) {}
   */
  bool DeleteShard(const bbf::sss::DeleteShardReq &req,
                   bbf::sss::ShardResp &resp) {
    VLOG(2) << "func: " << __PRETTY_FUNCTION__ << std::endl;
    grpc::ClientContext context;
    grpc::Status status = stub_->DeleteShard(&context, req, &resp);
    if (!status.ok()) {
      LOG(ERROR) << "failure: " << __FUNCTION__ << " code:" << status.error_code() << " reason:" << status.error_message() << std::endl;
      return false;
    }
    return true;
  };

  /**
   * rpc DeleteShard (DeleteShardReq) returns (ShardResp) {}
   */
  bool DeleteShard(const std::string &id,
                   bbf::sss::ShardResp &resp) {
    VLOG(2) << "func: " << __PRETTY_FUNCTION__ << std::endl;
    bbf::sss::DeleteShardReq req;
    req.set_id(id);
    return DeleteShard(req, resp);
  };

  /**
   * rpc GetShard (GetShardReq) returns (ShardResp) {}
   */
  bool GetShard(const bbf::sss::GetShardReq &req,
		         bbf::sss::ShardResp &resp) {
    VLOG(2) << "func: " << __PRETTY_FUNCTION__ << std::endl;
    grpc::ClientContext context;
    grpc::Status status = stub_->GetShard(&context, req, &resp);
    if (!status.ok()) {
      LOG(ERROR) << "failure: " << __FUNCTION__ << " code:" << status.error_code() << " reason:" << status.error_message() << std::endl;
      return false;
    }
    return true;
  };

  /**
   * rpc GetShard (GetShardReq) returns (ShardResp) {}
   */
  bool GetShard(const std::string &id,
		         bbf::sss::ShardResp &resp) {
    VLOG(2) << "func: " << __PRETTY_FUNCTION__ << std::endl;
    bbf::sss::GetShardReq req;
    req.set_id(id);
    return GetShard(req, resp);
  };

  /**
   * rpc ListShards (ListShardReq) returns (ListShardResp) {}
   */
  bool ListShards(const bbf::sss::ListShardReq &req,
		         bbf::sss::ListShardResp &resp) {
    VLOG(2) << "func: " << __PRETTY_FUNCTION__ << std::endl;
    grpc::ClientContext context;
    grpc::Status status = stub_->ListShards(&context, req, &resp);
    if (!status.ok()) {
      LOG(ERROR) << "failure: " << __FUNCTION__ << " code:" << status.error_code() << " reason:" << status.error_message() << std::endl;
      return false;
    }
    return true;
  };

  /**
   * rpc ListShards (ListShardReq) returns (ListShardResp) {}
   */
  bool ListShards(const std::vector<std::string> &id_list,
		         bbf::sss::ListShardResp &resp) {
    VLOG(2) << "func: " << __PRETTY_FUNCTION__ << std::endl;
    bbf::sss::ListShardReq req;
    for (auto it : id_list) {
        req.mutable_filter()->add_id_list(it);
    }
    return ListShards(req, resp);
  };

  /**
   * rpc ListShards (ListShardReq) returns (ListShardResp) {}
   */
  bool ListShards(const std::string& id,
		         bbf::sss::ListShardResp &resp) {
    VLOG(2) << "func: " << __PRETTY_FUNCTION__ << std::endl;
    bbf::sss::ListShardReq req;
    req.mutable_filter()->add_id_list(id);
    return ListShards(req, resp);
  };

  /**
   * rpc ListShards (ListShardReq) returns (ListShardResp) {}
   */
  bool ListShards(bbf::sss::ListShardResp &resp) {
    VLOG(2) << "func: " << __PRETTY_FUNCTION__ << std::endl;
    bbf::sss::ListShardReq req;
    return ListShards(req, resp);
  };


  /****************************************
   * ServiceGateway
   ****************************************/

  /**
   * rpc UpdateServiceGateway (UpdateServiceGatewayReq) returns (ServiceGatewayResp) {}
   */
  bool UpdateServiceGateway(const bbf::sss::UpdateServiceGatewayReq &req,
                            bbf::sss::ServiceGatewayResp &resp) {
    VLOG(2) << "func: " << __PRETTY_FUNCTION__ << std::endl;
    grpc::ClientContext context;
    grpc::Status status = stub_->UpdateServiceGateway(&context, req, &resp);
    if (!status.ok()) {
      LOG(ERROR) << "failure: " << __FUNCTION__ << " code:" << status.error_code() << " reason:" << status.error_message() << std::endl;
      return false;
    }
    return true;
  };

  /**
   * rpc UpdateServiceGateway (UpdateServiceGatewayReq) returns (ServiceGatewayResp) {}
   */
  bool UpdateServiceGateway(const std::string &name,
                            const bbf::sss::ItemStatus &item_status,
                            bbf::sss::ServiceGatewayResp &resp) {
    VLOG(2) << "func: " << __PRETTY_FUNCTION__ << std::endl;
    bbf::sss::UpdateServiceGatewayReq req;
    req.mutable_service_gateway()->mutable_metadata()->set_name(name);
    req.mutable_service_gateway()->mutable_metadata()->set_item_status(item_status);
    return UpdateServiceGateway(req, resp);
  };

  /**
   * rpc DeleteServiceGateway (DeleteServiceGatewayReq) returns (ServiceGatewayResp) {}
   */
  bool DeleteServiceGateway(const bbf::sss::DeleteServiceGatewayReq &req,
		            bbf::sss::ServiceGatewayResp &resp) {
    VLOG(2) << "func: " << __PRETTY_FUNCTION__ << std::endl;
    grpc::ClientContext context;
    grpc::Status status = stub_->DeleteServiceGateway(&context, req, &resp);
    if (!status.ok()) {
      LOG(ERROR) << "failure: " << __FUNCTION__ << " code:" << status.error_code() << " reason:" << status.error_message() << std::endl;
      return false;
    }
    return true;
  };

  /**
   * rpc DeleteServiceGateway (DeleteServiceGatewayReq) returns (ServiceGatewayResp) {}
   */
  bool DeleteServiceGateway(const std::string &id,
                            bbf::sss::ServiceGatewayResp &resp) {
    VLOG(2) << "func: " << __PRETTY_FUNCTION__ << std::endl;
    bbf::sss::DeleteServiceGatewayReq req;
    req.set_id(id);
    return DeleteServiceGateway(req, resp);
  };

  /**
   * rpc GetServiceGateway (GetServiceGatewayReq) returns (ServiceGatewayResp) {}
   */
  bool GetServiceGateway(const bbf::sss::GetServiceGatewayReq &req,
		         bbf::sss::ServiceGatewayResp &resp) {
    VLOG(2) << "func: " << __PRETTY_FUNCTION__ << std::endl;
    grpc::ClientContext context;
    grpc::Status status = stub_->GetServiceGateway(&context, req, &resp);
    if (!status.ok()) {
      LOG(ERROR) << "failure: " << __FUNCTION__ << " code:" << status.error_code() << " reason:" << status.error_message() << std::endl;
      return false;
    }
    return true;
  };

  /**
   * rpc GetServiceGateway (GetServiceGatewayReq) returns (ServiceGatewayResp) {}
   */
  bool GetServiceGateway(const std::string &id,
		         bbf::sss::ServiceGatewayResp &resp) {
    VLOG(2) << "func: " << __PRETTY_FUNCTION__ << std::endl;
    bbf::sss::GetServiceGatewayReq req;
    req.set_id(id);
    return GetServiceGateway(req, resp);
  };

  /**
   * rpc ListServiceGateways (ListServiceGatewayReq) returns (ListServiceGatewayResp) {}
   */
  bool ListServiceGateways(const bbf::sss::ListServiceGatewayReq &req,
		         bbf::sss::ListServiceGatewayResp &resp) {
    VLOG(2) << "func: " << __PRETTY_FUNCTION__ << std::endl;
    grpc::ClientContext context;
    grpc::Status status = stub_->ListServiceGateways(&context, req, &resp);
    if (!status.ok()) {
      LOG(ERROR) << "failure: " << __FUNCTION__ << " code:" << status.error_code() << " reason:" << status.error_message() << std::endl;
      return false;
    }
    return true;
  };

  /**
   * rpc ListServiceGateways (ListServiceGatewayReq) returns (ListServiceGatewayResp) {}
   */
  bool ListServiceGateways(const std::vector<std::string> &id_list,
		         bbf::sss::ListServiceGatewayResp &resp) {
    VLOG(2) << "func: " << __PRETTY_FUNCTION__ << std::endl;
    bbf::sss::ListServiceGatewayReq req;
    for (auto it : id_list) {
        req.mutable_filter()->add_id_list(it);
    }
    return ListServiceGateways(req, resp);
  };

  /**
   * rpc ListServiceGateways (ListServiceGatewayReq) returns (ListServiceGatewayResp) {}
   */
  bool ListServiceGateways(const std::string& id,
		         bbf::sss::ListServiceGatewayResp &resp) {
    VLOG(2) << "func: " << __PRETTY_FUNCTION__ << std::endl;
    bbf::sss::ListServiceGatewayReq req;
    req.mutable_filter()->add_id_list(id);
    return ListServiceGateways(req, resp);
  };

  /**
   * rpc ListServiceGateways (ListServiceGatewayReq) returns (ListServiceGatewayResp) {}
   */
  bool ListServiceGateways(bbf::sss::ListServiceGatewayResp &resp) {
    VLOG(2) << "func: " << __PRETTY_FUNCTION__ << std::endl;
    bbf::sss::ListServiceGatewayReq req;
    return ListServiceGateways(req, resp);
  };


  /****************************************
   * ServiceGatewayUserPlane
   ****************************************/

  /**
   * rpc UpdateServiceGatewayUserPlane (UpdateServiceGatewayUserPlaneReq) returns (ServiceGatewayUserPlaneResp) {}
   */
  bool UpdateServiceGatewayUserPlane(const bbf::sss::UpdateServiceGatewayUserPlaneReq &req,
                                     bbf::sss::ServiceGatewayUserPlaneResp &resp) {
    VLOG(2) << "func: " << __PRETTY_FUNCTION__ << std::endl;
    grpc::ClientContext context;
    grpc::Status status = stub_->UpdateServiceGatewayUserPlane(&context, req, &resp);
    if (!status.ok()) {
      LOG(ERROR) << "failure: " << __FUNCTION__ << " code:" << status.error_code() << " reason:" << status.error_message() << std::endl;
      return false;
    }
    return true;
  };

  /**
   * rpc UpdateServiceGatewayUserPlane (UpdateServiceGatewayUserPlaneReq) returns (ServiceGatewayUserPlaneResp) {}
   */
  bool UpdateServiceGatewayUserPlane(const std::string &name,
                                     const bbf::sss::ItemStatus &item_status,
                                     const std::string &service_gateway_id,
                                     int32_t max_session_count,
                                     const std::vector<std::string> &supported_service_groups,
                                     bool maintenance,
                                     int32_t allocated_session_count,
                                     const std::vector<int32_t> &network_connection,
                                     bbf::sss::ServiceGatewayUserPlaneResp &resp) {
    VLOG(2) << "func: " << __PRETTY_FUNCTION__ << std::endl;
    bbf::sss::UpdateServiceGatewayUserPlaneReq req;
    req.mutable_service_gateway_user_plane()->mutable_metadata()->set_name(name);
    req.mutable_service_gateway_user_plane()->mutable_metadata()->set_item_status(item_status);
    req.mutable_service_gateway_user_plane()->set_service_gateway_id(service_gateway_id);
    req.mutable_service_gateway_user_plane()->mutable_spec()->set_max_session_count(max_session_count);
    for (auto it : supported_service_groups) {
      req.mutable_service_gateway_user_plane()->mutable_spec()->add_supported_service_group(it);
    }
    req.mutable_service_gateway_user_plane()->mutable_spec()->set_maintenence(maintenance);
    req.mutable_service_gateway_user_plane()->mutable_status()->set_allocated_session_count(allocated_session_count);
    for (auto it : network_connection) {
      req.mutable_service_gateway_user_plane()->mutable_status()->add_network_connection(it);
    }
    return UpdateServiceGatewayUserPlane(req, resp);
  };

  /**
   * rpc DeleteServiceGatewayUserPlane (DeleteServiceGatewayUserPlaneReq) returns (ServiceGatewayUserPlaneResp) {}
   */
  bool DeleteServiceGatewayUserPlane(const bbf::sss::DeleteServiceGatewayUserPlaneReq &req,
		                     bbf::sss::ServiceGatewayUserPlaneResp &resp) {
    VLOG(2) << "func: " << __PRETTY_FUNCTION__ << std::endl;
    grpc::ClientContext context;
    grpc::Status status = stub_->DeleteServiceGatewayUserPlane(&context, req, &resp);
    if (!status.ok()) {
      LOG(ERROR) << "failure: " << __FUNCTION__ << " code:" << status.error_code() << " reason:" << status.error_message() << std::endl;
      return false;
    }
    return true;
  };

  /**
   * rpc DeleteServiceGatewayUserPlane (DeleteServiceGatewayUserPlaneReq) returns (ServiceGatewayUserPlaneResp) {}
   */
  bool DeleteServiceGatewayUserPlane(const std::string &id,
                                     bbf::sss::ServiceGatewayUserPlaneResp &resp) {
    VLOG(2) << "func: " << __PRETTY_FUNCTION__ << std::endl;
    bbf::sss::DeleteServiceGatewayUserPlaneReq req;
    req.set_id(id);
    return DeleteServiceGatewayUserPlane(req, resp);
  };

  /**
   * rpc GetServiceGatewayUserPlane (GetServiceGatewayUserPlaneReq) returns (ServiceGatewayUserPlaneResp) {}
   */
  bool GetServiceGatewayUserPlane(const bbf::sss::GetServiceGatewayUserPlaneReq &req,
	                          bbf::sss::ServiceGatewayUserPlaneResp &resp) {
    VLOG(2) << "func: " << __PRETTY_FUNCTION__ << std::endl;
    grpc::ClientContext context;
    grpc::Status status = stub_->GetServiceGatewayUserPlane(&context, req, &resp);
    if (!status.ok()) {
      LOG(ERROR) << "failure: " << __FUNCTION__ << " code:" << status.error_code() << " reason:" << status.error_message() << std::endl;
      return false;
    }
    return true;
  };

  /**
   * rpc GetServiceGatewayUserPlane (GetServiceGatewayUserPlaneReq) returns (ServiceGatewayUserPlaneResp) {}
   */
  bool GetServiceGatewayUserPlane(const std::string &id,
		                  bbf::sss::ServiceGatewayUserPlaneResp &resp) {
    VLOG(2) << "func: " << __PRETTY_FUNCTION__ << std::endl;
    bbf::sss::GetServiceGatewayUserPlaneReq req;
    req.set_id(id);
    return GetServiceGatewayUserPlane(req, resp);
  };

  /**
   * rpc ListServiceGatewayUserPlanes (ListServiceGatewayUserPlaneReq) returns (ListServiceGatewayUserPlaneResp) {}
   */
  bool ListServiceGatewayUserPlanes(const bbf::sss::ListServiceGatewayUserPlaneReq &req,
		                    bbf::sss::ListServiceGatewayUserPlaneResp &resp) {
    VLOG(2) << "func: " << __PRETTY_FUNCTION__ << std::endl;
    grpc::ClientContext context;
    grpc::Status status = stub_->ListServiceGatewayUserPlanes(&context, req, &resp);
    if (!status.ok()) {
      LOG(ERROR) << "failure: " << __FUNCTION__ << " code:" << status.error_code() << " reason:" << status.error_message() << std::endl;
      return false;
    }
    return true;
  };

  /**
   * rpc ListServiceGatewayUserPlanes (ListServiceGatewayUserPlaneReq) returns (ListServiceGatewayUserPlaneResp) {}
   */
  bool ListServiceGatewayUserPlanes(const std::vector<std::string> &id_list,
		         bbf::sss::ListServiceGatewayUserPlaneResp &resp) {
    VLOG(2) << "func: " << __PRETTY_FUNCTION__ << std::endl;
    bbf::sss::ListServiceGatewayUserPlaneReq req;
    for (auto it : id_list) {
        req.mutable_filter()->add_id_list(it);
    }
    return ListServiceGatewayUserPlanes(req, resp);
  };

  /**
   * rpc ListServiceGatewayUserPlanes (ListServiceGatewayUserPlaneReq) returns (ListServiceGatewayUserPlaneResp) {}
   */
  bool ListServiceGatewayUserPlanes(const std::string& id,
		         bbf::sss::ListServiceGatewayUserPlaneResp &resp) {
    VLOG(2) << "func: " << __PRETTY_FUNCTION__ << std::endl;
    bbf::sss::ListServiceGatewayUserPlaneReq req;
    req.mutable_filter()->add_id_list(id);
    return ListServiceGatewayUserPlanes(req, resp);
  };

  /**
   * rpc ListServiceGatewayUserPlanes (ListServiceGatewayUserPlaneReq) returns (ListServiceGatewayUserPlaneResp) {}
   */
  bool ListServiceGatewayUserPlanes(bbf::sss::ListServiceGatewayUserPlaneResp &resp) {
    VLOG(2) << "func: " << __PRETTY_FUNCTION__ << std::endl;
    bbf::sss::ListServiceGatewayUserPlaneReq req;
    return ListServiceGatewayUserPlanes(req, resp);
  };


  /****************************************
   * SessionContext
   ****************************************/

  /**
   * rpc UpdateSessionContext (UpdateSessionContextReq) returns (SessionContextResp) {}
   */
  bool UpdateSessionContext(const bbf::sss::UpdateSessionContextReq &req,
                            bbf::sss::SessionContextResp &resp) {
    VLOG(2) << "func: " << __PRETTY_FUNCTION__ << std::endl;
    grpc::ClientContext context;
    grpc::Status status = stub_->UpdateSessionContext(&context, req, &resp);
    if (!status.ok()) {
      LOG(ERROR) << "failure: " << __FUNCTION__ << " code:" << status.error_code() << " reason:" << status.error_message() << std::endl;
      return false;
    }
    return true;
  };

  /**
   * rpc UpdateSessionContext (UpdateSessionContextReq) returns (SessionContextResp) {}
   */
  bool UpdateSessionContext(const std::string &name,
                            const bbf::sss::ItemStatus &item_status,
                            const std::string &tsf,
                            const std::string &desired_shard,
                            const std::string &current_shard,
                            const std::vector<std::string> &required_service_group,
                            const std::string &required_qos,
                            const bbf::sss::SessionContext_Spec_ContextType &context_type,
                            const std::string &mac_address,
                            const uint16_t s_tag,
                            const uint16_t c_tag,
                            const std::string &circuit_id,
                            const std::string &remote_id,
                            bbf::sss::SessionContextResp &resp) {
    VLOG(2) << "func: " << __PRETTY_FUNCTION__ << std::endl;
    bbf::sss::UpdateSessionContextReq req;
    req.mutable_session_context()->mutable_metadata()->set_name(name);
    req.mutable_session_context()->mutable_metadata()->set_item_status(item_status);
    req.mutable_session_context()->mutable_spec()->set_traffic_steering_function(tsf);
    req.mutable_session_context()->mutable_spec()->set_desired_shard(desired_shard);
    for (auto it : required_service_group) {
      req.mutable_session_context()->mutable_spec()->add_required_service_group(it);
    }
    req.mutable_session_context()->mutable_spec()->set_required_qos(required_qos);
    req.mutable_session_context()->mutable_spec()->set_context_type(context_type);
    req.mutable_session_context()->mutable_spec()->mutable_session_filter()->set_mac_address(mac_address);
    req.mutable_session_context()->mutable_spec()->mutable_session_filter()->set_svlan(s_tag);
    req.mutable_session_context()->mutable_spec()->mutable_session_filter()->set_cvlan(c_tag);
    req.mutable_session_context()->mutable_spec()->mutable_session_filter()->set_circuit_id(circuit_id);
    req.mutable_session_context()->mutable_spec()->mutable_session_filter()->set_remote_id(remote_id);
    return UpdateSessionContext(req, resp);
  };

  /**
   * rpc DeleteSessionContext (DeleteSessionContextReq) returns (SessionContextResp) {}
   */
  bool DeleteSessionContext(const bbf::sss::DeleteSessionContextReq &req,
		            bbf::sss::SessionContextResp &resp) {
    VLOG(2) << "func: " << __PRETTY_FUNCTION__ << std::endl;
    grpc::ClientContext context;
    grpc::Status status = stub_->DeleteSessionContext(&context, req, &resp);
    if (!status.ok()) {
      LOG(ERROR) << "failure: " << __FUNCTION__ << " code:" << status.error_code() << " reason:" << status.error_message() << std::endl;
      return false;
    }
    return true;
  };

  /**
   * rpc DeleteSessionContext (DeleteSessionContextReq) returns (SessionContextResp) {}
   */
  bool DeleteSessionContext(const std::string &id,
                            bbf::sss::SessionContextResp &resp) {
    VLOG(2) << "func: " << __PRETTY_FUNCTION__ << std::endl;
    bbf::sss::DeleteSessionContextReq req;
    req.set_id(id);
    return DeleteSessionContext(req, resp);
  };

  /**
   * rpc GetSessionContext (GetSessionContextReq) returns (SessionContextResp) {}
   */
  bool GetSessionContext(const bbf::sss::GetSessionContextReq &req,
		         bbf::sss::SessionContextResp &resp) {
    VLOG(2) << "func: " << __PRETTY_FUNCTION__ << std::endl;
    grpc::ClientContext context;
    grpc::Status status = stub_->GetSessionContext(&context, req, &resp);
    if (!status.ok()) {
      LOG(ERROR) << "failure: " << __FUNCTION__ << " code:" << status.error_code() << " reason:" << status.error_message() << std::endl;
      return false;
    }
    return true;
  };

  /**
   * rpc GetSessionContext (GetSessionContextReq) returns (SessionContextResp) {}
   */
  bool GetSessionContext(const std::string &id,
		         bbf::sss::SessionContextResp &resp) {
    VLOG(2) << "func: " << __PRETTY_FUNCTION__ << std::endl;
    bbf::sss::GetSessionContextReq req;
    req.set_id(id);
    return GetSessionContext(req, resp);
  };

  /**
   * rpc ListSessionContexts (ListSessionContextReq) returns (ListSessionContextResp) {}
   */
  bool ListSessionContexts(const bbf::sss::ListSessionContextReq &req,
		         bbf::sss::ListSessionContextResp &resp) {
    VLOG(2) << "func: " << __PRETTY_FUNCTION__ << std::endl;
    grpc::ClientContext context;
    grpc::Status status = stub_->ListSessionContexts(&context, req, &resp);
    if (!status.ok()) {
      LOG(ERROR) << "failure: " << __FUNCTION__ << " code:" << status.error_code() << " reason:" << status.error_message() << std::endl;
      return false;
    }
    return true;
  };

  /**
   * rpc ListSessionContexts (ListSessionContextReq) returns (ListSessionContextResp) {}
   */
  bool ListSessionContexts(const std::vector<std::string> &id_list,
		         bbf::sss::ListSessionContextResp &resp) {
    VLOG(2) << "func: " << __PRETTY_FUNCTION__ << std::endl;
    bbf::sss::ListSessionContextReq req;
    for (auto it : id_list) {
        req.mutable_filter()->add_id_list(it);
    }
    return ListSessionContexts(req, resp);
  };

  /**
   * rpc ListSessionContexts (ListSessionContextReq) returns (ListSessionContextResp) {}
   */
  bool ListSessionContexts(const std::string& id,
		         bbf::sss::ListSessionContextResp &resp) {
    VLOG(2) << "func: " << __PRETTY_FUNCTION__ << std::endl;
    bbf::sss::ListSessionContextReq req;
    req.mutable_filter()->add_id_list(id);
    return ListSessionContexts(req, resp);
  };

  /**
   * rpc ListSessionContexts (ListSessionContextReq) returns (ListSessionContextResp) {}
   */
  bool ListSessionContexts(bbf::sss::ListSessionContextResp &resp) {
    VLOG(2) << "func: " << __PRETTY_FUNCTION__ << std::endl;
    bbf::sss::ListSessionContextReq req;
    return ListSessionContexts(req, resp);
  };

  /****************************************
   * Lookup
   ****************************************/

  /**
   * rpc Lookup (LookupReq) returns (LookupResp) {}
   */
  bool Lookup(const bbf::sss::LookupReq &req,
              bbf::sss::LookupResp &resp) {
    VLOG(2) << "func: " << __PRETTY_FUNCTION__ << std::endl;
    grpc::ClientContext context;
    grpc::Status status = stub_->Lookup(&context, req, &resp);
    if (!status.ok()) {
      LOG(ERROR) << "failure: " << __FUNCTION__ << " code:" << status.error_code() << " reason:" << status.error_message() << std::endl;
      return false;
    }
    return true;
  };

  /**
   * rpc Lookup (LookupReq) returns (LookupResp) {}
   */
  bool Lookup(const std::string &tsf,
              const std::string &desired_shard,
              const std::vector<std::string> &required_service_group,
              const std::string &required_qos,
              const bbf::sss::SessionContext_Spec_ContextType &context_type,
              const std::string &mac_address,
              const uint16_t s_tag,
              const uint16_t c_tag,
              const std::string &circuit_id,
              const std::string &remote_id,
              bbf::sss::LookupResp &resp) {
    VLOG(2) << "func: " << __PRETTY_FUNCTION__ << std::endl;
    bbf::sss::LookupReq req;
    req.mutable_spec()->set_traffic_steering_function(tsf);
    req.mutable_spec()->set_desired_shard(desired_shard);
    for (auto it : required_service_group) {
      req.mutable_spec()->add_required_service_group(it);
    }
    req.mutable_spec()->set_required_qos(required_qos);
    req.mutable_spec()->set_context_type(context_type);
    req.mutable_spec()->mutable_session_filter()->set_mac_address(mac_address);
    req.mutable_spec()->mutable_session_filter()->set_svlan(s_tag);
    req.mutable_spec()->mutable_session_filter()->set_cvlan(c_tag);
    req.mutable_spec()->mutable_session_filter()->set_circuit_id(circuit_id);
    req.mutable_spec()->mutable_session_filter()->set_remote_id(remote_id);
    return Lookup(req, resp);
  };

  /****************************************
   * Subscribe
   ****************************************/

  bool Subscribe(UpsfSubscriber &subscriber) {
    VLOG(2) << "func: " << __PRETTY_FUNCTION__ << std::endl;
    bbf::sss::SubscribeReq req;
    bbf::sss::Update resp;
    grpc::ClientContext context;

    for (auto it : subscriber.subscriptions) {
      req.add_subscription(it);
    }

    std::unique_ptr<grpc::ClientReader<bbf::sss::Update> > reader(stub_->Subscribe(&context, req));
    while (reader->Read(&resp)) {

        bool cont = true;

        // shard
        if (resp.has_shard()) {
            cont = subscriber.update(resp.shard());
        }

        // session_context
        if (resp.has_session_context()) {
            cont = subscriber.update(resp.session_context());
        }

        // network_connection
        if (resp.has_network_connection()) {
            cont = subscriber.update(resp.network_connection());
        }

        // service_gateway_user_plane
        if (resp.has_service_gateway_user_plane()) {
            cont = subscriber.update(resp.service_gateway_user_plane());
        }

        // traffic_steering_function
        if (resp.has_traffic_steering_function()) {
            cont = subscriber.update(resp.traffic_steering_function());
        }

        // service_gateway
        if (resp.has_service_gateway()) {
            cont = subscriber.update(resp.service_gateway());
        }

        // break from loop?
        if (!cont) {
            break;
        }
    }
    grpc::Status status = reader->Finish();
    if (!status.ok()) {
      LOG(ERROR) << "failure: " << __FUNCTION__ << " code:" << status.error_code() << " reason:" << status.error_message() << std::endl;
      return false;
    }
    return true;
  };





private:
  std::shared_ptr<grpc::Channel> channel;
  std::unique_ptr<bbf::sss::sssUpsf::Stub> stub_;
};
} // namespace upsf

#endif
