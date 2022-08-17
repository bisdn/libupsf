#ifndef UPSF_MAPPING_HPP
#define UPSF_MAPPING_HPP

#include <mutex>
#include <shared_mutex>
#include <thread>

#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>
#include <glog/logging.h>

#include "upsf.h"

#include "wt474_upsf_messages/v1/messages_v1.pb.h"
#include "wt474_upsf_messages/v1/messages_v1.grpc.pb.h"
#include "wt474_upsf_service/v1/service_v1.pb.h"
#include "wt474_upsf_service/v1/service_v1.grpc.pb.h"

namespace upsf {

class UpsfMapping {
  public:

    /* service gateway */
    static bool map(
      const wt474_messages::v1::ServiceGateway& from,
      upsf_service_gateway_t& to);

    static bool map(
      const upsf_service_gateway_t& from,
      wt474_messages::v1::ServiceGateway& to);

    /* service gateway user plane */
    static bool map(
      const wt474_messages::v1::ServiceGatewayUserPlane& from,
      upsf_service_gateway_user_plane_t& to);

    static bool map(
      const upsf_service_gateway_user_plane_t& from,
      wt474_messages::v1::ServiceGatewayUserPlane& to);

    /* service gateway user plane: spec */
    static bool map(
      const wt474_messages::v1::ServiceGatewayUserPlane::Spec& from,
      upsf_service_gateway_user_plane_spec_t& to);

    static bool map(
      const upsf_service_gateway_user_plane_spec_t& from,
      wt474_messages::v1::ServiceGatewayUserPlane::Spec& to);

    /* service gateway user plane: status */
    static bool map(
      const wt474_messages::v1::ServiceGatewayUserPlane::Status& from,
      upsf_service_gateway_user_plane_status_t& to);

    static bool map(
      const upsf_service_gateway_user_plane_status_t& from,
      wt474_messages::v1::ServiceGatewayUserPlane::Status& to);
 
    /* shard */
    static bool map(
      const wt474_messages::v1::Shard& from,
      upsf_shard_t& to);

    static bool map(
      const upsf_shard_t& from,
      wt474_messages::v1::Shard& to);

    /* shard: spec */
    static bool map(
      const wt474_messages::v1::Shard::Spec& from,
      upsf_shard_spec_t& to);

    static bool map(
      const upsf_shard_spec_t& from,
      wt474_messages::v1::Shard::Spec& to);

    /* shard: spec: desired_state */
    static bool map(
      const wt474_messages::v1::Shard::Spec::DesiredState& from,
      upsf_shard_spec_desired_state_t& to);

    static bool map(
      const upsf_shard_spec_desired_state_t& from,
      wt474_messages::v1::Shard::Spec::DesiredState& to);

    /* shard: status */
    static bool map(
      const wt474_messages::v1::Shard::Status& from,
      upsf_shard_status_t& to);

    static bool map(
      const upsf_shard_status_t& from,
      wt474_messages::v1::Shard::Status& to);

    /* shard: status: current_state */
    static bool map(
      const wt474_messages::v1::Shard::Status::CurrentState& from,
      upsf_shard_status_current_state_t& to);

    static bool map(
      const upsf_shard_status_current_state_t& from,
      wt474_messages::v1::Shard::Status::CurrentState& to);

    /* shard: mbb */
    static bool map(
      const wt474_messages::v1::Shard::Mbb& from,
      upsf_shard_mbb_t& to);

    static bool map(
      const upsf_shard_mbb_t& from,
      wt474_messages::v1::Shard::Mbb& to);

    /* traffic_steering_function */
    static bool map(
      const wt474_messages::v1::TrafficSteeringFunction& from,
      upsf_traffic_steering_function_t& to);

    static bool map(
      const upsf_traffic_steering_function_t& from,
      wt474_messages::v1::TrafficSteeringFunction& to);

