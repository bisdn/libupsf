#ifndef UPSF_H
#define UPSF_H

#include <sys/types.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef int upsf_handle_t;

#define UPSF_MAX_STRING_SIZE 64
#define UPSF_MAX_NUM_REQUIRED_SERVICE_GROUPS 16
#define UPSF_MAX_NUM_NETWORK_CONNECTIONS 16
#define UPSF_MAX_NUM_DESIRED_NETWORK_CONNECTIONS 16
#define UPSF_MAX_NUM_TSF_NETWORK_CONNECTIONS 16
#define UPSF_MAX_NUM_IP_PREFIXES 16
#define UPSF_MAX_NUM_SUPPORTED_SERVICE_GROUPS 16
#define UPSF_MAX_NUM_ENDPOINTS 16

enum upsf_derived_state_t {
    UPSF_DERIVED_STATE_UNKNOWN = 0,
    UPSF_DERIVED_STATE_INACTIVE = 1,
    UPSF_DERIVED_STATE_ACTIVE = 2,
    UPSF_DERIVED_STATE_UPDATING = 3,
    UPSF_DERIVED_STATE_DELETING = 4,
    UPSF_DERIVED_STATE_DELETED = 5,
    UPSF_DERIVED_STATE_MAX,
};

const char* upsf_derived_state_to_name(int derived_state);

enum upsf_item_type_t {
    UPSF_ITEM_TYPE_SERVICE_GATEWAY = 0,
    UPSF_ITEM_TYPE_SERVICE_GATEWAY_USER_PLANE = 1,
    UPSF_ITEM_TYPE_TRAFFIC_STEERING_FUNCTION = 2,
    UPSF_ITEM_TYPE_NETWORK_CONNECTION = 3,
    UPSF_ITEM_TYPE_SHARD = 4,
    UPSF_ITEM_TYPE_SESSION_CONTEXT = 5,
    UPSF_ITEM_TYPE_MAX,
};

const char* upsf_item_type_to_name(int item_type);

enum upsf_maintenance_req_t {
    UPSF_MAINTENANCE_REQ_NONE = 0,
    UPSF_MAINTENANCE_REQ_DRAIN = 1,
    UPSF_MAINTENANCE_REQ_DRAIN_AND_DELETE = 2,
    UPSF_MAINTENANCE_REQ_MAX,
};

const char* upsf_maintenance_req_to_name(int maintenance_req);

enum upsf_mbb_state_t {
    UPSF_MBB_STATE_NON_MBB_MOVE_REQUIRED = 0,
    UPSF_MBB_STATE_USERPLANE_MBB_INITIATION_REQUIRED = 1,
    UPSF_MBB_STATE_UPSTREAM_SWITCHOVER_REQUIRED = 2,
    UPSF_MBB_STATE_DOWNSTREAM_SWITCHOVER_REQUIRED = 3,
    UPSF_MBB_STATE_UPSREAM_FINALIZATION_REQUIRED = 4,
    UPSF_MBB_STATE_COMPLETE = 5,
    UPSF_MBB_STATE_MAX,
    //UPSF_MBB_STATE_FAILURE = 999,
};

enum upsf_nc_spec_type_t {
    UPSF_NC_SPEC_TYPE_SS_PTP = 0,
    UPSF_NC_SPEC_TYPE_SS_MPTP = 1,
    UPSF_NC_SPEC_TYPE_MS_PTP = 2,
    UPSF_NC_SPEC_TYPE_MS_MPTP = 3,
    UPSF_NC_SPEC_TYPE_MAX,
};

/* endpoint types */
enum upsf_ep_type_t {
    UPSF_EP_TYPE_UNSPECIFIED = 0,
    UPSF_EP_TYPE_VTEP = 1,
    UPSF_EP_TYPE_L2VPN = 2,
    UPSF_EP_TYPE_PORT_VLAN = 3,
    UPSF_EP_TYPE_MAX,
};

