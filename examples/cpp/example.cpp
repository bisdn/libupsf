#include "example.hpp"

#include <string>
#include <gflags/gflags.h>

using namespace upsf;

DEFINE_string(upsfhost, "localhost", "UPSF host");
DEFINE_int32(upsfport, 50051, "UPSF port");

void
Example::sg_list()
{
  /* list service gateways */
  std::vector<wt474_messages::v1::ServiceGateway> service_gateways;
  if (!client.ReadV1(service_gateways)) {
    LOG(ERROR) << "failure: read service gateways" << std::endl;
    return;
  }
  std::cout << "### <<ServiceGateways>>" << std::endl;
  for (auto sg : service_gateways) {
    std::cout << ServiceGatewayStream(sg) << std::endl;
  }
}

void
Example::sg_read()
{
  /* read service gateway */
  std::cout << "### Read ServiceGateway" << std::endl;
  wt474_messages::v1::ServiceGateway r_reply;
  if (!client.ReadV1(sg_name, r_reply)) {
    LOG(ERROR) << "failure: read service gateway" << std::endl;
    return;
  }
  std::cout << ServiceGatewayStream(r_reply) << std::endl;
}

void
Example::sg_create()
{
  /* create service gateway */
  std::cout << "### Create ServiceGateway" << std::endl;
  wt474_messages::v1::ServiceGateway c_request;
  wt474_messages::v1::ServiceGateway c_reply;
  c_request.set_name(sg_name);
  if (!client.CreateV1(c_request, c_reply)) {
    LOG(ERROR) << "failure: create service gateway" << std::endl;
    return;
  }
  std::cout << ServiceGatewayStream(c_reply) << std::endl;

  /* update service gateway */
  std::cout << "### Update ServiceGateway description" << std::endl;
  wt474_messages::v1::ServiceGateway u_request;
  wt474_messages::v1::ServiceGateway u_reply;
  wt474_upsf_service::v1::UpdateReq::UpdateOptions options;
  u_request.set_name(sg_name);
  u_request.mutable_metadata()->set_description(sg_desc);
  u_request.mutable_metadata()->set_derived_state(wt474_messages::v1::DerivedState::updating);
  if (!client.UpdateV1(u_request, u_reply, options)) {
    LOG(ERROR) << "failure: update service gateway" << std::endl;
    return;
  }
  std::cout << ServiceGatewayStream(u_reply) << std::endl;
}

void
Example::sg_delete()
{
  /* delete service gateway */
  std::cout << "### Delete ServiceGateway" << std::endl;
  std::string d_reply;
  if (!client.DeleteV1(sg_name, d_reply)) {
    LOG(ERROR) << "failure: delete service gateway" << std::endl;
    return;
  }
  std::cout << d_reply << std::endl;
}

void
Example::up_list()
{
  /* list service gateway user planes */
  std::vector<wt474_messages::v1::ServiceGatewayUserPlane> service_gateway_user_planes;
  if (!client.ReadV1(service_gateway_user_planes)) {
    LOG(ERROR) << "failure: read service gateway_user_planes" << std::endl;
    return;
  }
  std::cout << "### <<ServiceGatewayUserPlanes>>" << std::endl;
  for (auto sgup : service_gateway_user_planes) {
    std::cout << ServiceGatewayUserPlaneStream(sgup) << std::endl;
  }
}

void
Example::up_read(
  const std::string& up_name)
{
  /* read service gateway user plane */
  std::cout << "### Read ServiceGatewayUserPlane" << std::endl;
  wt474_messages::v1::ServiceGatewayUserPlane r_reply;
  if (!client.ReadV1(up_name, r_reply)) {
    LOG(ERROR) << "failure: read service gateway user plane" << std::endl;
    return;
  }
  std::cout << ServiceGatewayUserPlaneStream(r_reply) << std::endl;
}

