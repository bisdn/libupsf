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

enum session_context_type_t {
    UPSF_SESSION_CONTEXT_DEFAULT = 0,
    UPSF_SESSION_CONTEXT_PPPOE = 1,
    UPSF_SESSION_CONTEXT_IPOE = 2,
    UPSF_SESSION_CONTEXT_MAX,
};

enum upsf_item_status_t {
    UPSF_ITEM_STATUS_UNKNOWN = 0,
    UPSF_ITEM_STATUS_ACTIVE = 1,
    UPSF_ITEM_STATUS_UPDATING = 2,
    UPSF_ITEM_STATUS_DELETING = 3,
    UPSF_ITEM_STATUS_DELETED = 4,
    UPSF_ITEM_STATUS_INACTIVE = 5,
    UPSF_ITEM_STATUS_MAX,
};

/* string type */
typedef struct {
    char str[UPSF_MAX_STRING_SIZE];
} upsf_string_t;

/* map type */
typedef struct {
    char key[UPSF_MAX_STRING_SIZE];
    char value[UPSF_MAX_STRING_SIZE];
} upsf_key_value_t;

/* service gateway */
typedef struct {
    char id[UPSF_MAX_STRING_SIZE];
    char name[UPSF_MAX_STRING_SIZE];
    int item_status;
} upsf_service_gateway_t;

/* session context */
typedef struct {
    char id[UPSF_MAX_STRING_SIZE];
    char name[UPSF_MAX_STRING_SIZE];
    int item_status;
    char tsf[UPSF_MAX_STRING_SIZE];
    char desired_shard[UPSF_MAX_STRING_SIZE];
    char current_shard[UPSF_MAX_STRING_SIZE];
    upsf_string_t required_service_groups[UPSF_MAX_NUM_REQUIRED_SERVICE_GROUPS];
    size_t required_service_groups_size;
    char required_qos[UPSF_MAX_STRING_SIZE];
    enum session_context_type_t context_type;
    char mac_address[UPSF_MAX_STRING_SIZE];
    uint16_t s_tag;
    uint16_t c_tag;
    char circuit_id[UPSF_MAX_STRING_SIZE];
    char remote_id[UPSF_MAX_STRING_SIZE];
} upsf_session_context_t;

/* shard */
typedef struct {
    char id[UPSF_MAX_STRING_SIZE];
    char name[UPSF_MAX_STRING_SIZE];
    int item_status;
    int32_t max_session_count;
    char desired_service_gateway_user_plane[UPSF_MAX_STRING_SIZE];
    upsf_string_t desired_network_connections[UPSF_MAX_NUM_DESIRED_NETWORK_CONNECTIONS];
    size_t desired_network_connections_size;
    char required_qos[UPSF_MAX_STRING_SIZE];
    char current_service_gateway_user_plane[UPSF_MAX_STRING_SIZE];
    upsf_key_value_t tsf_network_connections[UPSF_MAX_NUM_TSF_NETWORK_CONNECTIONS];
    size_t tsf_network_connections_size;
    upsf_string_t ip_prefixes[UPSF_MAX_NUM_IP_PREFIXES];
    size_t ip_prefixes_size;
} upsf_shard_t;

/* network connection */
typedef struct {
    /* todo */
} upsf_network_connection_t;

/* traffic steering function */
typedef struct {
    /* todo */
} upsf_traffic_steering_function_t;

/* service gateway user plane */
typedef struct {
    char id[UPSF_MAX_STRING_SIZE];
    char name[UPSF_MAX_STRING_SIZE];
    int item_status;
    char service_gateway_id[UPSF_MAX_STRING_SIZE];
    int32_t max_session_count;
    upsf_string_t supported_service_groups[UPSF_MAX_NUM_SUPPORTED_SERVICE_GROUPS];
    size_t supported_service_groups_size;
    int maintenance;
    int32_t allocated_session_count;
    int32_t network_connections[UPSF_MAX_NUM_NETWORK_CONNECTIONS];
    size_t network_connections_size;
} upsf_service_gateway_user_plane_t;

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

const char* upsf_item_status_to_name(
    int item_status);

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

upsf_session_context_t* upsf_lookup(
    upsf_handle_t upsf_handle,
    upsf_session_context_t* session_context);

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
