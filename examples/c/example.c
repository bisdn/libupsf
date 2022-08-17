#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "upsf.h"

/*******************************************************
 * ServiceGateway
 *******************************************************/
int list_service_gateways(upsf_handle_t upsf_handle)
{
  char buf[1024];
  size_t buflen = sizeof(buf) - 1;
  upsf_service_gateway_t service_gateways[64];
  int n_elems = 0;

  /* list service gateways */
  if ((n_elems = upsf_list_service_gateways(upsf_handle, service_gateways, 64)) < 0) {
    return -1;
  }

  for (int i = 0; i < n_elems; i++) {
    fprintf(stderr, "LIST service_gateway[%d]: %s\n", i,
      upsf_dump_service_gateway(buf, buflen, &service_gateways[i]));
  }

  return 0;
}

void dump_service_gateway(const char* intro, upsf_service_gateway_t *service_gateway)
{
  char buf[1024];
  size_t buflen = sizeof(buf) - 1;

  fprintf(stderr, "%s %s\n", intro,
    upsf_dump_service_gateway(buf, buflen, service_gateway));
}

int test_service_gateway(upsf_handle_t upsf_handle)
{
  upsf_service_gateway_t service_gateway;
  memset(&service_gateway, 0, sizeof(service_gateway));

  list_service_gateways(upsf_handle);


  /* create service gateway */
  snprintf(service_gateway.name.str, sizeof(service_gateway.name.str), "sg-test");

  if (upsf_create_service_gateway(upsf_handle, &service_gateway) == NULL) {
    return -1;
  }
  dump_service_gateway("CREATE service_gateway:", &service_gateway);


  /* update service gateway */
  snprintf(service_gateway.name.str, sizeof(service_gateway.name.str), "sg-test");
  snprintf(service_gateway.metadata.description.str, sizeof(service_gateway.metadata.description.str), "sg-test description");

  if (upsf_update_service_gateway(upsf_handle, &service_gateway) == NULL) {
    return -1;
  }
  dump_service_gateway("UPDATE service_gateway:", &service_gateway);


  /* read service gateway */
  snprintf(service_gateway.name.str, sizeof(service_gateway.name.str), "sg-test");

  if (upsf_get_service_gateway(upsf_handle, &service_gateway) < 0) {
    return -1;
  }
  dump_service_gateway("GET service_gateway:", &service_gateway);


  /* list service gateways */
  list_service_gateways(upsf_handle);


  /* delete service gateway */
  if (upsf_delete_service_gateway(upsf_handle, &service_gateway) < 0) {
    return -1;
  }
  dump_service_gateway("DELETE service_gateway:", &service_gateway);


  /* list service gateways */
  list_service_gateways(upsf_handle);

  return 0;
}

/*******************************************************
 * ServiceGatewayUserPlane
 *******************************************************/
int list_service_gateway_user_planes(upsf_handle_t upsf_handle)
{
  char buf[1024];
  size_t buflen = sizeof(buf) - 1;
  upsf_service_gateway_user_plane_t service_gateway_user_planes[64];
  int n_elems = 0;

  /* list service gateways */
  if ((n_elems = upsf_list_service_gateway_user_planes(upsf_handle, service_gateway_user_planes, 64)) < 0) {
    return -1;
  }

  for (int i = 0; i < n_elems; i++) {
    fprintf(stderr, "LIST service_gateway_user_plane[%d]: %s\n", i,
      upsf_dump_service_gateway_user_plane(buf, buflen, &service_gateway_user_planes[i]));
  }

  return 0;
}

void dump_service_gateway_user_plane(const char* intro, upsf_service_gateway_user_plane_t *service_gateway_user_plane)
{
  char buf[1024];
  size_t buflen = sizeof(buf) - 1;

  fprintf(stderr, "%s %s\n", intro,
    upsf_dump_service_gateway_user_plane(buf, buflen, service_gateway_user_plane));
}