    /* traffic_steering_function: spec */
    static bool map(
      const wt474_messages::v1::TrafficSteeringFunction::Spec& from,
      upsf_traffic_steering_function_spec_t& to);

    static bool map(
      const upsf_traffic_steering_function_spec_t& from,
      wt474_messages::v1::TrafficSteeringFunction::Spec& to);

    /* network_connection */
    static bool map(
      const wt474_messages::v1::NetworkConnection& from,
      upsf_network_connection_t& to);

    static bool map(
      const upsf_network_connection_t& from,
      wt474_messages::v1::NetworkConnection& to);

    /* network_connection: spec */
    static bool map(
      const wt474_messages::v1::NetworkConnection::Spec& from,
      upsf_network_connection_spec_t& to);

    static bool map(
      const upsf_network_connection_spec_t& from,
      wt474_messages::v1::NetworkConnection::Spec& to);

    /* network_connection: spec: endpoint */
    static bool map(
      const wt474_messages::v1::NetworkConnection::Spec::Endpoint& from,
      upsf_network_connection_spec_endpoint_t& to);

    static bool map(
      const upsf_network_connection_spec_endpoint_t& from,
      wt474_messages::v1::NetworkConnection::Spec::Endpoint& to);

    /* network_connection: status */
    static bool map(
      const wt474_messages::v1::NetworkConnection::Status& from,
      upsf_network_connection_status_t& to);

    static bool map(
      const upsf_network_connection_status_t& from,
      wt474_messages::v1::NetworkConnection::Status& to);

    /* vtep */
    static bool map(
      const wt474_messages::v1::Vtep& from,
      upsf_vtep_t& to);

    static bool map(
      const upsf_vtep_t& from,
      wt474_messages::v1::Vtep& to);

    /* l2vpn */
    static bool map(
      const wt474_messages::v1::L2vpn& from,
      upsf_l2vpn_t& to);

    static bool map(
      const upsf_l2vpn_t& from,
      wt474_messages::v1::L2vpn& to);

    /* port_vlan */
    static bool map(
      const wt474_messages::v1::PortVlan& from,
      upsf_port_vlan_t& to);

    static bool map(
      const upsf_port_vlan_t& from,
      wt474_messages::v1::PortVlan& to);

    /* session_filter */
    static bool map(
      const wt474_messages::v1::SessionFilter& from,
      upsf_session_filter_t& to);

    static bool map(
      const upsf_session_filter_t& from,
      wt474_messages::v1::SessionFilter& to);

    /* session_context */
    static bool map(
      const wt474_messages::v1::SessionContext& from,
      upsf_session_context_t& to);

    static bool map(
      const upsf_session_context_t& from,
      wt474_messages::v1::SessionContext& to);

    /* session_context: spec */
    static bool map(
      const wt474_messages::v1::SessionContext::Spec& from,
      upsf_session_context_spec_t& to);

    static bool map(
      const upsf_session_context_spec_t& from,
      wt474_messages::v1::SessionContext::Spec& to);

    /* session_context: spec: desired_state */
    static bool map(
      const wt474_messages::v1::SessionContext::Spec::DesiredState& from,
      upsf_session_context_spec_desired_state_t& to);

    static bool map(
      const upsf_session_context_spec_desired_state_t& from,
      wt474_messages::v1::SessionContext::Spec::DesiredState& to);

    /* session_context: status */
    static bool map(
      const wt474_messages::v1::SessionContext::Status& from,
      upsf_session_context_status_t& to);

    static bool map(
      const upsf_session_context_status_t& from,
      wt474_messages::v1::SessionContext::Status& to);

    /* session_context: status: current_state */
    static bool map(
      const wt474_messages::v1::SessionContext::Status::CurrentState& from,
      upsf_session_context_status_current_state_t& to);

    static bool map(
      const upsf_session_context_status_current_state_t& from,
      wt474_messages::v1::SessionContext::Status::CurrentState& to);
};

}; // end namespace upsf

#endif
