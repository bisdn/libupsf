#include "upsf.h"
#include "upsf.hpp"

#include <stdlib.h>
#include <string>
#include <shared_mutex>
#include <thread>
#include <algorithm>

class UpsfSlot final {
  public:
    UpsfSlot(const std::string &upsf_addr)
      : client(std::unique_ptr<upsf::UpsfClient>(
          new upsf::UpsfClient(
            grpc::CreateChannel(
              upsf_addr,
              grpc::InsecureChannelCredentials())))) {}

    std::unique_ptr<upsf::UpsfClient> client;
    mutable std::shared_mutex mutex;
};

class UpsfSubscriberWrapper : public upsf::UpsfSubscriber {
  public:
    UpsfSubscriberWrapper(void* userdata = nullptr,
                          upsf_shard_cb_t shard_cb = nullptr,
                          upsf_session_context_cb_t session_context_cb = nullptr,
                          upsf_network_connection_cb_t network_connection_cb = nullptr,
                          upsf_service_gateway_user_plane_cb_t service_gateway_user_plane_cb = nullptr,
                          upsf_traffic_steering_function_cb_t traffic_steering_function_cb = nullptr,
                          upsf_service_gateway_cb_t service_gateway_cb = nullptr) 
      : userdata(userdata),
        shard_cb(shard_cb),
        session_context_cb(session_context_cb),
        network_connection_cb(network_connection_cb),
        service_gateway_user_plane_cb(service_gateway_user_plane_cb),
        traffic_steering_function_cb(traffic_steering_function_cb),
        service_gateway_cb(service_gateway_cb) {}

  public:
    virtual bool update(const bbf::sss::Shard &shard) { 
      if (!shard_cb) {
        return true;
      }
      upsf_shard_t upsf_shard;

      /* init destination buffer */
      memset(&upsf_shard, 0, sizeof(upsf_shard_t));
    
      /* copy elements */
      strncpy(upsf_shard.id, shard.id().c_str(), sizeof(upsf_shard.id) - 1); 
      strncpy(upsf_shard.name, shard.metadata().name().c_str(), sizeof(upsf_shard.name) - 1); 
      upsf_shard.item_status = shard.metadata().item_status();
      upsf_shard.max_session_count = shard.spec().max_session_count();
      strncpy(upsf_shard.desired_service_gateway_user_plane, shard.spec().desired_service_gateway_user_plane().c_str(), sizeof(upsf_shard.desired_service_gateway_user_plane) - 1); 
      strncpy(upsf_shard.required_qos, shard.spec().required_qos().c_str(), sizeof(upsf_shard.required_qos) - 1); 
      strncpy(upsf_shard.current_service_gateway_user_plane, shard.status().current_service_gateway_user_plane().c_str(), sizeof(upsf_shard.current_service_gateway_user_plane) - 1); 
    
      upsf_shard.desired_network_connections_size = std::min(shard.spec().desired_network_connection_size(), UPSF_MAX_NUM_DESIRED_NETWORK_CONNECTIONS);
      for (int i = 0; i < upsf_shard.desired_network_connections_size; i++) {
        strncpy(upsf_shard.desired_network_connections[i].str, shard.spec().desired_network_connection(i).c_str(), sizeof(upsf_shard.desired_network_connections[i].str) - 1); 
      }
    
      upsf_shard.tsf_network_connections_size = std::min(shard.status().tsf_network_connection_size(), UPSF_MAX_NUM_TSF_NETWORK_CONNECTIONS);
      int i = 0;
      for (auto it : shard.status().tsf_network_connection()) {
        strncpy(upsf_shard.tsf_network_connections[i].key, it.first.c_str(), sizeof(upsf_shard.tsf_network_connections[i].key) - 1); 
        strncpy(upsf_shard.tsf_network_connections[i].value, it.second.c_str(), sizeof(upsf_shard.tsf_network_connections[i].value) - 1); 
        i++;
      }
    
      upsf_shard.ip_prefixes_size = std::min(shard.status().ip_prefixes_size(), UPSF_MAX_NUM_IP_PREFIXES);
      for (int i = 0; i < upsf_shard.ip_prefixes_size; i++) {
        strncpy(upsf_shard.ip_prefixes[i].str, shard.status().ip_prefixes(i).c_str(), sizeof(upsf_shard.ip_prefixes[i].str) - 1); 
      }

      return (*shard_cb)(&upsf_shard, userdata);
    };
  
    virtual bool update(const bbf::sss::SessionContext &session_context) {
      if (!session_context_cb) {
        return true;
      }
      upsf_session_context_t upsf_session_context;

      /* clear buffer */
      memset(&upsf_session_context, 0, sizeof(upsf_session_context_t));
    
      /* copy elements */
      strncpy(upsf_session_context.id, session_context.id().c_str(), sizeof(upsf_session_context.id) - 1);
      strncpy(upsf_session_context.name, session_context.metadata().name().c_str(), sizeof(upsf_session_context.name) - 1);
      upsf_session_context.item_status = session_context.metadata().item_status();
      strncpy(upsf_session_context.tsf, session_context.spec().traffic_steering_function().c_str(), sizeof(upsf_session_context.tsf) - 1);
      strncpy(upsf_session_context.desired_shard, session_context.spec().desired_shard().c_str(), sizeof(upsf_session_context.desired_shard) - 1);
      strncpy(upsf_session_context.current_shard, session_context.status().current_shard().c_str(), sizeof(upsf_session_context.current_shard) - 1);
      for (int i = 0; i < std::min(session_context.spec().required_service_group_size(), UPSF_MAX_NUM_REQUIRED_SERVICE_GROUPS); i++) {
        strncpy(upsf_session_context.required_service_groups[i].str, session_context.spec().required_service_group(i).c_str(), sizeof(upsf_session_context.required_service_groups[i].str) - 1);
      }
      upsf_session_context.required_service_groups_size = std::min(session_context.spec().required_service_group_size(), UPSF_MAX_NUM_REQUIRED_SERVICE_GROUPS);
      strncpy(upsf_session_context.required_qos, session_context.spec().required_qos().c_str(), sizeof(upsf_session_context.required_qos) - 1);
      upsf_session_context.context_type = session_context_type_t(session_context.spec().context_type());
      strncpy(upsf_session_context.mac_address, session_context.spec().session_filter().mac_address().c_str(), sizeof(upsf_session_context.mac_address) - 1);
      upsf_session_context.s_tag = session_context.spec().session_filter().svlan();
      upsf_session_context.c_tag = session_context.spec().session_filter().cvlan();
      strncpy(upsf_session_context.circuit_id, session_context.spec().session_filter().circuit_id().c_str(), sizeof(upsf_session_context.circuit_id) - 1);
      strncpy(upsf_session_context.remote_id, session_context.spec().session_filter().remote_id().c_str(), sizeof(upsf_session_context.remote_id) - 1);
    
      return (*session_context_cb)(&upsf_session_context, userdata);
    };
  
    virtual bool update(const bbf::sss::NetworkConnection &network_connection) {
      if (!network_connection_cb) {
        return true;
      }
      upsf_network_connection_t upsf_network_connection;
      // TODO: fill in network_connection struct
      return (*network_connection_cb)(&upsf_network_connection, userdata);
    };
  
