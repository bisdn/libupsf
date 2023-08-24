/* upsf_c_example
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "upsf.h"

/*******************************************************
 * ServiceGateway
 *******************************************************/
int list_service_gateways()
{
    char buf[1024];
    size_t buflen = sizeof(buf) - 1;
    upsf_service_gateway_t items[64];
    int n_elems = 0, i = 0;

    /* list service gateways */
    if ((n_elems = upsf_list_service_gateways(items, 64)) < 0) {
        return -1;
    }

    for (i = 0; i < n_elems; i++) {
        fprintf(stderr, "LIST service_gateway[%d]: %s\n", i,
            upsf_dump_service_gateway(buf, buflen, &items[i]));
    }

    return 0;
}

void dump_service_gateway(const char* intro, upsf_service_gateway_t* item)
{
    char buf[1024];
    size_t buflen = sizeof(buf) - 1;

    fprintf(stderr, "%s %s\n", intro,
        upsf_dump_service_gateway(buf, buflen, item));
}

int test_service_gateway()
{
    upsf_service_gateway_t item;
    memset(&item, 0, sizeof(item));

    list_service_gateways();

    /* create service gateway */
    snprintf(item.name.str, sizeof(item.name.str), "sg-test");

    if (upsf_create_service_gateway(&item) == NULL) {
        return -1;
    }
    dump_service_gateway("CREATE service_gateway:", &item);

    /* update service gateway */
    snprintf(item.name.str, sizeof(item.name.str), "sg-test");
    snprintf(item.metadata.description.str, sizeof(item.metadata.description.str), "sg-test description");

    if (upsf_update_service_gateway(&item) == NULL) {
        return -1;
    }
    dump_service_gateway("UPDATE service_gateway:", &item);

    /* read service gateway */
    snprintf(item.name.str, sizeof(item.name.str), "sg-test");

    if (upsf_get_service_gateway(&item) < 0) {
        return -1;
    }
    dump_service_gateway("GET service_gateway:", &item);

    /* list service gateways */
    list_service_gateways();

    /* delete service gateway */
    if (upsf_delete_service_gateway(&item) < 0) {
        return -1;
    }
    dump_service_gateway("DELETE service_gateway:", &item);

    /* list service gateways */
    list_service_gateways();

    return 0;
}

/*******************************************************
 * ServiceGatewayUserPlane
 *******************************************************/
int list_service_gateway_user_planes()
{
    char buf[1024];
    size_t buflen = sizeof(buf) - 1;
    upsf_service_gateway_user_plane_t items[64];
    int n_elems = 0, i = 0;

    /* list service gateways */
    if ((n_elems = upsf_list_service_gateway_user_planes(items, 64)) < 0) {
        return -1;
    }

    for (i = 0; i < n_elems; i++) {
        fprintf(stderr, "LIST service_gateway_user_plane[%d]: %s\n", i,
            upsf_dump_service_gateway_user_plane(buf, buflen, &items[i]));
    }

    return 0;
}

void dump_service_gateway_user_plane(const char* intro, upsf_service_gateway_user_plane_t* item)
{
    char buf[1024];
    size_t buflen = sizeof(buf) - 1;

    fprintf(stderr, "%s %s\n", intro,
        upsf_dump_service_gateway_user_plane(buf, buflen, item));
}

int test_service_gateway_user_plane()
{
    upsf_service_gateway_user_plane_t item;
    memset(&item, 0, sizeof(item));

    list_service_gateway_user_planes();

    /* create service gateway */
    snprintf(item.name.str, sizeof(item.name.str), "up-test");
    snprintf(item.service_gateway_name.str, sizeof(item.service_gateway_name.str), "sg-test");
    item.spec.default_endpoint.ep_type = UPSF_EP_TYPE_VTEP;
    snprintf(item.spec.default_endpoint.endpoint_name.str, sizeof(item.spec.default_endpoint.endpoint_name.str), "up-test");
    snprintf(item.spec.default_endpoint.ep_spec.vtep.ip_address.str, sizeof(item.spec.default_endpoint.ep_spec.vtep.ip_address.str), "1.2.3.4");
    item.spec.default_endpoint.ep_spec.vtep.udp_port = 4789;
    item.spec.default_endpoint.ep_spec.vtep.vni = 1234;

    if (upsf_create_service_gateway_user_plane(&item) == NULL) {
        return -1;
    }
    dump_service_gateway_user_plane("CREATE service_gateway_user_plane:", &item);

    /* update service gateway */
    snprintf(item.name.str, sizeof(item.name.str), "up-test");
    snprintf(item.metadata.description.str, sizeof(item.metadata.description.str), "up-test description");

    if (upsf_update_service_gateway_user_plane(&item) == NULL) {
        return -1;
    }
    dump_service_gateway_user_plane("UPDATE service_gateway_user_plane:", &item);

    /* read service gateway */
    snprintf(item.name.str, sizeof(item.name.str), "up-test");

    if (upsf_get_service_gateway_user_plane(&item) < 0) {
        return -1;
    }
    dump_service_gateway_user_plane("GET service_gateway_user_plane:", &item);

    /* list service gateways */
    list_service_gateway_user_planes();

    /* delete service gateway */
    if (upsf_delete_service_gateway_user_plane(&item) < 0) {
        return -1;
    }
    dump_service_gateway_user_plane("DELETE service_gateway_user_plane:", &item);

    /* list service gateways */
    list_service_gateway_user_planes();

    return 0;
}

