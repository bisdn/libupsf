#include "upsf.h"
#include "upsf.hpp"
#include "upsf_stream.hpp"
#include "upsf_c_mapping.hpp"

#include <stdlib.h>
#include <string>
#include <shared_mutex>
#include <thread>
#include <algorithm>

#include <sstream>

class UpsfSlot final {
  public:
    UpsfSlot(const std::string &upsf_addr)
      : client(std::unique_ptr<upsf::UpsfClient>(
          new upsf::UpsfClient(
            grpc::CreateChannel(
              upsf_addr,
              grpc::InsecureChannelCredentials())))) {}

    std::unique_ptr<upsf::UpsfClient> client;
};

class UpsfSubscriberWrapper : public upsf::UpsfSubscriber {
  public:
    UpsfSubscriberWrapper(
      void* userdata = nullptr,
      upsf_shard_cb_t shard_cb = nullptr,
      upsf_session_context_cb_t session_context_cb = nullptr,
      upsf_network_connection_cb_t network_connection_cb = nullptr,
      upsf_service_gateway_user_plane_cb_t service_gateway_user_plane_cb = nullptr,
      upsf_traffic_steering_function_cb_t traffic_steering_function_cb = nullptr,
      upsf_service_gateway_cb_t service_gateway_cb = nullptr) :
        UpsfSubscriber(/*watch=*/true),
        userdata(userdata),
        shard_cb(shard_cb),
        session_context_cb(session_context_cb),
        network_connection_cb(network_connection_cb),
        service_gateway_user_plane_cb(service_gateway_user_plane_cb),
        traffic_steering_function_cb(traffic_steering_function_cb),
        service_gateway_cb(service_gateway_cb)
    {};

  public:

    virtual void notify(
      const wt474_messages::v1::ServiceGateway& service_gateway)
    {
      if (!service_gateway_cb) {
        return;
      }
      upsf_service_gateway_t upsf_service_gateway;

      upsf::UpsfMapping::map(service_gateway, upsf_service_gateway);
    
      (*service_gateway_cb)(&upsf_service_gateway, userdata);
    };

    virtual void notify(
      const wt474_messages::v1::ServiceGatewayUserPlane& service_gateway_user_plane)
    {
      if (!service_gateway_user_plane_cb) {
        return;
      }
      upsf_service_gateway_user_plane_t upsf_service_gateway_user_plane;

      upsf::UpsfMapping::map(service_gateway_user_plane, upsf_service_gateway_user_plane);
    
      (*service_gateway_user_plane_cb)(&upsf_service_gateway_user_plane, userdata);
    };
 
    virtual void notify(
      const wt474_messages::v1::Shard& shard)
    { 
      if (!shard_cb) {
        return;
      }
      upsf_shard_t upsf_shard;

      upsf::UpsfMapping::map(shard, upsf_shard);
    
      (*shard_cb)(&upsf_shard, userdata);
    };
  
    virtual void notify(
      const wt474_messages::v1::SessionContext& session_context)
    {
      if (!session_context_cb) {
        return;
      }
      upsf_session_context_t upsf_session_context;

      upsf::UpsfMapping::map(session_context, upsf_session_context);
    
      (*session_context_cb)(&upsf_session_context, userdata);
    };
  
    virtual void notify(
      const wt474_messages::v1::NetworkConnection& network_connection)
    {
      if (!network_connection_cb) {
        return;
      }
      upsf_network_connection_t upsf_network_connection;

      upsf::UpsfMapping::map(network_connection, upsf_network_connection);

      (*network_connection_cb)(&upsf_network_connection, userdata);
    };
  
    virtual void notify(
      const wt474_messages::v1::TrafficSteeringFunction& traffic_steering_function)
    {
      if (!traffic_steering_function_cb) {
        return;
      }
      upsf_traffic_steering_function_t upsf_traffic_steering_function;

      upsf::UpsfMapping::map(traffic_steering_function, upsf_traffic_steering_function);

      (*traffic_steering_function_cb)(&upsf_traffic_steering_function, userdata);
    };
  
  public:
    void* userdata;
    upsf_shard_cb_t shard_cb;
    upsf_session_context_cb_t session_context_cb;
    upsf_network_connection_cb_t network_connection_cb;
    upsf_service_gateway_user_plane_cb_t service_gateway_user_plane_cb;
    upsf_traffic_steering_function_cb_t traffic_steering_function_cb;
    upsf_service_gateway_cb_t service_gateway_cb;
};

#define UPSF_MAX_SLOTS 128
std::map<upsf_handle_t, std::shared_ptr<UpsfSlot> > upsf_slots;
std::shared_mutex upsf_slots_mutex;

const char* upsf_derived_state_names[] = {
    "unknown",
    "inactive",
    "active",
    "updating",
    "deleting",
    "deleted",
    "void",
};

const char* upsf_item_type_names[] = {
    "service-gateway",
    "service-gateway-user-plane",
    "traffic-steering-funtion",
    "network-connection",
    "shard",
    "session-context",
    "void",
};

const char* upsf_maintenance_req_names[] = {
    "none",
    "drain",
    "drain-and-delete",
    "void",
};

const char* upsf_mbb_state_names[] = {
    "non-mbb-move-required",
    "userplane-mbb-initiatiation-required",
    "upstream-switchover-required",
    "downstream-switchover-required",
    "upstream-finalization-required",
    "complete",
    "void",
};

const char* upsf_derived_state_to_name(int derived_state)
{
    derived_state = derived_state < UPSF_DERIVED_STATE_MAX ? derived_state : UPSF_DERIVED_STATE_MAX;
    return upsf_derived_state_names[derived_state];
}

const char* upsf_item_type_to_name(int item_type)
{
    item_type = item_type < UPSF_ITEM_TYPE_MAX ? item_type : UPSF_ITEM_TYPE_MAX;
    return upsf_item_type_names[item_type];
}

const char* upsf_maintenance_req_to_name(int maintenance_req)
{
    maintenance_req = maintenance_req < UPSF_MAINTENANCE_REQ_MAX ? maintenance_req : UPSF_MAINTENANCE_REQ_MAX;
    return upsf_maintenance_req_names[maintenance_req];
}

const char* upsf_mbb_state_to_name(int mbb_state)
{
    mbb_state = mbb_state < UPSF_MBB_STATE_MAX ? mbb_state : UPSF_MBB_STATE_MAX;
    return upsf_mbb_state_names[mbb_state];
}

/**
 * open upsf connection
 */