/* string type */
typedef struct {
    char str[UPSF_MAX_STRING_SIZE];
    size_t len; // excluding terminating '\0'
} upsf_string_t;

/* map type */
typedef struct {
    upsf_string_t key;
    upsf_string_t value;
} upsf_key_value_t;

/* message: metadata */
typedef struct {
    upsf_string_t description;
    /* TODO: timestamps */
    enum upsf_derived_state_t derived_state;
} upsf_metadata_t;

/* message: maintenance */
typedef struct {
    enum upsf_maintenance_req_t maintenance_req;
} upsf_maintenance_t;

/* message: vtep */
typedef struct {
    upsf_string_t ip_address;
    int udp_port;
    int vni;
} upsf_vtep_t;

/* message: port_vlan */
typedef struct {
    upsf_string_t logical_port;
    int svlan;
    int cvlan;
} upsf_port_vlan_t;

/* message: l2vpn */
typedef struct {
    int vpn_id;
} upsf_l2vpn_t;

/* message: network_connection.spec.endpoint */
typedef struct {
    upsf_string_t endpoint_name;
    enum upsf_ep_type_t ep_type;
    union ep_spec_u {
        upsf_vtep_t vtep;
        upsf_l2vpn_t l2vpn;
        upsf_port_vlan_t port_vlan;
    } ep_spec;
} upsf_network_connection_spec_endpoint_t;

/* message: ss_ptp_spec */
typedef struct {
    upsf_network_connection_spec_endpoint_t sgup_endpoint[UPSF_MAX_NUM_ENDPOINTS];
    size_t sgup_endpoint_size;
    upsf_network_connection_spec_endpoint_t tsf_endpoint;
} upsf_network_connection_spec_ss_ptp_spec_t;

/* message: ss_mptp_spec */
typedef struct {
    upsf_network_connection_spec_endpoint_t sgup_endpoint[UPSF_MAX_NUM_ENDPOINTS];
    size_t sgup_endpoint_size;
    upsf_network_connection_spec_endpoint_t tsf_endpoint[UPSF_MAX_NUM_ENDPOINTS];
    size_t tsf_endpoint_size;
} upsf_network_connection_spec_ss_mptp_spec_t;

/* message: ms_ptp_spec */
typedef struct {
    upsf_network_connection_spec_endpoint_t sgup_endpoint;
    upsf_network_connection_spec_endpoint_t tsf_endpoint;
} upsf_network_connection_spec_ms_ptp_spec_t;

/* message: ms_mptp_spec */
typedef struct {
    upsf_network_connection_spec_endpoint_t sgup_endpoint;
    upsf_network_connection_spec_endpoint_t tsf_endpoint[UPSF_MAX_NUM_ENDPOINTS];
    size_t tsf_endpoint_size;
} upsf_network_connection_spec_ms_mptp_spec_t;

/* message: network_connection.spec */
typedef struct {
    int maximum_supported_quality;    
    enum upsf_nc_spec_type_t nc_spec_type;
    union nc_spec_u {
        upsf_network_connection_spec_ss_ptp_spec_t ss_ptp;
        upsf_network_connection_spec_ss_mptp_spec_t ss_mptp;
        upsf_network_connection_spec_ms_ptp_spec_t ms_ptp;
        upsf_network_connection_spec_ms_mptp_spec_t ms_mptp;
    } nc_spec;
} upsf_network_connection_spec_t;

/* message: network_connection.status */
typedef struct {
    upsf_key_value_t nc_active[UPSF_MAX_NUM_ENDPOINTS];
    size_t nc_active_size;
    int allocated_shards;
} upsf_network_connection_status_t;

/* message: network connection */
typedef struct {
    upsf_string_t name;
    upsf_metadata_t metadata;
    upsf_maintenance_t maintenance;
    upsf_network_connection_spec_t spec;
    upsf_network_connection_status_t status;
} upsf_network_connection_t;

