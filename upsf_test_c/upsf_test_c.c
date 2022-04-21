#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "upsf.h"

/*******************************************************
 * ServiceGateway
 *******************************************************/
int list_service_gateways(upsf_handle_t upsf_handle) {

  upsf_service_gateway_t service_gateways[64];
  int n_elems = 0;

  /* list service gateways */
  if ((n_elems = upsf_list_service_gateways(upsf_handle, service_gateways, 64)) < 0) {
    return -1;
  }

  for (int i = 0; i < n_elems; i++) {
    fprintf(stderr, "ServiceGateway[%d].id=%s\n", i, service_gateways[i].id);
    fprintf(stderr, "ServiceGateway[%d].name=%s\n", i, service_gateways[i].name);
  }

  return 0;
}

int test_service_gateway(upsf_handle_t upsf_handle) {

  upsf_service_gateway_t service_gateway;
  memset(&service_gateway, 0, sizeof(service_gateway));

  list_service_gateways(upsf_handle);

  /* update service gateway */
  snprintf(service_gateway.name, sizeof(service_gateway.name), "cp1.1");

  if (upsf_update_service_gateway(upsf_handle, &service_gateway) == NULL) {
    return -1;
  }
  fprintf(stderr, "update service_gateway().id() = %s\n", service_gateway.id);

  list_service_gateways(upsf_handle);

  /* delete service gateway */
  if (upsf_delete_service_gateway(upsf_handle, &service_gateway) < 0) {
    return -1;
  }
  fprintf(stderr, "delete service_gateway().id() = %s\n", service_gateway.id);

  list_service_gateways(upsf_handle);

  return 0;
}


/*******************************************************
 * Lookup
 *******************************************************/
int test_lookup(upsf_handle_t upsf_handle) {

  upsf_session_context_t session_context;
  memset(&session_context, 0, sizeof(session_context));

  snprintf(session_context.mac_address, sizeof(session_context.mac_address), "aa:aa:aa:aa:aa:aa");
  session_context.s_tag = 50;
  session_context.c_tag = 100;
  snprintf(session_context.circuit_id, sizeof(session_context.circuit_id), "circuit_id");
  snprintf(session_context.remote_id, sizeof(session_context.remote_id), "remote_id");

  if (upsf_lookup(upsf_handle, &session_context) == NULL) {
    return -1;
  }
  fprintf(stderr, "lookup session_context().id=%s\n", session_context.id);
  fprintf(stderr, "lookup session_context().name=%s\n", session_context.name);
  fprintf(stderr, "lookup session_context().tsf=%s\n", session_context.tsf);
  fprintf(stderr, "lookup session_context().desired_shard=%s\n", session_context.desired_shard);
  for (int j = 0; j < session_context.required_service_groups_size; j++) {
    fprintf(stderr, "lookup session_context().required_service_group[%d].name=%s\n", j, session_context.required_service_groups[j].str);
  }
  fprintf(stderr, "lookup session_context().required_qos=%s\n", session_context.required_qos);
  fprintf(stderr, "lookup session_context().context_type=%d\n", session_context.context_type);
  fprintf(stderr, "lookup session_context().mac_address=%s\n", session_context.mac_address);
  fprintf(stderr, "lookup session_context().s_tag=%d\n", session_context.s_tag);
  fprintf(stderr, "lookup session_context().c_tag=%d\n", session_context.c_tag);
  fprintf(stderr, "lookup session_context().circuit_id=%s\n", session_context.circuit_id);
  fprintf(stderr, "lookup session_context().remote_id=%s\n", session_context.remote_id);

  return 0;
}


/*******************************************************
 * SessionContext
 *******************************************************/
int list_session_contexts(upsf_handle_t upsf_handle) {

  upsf_session_context_t session_contexts[64];
  int n_elems = 0;

  /* list service gateways */
  if ((n_elems = upsf_list_session_contexts(upsf_handle, session_contexts, 64)) < 0) {
    return -1;
  }

  for (int i = 0; i < n_elems; i++) {
    fprintf(stderr, "SessionContext[%d].id=%s\n", i, session_contexts[i].id);
    fprintf(stderr, "SessionContext[%d].name=%s\n", i, session_contexts[i].name);
    fprintf(stderr, "SessionContext[%d].tsf=%s\n", i, session_contexts[i].tsf);
    fprintf(stderr, "SessionContext[%d].desired_shard=%s\n", i, session_contexts[i].desired_shard);
    for (int j = 0; j < session_contexts[i].required_service_groups_size; j++) {
      fprintf(stderr, "SessionContext[%d].required_service_group[%d].name=%s\n", i, j, session_contexts[i].required_service_groups[j].str);
    }
    fprintf(stderr, "SessionContext[%d].required_qos=%s\n", i, session_contexts[i].required_qos);
    fprintf(stderr, "SessionContext[%d].context_type=%d\n", i, session_contexts[i].context_type);
    fprintf(stderr, "SessionContext[%d].mac_address=%s\n", i, session_contexts[i].mac_address);
    fprintf(stderr, "SessionContext[%d].s_tag=%d\n", i, session_contexts[i].s_tag);
    fprintf(stderr, "SessionContext[%d].c_tag=%d\n", i, session_contexts[i].c_tag);
    fprintf(stderr, "SessionContext[%d].circuit_id=%s\n", i, session_contexts[i].circuit_id);
    fprintf(stderr, "SessionContext[%d].remote_id=%s\n", i, session_contexts[i].remote_id);
  }
}