upsf_handle_t upsf_open(const char* upsf_host, const int upsf_port) {

  /* get rw lock on upsf slots */
  std::unique_lock lock(upsf_slots_mutex);

  /* returned as upsf handle */
  upsf_handle_t handle = 0;

  /* search for idle upsf slot */
  for (handle = 0; handle < UPSF_MAX_SLOTS; handle++) {
    if (upsf_slots.find(handle) == upsf_slots.end()) {
      break;
    }
  }

  /* error: no idle slot available */
  if (UPSF_MAX_SLOTS == handle) {
    return -1;
  }

  /* upsf address */
  std::stringstream upsfaddr;
  upsfaddr << upsf_host << ":" << upsf_port;

  /* create new upsf slot */
  upsf_slots.emplace(handle, new UpsfSlot(upsfaddr.str()));

  return handle;
}

/**
 * close upsf connection
 */
int upsf_close(upsf_handle_t handle) {

  /* get rw lock on upsf slots */
  std::unique_lock lock(upsf_slots_mutex);

  /* upsf slot already released? */
  if (upsf_slots.find(handle) == upsf_slots.end()) {
    return 1;
  }

  /* free memory used for upsf slot */
  upsf_slots.erase(handle);

  /* done */
  return 0;
}



/******************************************************************
 * ServiceGateway
 ******************************************************************/

/**
 * CreateV1
 */
upsf_service_gateway_t* upsf_create_service_gateway(upsf_handle_t upsf_handle, upsf_service_gateway_t* service_gateway)
{
  /* target buffer */
  if (!service_gateway) {
      return nullptr;
  }

  /* get r lock on upsf slots */
  std::shared_lock rlock(upsf_slots_mutex);

  /* get upsf_slot for given handle */
  if (upsf_slots.find(upsf_handle) == upsf_slots.end()) {
    return nullptr; 
  }
  UpsfSlot* slot = upsf_slots[upsf_handle].get();

  wt474_messages::v1::ServiceGateway request;
  wt474_messages::v1::ServiceGateway reply;

  upsf::UpsfMapping::map(*service_gateway, request);
  VLOG(1) << "libupsf: " << __PRETTY_FUNCTION__ << " request=" << upsf::ServiceGatewayStream(request) << std::endl;

  /* call upsf client instance */
  if (!slot->client->CreateV1(request, reply)) {
    return nullptr;
  }

  upsf::UpsfMapping::map(reply, *service_gateway);
  VLOG(1) << "libupsf: " << __PRETTY_FUNCTION__ << " reply=" << upsf::ServiceGatewayStream(reply) << std::endl;

  return service_gateway;
}

/**
 * UpdateV1
 */
upsf_service_gateway_t* upsf_update_service_gateway(upsf_handle_t upsf_handle, upsf_service_gateway_t* service_gateway)
{
  /* target buffer */
  if (!service_gateway) {
      return nullptr;
  }

  /* get r lock on upsf slots */
  std::shared_lock rlock(upsf_slots_mutex);

  /* get upsf_slot for given handle */
  if (upsf_slots.find(upsf_handle) == upsf_slots.end()) {
    return nullptr; 
  }
  UpsfSlot* slot = upsf_slots[upsf_handle].get();

  wt474_upsf_service::v1::UpdateReq::UpdateOptions update_options;
  wt474_messages::v1::ServiceGateway request;
  wt474_messages::v1::ServiceGateway reply;

  upsf::UpsfMapping::map(*service_gateway, request);
  VLOG(1) << "libupsf: " << __PRETTY_FUNCTION__ << " request=" << upsf::ServiceGatewayStream(request) << std::endl;

  /* call upsf client instance */
  if (!slot->client->UpdateV1(request, reply, update_options)) {
    return nullptr;
  }

  upsf::UpsfMapping::map(reply, *service_gateway);
  VLOG(1) << "libupsf: " << __PRETTY_FUNCTION__ << " reply=" << upsf::ServiceGatewayStream(reply) << std::endl;

  return service_gateway;
}

/**
 * ReadV1
 */
upsf_service_gateway_t* upsf_get_service_gateway(upsf_handle_t upsf_handle, upsf_service_gateway_t *upsf_service_gateway)
{
  /* target buffer */
  if (!upsf_service_gateway) {
      return nullptr;
  }

  /* get r lock on upsf slots */
  std::shared_lock rlock(upsf_slots_mutex);

  /* get upsf_slot for given handle */
  if (upsf_slots.find(upsf_handle) == upsf_slots.end()) {
    return nullptr;
  }
  UpsfSlot* slot = upsf_slots[upsf_handle].get();

  wt474_messages::v1::ServiceGateway reply;
  VLOG(1) << "libupsf: " << __PRETTY_FUNCTION__ << " request=" << upsf_service_gateway->name.str << std::endl;

  /* call upsf client instance */
  if (!slot->client->ReadV1(std::string(upsf_service_gateway->name.str), reply)) {
    return nullptr;
  }

  /* empty name: not found */
  if (reply.name().empty()) {
    return nullptr;
  }

  /* map cpp-object to c-struct */
  upsf::UpsfMapping::map(reply, *upsf_service_gateway);
  VLOG(1) << "libupsf: " << __PRETTY_FUNCTION__ << " reply=" << upsf::ServiceGatewayStream(reply) << std::endl;

  return upsf_service_gateway;
}

/**
 * DeleteV1
 */
int upsf_delete_service_gateway(upsf_handle_t upsf_handle, upsf_service_gateway_t *upsf_service_gateway)
{
  /* target buffer */
  if (!upsf_service_gateway) {
      return -1;
  }

  /* get r lock on upsf slots */
  std::shared_lock rlock(upsf_slots_mutex);

  /* get upsf_slot for given handle */
  if (upsf_slots.find(upsf_handle) == upsf_slots.end()) {
    return -1; 
  }
  UpsfSlot* slot = upsf_slots[upsf_handle].get();

  google::protobuf::StringValue req;
  google::protobuf::StringValue reply;
  req.set_value(std::string(upsf_service_gateway->name.str));

  /* call upsf client instance */
  if (!slot->client->DeleteV1(req, reply)) {
    return -1;
  }

  return 0;
}

/**
 * ReadV1 (List)
 */