/* message: service gateway user plane spec */
typedef struct {
    int max_session_count;
    int max_shards;
    upsf_string_t supported_service_group[UPSF_MAX_NUM_SUPPORTED_SERVICE_GROUPS];
    size_t supported_service_group_size;
    upsf_network_connection_spec_endpoint_t default_endpoint;
} upsf_service_gateway_user_plane_spec_t;

/* message: service gateway userplane status */
typedef struct {
    int allocated_session_count;
    int allocated_shards;
} upsf_service_gateway_user_plane_status_t;

/* message: service gateway */
typedef struct {
    upsf_string_t name;
    upsf_metadata_t metadata;
} upsf_service_gateway_t;

/* message: service gateway user plane */
typedef struct {
    upsf_string_t name;
    upsf_metadata_t metadata;
    upsf_string_t service_gateway_name;
    upsf_maintenance_t maintenance;
    upsf_service_gateway_user_plane_spec_t spec;
    upsf_service_gateway_user_plane_status_t status;
} upsf_service_gateway_user_plane_t;

/* message: traffic_steering_function_spec */
typedef struct {
    upsf_network_connection_spec_endpoint_t default_endpoint;
} upsf_traffic_steering_function_spec_t;

/* message: traffic steering function */
typedef struct {
    upsf_string_t name;
    upsf_metadata_t metadata;
    upsf_traffic_steering_function_spec_t spec;
} upsf_traffic_steering_function_t;

/* message: shard_spec_desired_state */
typedef struct {
    upsf_string_t service_gateway_user_plane;
    upsf_string_t network_connection[UPSF_MAX_NUM_NETWORK_CONNECTIONS];
    size_t network_connection_size;
} upsf_shard_spec_desired_state_t;

/* message: shard_spec */
typedef struct {
    int max_session_count;
    upsf_string_t virtual_mac;
    upsf_shard_spec_desired_state_t desired_state;
    upsf_string_t prefix[UPSF_MAX_NUM_IP_PREFIXES];
    size_t prefix_size;
} upsf_shard_spec_t;

/* message: shard_status_current_state */
typedef struct {
    upsf_string_t service_gateway_user_plane;
    upsf_key_value_t tsf_network_connection[UPSF_MAX_NUM_TSF_NETWORK_CONNECTIONS];
    size_t tsf_network_connection_size;
} upsf_shard_status_current_state_t;

/* message: shard_status */
typedef struct {
    int allocated_session_count;
    int maximum_allocated_quality;
    upsf_shard_status_current_state_t current_state;
} upsf_shard_status_t;

/* message: shard_mbb */
typedef struct {
    enum upsf_mbb_state_t mbb_state;
} upsf_shard_mbb_t;

/* message: shard */
typedef struct {
    upsf_string_t name;
    upsf_metadata_t metadata;
    upsf_shard_spec_t spec;
    upsf_shard_status_t status;
    upsf_shard_mbb_t mbb;
} upsf_shard_t;

/* message: session_filter */
typedef struct {
    upsf_string_t source_mac_address;
    int svlan;
    int cvlan;
} upsf_session_filter_t;

/* message: session_context_spec_desired_state */
typedef struct {
    upsf_string_t shard;
} upsf_session_context_spec_desired_state_t;

/* message: session_context_spec */
typedef struct {
    upsf_string_t traffic_steering_function;
    upsf_string_t required_service_group[UPSF_MAX_NUM_REQUIRED_SERVICE_GROUPS];
    size_t required_service_group_size;
    int required_quality;
    upsf_string_t circuit_id;
    upsf_string_t remote_id;
    upsf_session_filter_t session_filter;
    upsf_session_context_spec_desired_state_t desired_state;
} upsf_session_context_spec_t;

/* message: session_context_status_current_state */
typedef struct {
    upsf_string_t user_plane_shard;
    upsf_string_t tsf_shard;
} upsf_session_context_status_current_state_t;