int test_session_context(upsf_handle_t upsf_handle) {

  upsf_session_context_t session_context;
  memset(&session_context, 0, sizeof(session_context));

  list_session_contexts(upsf_handle);

  snprintf(session_context.name, sizeof(session_context.name), "sctxname");
  snprintf(session_context.tsf, sizeof(session_context.tsf), "tsfname");
  snprintf(session_context.desired_shard, sizeof(session_context.desired_shard), "desired_shard");
  for (int i = 0; i < 4; i++) {
    snprintf(session_context.required_service_groups[i].str, sizeof(session_context.required_service_groups[i].str), "rsg%d", i);
  }
  session_context.required_service_groups_size = 4;
  session_context.context_type = PPPOE;
  snprintf(session_context.required_qos, sizeof(session_context.required_qos), "required_qos");
  snprintf(session_context.mac_address, sizeof(session_context.mac_address), "aa:aa:aa:aa:aa:aa");
  session_context.s_tag = 50;
  session_context.c_tag = 100;
  snprintf(session_context.circuit_id, sizeof(session_context.circuit_id), "circuit_id");
  snprintf(session_context.remote_id, sizeof(session_context.remote_id), "remote_id");

  if (upsf_update_session_context(upsf_handle, &session_context) == NULL) {
    return -1;
  }
  fprintf(stderr, "update session_context().id() = %s\n", session_context.id);

  list_session_contexts(upsf_handle);

  test_lookup(upsf_handle);

  if (upsf_delete_session_context(upsf_handle, &session_context) < 0) {
    return -1;
  }
  fprintf(stderr, "delete session_context().id() = %s\n", session_context.id);

  list_session_contexts(upsf_handle);

  return 0;
}


/*******************************************************
 * Subscribe
 *******************************************************/
int shard_cb(upsf_shard_t* data, void* userdata) {
  fprintf(stderr, "callback: shards\n");

  return 1; /* continue subscribe */
}

int session_context_cb(upsf_session_context_t* session_context, void* userdata) {
  fprintf(stderr, "callback: session_contexts\n");

  fprintf(stderr, "Updated SessionContext.id=%s\n", session_context->id);
  fprintf(stderr, "Updated SessionContext.name=%s\n", session_context->name);
  fprintf(stderr, "Updated SessionContext.tsf=%s\n", session_context->tsf);
  fprintf(stderr, "Updated SessionContext.desired_shard=%s\n", session_context->desired_shard);
  for (int j = 0; j < session_context->required_service_groups_size; j++) {
    fprintf(stderr, "Updated SessionContext.required_service_group[%d].name=%s\n", j, session_context->required_service_groups[j].str);
  }
  fprintf(stderr, "Updated SessionContext.required_qos=%s\n", session_context->required_qos);
  fprintf(stderr, "Updated SessionContext.context_type=%d\n", session_context->context_type);
  fprintf(stderr, "Updated SessionContext.mac_address=%s\n", session_context->mac_address);
  fprintf(stderr, "Updated SessionContext.s_tag=%d\n", session_context->s_tag);
  fprintf(stderr, "Updated SessionContext.c_tag=%d\n", session_context->c_tag);
  fprintf(stderr, "Updated SessionContext.circuit_id=%s\n", session_context->circuit_id);
  fprintf(stderr, "Updated SessionContext.remote_id=%s\n", session_context->remote_id);

  return 1; /* continue subscribe */
}

int network_connection_cb(upsf_network_connection_t* data, void* userdata) {
  fprintf(stderr, "callback: network_connections\n");

  return 1; /* continue subscribe */
}

int service_gateway_user_plane_cb(upsf_service_gateway_user_plane_t* data, void* userdata) {
  fprintf(stderr, "callback: service_gateway_user_planes\n");

  return 1; /* continue subscribe */
}

int traffic_steering_function_cb(upsf_traffic_steering_function_t* data, void* userdata) {
  fprintf(stderr, "callback: traffic_steering_functions\n");

  return 1; /* continue subscribe */
}

int service_gateway_cb(upsf_service_gateway_t* service_gateway, void* userdata) {
  fprintf(stderr, "callback: service_gateways\n");

  fprintf(stderr, "Updated ServiceGateway.id=%s\n", service_gateway->id);
  fprintf(stderr, "Updated ServiceGateway.name=%s\n", service_gateway->name);

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