int upsf_list_service_gateways(upsf_handle_t upsf_handle, upsf_service_gateway_t* elems, size_t n_elems) {

  /* target buffer */
  if (!elems) {
      return -1;
  }

  /* get r lock on upsf slots */
  std::shared_lock rlock(upsf_slots_mutex);

  /* get upsf_slot for given handle */
  if (upsf_slots.find(upsf_handle) == upsf_slots.end()) {
    return -1; 
  }
  UpsfSlot* slot = upsf_slots[upsf_handle].get();

  std::vector<wt474_messages::v1::ServiceGateway> service_gateways;

  /* call upsf client instance */
  if (!slot->client->ReadV1(service_gateways)) {
    return -1;
  }

  /* clear buffer */
  memset(elems, 0, sizeof(upsf_service_gateway_t) * n_elems);

  int max_elems = std::min(service_gateways.size(), size_t(n_elems));
  for (int i = 0; i < max_elems; i++) {
    /* map cpp-object to c-struct */
    upsf::UpsfMapping::map(service_gateways[i], elems[i]);
    VLOG(1) << "libupsf: " << __PRETTY_FUNCTION__ << " reply[" << i << "]=" << upsf::ServiceGatewayStream(service_gateways[i]) << std::endl;
  }

  return service_gateways.size();
}

char* upsf_dump_service_gateway(char* str, size_t size, upsf_service_gateway_t* upsf_service_gateway)
{
  /* sanity check */
  if (!upsf_service_gateway || !str || size == 0) {
    return nullptr;
  }

  /* map service_gateway */
  wt474_messages::v1::ServiceGateway service_gateway;
  upsf::UpsfMapping::map(*upsf_service_gateway, service_gateway);

  std::stringstream sstr;
  sstr << upsf::ServiceGatewayStream(service_gateway);

  strncpy(str, sstr.str().c_str(), std::min(size, sstr.str().size()));

  return str;
}

/******************************************************************
 * ServiceGatewayUserPlane
 ******************************************************************/

/**
 * CreateV1
 */
upsf_service_gateway_user_plane_t* upsf_create_service_gateway_user_plane(upsf_handle_t upsf_handle, upsf_service_gateway_user_plane_t* service_gateway_user_plane)
{
  /* target buffer */
  if (!service_gateway_user_plane) {
      return nullptr;
  }

  /* get r lock on upsf slots */
  std::shared_lock rlock(upsf_slots_mutex);

  /* get upsf_slot for given handle */
  if (upsf_slots.find(upsf_handle) == upsf_slots.end()) {
    return nullptr; 
  }
  UpsfSlot* slot = upsf_slots[upsf_handle].get();

  wt474_messages::v1::ServiceGatewayUserPlane request;
  wt474_messages::v1::ServiceGatewayUserPlane reply;

  upsf::UpsfMapping::map(*service_gateway_user_plane, request);
  VLOG(1) << "libupsf: " << __PRETTY_FUNCTION__ << " request=" << upsf::ServiceGatewayUserPlaneStream(request) << std::endl;

  /* call upsf client instance */
  if (!slot->client->CreateV1(request, reply)) {
    return nullptr;
  }

  upsf::UpsfMapping::map(reply, *service_gateway_user_plane);
  VLOG(1) << "libupsf: " << __PRETTY_FUNCTION__ << " reply=" << upsf::ServiceGatewayUserPlaneStream(reply) << std::endl;

  return service_gateway_user_plane;
}

/**
 * UpdateV1
 */
upsf_service_gateway_user_plane_t* upsf_update_service_gateway_user_plane(upsf_handle_t upsf_handle, upsf_service_gateway_user_plane_t* service_gateway_user_plane)
{
  /* target buffer */
  if (!service_gateway_user_plane) {
      return nullptr;
  }

  /* get r lock on upsf slots */
  std::shared_lock rlock(upsf_slots_mutex);

  /* get upsf_slot for given handle */
  if (upsf_slots.find(upsf_handle) == upsf_slots.end()) {
    return nullptr; 
  }
  UpsfSlot* slot = upsf_slots[upsf_handle].get();

  wt474_upsf_service::v1::UpdateReq::UpdateOptions update_options;
  wt474_messages::v1::ServiceGatewayUserPlane request;
  wt474_messages::v1::ServiceGatewayUserPlane reply;

  upsf::UpsfMapping::map(*service_gateway_user_plane, request);
  VLOG(1) << "libupsf: " << __PRETTY_FUNCTION__ << " request=" << upsf::ServiceGatewayUserPlaneStream(request) << std::endl;

  /* call upsf client instance */
  if (!slot->client->UpdateV1(request, reply, update_options)) {
    return nullptr;
  }

  upsf::UpsfMapping::map(reply, *service_gateway_user_plane);
  VLOG(1) << "libupsf: " << __PRETTY_FUNCTION__ << " reply=" << upsf::ServiceGatewayUserPlaneStream(reply) << std::endl;

  return service_gateway_user_plane;
}

/**
 * ReadV1
 */
upsf_service_gateway_user_plane_t* upsf_get_service_gateway_user_plane(upsf_handle_t upsf_handle, upsf_service_gateway_user_plane_t *upsf_service_gateway_user_plane)
{
  /* target buffer */
  if (!upsf_service_gateway_user_plane) {
      return nullptr;
  }

  /* get r lock on upsf slots */
  std::shared_lock rlock(upsf_slots_mutex);

  /* get upsf_slot for given handle */
  if (upsf_slots.find(upsf_handle) == upsf_slots.end()) {
    return nullptr;
  }
  UpsfSlot* slot = upsf_slots[upsf_handle].get();

  wt474_messages::v1::ServiceGatewayUserPlane reply;
  VLOG(1) << "libupsf: " << __PRETTY_FUNCTION__ << " request=" << upsf_service_gateway_user_plane->name.str << std::endl;

  /* call upsf client instance */
  if (!slot->client->ReadV1(std::string(upsf_service_gateway_user_plane->name.str), reply)) {
    return nullptr;
  }

  /* empty name: not found */
  if (reply.name().empty()) {
    return nullptr;
  }

  /* map cpp-object to c-struct */
  upsf::UpsfMapping::map(reply, *upsf_service_gateway_user_plane);
  VLOG(1) << "libupsf: " << __PRETTY_FUNCTION__ << " reply=" << upsf::ServiceGatewayUserPlaneStream(reply) << std::endl;

  return upsf_service_gateway_user_plane;
}

/**
 * DeleteV1
 */