void
Example::up_create(
  const std::string& up_name,
  const std::string& up_desc)
{
  /* create service gateway */
  std::cout << "### Create ServiceGatewayUserPlane <" << up_name << ">" << std::endl;
  wt474_messages::v1::ServiceGatewayUserPlane c_request;
  wt474_messages::v1::ServiceGatewayUserPlane c_reply;
  c_request.set_name(up_name);
  if (!client.CreateV1(c_request, c_reply)) {
    LOG(ERROR) << "failure: create service gateway user plane" << std::endl;
    return;
  }
  std::cout << ServiceGatewayUserPlaneStream(c_reply) << std::endl;

  /* update service gateway */
  wt474_messages::v1::ServiceGatewayUserPlane u_request;
  wt474_messages::v1::ServiceGatewayUserPlane u_reply;
  wt474_upsf_service::v1::UpdateReq::UpdateOptions options;
  u_request.set_name(up_name);
  u_request.set_service_gateway_name(sg_name);
  u_request.mutable_metadata()->set_description(up_desc);
  u_request.mutable_metadata()->set_derived_state(wt474_messages::v1::DerivedState::updating);
  u_request.mutable_spec()->set_max_session_count(128);
  u_request.mutable_spec()->set_max_shards(16);
  u_request.mutable_spec()->add_supported_service_group("ssg-1");
  u_request.mutable_spec()->add_supported_service_group("ssg-2");
  u_request.mutable_spec()->mutable_default_endpoint()->set_endpoint_name("ep-1");
  u_request.mutable_spec()->mutable_default_endpoint()->mutable_vtep()->set_ip_address("192.168.56.11");
  u_request.mutable_spec()->mutable_default_endpoint()->mutable_vtep()->set_udp_port(4789);
  u_request.mutable_spec()->mutable_default_endpoint()->mutable_vtep()->set_vni(12345);
  u_request.mutable_status()->set_allocated_session_count(7);
  u_request.mutable_status()->set_allocated_shards(1);
  std::cout << "### Update ServiceGatewayUserPlane <" << up_name << "> request" << std::endl;
  std::cout << ServiceGatewayUserPlaneStream(u_request) << std::endl;
  if (!client.UpdateV1(u_request, u_reply, options)) {
    LOG(ERROR) << "failure: update service gateway user plane" << std::endl;
    return;
  }
  std::cout << "### Update ServiceGatewayUserPlane <" << up_name << "> reply" << std::endl;
  std::cout << ServiceGatewayUserPlaneStream(u_reply) << std::endl;
}

void
Example::up_delete(
  const std::string& up_name)
{
  /* delete service gateway */
  std::cout << "### Delete ServiceGateway <" << up_name << ">" << std::endl;
  std::string d_reply;
  if (!client.DeleteV1(up_name, d_reply)) {
    LOG(ERROR) << "failure: delete service gateway user plane" << std::endl;
    return;
  }
  std::cout << d_reply << std::endl;
}

void
Example::shard_list()
{
  /* list service gateways */
  std::vector<wt474_messages::v1::Shard> service_gateways;
  if (!client.ReadV1(service_gateways)) {
    LOG(ERROR) << "failure: read service gateways" << std::endl;
    return;
  }
  std::cout << "### <<Shards>>" << std::endl;
  for (auto sg : service_gateways) {
    std::cout << ShardStream(sg) << std::endl;
  }
}

void
Example::shard_read()
{
  /* read shard */
  std::cout << "### Read Shard" << std::endl;
  wt474_messages::v1::Shard r_reply;
  if (!client.ReadV1(shard_name, r_reply)) {
    LOG(ERROR) << "failure: read shard" << std::endl;
    return;
  }
  std::cout << ShardStream(r_reply) << std::endl;
}