/*******************************************************
 * Shard
 *******************************************************/
int list_shards()
{
    char buf[1024];
    size_t buflen = sizeof(buf) - 1;
    upsf_shard_t items[64];
    int n_elems = 0, i = 0;

    /* list shards */
    if ((n_elems = upsf_list_shards(items, 64)) < 0) {
        return -1;
    }

    for (i = 0; i < n_elems; i++) {
        fprintf(stderr, "LIST shard[%d]: %s\n", i,
            upsf_dump_shard(buf, buflen, &items[i]));
    }

    return 0;
}

void dump_shard(const char* intro, upsf_shard_t* item)
{
    char buf[1024];
    size_t buflen = sizeof(buf) - 1;

    fprintf(stderr, "%s %s\n", intro,
        upsf_dump_shard(buf, buflen, item));
}

int test_shard()
{
    upsf_shard_t item;
    memset(&item, 0, sizeof(item));

    list_shards();

    /* create shard */
    snprintf(item.name.str, sizeof(item.name.str), "shard-test");

    if (upsf_create_shard(&item) == NULL) {
        return -1;
    }
    dump_shard("CREATE shard:", &item);

    /* update shard */
    snprintf(item.name.str, sizeof(item.name.str), "shard-test");
    snprintf(item.metadata.description.str, sizeof(item.metadata.description.str), "shard-test description");

    if (upsf_update_shard(&item) == NULL) {
        return -1;
    }
    dump_shard("UPDATE shard:", &item);

    /* read shard */
    snprintf(item.name.str, sizeof(item.name.str), "shard-test");

    if (upsf_get_shard(&item) < 0) {
        return -1;
    }
    dump_shard("GET shard:", &item);

    /* list shards */
    list_shards();

    /* delete shard */
    if (upsf_delete_shard(&item) < 0) {
        return -1;
    }
    dump_shard("DELETE shard:", &item);

    /* list shards */
    list_shards();

    return 0;
}

/*******************************************************
 * SessionContext
 *******************************************************/
int list_session_contexts()
{
    char buf[1024];
    size_t buflen = sizeof(buf) - 1;
    upsf_session_context_t items[64];
    int n_elems = 0, i = 0;

    /* list session_contexts */
    if ((n_elems = upsf_list_session_contexts(items, 64)) < 0) {
        return -1;
    }

    for (i = 0; i < n_elems; i++) {
        fprintf(stderr, "LIST session_context[%d]: %s\n", i,
            upsf_dump_session_context(buf, buflen, &items[i]));
    }

    return 0;
}

void dump_session_context(const char* intro, upsf_session_context_t* item)
{
    char buf[1024];
    size_t buflen = sizeof(buf) - 1;

    fprintf(stderr, "%s %s\n", intro,
        upsf_dump_session_context(buf, buflen, item));
}

int test_session_context()
{
    upsf_session_context_t item;
    memset(&item, 0, sizeof(item));

    list_session_contexts();

    /* create session_context */
    snprintf(item.name.str, sizeof(item.name.str), "session-context-test");

    if (upsf_create_session_context(&item) == NULL) {
        return -1;
    }
    dump_session_context("CREATE session_context:", &item);

    /* update session_context */
    snprintf(item.name.str, sizeof(item.name.str), "session-context-test");
    snprintf(item.metadata.description.str, sizeof(item.metadata.description.str), "session-context-test description");

    if (upsf_update_session_context(&item) == NULL) {
        return -1;
    }
    dump_session_context("UPDATE session_context:", &item);

    /* read session_context */
    snprintf(item.name.str, sizeof(item.name.str), "session-context-test");

    if (upsf_get_session_context(&item) < 0) {
        return -1;
    }
    dump_session_context("GET session_context:", &item);

    /* list session_contexts */
    list_session_contexts();

    /* delete session_context */
    if (upsf_delete_session_context(&item) < 0) {
        return -1;
    }
    dump_session_context("DELETE session_context:", &item);

    /* list session_contexts */
    list_session_contexts();

    return 0;
}

/*******************************************************
 * NetworkConnection
 *******************************************************/