int upsf_delete_service_gateway_user_plane(upsf_handle_t upsf_handle, upsf_service_gateway_user_plane_t *upsf_service_gateway_user_plane)
{
  /* target buffer */
  if (!upsf_service_gateway_user_plane) {
      return -1;
  }

  /* get r lock on upsf slots */
  std::shared_lock rlock(upsf_slots_mutex);

  /* get upsf_slot for given handle */
  if (upsf_slots.find(upsf_handle) == upsf_slots.end()) {
    return -1; 
  }
  UpsfSlot* slot = upsf_slots[upsf_handle].get();

  google::protobuf::StringValue req;
  google::protobuf::StringValue reply;
  req.set_value(std::string(upsf_service_gateway_user_plane->name.str));

  /* call upsf client instance */
  if (!slot->client->DeleteV1(req, reply)) {
    return -1;
  }

  return 0;
}

/**
 * ReadV1 (List)
 */
int upsf_list_service_gateway_user_planes(upsf_handle_t upsf_handle, upsf_service_gateway_user_plane_t* elems, size_t n_elems) {

  /* target buffer */
  if (!elems) {
      return -1;
  }

  /* get r lock on upsf slots */
  std::shared_lock rlock(upsf_slots_mutex);

  /* get upsf_slot for given handle */
  if (upsf_slots.find(upsf_handle) == upsf_slots.end()) {
    return -1; 
  }
  UpsfSlot* slot = upsf_slots[upsf_handle].get();

  std::vector<wt474_messages::v1::ServiceGatewayUserPlane> service_gateway_user_planes;

  /* call upsf client instance */
  if (!slot->client->ReadV1(service_gateway_user_planes)) {
    return -1;
  }

  /* clear buffer */
  memset(elems, 0, sizeof(upsf_service_gateway_user_plane_t) * n_elems);

  int max_elems = std::min(service_gateway_user_planes.size(), size_t(n_elems));
  for (int i = 0; i < max_elems; i++) {
    /* map cpp-object to c-struct */
    upsf::UpsfMapping::map(service_gateway_user_planes[i], elems[i]);
    VLOG(1) << "libupsf: " << __PRETTY_FUNCTION__ << " reply[" << i << "]=" << upsf::ServiceGatewayUserPlaneStream(service_gateway_user_planes[i]) << std::endl;
  }

  return service_gateway_user_planes.size();
}

char* upsf_dump_service_gateway_user_plane(char* str, size_t size, upsf_service_gateway_user_plane_t* upsf_service_gateway_user_plane)
{
  /* sanity check */
  if (!upsf_service_gateway_user_plane || !str || size == 0) {
    return nullptr;
  }

  /* map service_gateway_user_plane */
  wt474_messages::v1::ServiceGatewayUserPlane service_gateway_user_plane;
  upsf::UpsfMapping::map(*upsf_service_gateway_user_plane, service_gateway_user_plane);

  std::stringstream sstr;
  sstr << upsf::ServiceGatewayUserPlaneStream(service_gateway_user_plane);

  strncpy(str, sstr.str().c_str(), std::min(size, sstr.str().size()));

  return str;
}

/******************************************************************
 * TrafficSteeringFunction
 ******************************************************************/

/**
 * CreateV1
 */
upsf_traffic_steering_function_t* upsf_create_traffic_steering_function(upsf_handle_t upsf_handle, upsf_traffic_steering_function_t* traffic_steering_function)
{
  /* target buffer */
  if (!traffic_steering_function) {
      return nullptr;
  }

  /* get r lock on upsf slots */
  std::shared_lock rlock(upsf_slots_mutex);

  /* get upsf_slot for given handle */
  if (upsf_slots.find(upsf_handle) == upsf_slots.end()) {
    return nullptr; 
  }
  UpsfSlot* slot = upsf_slots[upsf_handle].get();

  wt474_messages::v1::TrafficSteeringFunction request;
  wt474_messages::v1::TrafficSteeringFunction reply;

  upsf::UpsfMapping::map(*traffic_steering_function, request);
  VLOG(1) << "libupsf: " << __PRETTY_FUNCTION__ << " request=" << upsf::TrafficSteeringFunctionStream(request) << std::endl;

  /* call upsf client instance */
  if (!slot->client->CreateV1(request, reply)) {
    return nullptr;
  }

  upsf::UpsfMapping::map(reply, *traffic_steering_function);
  VLOG(1) << "libupsf: " << __PRETTY_FUNCTION__ << " reply=" << upsf::TrafficSteeringFunctionStream(reply) << std::endl;

  return traffic_steering_function;
}

/**
 * UpdateV1
 */
upsf_traffic_steering_function_t* upsf_update_traffic_steering_function(upsf_handle_t upsf_handle, upsf_traffic_steering_function_t* traffic_steering_function)
{
  /* target buffer */
  if (!traffic_steering_function) {
      return nullptr;
  }

  /* get r lock on upsf slots */
  std::shared_lock rlock(upsf_slots_mutex);

  /* get upsf_slot for given handle */
  if (upsf_slots.find(upsf_handle) == upsf_slots.end()) {
    return nullptr; 
  }
  UpsfSlot* slot = upsf_slots[upsf_handle].get();

  wt474_upsf_service::v1::UpdateReq::UpdateOptions update_options;
  wt474_messages::v1::TrafficSteeringFunction request;
  wt474_messages::v1::TrafficSteeringFunction reply;

  upsf::UpsfMapping::map(*traffic_steering_function, request);
  VLOG(1) << "libupsf: " << __PRETTY_FUNCTION__ << " request=" << upsf::TrafficSteeringFunctionStream(request) << std::endl;

  /* call upsf client instance */
  if (!slot->client->UpdateV1(request, reply, update_options)) {
    return nullptr;
  }

  upsf::UpsfMapping::map(reply, *traffic_steering_function);
  VLOG(1) << "libupsf: " << __PRETTY_FUNCTION__ << " reply=" << upsf::TrafficSteeringFunctionStream(reply) << std::endl;

  return traffic_steering_function;
}

/**
 * ReadV1
 */
upsf_traffic_steering_function_t* upsf_get_traffic_steering_function(upsf_handle_t upsf_handle, upsf_traffic_steering_function_t *upsf_traffic_steering_function)
{
  /* target buffer */
  if (!upsf_traffic_steering_function) {
      return nullptr;
  }

  /* get r lock on upsf slots */
  std::shared_lock rlock(upsf_slots_mutex);

  /* get upsf_slot for given handle */
  if (upsf_slots.find(upsf_handle) == upsf_slots.end()) {
    return nullptr;
  }
  UpsfSlot* slot = upsf_slots[upsf_handle].get();

  wt474_messages::v1::TrafficSteeringFunction reply;
  VLOG(1) << "libupsf: " << __PRETTY_FUNCTION__ << " request=" << upsf_traffic_steering_function->name.str << std::endl;

  /* call upsf client instance */
  if (!slot->client->ReadV1(std::string(upsf_traffic_steering_function->name.str), reply)) {
    return nullptr;
  }

  /* empty name: not found */
  if (reply.name().empty()) {
    return nullptr;
  }

  /* map cpp-object to c-struct */
  upsf::UpsfMapping::map(reply, *upsf_traffic_steering_function);
  VLOG(1) << "libupsf: " << __PRETTY_FUNCTION__ << " reply=" << upsf::TrafficSteeringFunctionStream(reply) << std::endl;

  return upsf_traffic_steering_function;
}