void
Example::shard_create()
{
  /* create shard */
  std::cout << "### Create Shard" << std::endl;
  wt474_messages::v1::Shard c_request;
  wt474_messages::v1::Shard c_reply;
  c_request.set_name(shard_name);
  if (!client.CreateV1(c_request, c_reply)) {
    LOG(ERROR) << "failure: create shard" << std::endl;
    return;
  }
  std::cout << ShardStream(c_reply) << std::endl;

  /* update shard */
  wt474_messages::v1::Shard u_request;
  wt474_messages::v1::Shard u_reply;
  wt474_upsf_service::v1::UpdateReq::UpdateOptions options;
  u_request.set_name(shard_name);
  u_request.mutable_metadata()->set_description(shard_desc);
  u_request.mutable_metadata()->set_derived_state(wt474_messages::v1::DerivedState::updating);
  u_request.mutable_spec()->set_max_session_count(4096);
  u_request.mutable_spec()->set_virtual_mac("22:22:22:22:22:22");
  u_request.mutable_spec()->mutable_desired_state()->set_service_gateway_user_plane("up-a");
  u_request.mutable_spec()->mutable_desired_state()->add_network_connection("net-conn-1");
  u_request.mutable_spec()->mutable_desired_state()->add_network_connection("net-conn-2");
  u_request.mutable_spec()->add_prefix("192.168.2.0/24");
  u_request.mutable_spec()->add_prefix("192.168.3.0/24");
  u_request.mutable_status()->set_allocated_session_count(64);
  u_request.mutable_status()->mutable_current_state()->set_service_gateway_user_plane("up-b");
  (*u_request.mutable_status()->mutable_current_state()->mutable_tsf_network_connection())["key1"] = "value1";
  (*u_request.mutable_status()->mutable_current_state()->mutable_tsf_network_connection())["key2"] = "value2";
  u_request.mutable_mbb()->set_mbb_state(wt474_messages::v1::Shard_Mbb_MbbState::Shard_Mbb_MbbState_upstream_switchover_required);
  std::cout << "### Update Shard request" << std::endl;
  std::cout << ShardStream(u_request) << std::endl;
  if (!client.UpdateV1(u_request, u_reply, options)) {
    LOG(ERROR) << "failure: update shard" << std::endl;
    return;
  }
  std::cout << "### Update Shard reply" << std::endl;
  std::cout << ShardStream(u_reply) << std::endl;
}

void
Example::shard_delete()
{
  /* delete shard */
  std::cout << "### Delete Shard" << std::endl;
  std::string d_reply;
  if (!client.DeleteV1(shard_name, d_reply)) {
    LOG(ERROR) << "failure: delete shard" << std::endl;
    return;
  }
  std::cout << d_reply << std::endl;
}

void
Example::session_context_list()
{
  /* list service gateways */
  std::vector<wt474_messages::v1::SessionContext> service_gateways;
  if (!client.ReadV1(service_gateways)) {
    LOG(ERROR) << "failure: read service gateways" << std::endl;
    return;
  }
  std::cout << "### <<SessionContexts>>" << std::endl;
  for (auto sg : service_gateways) {
    std::cout << SessionContextStream(sg) << std::endl;
  }
}

void
Example::session_context_read()
{
  /* read session_context */
  std::cout << "### Read SessionContext" << std::endl;
  wt474_messages::v1::SessionContext r_reply;
  if (!client.ReadV1(session_context_name, r_reply)) {
    LOG(ERROR) << "failure: read session_context" << std::endl;
    return;
  }
  std::cout << SessionContextStream(r_reply) << std::endl;
}