int list_network_connections()
{
    char buf[1024];
    size_t buflen = sizeof(buf) - 1;
    upsf_network_connection_t items[64];
    int n_elems = 0, i = 0;

    /* list session_contexts */
    if ((n_elems = upsf_list_network_connections(items, 64)) < 0) {
        return -1;
    }

    for (i = 0; i < n_elems; i++) {
        fprintf(stderr, "LIST network_connection[%d]: %s\n", i,
            upsf_dump_network_connection(buf, buflen, &items[i]));
    }

    return 0;
}

void dump_network_connection(const char* intro, upsf_network_connection_t* item)
{
    char buf[1024];
    size_t buflen = sizeof(buf) - 1;

    fprintf(stderr, "%s %s\n", intro,
        upsf_dump_network_connection(buf, buflen, item));
}

int test_network_connection()
{
    upsf_network_connection_t item;
    memset(&item, 0, sizeof(item));

    list_network_connections();

    /* create network connection */
    snprintf(item.name.str, sizeof(item.name.str), "nc-test");
    item.spec.nc_spec_type = UPSF_NC_SPEC_TYPE_SS_PTP;
    item.spec.nc_spec.ss_ptp.sgup_endpoint_size = 2;
    snprintf(item.spec.nc_spec.ss_ptp.sgup_endpoint[0].endpoint_name.str, UPSF_MAX_STRING_SIZE, "sgup-ep-name-1");
    item.spec.nc_spec.ss_ptp.sgup_endpoint[0].ep_type = UPSF_EP_TYPE_L2VPN;
    item.spec.nc_spec.ss_ptp.sgup_endpoint[0].ep_spec.l2vpn.vpn_id = 1111;
    snprintf(item.spec.nc_spec.ss_ptp.sgup_endpoint[1].endpoint_name.str, UPSF_MAX_STRING_SIZE, "sgup-ep-name-2");
    item.spec.nc_spec.ss_ptp.sgup_endpoint[1].ep_type = UPSF_EP_TYPE_PORT_VLAN;
    snprintf(item.spec.nc_spec.ss_ptp.sgup_endpoint[1].ep_spec.port_vlan.logical_port.str, UPSF_MAX_STRING_SIZE, "eth0");
    item.spec.nc_spec.ss_ptp.sgup_endpoint[1].ep_spec.port_vlan.svlan = 50;
    item.spec.nc_spec.ss_ptp.sgup_endpoint[1].ep_spec.port_vlan.cvlan = 10;
    snprintf(item.spec.nc_spec.ss_ptp.tsf_endpoint.endpoint_name.str, UPSF_MAX_STRING_SIZE, "tsf-ep-name");
    item.spec.nc_spec.ss_ptp.tsf_endpoint.ep_type = UPSF_EP_TYPE_VTEP;
    snprintf(item.spec.nc_spec.ss_ptp.tsf_endpoint.ep_spec.vtep.ip_address.str, UPSF_MAX_STRING_SIZE, "1.2.3.4");
    item.spec.nc_spec.ss_ptp.tsf_endpoint.ep_spec.vtep.udp_port = 4789;
    item.spec.nc_spec.ss_ptp.tsf_endpoint.ep_spec.vtep.vni = 1234;

    if (upsf_create_network_connection(&item) == NULL) {
        return -1;
    }
    dump_network_connection("CREATE network connection:", &item);

    /* update network connection */
    snprintf(item.name.str, sizeof(item.name.str), "nc-test");
    snprintf(item.metadata.description.str, sizeof(item.metadata.description.str), "nc-test description");

    if (upsf_update_network_connection(&item) == NULL) {
        return -1;
    }
    dump_network_connection("UPDATE network connection:", &item);

    /* read network connection */
    snprintf(item.name.str, sizeof(item.name.str), "nc-test");

    if (upsf_get_network_connection(&item) < 0) {
        return -1;
    }
    dump_network_connection("GET network connection:", &item);

    /* list network connections */
    list_network_connections();

    /* delete network connection */
    if (upsf_delete_network_connection(&item) < 0) {
        return -1;
    }
    dump_network_connection("DELETE network connection:", &item);

    /* list network connections */
    list_network_connections();

    return 0;
}

/*******************************************************
 * TrafficSteeringFunction
 *******************************************************/
int list_traffic_steering_functions()
{
    char buf[1024];
    size_t buflen = sizeof(buf) - 1;
    upsf_traffic_steering_function_t items[64];
    int n_elems = 0, i = 0;

    /* list session_contexts */
    if ((n_elems = upsf_list_traffic_steering_functions(items, 64)) < 0) {
        return -1;
    }

    for (i = 0; i < n_elems; i++) {
        fprintf(stderr, "LIST traffic_steering_function[%d]: %s\n", i,
            upsf_dump_traffic_steering_function(buf, buflen, &items[i]));
    }

    return 0;
}