/**
 * DeleteV1
 */
int upsf_delete_traffic_steering_function(upsf_handle_t upsf_handle, upsf_traffic_steering_function_t *upsf_traffic_steering_function)
{
  /* target buffer */
  if (!upsf_traffic_steering_function) {
      return -1;
  }

  /* get r lock on upsf slots */
  std::shared_lock rlock(upsf_slots_mutex);

  /* get upsf_slot for given handle */
  if (upsf_slots.find(upsf_handle) == upsf_slots.end()) {
    return -1; 
  }
  UpsfSlot* slot = upsf_slots[upsf_handle].get();

  google::protobuf::StringValue req;
  google::protobuf::StringValue reply;
  req.set_value(std::string(upsf_traffic_steering_function->name.str));

  /* call upsf client instance */
  if (!slot->client->DeleteV1(req, reply)) {
    return -1;
  }

  return 0;
}

/**
 * ReadV1 (List)
 */
int upsf_list_traffic_steering_functions(upsf_handle_t upsf_handle, upsf_traffic_steering_function_t* elems, size_t n_elems) {

  /* target buffer */
  if (!elems) {
      return -1;
  }

  /* get r lock on upsf slots */
  std::shared_lock rlock(upsf_slots_mutex);

  /* get upsf_slot for given handle */
  if (upsf_slots.find(upsf_handle) == upsf_slots.end()) {
    return -1; 
  }
  UpsfSlot* slot = upsf_slots[upsf_handle].get();

  std::vector<wt474_messages::v1::TrafficSteeringFunction> traffic_steering_functions;

  /* call upsf client instance */
  if (!slot->client->ReadV1(traffic_steering_functions)) {
    return -1;
  }

  /* clear buffer */
  memset(elems, 0, sizeof(upsf_traffic_steering_function_t) * n_elems);

  int max_elems = std::min(traffic_steering_functions.size(), size_t(n_elems));
  for (int i = 0; i < max_elems; i++) {
    /* map cpp-object to c-struct */
    upsf::UpsfMapping::map(traffic_steering_functions[i], elems[i]);
    VLOG(1) << "libupsf: " << __PRETTY_FUNCTION__ << " reply[" << i << "]=" << upsf::TrafficSteeringFunctionStream(traffic_steering_functions[i]) << std::endl;
  }

  return traffic_steering_functions.size();
}

char* upsf_dump_traffic_steering_function(char* str, size_t size, upsf_traffic_steering_function_t* upsf_traffic_steering_function)
{
  /* sanity check */
  if (!upsf_traffic_steering_function || !str || size == 0) {
    return nullptr;
  }

  /* map traffic_steering_function */
  wt474_messages::v1::TrafficSteeringFunction traffic_steering_function;
  upsf::UpsfMapping::map(*upsf_traffic_steering_function, traffic_steering_function);

  std::stringstream sstr;
  sstr << upsf::TrafficSteeringFunctionStream(traffic_steering_function);

  strncpy(str, sstr.str().c_str(), std::min(size, sstr.str().size()));

  return str;
}

/******************************************************************
 * NetworkConnection
 ******************************************************************/

/**
 * CreateV1
 */
upsf_network_connection_t* upsf_create_network_connection(upsf_handle_t upsf_handle, upsf_network_connection_t* network_connection)
{
  /* target buffer */
  if (!network_connection) {
      return nullptr;
  }

  /* get r lock on upsf slots */
  std::shared_lock rlock(upsf_slots_mutex);

  /* get upsf_slot for given handle */
  if (upsf_slots.find(upsf_handle) == upsf_slots.end()) {
    return nullptr; 
  }
  UpsfSlot* slot = upsf_slots[upsf_handle].get();

  wt474_messages::v1::NetworkConnection request;
  wt474_messages::v1::NetworkConnection reply;

  upsf::UpsfMapping::map(*network_connection, request);
  VLOG(1) << "libupsf: " << __PRETTY_FUNCTION__ << " request=" << upsf::NetworkConnectionStream(request) << std::endl;

  /* call upsf client instance */
  if (!slot->client->CreateV1(request, reply)) {
    return nullptr;
  }

  upsf::UpsfMapping::map(reply, *network_connection);
  VLOG(1) << "libupsf: " << __PRETTY_FUNCTION__ << " reply=" << upsf::NetworkConnectionStream(reply) << std::endl;

  return network_connection;
}

/**
 * UpdateV1
 */
upsf_network_connection_t* upsf_update_network_connection(upsf_handle_t upsf_handle, upsf_network_connection_t* network_connection)
{
  /* target buffer */
  if (!network_connection) {
      return nullptr;
  }

  /* get r lock on upsf slots */
  std::shared_lock rlock(upsf_slots_mutex);

  /* get upsf_slot for given handle */
  if (upsf_slots.find(upsf_handle) == upsf_slots.end()) {
    return nullptr; 
  }
  UpsfSlot* slot = upsf_slots[upsf_handle].get();

  wt474_upsf_service::v1::UpdateReq::UpdateOptions update_options;
  wt474_messages::v1::NetworkConnection request;
  wt474_messages::v1::NetworkConnection reply;

  upsf::UpsfMapping::map(*network_connection, request);
  VLOG(1) << "libupsf: " << __PRETTY_FUNCTION__ << " request=" << upsf::NetworkConnectionStream(request) << std::endl;

  /* call upsf client instance */
  if (!slot->client->UpdateV1(request, reply, update_options)) {
    return nullptr;
  }

  upsf::UpsfMapping::map(reply, *network_connection);
  VLOG(1) << "libupsf: " << __PRETTY_FUNCTION__ << " reply=" << upsf::NetworkConnectionStream(reply) << std::endl;

  return network_connection;
}

/**
 * ReadV1
 */