void
Example::session_context_create()
{
  /* create session_context */
  std::cout << "### Create SessionContext" << std::endl;
  wt474_messages::v1::SessionContext c_request;
  wt474_messages::v1::SessionContext c_reply;
  c_request.set_name(session_context_name);
  if (!client.CreateV1(c_request, c_reply)) {
    LOG(ERROR) << "failure: create session_context" << std::endl;
    return;
  }
  std::cout << SessionContextStream(c_reply) << std::endl;

  /* update session_context */
  wt474_messages::v1::SessionContext u_request;
  wt474_messages::v1::SessionContext u_reply;
  wt474_upsf_service::v1::UpdateReq::UpdateOptions options;
  u_request.set_name(session_context_name);
  u_request.mutable_metadata()->set_description(session_context_desc);
  u_request.mutable_metadata()->set_derived_state(wt474_messages::v1::DerivedState::updating);
  u_request.mutable_spec()->set_traffic_steering_function("tsf-a");
  u_request.mutable_spec()->add_required_service_group("rsg-1");
  u_request.mutable_spec()->add_required_service_group("rsg-2");
  u_request.mutable_spec()->set_required_quality(3);
  u_request.mutable_spec()->set_circuit_id("circuit-id");
  u_request.mutable_spec()->set_remote_id("remote-id");
  u_request.mutable_spec()->mutable_session_filter()->set_source_mac_address("00:11:22:33:44:55");
  u_request.mutable_spec()->mutable_session_filter()->set_svlan(50);
  u_request.mutable_spec()->mutable_session_filter()->set_cvlan(100);
  u_request.mutable_spec()->mutable_desired_state()->set_shard(shard_name);
  u_request.mutable_status()->mutable_current_state()->set_user_plane_shard("up-a");
  u_request.mutable_status()->mutable_current_state()->set_tsf_shard("tsf-a");
  std::cout << "### Update SessionContext request" << std::endl;
  std::cout << SessionContextStream(u_request) << std::endl;
  if (!client.UpdateV1(u_request, u_reply, options)) {
    LOG(ERROR) << "failure: update session_context" << std::endl;
    return;
  }
  std::cout << "### Update SessionContext reply" << std::endl;
  std::cout << SessionContextStream(u_reply) << std::endl;
}

void
Example::session_context_delete()
{
  /* delete session_context */
  std::cout << "### Delete SessionContext" << std::endl;
  std::string d_reply;
  if (!client.DeleteV1(session_context_name, d_reply)) {
    LOG(ERROR) << "failure: delete session_context" << std::endl;
    return;
  }
  std::cout << d_reply << std::endl;
}

void
Example::tsf_list()
{
  /* list service gateways */
  std::vector<wt474_messages::v1::TrafficSteeringFunction> service_gateways;
  if (!client.ReadV1(service_gateways)) {
    LOG(ERROR) << "failure: read service gateways" << std::endl;
    return;
  }
  std::cout << "### <<TrafficSteeringFunctions>>" << std::endl;
  for (auto sg : service_gateways) {
    std::cout << TrafficSteeringFunctionStream(sg) << std::endl;
  }
}

void
Example::tsf_read()
{
  /* read traffic steering function */
  std::cout << "### Read TrafficSteeringFunction" << std::endl;
  wt474_messages::v1::TrafficSteeringFunction r_reply;
  if (!client.ReadV1(tsf_name, r_reply)) {
    LOG(ERROR) << "failure: read traffic steering function" << std::endl;
    return;
  }
  std::cout << TrafficSteeringFunctionStream(r_reply) << std::endl;
}