/* message: session_context_status */
typedef struct {
    upsf_session_context_status_current_state_t current_state;
} upsf_session_context_status_t;

/* message: session_context */
typedef struct {
    upsf_string_t name;
    upsf_metadata_t metadata;
    upsf_session_context_spec_t spec;
    upsf_session_context_status_t status;
} upsf_session_context_t;


/*
 *
 */
typedef int (*upsf_shard_cb_t)(upsf_shard_t *shard, void *userdata);
typedef int (*upsf_session_context_cb_t)(upsf_session_context_t *session_context, void *userdata);
typedef int (*upsf_network_connection_cb_t)(upsf_network_connection_t *network_connection, void *userdata);
typedef int (*upsf_service_gateway_user_plane_cb_t)(upsf_service_gateway_user_plane_t *service_gateway_user_plane, void *userdata);
typedef int (*upsf_traffic_steering_function_cb_t)(upsf_traffic_steering_function_t *traffic_steering_function, void *userdata);
typedef int (*upsf_service_gateway_cb_t)(upsf_service_gateway_t *service_gateway, void *userdata);



upsf_handle_t upsf_open(
    const char* upsf_host,
    const int upsf_port);

int upsf_close(
    upsf_handle_t upsf_handle);

const char* upsf_derived_state_to_name(int derived_state);
const char* upsf_item_type_to_name(int item_type);
const char* upsf_maintenance_req_to_name(int maintenance_req);
const char* upsf_mbb_state_to_name(int mbb_state);

/* service_gateway */
upsf_service_gateway_t* upsf_create_service_gateway(
    upsf_handle_t upsf_handle,
    upsf_service_gateway_t* service_gateway);

upsf_service_gateway_t* upsf_update_service_gateway(
    upsf_handle_t upsf_handle,
    upsf_service_gateway_t* service_gateway);

upsf_service_gateway_t* upsf_get_service_gateway(
    upsf_handle_t upsf_handle,
    upsf_service_gateway_t* service_gateway);

int upsf_delete_service_gateway(
    upsf_handle_t upsf_handle,
    upsf_service_gateway_t* service_gateway);

int upsf_list_service_gateways(
    upsf_handle_t upsf_handle,
    upsf_service_gateway_t* elems, size_t n_elems);

char* upsf_dump_service_gateway(
    char* str, size_t size,
    upsf_service_gateway_t* service_gateway);

/* service_gateway_user_plane */
upsf_service_gateway_user_plane_t* upsf_create_service_gateway_user_plane(
    upsf_handle_t upsf_handle,
    upsf_service_gateway_user_plane_t* service_gateway_user_plane);

upsf_service_gateway_user_plane_t* upsf_update_service_gateway_user_plane(
    upsf_handle_t upsf_handle,
    upsf_service_gateway_user_plane_t* service_gateway_user_plane);

upsf_service_gateway_user_plane_t* upsf_get_service_gateway_user_plane(
    upsf_handle_t upsf_handle,
    upsf_service_gateway_user_plane_t* service_gateway_user_plane);

int upsf_delete_service_gateway_user_plane(
    upsf_handle_t upsf_handle,
    upsf_service_gateway_user_plane_t* service_gateway_user_plane);

int upsf_list_service_gateway_user_planes(
    upsf_handle_t upsf_handle,
    upsf_service_gateway_user_plane_t* elems, size_t n_elems);

char* upsf_dump_service_gateway_user_plane(
    char* str, size_t size,
    upsf_service_gateway_user_plane_t* service_gateway_user_plane);

/* traffic_steering_function */
upsf_traffic_steering_function_t* upsf_create_traffic_steering_function(
    upsf_handle_t upsf_handle,
    upsf_traffic_steering_function_t* traffic_steering_function);