upsf_network_connection_t* upsf_get_network_connection(upsf_handle_t upsf_handle, upsf_network_connection_t *upsf_network_connection)
{
  /* target buffer */
  if (!upsf_network_connection) {
      return nullptr;
  }

  /* get r lock on upsf slots */
  std::shared_lock rlock(upsf_slots_mutex);

  /* get upsf_slot for given handle */
  if (upsf_slots.find(upsf_handle) == upsf_slots.end()) {
    return nullptr;
  }
  UpsfSlot* slot = upsf_slots[upsf_handle].get();

  wt474_messages::v1::NetworkConnection reply;
  VLOG(1) << "libupsf: " << __PRETTY_FUNCTION__ << " request=" << upsf_network_connection->name.str << std::endl;

  /* call upsf client instance */
  if (!slot->client->ReadV1(std::string(upsf_network_connection->name.str), reply)) {
    return nullptr;
  }

  /* empty name: not found */
  if (reply.name().empty()) {
    return nullptr;
  }

  /* map cpp-object to c-struct */
  upsf::UpsfMapping::map(reply, *upsf_network_connection);
  VLOG(1) << "libupsf: " << __PRETTY_FUNCTION__ << " reply=" << upsf::NetworkConnectionStream(reply) << std::endl;

  return upsf_network_connection;
}

/**
 * DeleteV1
 */
int upsf_delete_network_connection(upsf_handle_t upsf_handle, upsf_network_connection_t *upsf_network_connection)
{
  /* target buffer */
  if (!upsf_network_connection) {
      return -1;
  }

  /* get r lock on upsf slots */
  std::shared_lock rlock(upsf_slots_mutex);

  /* get upsf_slot for given handle */
  if (upsf_slots.find(upsf_handle) == upsf_slots.end()) {
    return -1; 
  }
  UpsfSlot* slot = upsf_slots[upsf_handle].get();

  google::protobuf::StringValue req;
  google::protobuf::StringValue reply;
  req.set_value(std::string(upsf_network_connection->name.str));

  /* call upsf client instance */
  if (!slot->client->DeleteV1(req, reply)) {
    return -1;
  }

  return 0;
}

/**
 * ReadV1 (List)
 */
int upsf_list_network_connections(upsf_handle_t upsf_handle, upsf_network_connection_t* elems, size_t n_elems) {

  /* target buffer */
  if (!elems) {
      return -1;
  }

  /* get r lock on upsf slots */
  std::shared_lock rlock(upsf_slots_mutex);

  /* get upsf_slot for given handle */
  if (upsf_slots.find(upsf_handle) == upsf_slots.end()) {
    return -1; 
  }
  UpsfSlot* slot = upsf_slots[upsf_handle].get();

  std::vector<wt474_messages::v1::NetworkConnection> network_connections;

  /* call upsf client instance */
  if (!slot->client->ReadV1(network_connections)) {
    return -1;
  }

  /* clear buffer */
  memset(elems, 0, sizeof(upsf_network_connection_t) * n_elems);

  int max_elems = std::min(network_connections.size(), size_t(n_elems));
  for (int i = 0; i < max_elems; i++) {
    /* map cpp-object to c-struct */
    upsf::UpsfMapping::map(network_connections[i], elems[i]);
    VLOG(1) << "libupsf: " << __PRETTY_FUNCTION__ << " reply[" << i << "]=" << upsf::NetworkConnectionStream(network_connections[i]) << std::endl;
  }

  return network_connections.size();
}

char* upsf_dump_network_connection(char* str, size_t size, upsf_network_connection_t* upsf_network_connection)
{
  /* sanity check */
  if (!upsf_network_connection || !str || size == 0) {
    return nullptr;
  }

  /* map network_connection */
  wt474_messages::v1::NetworkConnection network_connection;
  upsf::UpsfMapping::map(*upsf_network_connection, network_connection);

  std::stringstream sstr;
  sstr << upsf::NetworkConnectionStream(network_connection);

  strncpy(str, sstr.str().c_str(), std::min(size, sstr.str().size()));

  return str;
}

/******************************************************************
 * Shard
 ******************************************************************/

/**
 * CreateV1
 */
upsf_shard_t* upsf_create_shard(upsf_handle_t upsf_handle, upsf_shard_t* shard)
{
  /* target buffer */
  if (!shard) {
      return nullptr;
  }

  /* get r lock on upsf slots */
  std::shared_lock rlock(upsf_slots_mutex);

  /* get upsf_slot for given handle */
  if (upsf_slots.find(upsf_handle) == upsf_slots.end()) {
    return nullptr; 
  }
  UpsfSlot* slot = upsf_slots[upsf_handle].get();

  wt474_messages::v1::Shard request;
  wt474_messages::v1::Shard reply;

  upsf::UpsfMapping::map(*shard, request);
  VLOG(1) << "libupsf: " << __PRETTY_FUNCTION__ << " request=" << upsf::ShardStream(request) << std::endl;

  /* call upsf client instance */
  if (!slot->client->CreateV1(request, reply)) {
    return nullptr;
  }

  upsf::UpsfMapping::map(reply, *shard);
  VLOG(1) << "libupsf: " << __PRETTY_FUNCTION__ << " reply=" << upsf::ShardStream(reply) << std::endl;

  return shard;
}

/**
 * UpdateV1
 */
upsf_shard_t* upsf_update_shard(upsf_handle_t upsf_handle, upsf_shard_t* shard)
{
  /* target buffer */
  if (!shard) {
      return nullptr;
  }

  /* get r lock on upsf slots */
  std::shared_lock rlock(upsf_slots_mutex);

  /* get upsf_slot for given handle */
  if (upsf_slots.find(upsf_handle) == upsf_slots.end()) {
    return nullptr; 
  }
  UpsfSlot* slot = upsf_slots[upsf_handle].get();

  wt474_upsf_service::v1::UpdateReq::UpdateOptions update_options;
  wt474_messages::v1::Shard request;
  wt474_messages::v1::Shard reply;

  upsf::UpsfMapping::map(*shard, request);
  VLOG(1) << "libupsf: " << __PRETTY_FUNCTION__ << " request=" << upsf::ShardStream(request) << std::endl;

  /* call upsf client instance */
  if (!slot->client->UpdateV1(request, reply, update_options)) {
    return nullptr;
  }

  upsf::UpsfMapping::map(reply, *shard);
  VLOG(1) << "libupsf: " << __PRETTY_FUNCTION__ << " reply=" << upsf::ShardStream(reply) << std::endl;

  return shard;
}

/**
 * ReadV1
 */