void
Example::tsf_create()
{
  /* create traffic steering function */
  std::cout << "### Create TrafficSteeringFunction" << std::endl;
  wt474_messages::v1::TrafficSteeringFunction c_request;
  wt474_messages::v1::TrafficSteeringFunction c_reply;
  c_request.set_name(tsf_name);
  if (!client.CreateV1(c_request, c_reply)) {
    LOG(ERROR) << "failure: create traffic steering function" << std::endl;
    return;
  }
  std::cout << TrafficSteeringFunctionStream(c_reply) << std::endl;

  /* update traffic steering function */
  wt474_messages::v1::TrafficSteeringFunction u_request;
  wt474_messages::v1::TrafficSteeringFunction u_reply;
  wt474_upsf_service::v1::UpdateReq::UpdateOptions options;
  u_request.set_name(tsf_name);
  u_request.mutable_metadata()->set_description(tsf_desc);
  u_request.mutable_metadata()->set_derived_state(wt474_messages::v1::DerivedState::updating);
  u_request.mutable_spec()->mutable_default_endpoint()->set_endpoint_name("ep-0");
  u_request.mutable_spec()->mutable_default_endpoint()->mutable_vtep()->set_ip_address("192.168.30.11");
  u_request.mutable_spec()->mutable_default_endpoint()->mutable_vtep()->set_udp_port(4789);
  u_request.mutable_spec()->mutable_default_endpoint()->mutable_vtep()->set_vni(12345);
  std::cout << "### Update TrafficSteeringFunction request" << std::endl;
  std::cout << TrafficSteeringFunctionStream(u_request) << std::endl;
  if (!client.UpdateV1(u_request, u_reply, options)) {
    LOG(ERROR) << "failure: update traffic steering function" << std::endl;
    return;
  }
  std::cout << "### Update TrafficSteeringFunction reply" << std::endl;
  std::cout << TrafficSteeringFunctionStream(u_reply) << std::endl;
}

void
Example::tsf_delete()
{
  /* delete traffic steering function */
  std::cout << "### Delete TrafficSteeringFunction" << std::endl;
  std::string d_reply;
  if (!client.DeleteV1(tsf_name, d_reply)) {
    LOG(ERROR) << "failure: delete traffic steering function" << std::endl;
    return;
  }
  std::cout << d_reply << std::endl;
}

void
Example::nc_list()
{
  /* list service gateways */
  std::vector<wt474_messages::v1::NetworkConnection> service_gateways;
  if (!client.ReadV1(service_gateways)) {
    LOG(ERROR) << "failure: read service gateways" << std::endl;
    return;
  }
  std::cout << "### <<NetworkConnections>>" << std::endl;
  for (auto sg : service_gateways) {
    std::cout << NetworkConnectionStream(sg) << std::endl;
  }
}

void
Example::nc_read()
{
  /* read traffic steering function */
  std::cout << "### Read NetworkConnection" << std::endl;
  wt474_messages::v1::NetworkConnection r_reply;
  if (!client.ReadV1(nc_name, r_reply)) {
    LOG(ERROR) << "failure: read traffic steering function" << std::endl;
    return;
  }
  std::cout << NetworkConnectionStream(r_reply) << std::endl;
}