upsf_traffic_steering_function_t* upsf_update_traffic_steering_function(
    upsf_handle_t upsf_handle,
    upsf_traffic_steering_function_t* traffic_steering_function);

upsf_traffic_steering_function_t* upsf_get_traffic_steering_function(
    upsf_handle_t upsf_handle,
    upsf_traffic_steering_function_t* traffic_steering_function);

int upsf_delete_traffic_steering_function(
    upsf_handle_t upsf_handle,
    upsf_traffic_steering_function_t* traffic_steering_function);

int upsf_list_traffic_steering_functions(
    upsf_handle_t upsf_handle,
    upsf_traffic_steering_function_t* elems, size_t n_elems);

char* upsf_dump_traffic_steering_function(
    char* str, size_t size,
    upsf_traffic_steering_function_t* traffic_steering_function);

/* network_connection */
upsf_network_connection_t* upsf_create_network_connection(
    upsf_handle_t upsf_handle,
    upsf_network_connection_t* network_connection);

upsf_network_connection_t* upsf_update_network_connection(
    upsf_handle_t upsf_handle,
    upsf_network_connection_t* network_connection);

upsf_network_connection_t* upsf_get_network_connection(
    upsf_handle_t upsf_handle,
    upsf_network_connection_t* network_connection);

int upsf_delete_network_connection(
    upsf_handle_t upsf_handle,
    upsf_network_connection_t* network_connection);

int upsf_list_network_connections(
    upsf_handle_t upsf_handle,
    upsf_network_connection_t* elems, size_t n_elems);

char* upsf_dump_network_connection(
    char* str, size_t size,
    upsf_network_connection_t* network_connection);

/* shard */
upsf_shard_t* upsf_create_shard(
    upsf_handle_t upsf_handle,
    upsf_shard_t* shard);

upsf_shard_t* upsf_update_shard(
    upsf_handle_t upsf_handle,
    upsf_shard_t* shard);

upsf_shard_t* upsf_get_shard(
    upsf_handle_t upsf_handle,
    upsf_shard_t* shard);

int upsf_delete_shard(
    upsf_handle_t upsf_handle,
    upsf_shard_t* shard);

int upsf_list_shards(
    upsf_handle_t upsf_handle,
    upsf_shard_t* elems, size_t n_elems);

char* upsf_dump_shard(
    char* str, size_t size,
    upsf_shard_t* shard);

/* session_context */
upsf_session_context_t* upsf_create_session_context(
    upsf_handle_t upsf_handle,
    upsf_session_context_t* session_context);

upsf_session_context_t* upsf_update_session_context(
    upsf_handle_t upsf_handle,
    upsf_session_context_t* session_context);

upsf_session_context_t* upsf_get_session_context(
    upsf_handle_t upsf_handle,
    upsf_session_context_t* session_context);

int upsf_delete_session_context(
    upsf_handle_t upsf_handle,
    upsf_session_context_t* session_context);

int upsf_list_session_contexts(
    upsf_handle_t upsf_handle,
    upsf_session_context_t* elems, size_t n_elems);

char* upsf_dump_session_context(
    char* str, size_t size,
    upsf_session_context_t* session_context);

/* lookup */
upsf_session_context_t* upsf_lookup(
    upsf_handle_t upsf_handle,
    upsf_session_context_t* session_context);

/* subscribe */
int upsf_subscribe(
    upsf_handle_t upsf_handle,
    void* userdata,
    upsf_shard_cb_t upsf_shard_cb,
    upsf_session_context_cb_t upsf_session_context_cb,
    upsf_network_connection_cb_t upsf_network_connection_cb,
    upsf_service_gateway_user_plane_cb_t upsf_service_gateway_user_plane_cb,
    upsf_traffic_steering_function_cb_t upsf_traffic_steering_function_cb,
    upsf_service_gateway_cb_t upsf_service_gateway_cb);

#ifdef __cplusplus
}
#endif

#endif