upsf_shard_t* upsf_get_shard(upsf_handle_t upsf_handle, upsf_shard_t *upsf_shard)
{
  /* target buffer */
  if (!upsf_shard) {
      return nullptr;
  }

  /* get r lock on upsf slots */
  std::shared_lock rlock(upsf_slots_mutex);

  /* get upsf_slot for given handle */
  if (upsf_slots.find(upsf_handle) == upsf_slots.end()) {
    return nullptr;
  }
  UpsfSlot* slot = upsf_slots[upsf_handle].get();

  wt474_messages::v1::Shard reply;
  VLOG(1) << "libupsf: " << __PRETTY_FUNCTION__ << " request=" << upsf_shard->name.str << std::endl;

  /* call upsf client instance */
  if (!slot->client->ReadV1(std::string(upsf_shard->name.str), reply)) {
    return nullptr;
  }

  /* empty name: not found */
  if (reply.name().empty()) {
    return nullptr;
  }

  /* map cpp-object to c-struct */
  upsf::UpsfMapping::map(reply, *upsf_shard);
  VLOG(1) << "libupsf: " << __PRETTY_FUNCTION__ << " reply=" << upsf::ShardStream(reply) << std::endl;

  return upsf_shard;
}

/**
 * DeleteV1
 */
int upsf_delete_shard(upsf_handle_t upsf_handle, upsf_shard_t *upsf_shard)
{
  /* target buffer */
  if (!upsf_shard) {
      return -1;
  }

  /* get r lock on upsf slots */
  std::shared_lock rlock(upsf_slots_mutex);

  /* get upsf_slot for given handle */
  if (upsf_slots.find(upsf_handle) == upsf_slots.end()) {
    return -1; 
  }
  UpsfSlot* slot = upsf_slots[upsf_handle].get();

  google::protobuf::StringValue req;
  google::protobuf::StringValue reply;
  req.set_value(std::string(upsf_shard->name.str));

  /* call upsf client instance */
  if (!slot->client->DeleteV1(req, reply)) {
    return -1;
  }

  return 0;
}

/**
 * ReadV1 (List)
 */
int upsf_list_shards(upsf_handle_t upsf_handle, upsf_shard_t* elems, size_t n_elems) {

  /* target buffer */
  if (!elems) {
      return -1;
  }

  /* get r lock on upsf slots */
  std::shared_lock rlock(upsf_slots_mutex);

  /* get upsf_slot for given handle */
  if (upsf_slots.find(upsf_handle) == upsf_slots.end()) {
    return -1; 
  }
  UpsfSlot* slot = upsf_slots[upsf_handle].get();

  std::vector<wt474_messages::v1::Shard> shards;

  /* call upsf client instance */
  if (!slot->client->ReadV1(shards)) {
    return -1;
  }

  /* clear buffer */
  memset(elems, 0, sizeof(upsf_shard_t) * n_elems);

  int max_elems = std::min(shards.size(), size_t(n_elems));
  for (int i = 0; i < max_elems; i++) {
    /* map cpp-object to c-struct */
    upsf::UpsfMapping::map(shards[i], elems[i]);
    VLOG(1) << "libupsf: " << __PRETTY_FUNCTION__ << " reply[" << i << "]=" << upsf::ShardStream(shards[i]) << std::endl;
  }

  return shards.size();
}

char* upsf_dump_shard(char* str, size_t size, upsf_shard_t* upsf_shard)
{
  /* sanity check */
  if (!upsf_shard || !str || size == 0) {
    return nullptr;
  }

  /* map shard */
  wt474_messages::v1::Shard shard;
  upsf::UpsfMapping::map(*upsf_shard, shard);

  std::stringstream sstr;
  sstr << upsf::ShardStream(shard);

  strncpy(str, sstr.str().c_str(), std::min(size, sstr.str().size()));

  return str;
}

/******************************************************************
 * SessionContext
 ******************************************************************/

/**
 * CreateV1
 */
upsf_session_context_t* upsf_create_session_context(upsf_handle_t upsf_handle, upsf_session_context_t* session_context)
{
  /* target buffer */
  if (!session_context) {
      return nullptr;
  }

  /* get r lock on upsf slots */
  std::shared_lock rlock(upsf_slots_mutex);

  /* get upsf_slot for given handle */
  if (upsf_slots.find(upsf_handle) == upsf_slots.end()) {
    return nullptr; 
  }
  UpsfSlot* slot = upsf_slots[upsf_handle].get();

  wt474_messages::v1::SessionContext request;
  wt474_messages::v1::SessionContext reply;

  upsf::UpsfMapping::map(*session_context, request);
  VLOG(1) << "libupsf: " << __PRETTY_FUNCTION__ << " request=" << upsf::SessionContextStream(request) << std::endl;

  /* call upsf client instance */
  if (!slot->client->CreateV1(request, reply)) {
    return nullptr;
  }

  upsf::UpsfMapping::map(reply, *session_context);
  VLOG(1) << "libupsf: " << __PRETTY_FUNCTION__ << " reply=" << upsf::SessionContextStream(reply) << std::endl;

  return session_context;
}

/**
 * UpdateV1
 */
upsf_session_context_t* upsf_update_session_context(upsf_handle_t upsf_handle, upsf_session_context_t* session_context)
{
  /* target buffer */
  if (!session_context) {
      return nullptr;
  }

  /* get r lock on upsf slots */
  std::shared_lock rlock(upsf_slots_mutex);

  /* get upsf_slot for given handle */
  if (upsf_slots.find(upsf_handle) == upsf_slots.end()) {
    return nullptr; 
  }
  UpsfSlot* slot = upsf_slots[upsf_handle].get();

  wt474_upsf_service::v1::UpdateReq::UpdateOptions update_options;
  wt474_messages::v1::SessionContext request;
  wt474_messages::v1::SessionContext reply;

  upsf::UpsfMapping::map(*session_context, request);
  VLOG(1) << "libupsf: " << __PRETTY_FUNCTION__ << " request=" << upsf::SessionContextStream(request) << std::endl;

  /* call upsf client instance */
  if (!slot->client->UpdateV1(request, reply, update_options)) {
    return nullptr;
  }

  upsf::UpsfMapping::map(reply, *session_context);
  VLOG(1) << "libupsf: " << __PRETTY_FUNCTION__ << " reply=" << upsf::SessionContextStream(reply) << std::endl;

  return session_context;
}

/**
 * ReadV1
 */