void dump_traffic_steering_function(const char* intro, upsf_traffic_steering_function_t* item)
{
    char buf[1024];
    size_t buflen = sizeof(buf) - 1;

    fprintf(stderr, "%s %s\n", intro,
        upsf_dump_traffic_steering_function(buf, buflen, item));
}

int test_traffic_steering_function()
{
    upsf_traffic_steering_function_t item;
    memset(&item, 0, sizeof(item));

    list_traffic_steering_functions();

    /* create traffic steering function */
    snprintf(item.name.str, sizeof(item.name.str), "tsf-test");
    snprintf(item.spec.default_endpoint.endpoint_name.str, UPSF_MAX_STRING_SIZE, "tsf-ep-name");
    item.spec.default_endpoint.ep_type = UPSF_EP_TYPE_L2VPN;
    item.spec.default_endpoint.ep_spec.l2vpn.vpn_id = 1234;

    if (upsf_create_traffic_steering_function(&item) == NULL) {
        return -1;
    }
    dump_traffic_steering_function("CREATE traffic steering function:", &item);

    /* update traffic steering function */
    snprintf(item.name.str, sizeof(item.name.str), "tsf-test");
    snprintf(item.metadata.description.str, sizeof(item.metadata.description.str), "tsf-test description");
    snprintf(item.spec.default_endpoint.endpoint_name.str, UPSF_MAX_STRING_SIZE, "tsf-ep-name");
    item.spec.default_endpoint.ep_type = UPSF_EP_TYPE_L2VPN;
    item.spec.default_endpoint.ep_spec.l2vpn.vpn_id = 1234;

    if (upsf_update_traffic_steering_function(&item) == NULL) {
        return -1;
    }
    dump_traffic_steering_function("UPDATE traffic steering function:", &item);

    /* read traffic steering function */
    snprintf(item.name.str, sizeof(item.name.str), "tsf-test");

    if (upsf_get_traffic_steering_function(&item) < 0) {
        return -1;
    }
    dump_traffic_steering_function("GET traffic steering function:", &item);

    /* list traffic steering functions */
    list_traffic_steering_functions();

    /* delete traffic steering function */
    if (upsf_delete_traffic_steering_function(&item) < 0) {
        return -1;
    }
    dump_traffic_steering_function("DELETE traffic steering function:", &item);

    /* list traffic steering functions */
    list_traffic_steering_functions();

    return 0;
}

/*******************************************************
 * Subscribe
 *******************************************************/
int service_gateway_cb(upsf_service_gateway_t* data, void* userdata)
{
    dump_service_gateway("callback: service gateway", data);

    return 1; /* continue subscribe */
}

int service_gateway_user_plane_cb(upsf_service_gateway_user_plane_t* data, void* userdata)
{
    dump_service_gateway_user_plane("callback: service gateway user plane", data);

    return 1; /* continue subscribe */
}

int shard_cb(upsf_shard_t* data, void* userdata)
{
    dump_shard("callback: shard", data);

    return 1; /* continue subscribe */
}

int session_context_cb(upsf_session_context_t* data, void* userdata)
{
    dump_session_context("callback: session context", data);

    return 1; /* continue subscribe */
}

int network_connection_cb(upsf_network_connection_t* data, void* userdata)
{
    dump_network_connection("callback: network connection", data);

    return 1; /* continue subscribe */
}

int traffic_steering_function_cb(upsf_traffic_steering_function_t* data, void* userdata)
{
    dump_traffic_steering_function("callback: traffic steering function", data);

    return 1; /* continue subscribe */
}

int test_subscribe()
{
    void* userdata = NULL;

    if (upsf_subscribe("127.0.0.1", 50051, userdata, &shard_cb, &session_context_cb, &network_connection_cb, &service_gateway_user_plane_cb, &traffic_steering_function_cb, &service_gateway_cb) < 0) {
        return -1;
    }

    return 0;
}

/*******************************************************
 * main
 *******************************************************/
int main(char* argv, int argc)
{

    char* upsf_host = "127.0.0.1";
    int upsf_port = 50051;

    /* open upsf connection */
    if ((upsf_open(upsf_host, upsf_port)) < 0) {
        return -1;
    }

    if (test_service_gateway() < 0) {
        return -1;
    }

    if (test_service_gateway_user_plane() < 0) {
        return -1;
    }

    if (test_shard() < 0) {
        return -1;
    }

    if (test_session_context() < 0) {
        return -1;
    }

    if (test_network_connection() < 0) {
        return -1;
    }

    if (test_traffic_steering_function() < 0) {
        return -1;
    }

    if (test_subscribe() < 0) {
        return -1;
    }

    /* close upsf connection */
    if (upsf_close() < 0) {
        return -1;
    }

    return 0;
}