void
Example::nc_create()
{
  /* create traffic steering function */
  std::cout << "### Create NetworkConnection" << std::endl;
  wt474_messages::v1::NetworkConnection c_request;
  wt474_messages::v1::NetworkConnection c_reply;
  c_request.set_name(nc_name);
  if (!client.CreateV1(c_request, c_reply)) {
    LOG(ERROR) << "failure: create traffic steering function" << std::endl;
    return;
  }
  std::cout << NetworkConnectionStream(c_reply) << std::endl;

  /* update traffic steering function */
  wt474_messages::v1::NetworkConnection u_request;
  wt474_messages::v1::NetworkConnection u_reply;
  wt474_upsf_service::v1::UpdateReq::UpdateOptions options;
  u_request.set_name(nc_name);
  u_request.mutable_metadata()->set_description(nc_desc);
  u_request.mutable_metadata()->set_derived_state(wt474_messages::v1::DerivedState::updating);
  u_request.mutable_spec()->set_maximum_supported_quality(7);
  u_request.mutable_spec()->mutable_ss_ptp()->add_sgup_endpoint()->set_endpoint_name("vxlan0");
  u_request.mutable_spec()->mutable_ss_ptp()->mutable_sgup_endpoint(0)->mutable_vtep()->set_ip_address("1.1.1.1");
  u_request.mutable_spec()->mutable_ss_ptp()->mutable_sgup_endpoint(0)->mutable_vtep()->set_udp_port(4789);
  u_request.mutable_spec()->mutable_ss_ptp()->mutable_sgup_endpoint(0)->mutable_vtep()->set_vni(12345);
  u_request.mutable_spec()->mutable_ss_ptp()->add_sgup_endpoint()->set_endpoint_name("l2vpn0");
  u_request.mutable_spec()->mutable_ss_ptp()->mutable_sgup_endpoint(1)->mutable_l2vpn()->set_vpn_id(3333);
  u_request.mutable_spec()->mutable_ss_ptp()->add_sgup_endpoint()->set_endpoint_name("port0");
  u_request.mutable_spec()->mutable_ss_ptp()->mutable_sgup_endpoint(2)->mutable_port_vlan()->set_logical_port("eth0");
  u_request.mutable_spec()->mutable_ss_ptp()->mutable_sgup_endpoint(2)->mutable_port_vlan()->set_logical_port("eth0");
  u_request.mutable_spec()->mutable_ss_ptp()->mutable_sgup_endpoint(2)->mutable_port_vlan()->set_svlan(50);
  u_request.mutable_spec()->mutable_ss_ptp()->mutable_tsf_endpoint()->set_endpoint_name("vxl100");
  u_request.mutable_spec()->mutable_ss_ptp()->mutable_tsf_endpoint()->mutable_vtep()->set_ip_address("192.168.30.11");
  u_request.mutable_spec()->mutable_ss_ptp()->mutable_tsf_endpoint()->mutable_vtep()->set_udp_port(4789);
  u_request.mutable_spec()->mutable_ss_ptp()->mutable_tsf_endpoint()->mutable_vtep()->set_vni(12345);
  (*u_request.mutable_status()->mutable_nc_active())["net-conn-1"] = true;
  (*u_request.mutable_status()->mutable_nc_active())["net-conn-2"] = true;
  (*u_request.mutable_status()->mutable_nc_active())["net-conn-3"] = true;
  u_request.mutable_status()->set_allocated_shards(97);
  std::cout << "### Update NetworkConnection request" << std::endl;
  std::cout << NetworkConnectionStream(u_request) << std::endl;
  if (!client.UpdateV1(u_request, u_reply, options)) {
    LOG(ERROR) << "failure: update traffic steering function" << std::endl;
    return;
  }
  std::cout << "### Update NetworkConnection reply" << std::endl;
  std::cout << NetworkConnectionStream(u_reply) << std::endl;
}

void
Example::nc_delete()
{
  /* delete traffic steering function */
  std::cout << "### Delete NetworkConnection" << std::endl;
  std::string d_reply;
  if (!client.DeleteV1(nc_name, d_reply)) {
    LOG(ERROR) << "failure: delete traffic steering function" << std::endl;
    return;
  }
  std::cout << d_reply << std::endl;
}

void
Example::setup()
{
  //using namespace std::chrono_literals;

  sg_list();

  sg_create();

  //std::this_thread::sleep_for(2000ms);

  sg_read();

  sg_list();

  sg_read();

  up_list();

  for (auto up : up_names) {
    up_create(up.first, up.second);
    up_read(up.first);
  }

  up_list();

  shard_list();

  shard_create();

  shard_read();

  shard_list();

  shard_read();

  shard_list();

  session_context_list();

  session_context_create();

  session_context_read();

  session_context_list();

  session_context_read();

  session_context_list();

  tsf_list();

  tsf_create();

  tsf_read();

  tsf_list();

  tsf_read();

  tsf_list();

  nc_list();

  nc_create();

  nc_read();

  nc_list();

  nc_read();

  nc_list();
}

void
Example::release()
{
  nc_delete();

  nc_list();

  tsf_delete();

  tsf_list();

  session_context_delete();

  session_context_list();

  shard_delete();

  shard_list();

  for (auto up : up_names) {
    up_delete(up.first);
  }

  up_list();

  sg_delete();

  sg_list();
}

int
main(int argc, char** argv)
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
  Example example = Example(upsfaddr.str());

  return 0;
};