upsf_session_context_t* upsf_get_session_context(upsf_handle_t upsf_handle, upsf_session_context_t *upsf_session_context)
{
  /* target buffer */
  if (!upsf_session_context) {
      return nullptr;
  }

  /* get r lock on upsf slots */
  std::shared_lock rlock(upsf_slots_mutex);

  /* get upsf_slot for given handle */
  if (upsf_slots.find(upsf_handle) == upsf_slots.end()) {
    return nullptr;
  }
  UpsfSlot* slot = upsf_slots[upsf_handle].get();

  wt474_messages::v1::SessionContext reply;
  VLOG(1) << "libupsf: " << __PRETTY_FUNCTION__ << " request=" << upsf_session_context->name.str << std::endl;

  /* call upsf client instance */
  if (!slot->client->ReadV1(std::string(upsf_session_context->name.str), reply)) {
    return nullptr;
  }

  /* empty name: not found */
  if (reply.name().empty()) {
    return nullptr;
  }

  /* map cpp-object to c-struct */
  upsf::UpsfMapping::map(reply, *upsf_session_context);
  VLOG(1) << "libupsf: " << __PRETTY_FUNCTION__ << " reply=" << upsf::SessionContextStream(reply) << std::endl;

  return upsf_session_context;
}

/**
 * DeleteV1
 */
int upsf_delete_session_context(upsf_handle_t upsf_handle, upsf_session_context_t *upsf_session_context)
{
  /* target buffer */
  if (!upsf_session_context) {
      return -1;
  }

  /* get r lock on upsf slots */
  std::shared_lock rlock(upsf_slots_mutex);

  /* get upsf_slot for given handle */
  if (upsf_slots.find(upsf_handle) == upsf_slots.end()) {
    return -1; 
  }
  UpsfSlot* slot = upsf_slots[upsf_handle].get();

  google::protobuf::StringValue req;
  google::protobuf::StringValue reply;
  req.set_value(std::string(upsf_session_context->name.str));

  /* call upsf client instance */
  if (!slot->client->DeleteV1(req, reply)) {
    return -1;
  }

  return 0;
}

/**
 * ReadV1 (List)
 */
int upsf_list_session_contexts(upsf_handle_t upsf_handle, upsf_session_context_t* elems, size_t n_elems) {

  /* target buffer */
  if (!elems) {
      return -1;
  }

  /* get r lock on upsf slots */
  std::shared_lock rlock(upsf_slots_mutex);

  /* get upsf_slot for given handle */
  if (upsf_slots.find(upsf_handle) == upsf_slots.end()) {
    return -1; 
  }
  UpsfSlot* slot = upsf_slots[upsf_handle].get();

  std::vector<wt474_messages::v1::SessionContext> session_contexts;

  /* call upsf client instance */
  if (!slot->client->ReadV1(session_contexts)) {
    return -1;
  }

  /* clear buffer */
  memset(elems, 0, sizeof(upsf_session_context_t) * n_elems);

  int max_elems = std::min(session_contexts.size(), size_t(n_elems));
  for (int i = 0; i < max_elems; i++) {
    /* map cpp-object to c-struct */
    upsf::UpsfMapping::map(session_contexts[i], elems[i]);
    VLOG(1) << "libupsf: " << __PRETTY_FUNCTION__ << " reply[" << i << "]=" << upsf::SessionContextStream(session_contexts[i]) << std::endl;
  }

  return session_contexts.size();
}

char* upsf_dump_session_context(char* str, size_t size, upsf_session_context_t* upsf_session_context)
{
  /* sanity check */
  if (!upsf_session_context || !str || size == 0) {
    return nullptr;
  }

  /* map session_context */
  wt474_messages::v1::SessionContext session_context;
  upsf::UpsfMapping::map(*upsf_session_context, session_context);

  std::stringstream sstr;
  sstr << upsf::SessionContextStream(session_context);

  strncpy(str, sstr.str().c_str(), std::min(size, sstr.str().size()));

  return str;
}

/******************************************************************
 * Lookup
 ******************************************************************/

/**
 * LookupV1
 */
upsf_session_context_t* upsf_lookup(upsf_handle_t upsf_handle, upsf_session_context_t *upsf_session_context)
{
  /* target buffer */
  if (!upsf_session_context) {
      return nullptr;
  }

  /* get r lock on upsf slots */
  std::shared_lock rlock(upsf_slots_mutex);

  /* get upsf_slot for given handle */
  if (upsf_slots.find(upsf_handle) == upsf_slots.end()) {
    return nullptr;
  }
  UpsfSlot* slot = upsf_slots[upsf_handle].get();

  wt474_messages::v1::SessionContext::Spec request;
  wt474_messages::v1::SessionContext reply;

  upsf::UpsfMapping::map(upsf_session_context->spec, request);

  VLOG(1) << "libupsf: " << __PRETTY_FUNCTION__ << " request=" << upsf::SessionContextSpecStream(request) << std::endl;
  /* call upsf client instance */
  if (!slot->client->LookupV1(request, reply)) {
    return nullptr;
  }
  VLOG(1) << "libupsf: " << __PRETTY_FUNCTION__ << " reply=" << upsf::SessionContextStream(reply) << std::endl;

  /* empty name: not found */
  if (reply.name().empty()) {
    return nullptr;
  }

  /* map cpp-object to c-struct */
  upsf::UpsfMapping::map(reply, *upsf_session_context);

  return upsf_session_context;
}

/******************************************************************
 * Subscribe
 ******************************************************************/

int upsf_subscribe(
    upsf_handle_t upsf_handle,
    void* userdata,
    upsf_shard_cb_t shard_cb,
    upsf_session_context_cb_t session_context_cb,
    upsf_network_connection_cb_t network_connection_cb,
    upsf_service_gateway_user_plane_cb_t service_gateway_user_plane_cb,
    upsf_traffic_steering_function_cb_t traffic_steering_function_cb,
    upsf_service_gateway_cb_t service_gateway_cb) {

  UpsfSlot* slot = nullptr;

  {
    /* get r lock on upsf slots */
    std::shared_lock rlock(upsf_slots_mutex);

    /* get upsf_slot for given handle */
    if (upsf_slots.find(upsf_handle) == upsf_slots.end()) {
      return -1;
    }
    slot = upsf_slots[upsf_handle].get();
  }

  UpsfSubscriberWrapper subscriber(
                            userdata,
                            shard_cb,
                            session_context_cb,
                            network_connection_cb,
                            service_gateway_user_plane_cb,
                            traffic_steering_function_cb,
                            service_gateway_cb); 

  if (slot->client->ReadV1(subscriber) == false) {
    return -1;
  }

  return 0;
}