    virtual bool update(const bbf::sss::ServiceGatewayUserPlane &service_gateway_user_plane) {
      if (!service_gateway_user_plane_cb) {
        return true;
      }
      upsf_service_gateway_user_plane_t upsf_service_gateway_user_plane;

      /* clear buffer */
      memset(&upsf_service_gateway_user_plane, 0, sizeof(upsf_service_gateway_user_plane_t));
    
      /* copy ServiceGatewayUserPlane id to destination buffer */
      strncpy(upsf_service_gateway_user_plane.id, service_gateway_user_plane.id().c_str(), sizeof(upsf_service_gateway_user_plane.id) - 1); 
      strncpy(upsf_service_gateway_user_plane.name, service_gateway_user_plane.metadata().name().c_str(), sizeof(upsf_service_gateway_user_plane.name) - 1); 
      upsf_service_gateway_user_plane.item_status = service_gateway_user_plane.metadata().item_status();
      strncpy(upsf_service_gateway_user_plane.service_gateway_id, service_gateway_user_plane.service_gateway_id().c_str(), sizeof(upsf_service_gateway_user_plane.service_gateway_id) - 1); 
      upsf_service_gateway_user_plane.max_session_count = service_gateway_user_plane.spec().max_session_count();
    
      upsf_service_gateway_user_plane.supported_service_groups_size = std::min(service_gateway_user_plane.spec().supported_service_group_size(), UPSF_MAX_NUM_SUPPORTED_SERVICE_GROUPS);
      for (int i = 0; i < upsf_service_gateway_user_plane.supported_service_groups_size; i++) {
        strncpy(upsf_service_gateway_user_plane.supported_service_groups[i].str, service_gateway_user_plane.spec().supported_service_group(i).c_str(), sizeof(upsf_service_gateway_user_plane.supported_service_groups[i].str) - 1); 
      }
    
      upsf_service_gateway_user_plane.maintenance = service_gateway_user_plane.spec().maintenence();
      upsf_service_gateway_user_plane.allocated_session_count = service_gateway_user_plane.status().allocated_session_count();
    
      upsf_service_gateway_user_plane.network_connections_size = std::min(service_gateway_user_plane.status().network_connection_size(), UPSF_MAX_NUM_NETWORK_CONNECTIONS);
      for (int i = 0; i < upsf_service_gateway_user_plane.network_connections_size; i++) {
        upsf_service_gateway_user_plane.network_connections[i] = service_gateway_user_plane.status().network_connection(i);
      }
    
      return (*service_gateway_user_plane_cb)(&upsf_service_gateway_user_plane, userdata);
    };
  
    virtual bool update(const bbf::sss::TrafficSteeringFunction &traffic_steering_function) {
      if (!traffic_steering_function_cb) {
        return true;
      }
      upsf_traffic_steering_function_t upsf_traffic_steering_function;
      // TODO: fill in traffic_steering_function struct
      return (*traffic_steering_function_cb)(&upsf_traffic_steering_function, userdata);
    };
  