int test_service_gateway_user_plane(upsf_handle_t upsf_handle)
{
  upsf_service_gateway_user_plane_t service_gateway_user_plane;
  memset(&service_gateway_user_plane, 0, sizeof(service_gateway_user_plane));

  list_service_gateway_user_planes(upsf_handle);


  /* create service gateway */
  snprintf(service_gateway_user_plane.name.str, sizeof(service_gateway_user_plane.name.str), "up-test");

  if (upsf_create_service_gateway_user_plane(upsf_handle, &service_gateway_user_plane) == NULL) {
    return -1;
  }
  dump_service_gateway_user_plane("CREATE service_gateway_user_plane:", &service_gateway_user_plane);


  /* update service gateway */
  snprintf(service_gateway_user_plane.name.str, sizeof(service_gateway_user_plane.name.str), "up-test");
  snprintf(service_gateway_user_plane.metadata.description.str, sizeof(service_gateway_user_plane.metadata.description.str), "up-test description");

  if (upsf_update_service_gateway_user_plane(upsf_handle, &service_gateway_user_plane) == NULL) {
    return -1;
  }
  dump_service_gateway_user_plane("UPDATE service_gateway_user_plane:", &service_gateway_user_plane);


  /* read service gateway */
  snprintf(service_gateway_user_plane.name.str, sizeof(service_gateway_user_plane.name.str), "up-test");

  if (upsf_get_service_gateway_user_plane(upsf_handle, &service_gateway_user_plane) < 0) {
    return -1;
  }
  dump_service_gateway_user_plane("GET service_gateway_user_plane:", &service_gateway_user_plane);


  /* list service gateways */
  list_service_gateway_user_planes(upsf_handle);


  /* delete service gateway */
  if (upsf_delete_service_gateway_user_plane(upsf_handle, &service_gateway_user_plane) < 0) {
    return -1;
  }
  dump_service_gateway_user_plane("DELETE service_gateway_user_plane:", &service_gateway_user_plane);


  /* list service gateways */
  list_service_gateway_user_planes(upsf_handle);

  return 0;
}

/*******************************************************
 * Shard
 *******************************************************/
int list_shards(upsf_handle_t upsf_handle)
{
  char buf[1024];
  size_t buflen = sizeof(buf) - 1;
  upsf_shard_t shards[64];
  int n_elems = 0;

  /* list shards */
  if ((n_elems = upsf_list_shards(upsf_handle, shards, 64)) < 0) {
    return -1;
  }

  for (int i = 0; i < n_elems; i++) {
    fprintf(stderr, "LIST shard[%d]: %s\n", i,
      upsf_dump_shard(buf, buflen, &shards[i]));
  }

  return 0;
}

void dump_shard(const char* intro, upsf_shard_t *shard)
{
  char buf[1024];
  size_t buflen = sizeof(buf) - 1;

  fprintf(stderr, "%s %s\n", intro,
    upsf_dump_shard(buf, buflen, shard));
}

int test_shard(upsf_handle_t upsf_handle)
{
  upsf_shard_t shard;
  memset(&shard, 0, sizeof(shard));

  list_shards(upsf_handle);


  /* create shard */
  snprintf(shard.name.str, sizeof(shard.name.str), "shard-test");

  if (upsf_create_shard(upsf_handle, &shard) == NULL) {
    return -1;
  }
  dump_shard("CREATE shard:", &shard);


  /* update shard */
  snprintf(shard.name.str, sizeof(shard.name.str), "shard-test");
  snprintf(shard.metadata.description.str, sizeof(shard.metadata.description.str), "shard-test description");

  if (upsf_update_shard(upsf_handle, &shard) == NULL) {
    return -1;
  }
  dump_shard("UPDATE shard:", &shard);


  /* read shard */
  snprintf(shard.name.str, sizeof(shard.name.str), "shard-test");

  if (upsf_get_shard(upsf_handle, &shard) < 0) {
    return -1;
  }
  dump_shard("GET shard:", &shard);


  /* list shards */
  list_shards(upsf_handle);


  /* delete shard */
  if (upsf_delete_shard(upsf_handle, &shard) < 0) {
    return -1;
  }
  dump_shard("DELETE shard:", &shard);


  /* list shards */
  list_shards(upsf_handle);

  return 0;
}

/*******************************************************
 * SessionContext
 *******************************************************/
int list_session_contexts(upsf_handle_t upsf_handle)
{
  char buf[1024];
  size_t buflen = sizeof(buf) - 1;
  upsf_session_context_t session_contexts[64];
  int n_elems = 0;

  /* list session_contexts */
  if ((n_elems = upsf_list_session_contexts(upsf_handle, session_contexts, 64)) < 0) {
    return -1;
  }

  for (int i = 0; i < n_elems; i++) {
    fprintf(stderr, "LIST session_context[%d]: %s\n", i,
      upsf_dump_session_context(buf, buflen, &session_contexts[i]));
  }

  return 0;
}