    virtual bool update(const bbf::sss::ServiceGateway &service_gateway) {
      if (!service_gateway_cb) {
        return true;
      }
      upsf_service_gateway_t upsf_service_gateway;

      /* clear buffer */
      memset(&upsf_service_gateway, 0, sizeof(upsf_service_gateway_t));

      strncpy(upsf_service_gateway.id, service_gateway.id().c_str(), sizeof(upsf_service_gateway.id) - 1); 
      strncpy(upsf_service_gateway.name, service_gateway.metadata().name().c_str(), sizeof(upsf_service_gateway.name) - 1); 
      upsf_service_gateway.item_status = service_gateway.metadata().item_status();
      return (*service_gateway_cb)(&upsf_service_gateway, userdata);
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

const char* upsf_item_status_names[] = {
    "unknown",
    "active",
    "updating",
    "deleting",
    "deleted",
    "inactive",
    "invalid",
};

/**
 * map upsf item status to associated name
 */
const char* upsf_item_status_to_name(int item_status)
{
    item_status = item_status < UPSF_ITEM_STATUS_MAX ? item_status : UPSF_ITEM_STATUS_MAX;
    return upsf_item_status_names[item_status];
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
 * Shard
 ******************************************************************/

/**
 * rpc UpdateShard (UpdateShardReq) returns (ShardResp) {}
 */
upsf_shard_t* upsf_update_shard(upsf_handle_t upsf_handle, upsf_shard_t* shard) {

  bbf::sss::ShardResp resp;

  /* get r lock on upsf slots */
  std::shared_lock rlock(upsf_slots_mutex);

  /* get upsf_slot for given handle */
  if (upsf_slots.find(upsf_handle) == upsf_slots.end()) {
    return nullptr; 
  }
  UpsfSlot* slot = upsf_slots[upsf_handle].get();

  /* destination buffer available? */
  if (shard == nullptr) {
    return nullptr;
  }

  /* get rw lock on upsf slot */
  std::unique_lock rwlock(slot->mutex);

  std::vector<std::string> desired_network_connection;
  std::map<std::string, std::string> tsf_network_connection;
  std::vector<std::string> ip_prefixes;

  /* call upsf client instance */
  if (!slot->client->UpdateShard(
    std::string(shard->name),
    bbf::sss::ItemStatus(shard->item_status),
    shard->max_session_count,
    std::string(shard->desired_service_gateway_user_plane),
    desired_network_connection,
    std::string(shard->required_qos),
    std::string(shard->current_service_gateway_user_plane),
    tsf_network_connection,
    ip_prefixes,
    resp)) {
    return nullptr;
  }

  /* check result */
  if (resp.result() > 0) {
    return nullptr;
  }

  /* init destination buffer */
  memset(shard, 0, sizeof(upsf_shard_t));

  /* copy Shard id to destination buffer */
  strncpy(shard->id, resp.shard().id().c_str(), sizeof(shard->id) - 1); 
  strncpy(shard->name, resp.shard().metadata().name().c_str(), sizeof(shard->name) - 1); 
  shard->item_status = resp.shard().metadata().item_status();
  shard->max_session_count = resp.shard().spec().max_session_count();
  strncpy(shard->desired_service_gateway_user_plane, resp.shard().spec().desired_service_gateway_user_plane().c_str(), sizeof(shard->desired_service_gateway_user_plane) - 1); 
  strncpy(shard->required_qos, resp.shard().spec().required_qos().c_str(), sizeof(shard->required_qos) - 1); 
  strncpy(shard->current_service_gateway_user_plane, resp.shard().status().current_service_gateway_user_plane().c_str(), sizeof(shard->current_service_gateway_user_plane) - 1); 

  shard->desired_network_connections_size = std::min(resp.shard().spec().desired_network_connection_size(), UPSF_MAX_NUM_DESIRED_NETWORK_CONNECTIONS);
  for (int i = 0; i < shard->desired_network_connections_size; i++) {
    strncpy(shard->desired_network_connections[i].str, resp.shard().spec().desired_network_connection(i).c_str(), sizeof(shard->desired_network_connections[i].str) - 1); 
  }

  shard->tsf_network_connections_size = std::min(resp.shard().status().tsf_network_connection_size(), UPSF_MAX_NUM_TSF_NETWORK_CONNECTIONS);
  int i = 0;
  for (auto it : resp.shard().status().tsf_network_connection()) {
    strncpy(shard->tsf_network_connections[i].key, it.first.c_str(), sizeof(shard->tsf_network_connections[i].key) - 1); 
    strncpy(shard->tsf_network_connections[i].value, it.second.c_str(), sizeof(shard->tsf_network_connections[i].value) - 1); 
    i++;
  }

  shard->ip_prefixes_size = std::min(resp.shard().status().ip_prefixes_size(), UPSF_MAX_NUM_IP_PREFIXES);
  for (int i = 0; i < shard->ip_prefixes_size; i++) {
    strncpy(shard->ip_prefixes[i].str, resp.shard().status().ip_prefixes(i).c_str(), sizeof(shard->ip_prefixes[i].str) - 1); 
  }

  return shard;
}

/**
 * rpc GetShard (GetShardReq) returns (ShardResp) {}
 */
upsf_shard_t* upsf_get_shard(upsf_handle_t upsf_handle, upsf_shard_t *shard) {

  bbf::sss::ShardResp resp;

  /* get r lock on upsf slots */
  std::shared_lock rlock(upsf_slots_mutex);

  /* get upsf_slot for given handle */
  if (upsf_slots.find(upsf_handle) == upsf_slots.end()) {
    return nullptr;
  }
  UpsfSlot* slot = upsf_slots[upsf_handle].get();

  /* destination buffer available? */
  if (shard == nullptr) {
    return nullptr;
  }

  /* get rw lock on upsf slot */
  std::unique_lock rwlock(slot->mutex);

  /* call upsf client instance */
  if (!slot->client->GetShard(std::string(shard->id), resp)) {
    return nullptr;
  }

  /* check result */
  if (resp.result() > 0) {
    return nullptr;
  }

  /* init destination buffer */
  memset(shard, 0, sizeof(upsf_shard_t));

  /* copy Shard id to destination buffer */
  strncpy(shard->id, resp.shard().id().c_str(), sizeof(shard->id) - 1); 
  strncpy(shard->name, resp.shard().metadata().name().c_str(), sizeof(shard->name) - 1); 
  shard->item_status = resp.shard().metadata().item_status();
  shard->max_session_count = resp.shard().spec().max_session_count();
  strncpy(shard->desired_service_gateway_user_plane, resp.shard().spec().desired_service_gateway_user_plane().c_str(), sizeof(shard->desired_service_gateway_user_plane) - 1); 
  strncpy(shard->required_qos, resp.shard().spec().required_qos().c_str(), sizeof(shard->required_qos) - 1); 
  strncpy(shard->current_service_gateway_user_plane, resp.shard().status().current_service_gateway_user_plane().c_str(), sizeof(shard->current_service_gateway_user_plane) - 1); 

  shard->desired_network_connections_size = std::min(resp.shard().spec().desired_network_connection_size(), UPSF_MAX_NUM_DESIRED_NETWORK_CONNECTIONS);
  for (int i = 0; i < shard->desired_network_connections_size; i++) {
    strncpy(shard->desired_network_connections[i].str, resp.shard().spec().desired_network_connection(i).c_str(), sizeof(shard->desired_network_connections[i].str) - 1); 
  }

  shard->tsf_network_connections_size = std::min(resp.shard().status().tsf_network_connection_size(), UPSF_MAX_NUM_TSF_NETWORK_CONNECTIONS);
  int i = 0;
  for (auto it : resp.shard().status().tsf_network_connection()) {
    strncpy(shard->tsf_network_connections[i].key, it.first.c_str(), sizeof(shard->tsf_network_connections[i].key) - 1); 
    strncpy(shard->tsf_network_connections[i].value, it.second.c_str(), sizeof(shard->tsf_network_connections[i].value) - 1); 
    i++;
  }

  shard->ip_prefixes_size = std::min(resp.shard().status().ip_prefixes_size(), UPSF_MAX_NUM_IP_PREFIXES);
  for (int i = 0; i < shard->ip_prefixes_size; i++) {
    strncpy(shard->ip_prefixes[i].str, resp.shard().status().ip_prefixes(i).c_str(), sizeof(shard->ip_prefixes[i].str) - 1); 
  }

  return shard;
}


/**
 * rpc DeleteShard (DeleteShardReq) returns (ShardResp) {}
 */
int upsf_delete_shard(upsf_handle_t upsf_handle, upsf_shard_t *shard) {

  bbf::sss::ShardResp resp;

  /* get r lock on upsf slots */
  std::shared_lock rlock(upsf_slots_mutex);

  /* get upsf_slot for given handle */
  if (upsf_slots.find(upsf_handle) == upsf_slots.end()) {
    return -1; 
  }
  UpsfSlot* slot = upsf_slots[upsf_handle].get();

  /* destination buffer available? */
  if (shard == nullptr) {
    return -1;
  }

  /* get rw lock on upsf slot */
  std::unique_lock rwlock(slot->mutex);

  /* call upsf client instance */
  if (!slot->client->DeleteShard(std::string(shard->id), resp)) {
    return -1;
  }

  /* check result */
  if (resp.result() > 0) {
    return -1;
  }

  return 0;
}


/**
 * rpc ListShards (ListShardReq) returns (ListShardResp) {}
 */
int upsf_list_shards(upsf_handle_t upsf_handle, upsf_shard_t* elems, size_t n_elems) {

  bbf::sss::ListShardResp resp;
  std::vector<std::string> id_vector;

  /* get r lock on upsf slots */
  std::shared_lock rlock(upsf_slots_mutex);

  /* get upsf_slot for given handle */
  if (upsf_slots.find(upsf_handle) == upsf_slots.end()) {
    return -1; 
  }
  UpsfSlot* slot = upsf_slots[upsf_handle].get();

  /* get rw lock on upsf slot */
  std::unique_lock rwlock(slot->mutex);

  /* call upsf client instance */
  if (!slot->client->ListShards(id_vector, resp)) {
    return -1;
  }

  /* check result */
  if (resp.result() > 0) {
    return -1;
  }

  /* clear buffer */
  memset(elems, 0, sizeof(upsf_shard_t) * n_elems);

  /* copy result */
  for (int i = 0; i < std::min(resp.shard_size(), int(n_elems)); i++) {
    strncpy(elems[i].id, resp.shard(i).id().c_str(), sizeof(elems[i].id) - 1); 
    strncpy(elems[i].name, resp.shard(i).metadata().name().c_str(), sizeof(elems[i].name) - 1); 
    elems[i].item_status = resp.shard(i).metadata().item_status();
    elems[i].max_session_count = resp.shard(i).spec().max_session_count();
    strncpy(elems[i].desired_service_gateway_user_plane, resp.shard(i).spec().desired_service_gateway_user_plane().c_str(), sizeof(elems[i].desired_service_gateway_user_plane) - 1); 
    strncpy(elems[i].required_qos, resp.shard(i).spec().required_qos().c_str(), sizeof(elems[i].required_qos) - 1); 
    strncpy(elems[i].current_service_gateway_user_plane, resp.shard(i).status().current_service_gateway_user_plane().c_str(), sizeof(elems[i].current_service_gateway_user_plane) - 1); 
  
    elems[i].desired_network_connections_size = std::min(resp.shard(i).spec().desired_network_connection_size(), UPSF_MAX_NUM_DESIRED_NETWORK_CONNECTIONS);
    for (int i = 0; i < elems[i].desired_network_connections_size; i++) {
      strncpy(elems[i].desired_network_connections[i].str, resp.shard(i).spec().desired_network_connection(i).c_str(), sizeof(elems[i].desired_network_connections[i].str) - 1); 
    }
  
    elems[i].tsf_network_connections_size = std::min(resp.shard(i).status().tsf_network_connection_size(), UPSF_MAX_NUM_TSF_NETWORK_CONNECTIONS);
    int j = 0;
    for (auto it : resp.shard(i).status().tsf_network_connection()) {
      strncpy(elems[i].tsf_network_connections[j].key, it.first.c_str(), sizeof(elems[i].tsf_network_connections[j].key) - 1); 
      strncpy(elems[i].tsf_network_connections[j].value, it.second.c_str(), sizeof(elems[i].tsf_network_connections[j].value) - 1); 
      j++;
    }
  
    elems[i].ip_prefixes_size = std::min(resp.shard(i).status().ip_prefixes_size(), UPSF_MAX_NUM_IP_PREFIXES);
    for (int i = 0; i < elems[i].ip_prefixes_size; i++) {
      strncpy(elems[i].ip_prefixes[i].str, resp.shard(i).status().ip_prefixes(i).c_str(), sizeof(elems[i].ip_prefixes[i].str) - 1); 
    }

  }
 
  return std::min(resp.shard_size(), int(n_elems));
}



/******************************************************************
 * ServiceGateway
 ******************************************************************/

/**
 * rpc UpdateServiceGateway (UpdateServiceGatewayReq) returns (ServiceGatewayResp) {}
 */
upsf_service_gateway_t* upsf_update_service_gateway(upsf_handle_t upsf_handle, upsf_service_gateway_t* service_gateway) {

  bbf::sss::ServiceGatewayResp resp;

  /* get r lock on upsf slots */
  std::shared_lock rlock(upsf_slots_mutex);

  /* get upsf_slot for given handle */
  if (upsf_slots.find(upsf_handle) == upsf_slots.end()) {
    return nullptr; 
  }
  UpsfSlot* slot = upsf_slots[upsf_handle].get();

  /* destination buffer available? */
  if (service_gateway == nullptr) {
    return nullptr;
  }

  /* get rw lock on upsf slot */
  std::unique_lock rwlock(slot->mutex);

  /* call upsf client instance */
  if (!slot->client->UpdateServiceGateway(std::string(service_gateway->name), bbf::sss::ItemStatus(service_gateway->item_status), resp)) {
    return nullptr;
  }

  /* check result */
  if (resp.result() > 0) {
    return nullptr;
  }

  /* init destination buffer */
  memset(service_gateway, 0, sizeof(upsf_service_gateway_t));

  /* copy ServiceGateway id to destination buffer */
  strncpy(service_gateway->id, resp.service_gateway().id().c_str(), sizeof(service_gateway->id) - 1); 
  strncpy(service_gateway->name, resp.service_gateway().metadata().name().c_str(), sizeof(service_gateway->name) - 1); 
  service_gateway->item_status = resp.service_gateway().metadata().item_status();

  return service_gateway;
}

/**
 * rpc GetServiceGateway (GetServiceGatewayReq) returns (ServiceGatewayResp) {}
 */
upsf_service_gateway_t* upsf_get_service_gateway(upsf_handle_t upsf_handle, upsf_service_gateway_t *service_gateway) {

  bbf::sss::ServiceGatewayResp resp;

  /* get r lock on upsf slots */
  std::shared_lock rlock(upsf_slots_mutex);

  /* get upsf_slot for given handle */
  if (upsf_slots.find(upsf_handle) == upsf_slots.end()) {
    return nullptr;
  }
  UpsfSlot* slot = upsf_slots[upsf_handle].get();

  /* destination buffer available? */
  if (service_gateway == nullptr) {
    return nullptr;
  }

  /* get rw lock on upsf slot */
  std::unique_lock rwlock(slot->mutex);

  /* call upsf client instance */
  if (!slot->client->GetServiceGateway(std::string(service_gateway->id), resp)) {
    return nullptr;
  }

  /* check result */
  if (resp.result() > 0) {
    return nullptr;
  }

  /* init destination buffer */
  memset(service_gateway, 0, sizeof(upsf_service_gateway_t));

  /* copy ServiceGateway id to destination buffer */
  strncpy(service_gateway->id, resp.service_gateway().id().c_str(), sizeof(service_gateway->id) - 1); 
  strncpy(service_gateway->name, resp.service_gateway().metadata().name().c_str(), sizeof(service_gateway->name) - 1); 
  service_gateway->item_status = resp.service_gateway().metadata().item_status();

  return service_gateway;
}


/**
 * rpc DeleteServiceGateway (DeleteServiceGatewayReq) returns (ServiceGatewayResp) {}
 */
int upsf_delete_service_gateway(upsf_handle_t upsf_handle, upsf_service_gateway_t *service_gateway) {

  bbf::sss::ServiceGatewayResp resp;

  /* get r lock on upsf slots */
  std::shared_lock rlock(upsf_slots_mutex);

  /* get upsf_slot for given handle */
  if (upsf_slots.find(upsf_handle) == upsf_slots.end()) {
    return -1; 
  }
  UpsfSlot* slot = upsf_slots[upsf_handle].get();

  /* destination buffer available? */
  if (service_gateway == nullptr) {
    return -1;
  }

  /* get rw lock on upsf slot */
  std::unique_lock rwlock(slot->mutex);

  /* call upsf client instance */
  if (!slot->client->DeleteServiceGateway(std::string(service_gateway->id), resp)) {
    return -1;
  }

  /* check result */
  if (resp.result() > 0) {
    return -1;
  }

  return 0;
}


/**
 * rpc ListServiceGateways (ListServiceGatewayReq) returns (ListServiceGatewayResp) {}
 */
int upsf_list_service_gateways(upsf_handle_t upsf_handle, upsf_service_gateway_t* elems, size_t n_elems) {

  bbf::sss::ListServiceGatewayResp resp;
  std::vector<std::string> id_vector;

  /* get r lock on upsf slots */
  std::shared_lock rlock(upsf_slots_mutex);

  /* get upsf_slot for given handle */
  if (upsf_slots.find(upsf_handle) == upsf_slots.end()) {
    return -1; 
  }
  UpsfSlot* slot = upsf_slots[upsf_handle].get();

  /* get rw lock on upsf slot */
  std::unique_lock rwlock(slot->mutex);

  /* call upsf client instance */
  if (!slot->client->ListServiceGateways(id_vector, resp)) {
    return -1;
  }

  /* check result */
  if (resp.result() > 0) {
    return -1;
  }

  /* clear buffer */
  memset(elems, 0, sizeof(upsf_service_gateway_t) * n_elems);

  /* copy result */
  for (int i = 0; i < std::min(resp.service_gateway_size(), int(n_elems)); i++) {
    strncpy(elems[i].id, resp.service_gateway(i).id().c_str(), sizeof(elems[i].id) - 1); 
    strncpy(elems[i].name, resp.service_gateway(i).metadata().name().c_str(), sizeof(elems[i].name) - 1); 
    elems[i].item_status = resp.service_gateway(i).metadata().item_status();
  }
 
  return std::min(resp.service_gateway_size(), int(n_elems));
}


/******************************************************************
 * ServiceGatewayUserPlane
 ******************************************************************/

/**
 * rpc UpdateServiceGatewayUserPlane (UpdateServiceGatewayUserPlaneReq) returns (ServiceGatewayUserPlaneResp) {}
 */
upsf_service_gateway_user_plane_t* upsf_update_service_gateway_user_plane(upsf_handle_t upsf_handle, upsf_service_gateway_user_plane_t* service_gateway_user_plane) {

  bbf::sss::ServiceGatewayUserPlaneResp resp;

  /* get r lock on upsf slots */
  std::shared_lock rlock(upsf_slots_mutex);

  /* get upsf_slot for given handle */
  if (upsf_slots.find(upsf_handle) == upsf_slots.end()) {
    return nullptr; 
  }
  UpsfSlot* slot = upsf_slots[upsf_handle].get();

  /* destination buffer available? */
  if (service_gateway_user_plane == nullptr) {
    return nullptr;
  }

  /* get rw lock on upsf slot */
  std::unique_lock rwlock(slot->mutex);

  std::vector<std::string> supported_service_groups;
  for (int i = 0; i < service_gateway_user_plane->supported_service_groups_size; i++) {
    supported_service_groups.emplace(supported_service_groups.end(), service_gateway_user_plane->supported_service_groups[i].str);
  }

  std::vector<int32_t> network_connections;
  for (int i = 0; i < service_gateway_user_plane->network_connections_size; i++) {
    network_connections.emplace(network_connections.end(), service_gateway_user_plane->network_connections[i]);
  }

  /* call upsf client instance */
  if (!slot->client->UpdateServiceGatewayUserPlane(
    std::string(service_gateway_user_plane->name),
    bbf::sss::ItemStatus(service_gateway_user_plane->item_status),
    std::string(service_gateway_user_plane->service_gateway_id),
    service_gateway_user_plane->max_session_count,
    supported_service_groups,
    service_gateway_user_plane->maintenance,
    service_gateway_user_plane->allocated_session_count,
    network_connections,
    resp)) {
    return nullptr;
  }

  /* check result */
  if (resp.result() > 0) {
    return nullptr;
  }

  /* init destination buffer */
  memset(service_gateway_user_plane, 0, sizeof(upsf_service_gateway_user_plane_t));

  /* copy ServiceGatewayUserPlane id to destination buffer */
  strncpy(service_gateway_user_plane->id, resp.service_gateway_user_plane().id().c_str(), sizeof(service_gateway_user_plane->id) - 1); 
  strncpy(service_gateway_user_plane->name, resp.service_gateway_user_plane().metadata().name().c_str(), sizeof(service_gateway_user_plane->name) - 1); 
  service_gateway_user_plane->item_status = resp.service_gateway_user_plane().metadata().item_status();
  strncpy(service_gateway_user_plane->service_gateway_id, resp.service_gateway_user_plane().service_gateway_id().c_str(), sizeof(service_gateway_user_plane->service_gateway_id) - 1); 
  service_gateway_user_plane->max_session_count = resp.service_gateway_user_plane().spec().max_session_count();

  service_gateway_user_plane->supported_service_groups_size = std::min(resp.service_gateway_user_plane().spec().supported_service_group_size(), UPSF_MAX_NUM_SUPPORTED_SERVICE_GROUPS);
  for (int i = 0; i < service_gateway_user_plane->supported_service_groups_size; i++) {
    strncpy(service_gateway_user_plane->supported_service_groups[i].str, resp.service_gateway_user_plane().spec().supported_service_group(i).c_str(), sizeof(service_gateway_user_plane->supported_service_groups[i].str) - 1); 
  }

  service_gateway_user_plane->maintenance = resp.service_gateway_user_plane().spec().maintenence();
  service_gateway_user_plane->allocated_session_count = resp.service_gateway_user_plane().status().allocated_session_count();

  service_gateway_user_plane->network_connections_size = std::min(resp.service_gateway_user_plane().status().network_connection_size(), UPSF_MAX_NUM_NETWORK_CONNECTIONS);
  for (int i = 0; i < service_gateway_user_plane->network_connections_size; i++) {
    service_gateway_user_plane->network_connections[i] = resp.service_gateway_user_plane().status().network_connection(i);
  }

  return service_gateway_user_plane;
}

/**
 * rpc GetServiceGatewayUserPlane (GetServiceGatewayUserPlaneReq) returns (ServiceGatewayUserPlaneResp) {}
 */
upsf_service_gateway_user_plane_t* upsf_get_service_gateway_user_plane(upsf_handle_t upsf_handle, upsf_service_gateway_user_plane_t *service_gateway_user_plane) {

  bbf::sss::ServiceGatewayUserPlaneResp resp;

  /* get r lock on upsf slots */
  std::shared_lock rlock(upsf_slots_mutex);

  /* get upsf_slot for given handle */
  if (upsf_slots.find(upsf_handle) == upsf_slots.end()) {
    return nullptr;
  }
  UpsfSlot* slot = upsf_slots[upsf_handle].get();

  /* destination buffer available? */
  if (service_gateway_user_plane == nullptr) {
    return nullptr;
  }

  /* get rw lock on upsf slot */
  std::unique_lock rwlock(slot->mutex);

  /* call upsf client instance */
  if (!slot->client->GetServiceGatewayUserPlane(std::string(service_gateway_user_plane->id), resp)) {
    return nullptr;
  }

  /* check result */
  if (resp.result() > 0) {
    return nullptr;
  }

  /* init destination buffer */
  memset(service_gateway_user_plane, 0, sizeof(upsf_service_gateway_user_plane_t));

  /* copy ServiceGatewayUserPlane id to destination buffer */
  strncpy(service_gateway_user_plane->id, resp.service_gateway_user_plane().id().c_str(), sizeof(service_gateway_user_plane->id) - 1); 
  strncpy(service_gateway_user_plane->name, resp.service_gateway_user_plane().metadata().name().c_str(), sizeof(service_gateway_user_plane->name) - 1); 
  service_gateway_user_plane->item_status = resp.service_gateway_user_plane().metadata().item_status();
  strncpy(service_gateway_user_plane->service_gateway_id, resp.service_gateway_user_plane().service_gateway_id().c_str(), sizeof(service_gateway_user_plane->service_gateway_id) - 1); 
  service_gateway_user_plane->max_session_count = resp.service_gateway_user_plane().spec().max_session_count();

  service_gateway_user_plane->supported_service_groups_size = std::min(resp.service_gateway_user_plane().spec().supported_service_group_size(), UPSF_MAX_NUM_SUPPORTED_SERVICE_GROUPS);
  for (int i = 0; i < service_gateway_user_plane->supported_service_groups_size; i++) {
    strncpy(service_gateway_user_plane->supported_service_groups[i].str, resp.service_gateway_user_plane().spec().supported_service_group(i).c_str(), sizeof(service_gateway_user_plane->supported_service_groups[i].str) - 1); 
  }

  service_gateway_user_plane->maintenance = resp.service_gateway_user_plane().spec().maintenence();
  service_gateway_user_plane->allocated_session_count = resp.service_gateway_user_plane().status().allocated_session_count();

  service_gateway_user_plane->network_connections_size = std::min(resp.service_gateway_user_plane().status().network_connection_size(), UPSF_MAX_NUM_NETWORK_CONNECTIONS);
  for (int i = 0; i < service_gateway_user_plane->network_connections_size; i++) {
    service_gateway_user_plane->network_connections[i] = resp.service_gateway_user_plane().status().network_connection(i);
  }

  return service_gateway_user_plane;
}


/**
 * rpc DeleteServiceGatewayUserPlane (DeleteServiceGatewayUserPlaneReq) returns (ServiceGatewayUserPlaneResp) {}
 */
int upsf_delete_service_gateway_user_plane(upsf_handle_t upsf_handle, upsf_service_gateway_user_plane_t *service_gateway_user_plane) {

  bbf::sss::ServiceGatewayUserPlaneResp resp;

  /* get r lock on upsf slots */
  std::shared_lock rlock(upsf_slots_mutex);

  /* get upsf_slot for given handle */
  if (upsf_slots.find(upsf_handle) == upsf_slots.end()) {
    return -1; 
  }
  UpsfSlot* slot = upsf_slots[upsf_handle].get();

  /* destination buffer available? */
  if (service_gateway_user_plane == nullptr) {
    return -1;
  }

  /* get rw lock on upsf slot */
  std::unique_lock rwlock(slot->mutex);

  /* call upsf client instance */
  if (!slot->client->DeleteServiceGatewayUserPlane(std::string(service_gateway_user_plane->id), resp)) {
    return -1;
  }

  /* check result */
  if (resp.result() > 0) {
    return -1;
  }

  return 0;
}


/**
 * rpc ListServiceGatewayUserPlanes (ListServiceGatewayUserPlaneReq) returns (ListServiceGatewayUserPlaneResp) {}
 */
int upsf_list_service_gateway_user_planes(upsf_handle_t upsf_handle, upsf_service_gateway_user_plane_t* elems, size_t n_elems) {

  bbf::sss::ListServiceGatewayUserPlaneResp resp;
  std::vector<std::string> id_vector;

  /* get r lock on upsf slots */
  std::shared_lock rlock(upsf_slots_mutex);

  /* get upsf_slot for given handle */
  if (upsf_slots.find(upsf_handle) == upsf_slots.end()) {
    return -1; 
  }
  UpsfSlot* slot = upsf_slots[upsf_handle].get();

  /* get rw lock on upsf slot */
  std::unique_lock rwlock(slot->mutex);

  /* call upsf client instance */
  if (!slot->client->ListServiceGatewayUserPlanes(id_vector, resp)) {
    return -1;
  }

  /* check result */
  if (resp.result() > 0) {
    return -1;
  }

  /* clear buffer */
  memset(elems, 0, sizeof(upsf_service_gateway_user_plane_t) * n_elems);

  /* copy result */
  for (int i = 0; i < std::min(resp.service_gateway_user_plane_size(), int(n_elems)); i++) {
    strncpy(elems[i].id, resp.service_gateway_user_plane(i).id().c_str(), sizeof(elems[i].id) - 1); 
    strncpy(elems[i].name, resp.service_gateway_user_plane(i).metadata().name().c_str(), sizeof(elems[i].name) - 1); 
    elems[i].item_status = resp.service_gateway_user_plane(i).metadata().item_status();

    /* copy ServiceGatewayUserPlane id to destination buffer */
    strncpy(elems[i].id, resp.service_gateway_user_plane(i).id().c_str(), sizeof(elems[i].id) - 1); 
    strncpy(elems[i].name, resp.service_gateway_user_plane(i).metadata().name().c_str(), sizeof(elems[i].name) - 1); 
    strncpy(elems[i].service_gateway_id, resp.service_gateway_user_plane(i).service_gateway_id().c_str(), sizeof(elems[i].service_gateway_id) - 1); 
    elems[i].max_session_count = resp.service_gateway_user_plane(i).spec().max_session_count();
  
    elems[i].supported_service_groups_size = std::min(resp.service_gateway_user_plane(i).spec().supported_service_group_size(), UPSF_MAX_NUM_SUPPORTED_SERVICE_GROUPS);
    for (int j = 0; j < elems[i].supported_service_groups_size; j++) {
      strncpy(elems[i].supported_service_groups[j].str, resp.service_gateway_user_plane(i).spec().supported_service_group(j).c_str(), sizeof(elems[i].supported_service_groups[j].str) - 1); 
    }
  
    elems[i].maintenance = resp.service_gateway_user_plane(i).spec().maintenence();
    elems[i].allocated_session_count = resp.service_gateway_user_plane(i).status().allocated_session_count();
  
    elems[i].network_connections_size = std::min(resp.service_gateway_user_plane(i).status().network_connection_size(), UPSF_MAX_NUM_NETWORK_CONNECTIONS);
    for (int j = 0; j < elems[i].network_connections_size; j++) {
      elems[i].network_connections[j] = resp.service_gateway_user_plane(i).status().network_connection(j);
    }

  }
 
  return std::min(resp.service_gateway_user_plane_size(), int(n_elems));
}


/******************************************************************
 * SessionContext
 ******************************************************************/

/**
 * rpc UpdateSessionContext (UpdateSessionContextReq) returns (SessionContextResp) {}
 */
upsf_session_context_t* upsf_update_session_context(upsf_handle_t upsf_handle, upsf_session_context_t* session_context) {

  bbf::sss::SessionContextResp resp;

  /* get r lock on upsf slots */
  std::shared_lock rlock(upsf_slots_mutex);

  /* get upsf_slot for given handle */
  if (upsf_slots.find(upsf_handle) == upsf_slots.end()) {
    return nullptr;
  }
  UpsfSlot* slot = upsf_slots[upsf_handle].get();

  /* destination buffer available? */
  if (session_context == nullptr) {
    return nullptr;
  }

  /* get rw lock on upsf slot */
  std::unique_lock rwlock(slot->mutex);

  std::vector<std::string> req_svc_grp;
  for (int i = 0; i < session_context->required_service_groups_size; i++) {
    req_svc_grp.emplace(req_svc_grp.end(), session_context->required_service_groups[i].str);
  }

  /* call upsf client instance */
  if (!slot->client->UpdateSessionContext(
    std::string(session_context->name),
    bbf::sss::ItemStatus(session_context->item_status),
    std::string(session_context->tsf),
    std::string(session_context->desired_shard),
    std::string(session_context->current_shard),
    req_svc_grp,
    std::string(session_context->required_qos),
    (bbf::sss::SessionContext_Spec_ContextType)session_context->context_type,
    std::string(session_context->mac_address),
    session_context->s_tag,
    session_context->c_tag,
    std::string(session_context->circuit_id),
    std::string(session_context->remote_id), resp)) {
    return nullptr;
  }

  /* check result */
  if (resp.result() > 0) {
    return nullptr;
  }

  /* clear buffer */
  memset(session_context, 0, sizeof(upsf_session_context_t));

  /* copy elements */
  strncpy(session_context->id, resp.session_context().id().c_str(), sizeof(session_context->id) - 1); 
  strncpy(session_context->name, resp.session_context().metadata().name().c_str(), sizeof(session_context->name) - 1); 
  session_context->item_status = resp.session_context().metadata().item_status();
  strncpy(session_context->tsf, resp.session_context().spec().traffic_steering_function().c_str(), sizeof(session_context->tsf) - 1); 
  strncpy(session_context->desired_shard, resp.session_context().spec().desired_shard().c_str(), sizeof(session_context->desired_shard) - 1); 
  strncpy(session_context->current_shard, resp.session_context().status().current_shard().c_str(), sizeof(session_context->current_shard) - 1); 
  for (int i = 0; i < std::min(resp.session_context().spec().required_service_group_size(), UPSF_MAX_NUM_REQUIRED_SERVICE_GROUPS); i++) {
    strncpy(session_context->required_service_groups[i].str, resp.session_context().spec().required_service_group(i).c_str(), sizeof(session_context->required_service_groups[i].str) - 1); 
  }
  session_context->required_service_groups_size = std::min(resp.session_context().spec().required_service_group_size(), UPSF_MAX_NUM_REQUIRED_SERVICE_GROUPS);
  strncpy(session_context->required_qos, resp.session_context().spec().required_qos().c_str(), sizeof(session_context->required_qos) - 1); 
  session_context->context_type = session_context_type_t(resp.session_context().spec().context_type());
  strncpy(session_context->mac_address, resp.session_context().spec().session_filter().mac_address().c_str(), sizeof(session_context->mac_address) - 1); 
  session_context->s_tag = resp.session_context().spec().session_filter().svlan();
  session_context->c_tag = resp.session_context().spec().session_filter().cvlan();
  strncpy(session_context->circuit_id, resp.session_context().spec().session_filter().circuit_id().c_str(), sizeof(session_context->circuit_id) - 1); 
  strncpy(session_context->remote_id, resp.session_context().spec().session_filter().remote_id().c_str(), sizeof(session_context->remote_id) - 1); 

  return session_context;
}


/**
 * rpc GetSessionContext (GetSessionContextReq) returns (SessionContextResp) {}
 */
upsf_session_context_t* upsf_get_session_context(upsf_handle_t upsf_handle, upsf_session_context_t *session_context) {

  bbf::sss::SessionContextResp resp;

  /* get r lock on upsf slots */
  std::shared_lock rlock(upsf_slots_mutex);

  /* get upsf_slot for given handle */
  if (upsf_slots.find(upsf_handle) == upsf_slots.end()) {
    return nullptr;
  }
  UpsfSlot* slot = upsf_slots[upsf_handle].get();

  /* destination buffer available? */
  if (session_context == nullptr) {
    return nullptr;
  }

  /* get rw lock on upsf slot */
  std::unique_lock rwlock(slot->mutex);

  /* call upsf client instance */
  if (!slot->client->GetSessionContext(std::string(session_context->id), resp)) {
    return nullptr;
  }

  /* check result */
  if (resp.result() > 0) {
    return nullptr;
  }

  /* check result */
  if (resp.result() > 0) {
    return nullptr;
  }

  /* clear buffer */
  memset(session_context, 0, sizeof(upsf_session_context_t));

  /* copy elements */
  strncpy(session_context->id, resp.session_context().id().c_str(), sizeof(session_context->id) - 1); 
  strncpy(session_context->name, resp.session_context().metadata().name().c_str(), sizeof(session_context->name) - 1); 
  session_context->item_status = resp.session_context().metadata().item_status();
  strncpy(session_context->tsf, resp.session_context().spec().traffic_steering_function().c_str(), sizeof(session_context->tsf) - 1); 
  strncpy(session_context->desired_shard, resp.session_context().spec().desired_shard().c_str(), sizeof(session_context->desired_shard) - 1); 
  strncpy(session_context->current_shard, resp.session_context().status().current_shard().c_str(), sizeof(session_context->current_shard) - 1); 
  for (int i = 0; i < std::min(resp.session_context().spec().required_service_group_size(), UPSF_MAX_NUM_REQUIRED_SERVICE_GROUPS); i++) {
    strncpy(session_context->required_service_groups[i].str, resp.session_context().spec().required_service_group(i).c_str(), sizeof(session_context->required_service_groups[i].str) - 1); 
  }
  session_context->required_service_groups_size = std::min(resp.session_context().spec().required_service_group_size(), UPSF_MAX_NUM_REQUIRED_SERVICE_GROUPS);
  strncpy(session_context->required_qos, resp.session_context().spec().required_qos().c_str(), sizeof(session_context->required_qos) - 1); 
  session_context->context_type = session_context_type_t(resp.session_context().spec().context_type());
  strncpy(session_context->mac_address, resp.session_context().spec().session_filter().mac_address().c_str(), sizeof(session_context->mac_address) - 1); 
  session_context->s_tag = resp.session_context().spec().session_filter().svlan();
  session_context->c_tag = resp.session_context().spec().session_filter().cvlan();
  strncpy(session_context->circuit_id, resp.session_context().spec().session_filter().circuit_id().c_str(), sizeof(session_context->circuit_id) - 1); 
  strncpy(session_context->remote_id, resp.session_context().spec().session_filter().remote_id().c_str(), sizeof(session_context->remote_id) - 1); 

  return session_context;
}


/**
 * rpc DeleteSessionContext (DeleteSessionContextReq) returns (SessionContextResp) {}
 */
int upsf_delete_session_context(upsf_handle_t upsf_handle, upsf_session_context_t *session_context) {

  bbf::sss::SessionContextResp resp;

  /* get r lock on upsf slots */
  std::shared_lock rlock(upsf_slots_mutex);

  /* get upsf_slot for given handle */
  if (upsf_slots.find(upsf_handle) == upsf_slots.end()) {
    return -1; 
  }
  UpsfSlot* slot = upsf_slots[upsf_handle].get();

  /* destination buffer available? */
  if (session_context == nullptr) {
    return -1;
  }

  /* get rw lock on upsf slot */
  std::unique_lock rwlock(slot->mutex);

  /* call upsf client instance */
  if (!slot->client->DeleteSessionContext(std::string(session_context->id), resp)) {
    return -1;
  }

  /* check result */
  if (resp.result() > 0) {
    return -1;
  }

  return 0;
}


/**
 * rpc ListSessionContexts (ListSessionContextReq) returns (ListSessionContextResp) {}
 */
int upsf_list_session_contexts(upsf_handle_t upsf_handle, upsf_session_context_t* elems, size_t n_elems) {

  bbf::sss::ListSessionContextResp resp;
  std::vector<std::string> id_vector;

  /* get r lock on upsf slots */
  std::shared_lock rlock(upsf_slots_mutex);

  /* get upsf_slot for given handle */
  if (upsf_slots.find(upsf_handle) == upsf_slots.end()) {
    return -1; 
  }
  UpsfSlot* slot = upsf_slots[upsf_handle].get();

  /* get rw lock on upsf slot */
  std::unique_lock rwlock(slot->mutex);

  /* call upsf client instance */
  if (!slot->client->ListSessionContexts(id_vector, resp)) {
    return -1;
  }

  /* clear buffer */
  memset(elems, 0, sizeof(upsf_session_context_t) * n_elems);

  /* copy result */
  for (int i = 0; i < std::min(resp.session_context_size(), int(n_elems)); i++) {
    strncpy(elems[i].id, resp.session_context(i).id().c_str(), sizeof(elems[i].id) - 1); 
    strncpy(elems[i].name, resp.session_context(i).metadata().name().c_str(), sizeof(elems[i].name) - 1); 
    elems[i].item_status = resp.session_context(i).metadata().item_status();
    strncpy(elems[i].tsf, resp.session_context(i).spec().traffic_steering_function().c_str(), sizeof(elems[i].tsf) - 1); 
    strncpy(elems[i].desired_shard, resp.session_context(i).spec().desired_shard().c_str(), sizeof(elems[i].desired_shard) - 1); 
    strncpy(elems[i].current_shard, resp.session_context(i).status().current_shard().c_str(), sizeof(elems[i].current_shard) - 1); 
    for (int j = 0; j < std::min(resp.session_context(i).spec().required_service_group_size(), UPSF_MAX_NUM_REQUIRED_SERVICE_GROUPS); j++) {
      strncpy(elems[i].required_service_groups[j].str, resp.session_context(i).spec().required_service_group(j).c_str(), sizeof(elems[i].required_service_groups[j].str) - 1); 
    }
    elems[i].required_service_groups_size = std::min(resp.session_context(i).spec().required_service_group_size(), UPSF_MAX_NUM_REQUIRED_SERVICE_GROUPS);
    strncpy(elems[i].required_qos, resp.session_context(i).spec().required_qos().c_str(), sizeof(elems[i].required_qos) - 1); 
    elems[i].context_type = session_context_type_t(resp.session_context(i).spec().context_type());
    strncpy(elems[i].mac_address, resp.session_context(i).spec().session_filter().mac_address().c_str(), sizeof(elems[i].mac_address) - 1); 
    elems[i].s_tag = resp.session_context(i).spec().session_filter().svlan();
    elems[i].c_tag = resp.session_context(i).spec().session_filter().cvlan();
    strncpy(elems[i].circuit_id, resp.session_context(i).spec().session_filter().circuit_id().c_str(), sizeof(elems[i].circuit_id) - 1); 
    strncpy(elems[i].remote_id, resp.session_context(i).spec().session_filter().remote_id().c_str(), sizeof(elems[i].remote_id) - 1); 
  }

  return std::min(resp.session_context_size(), int(n_elems));
}


/******************************************************************
 * Lookup
 ******************************************************************/

/**
 * rpc Lookup (LookupReq) returns (LookupResp) {}
 */
upsf_session_context_t* upsf_lookup(upsf_handle_t upsf_handle, upsf_session_context_t* session_context) {

  bbf::sss::LookupResp resp;

  /* get r lock on upsf slots */
  std::shared_lock rlock(upsf_slots_mutex);

  /* get upsf_slot for given handle */
  if (upsf_slots.find(upsf_handle) == upsf_slots.end()) {
    return nullptr;
  }
  UpsfSlot* slot = upsf_slots[upsf_handle].get();

  /* destination buffer available? */
  if (session_context == nullptr) {
    return nullptr;
  }

  /* get rw lock on upsf slot */
  std::unique_lock rwlock(slot->mutex);

  std::vector<std::string> req_svc_grp;
  for (int i = 0; i < session_context->required_service_groups_size; i++) {
    req_svc_grp.emplace(req_svc_grp.end(), session_context->required_service_groups[i].str);
  }

  /* call upsf client instance */
  if (!slot->client->Lookup(
    std::string(session_context->tsf),
    std::string(session_context->desired_shard),
    req_svc_grp,
    std::string(session_context->required_qos),
    (bbf::sss::SessionContext_Spec_ContextType)session_context->context_type,
    std::string(session_context->mac_address),
    session_context->s_tag,
    session_context->c_tag,
    std::string(session_context->circuit_id),
    std::string(session_context->remote_id), resp)) {
    return nullptr;
  }

  /* check result */
  if (resp.result() > 0) {
    return nullptr;
  }

  /* clear buffer */
  memset(session_context, 0, sizeof(upsf_session_context_t));

  /* copy elements */
  strncpy(session_context->id, resp.session_context().id().c_str(), sizeof(session_context->id) - 1); 
  strncpy(session_context->name, resp.session_context().metadata().name().c_str(), sizeof(session_context->name) - 1); 
  strncpy(session_context->tsf, resp.session_context().spec().traffic_steering_function().c_str(), sizeof(session_context->tsf) - 1); 
  strncpy(session_context->desired_shard, resp.session_context().spec().desired_shard().c_str(), sizeof(session_context->desired_shard) - 1); 
  for (int i = 0; i < std::min(resp.session_context().spec().required_service_group_size(), UPSF_MAX_NUM_REQUIRED_SERVICE_GROUPS); i++) {
    strncpy(session_context->required_service_groups[i].str, resp.session_context().spec().required_service_group(i).c_str(), sizeof(session_context->required_service_groups[i].str) - 1); 
  }
  session_context->required_service_groups_size = std::min(resp.session_context().spec().required_service_group_size(), UPSF_MAX_NUM_REQUIRED_SERVICE_GROUPS);
  strncpy(session_context->required_qos, resp.session_context().spec().required_qos().c_str(), sizeof(session_context->required_qos) - 1); 
  session_context->context_type = session_context_type_t(resp.session_context().spec().context_type());
  strncpy(session_context->mac_address, resp.session_context().spec().session_filter().mac_address().c_str(), sizeof(session_context->mac_address) - 1); 
  session_context->s_tag = resp.session_context().spec().session_filter().svlan();
  session_context->c_tag = resp.session_context().spec().session_filter().cvlan();
  strncpy(session_context->circuit_id, resp.session_context().spec().session_filter().circuit_id().c_str(), sizeof(session_context->circuit_id) - 1); 
  strncpy(session_context->remote_id, resp.session_context().spec().session_filter().remote_id().c_str(), sizeof(session_context->remote_id) - 1); 

  return session_context;
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

  /* get rw lock on upsf slot */
  std::unique_lock rwlock(slot->mutex);

  UpsfSubscriberWrapper subscriber(
                            userdata,
                            shard_cb,
                            session_context_cb,
                            network_connection_cb,
                            service_gateway_user_plane_cb,
                            traffic_steering_function_cb,
                            service_gateway_cb); 

  if (slot->client->Subscribe(subscriber) == false) {
    return -1;
  }

  return 0;
}