void dump_session_context(const char* intro, upsf_session_context_t *session_context)
{
  char buf[1024];
  size_t buflen = sizeof(buf) - 1;

  fprintf(stderr, "%s %s\n", intro,
    upsf_dump_session_context(buf, buflen, session_context));
}

int test_session_context(upsf_handle_t upsf_handle)
{
  upsf_session_context_t session_context;
  memset(&session_context, 0, sizeof(session_context));

  list_session_contexts(upsf_handle);


  /* create session_context */
  snprintf(session_context.name.str, sizeof(session_context.name.str), "session-context-test");

  if (upsf_create_session_context(upsf_handle, &session_context) == NULL) {
    return -1;
  }
  dump_session_context("CREATE session_context:", &session_context);


  /* update session_context */
  snprintf(session_context.name.str, sizeof(session_context.name.str), "session-context-test");
  snprintf(session_context.metadata.description.str, sizeof(session_context.metadata.description.str), "session-context-test description");

  if (upsf_update_session_context(upsf_handle, &session_context) == NULL) {
    return -1;
  }
  dump_session_context("UPDATE session_context:", &session_context);


  /* read session_context */
  snprintf(session_context.name.str, sizeof(session_context.name.str), "session-context-test");

  if (upsf_get_session_context(upsf_handle, &session_context) < 0) {
    return -1;
  }
  dump_session_context("GET session_context:", &session_context);


  /* list session_contexts */
  list_session_contexts(upsf_handle);


  /* delete session_context */
  if (upsf_delete_session_context(upsf_handle, &session_context) < 0) {
    return -1;
  }
  dump_session_context("DELETE session_context:", &session_context);


  /* list session_contexts */
  list_session_contexts(upsf_handle);

  return 0;
}

/*******************************************************
 * Subscribe
 *******************************************************/
int service_gateway_cb(upsf_service_gateway_t* data, void* userdata) {
  dump_service_gateway("callback: service_gateways", data);

  return 1; /* continue subscribe */
}

int service_gateway_user_plane_cb(upsf_service_gateway_user_plane_t* data, void* userdata) {
  dump_service_gateway_user_plane("callback: service_gateway_user_planes", data);

  return 1; /* continue subscribe */
}

int shard_cb(upsf_shard_t* data, void* userdata) {
  dump_shard("callback: shards", data);

  return 1; /* continue subscribe */
}

int session_context_cb(upsf_session_context_t* data, void* userdata) {
  dump_session_context("callback: session_contexts", data);

  return 1; /* continue subscribe */
}

int network_connection_cb(upsf_network_connection_t* data, void* userdata) {
  fprintf(stderr, "callback: network_connections\n");

  return 1; /* continue subscribe */
}

int traffic_steering_function_cb(upsf_traffic_steering_function_t* data, void* userdata) {
  fprintf(stderr, "callback: traffic_steering_functions\n");

  return 1; /* continue subscribe */
}


int test_subscribe(upsf_handle_t upsf_handle) {

  void *userdata = NULL;

  if (upsf_subscribe(upsf_handle, userdata, &shard_cb, &session_context_cb, &network_connection_cb, &service_gateway_user_plane_cb, &traffic_steering_function_cb, &service_gateway_cb) < 0) {
    return -1;
  }

  return 0;
}


/*******************************************************
 * main
 *******************************************************/
int main(char* argv, int argc) {

  char* upsf_host = "127.0.0.1";
  int upsf_port = 50051;
  upsf_handle_t upsf_handle = -1;

  /* open upsf connection */
  if ((upsf_handle = upsf_open(upsf_host, upsf_port)) < 0) {
    return -1;
  }
  fprintf(stderr, "upsf_handle=%d\n", upsf_handle);

  if (test_service_gateway(upsf_handle) < 0) {
    return -1;
  }

  if (test_service_gateway_user_plane(upsf_handle) < 0) {
    return -1;
  }

  if (test_shard(upsf_handle) < 0) {
    return -1;
  }

  if (test_session_context(upsf_handle) < 0) {
    return -1;
  }

  if (test_subscribe(upsf_handle) < 0) {
    return -1;
  }

  /* close upsf connection */
  if (upsf_close(upsf_handle) < 0) {
    return -1;
  }

  return 0;
}
