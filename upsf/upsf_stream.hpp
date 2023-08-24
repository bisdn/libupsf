/* upsf_stream.hpp
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

#ifndef UPSF_STREAM_HPP
#define UPSF_STREAM_HPP

#include <google/protobuf/util/time_util.h>
#include <iostream>

#include "upsf_c_mapping.hpp"

#include "wt474_upsf_messages/v1/messages_v1.grpc.pb.h"
#include "wt474_upsf_messages/v1/messages_v1.pb.h"
#include "wt474_upsf_service/v1/service_v1.grpc.pb.h"
#include "wt474_upsf_service/v1/service_v1.pb.h"

namespace upsf {

class MetaDataStream {
    const wt474_messages::v1::MetaData& metadata;

public:
    MetaDataStream(
        const wt474_messages::v1::MetaData& metadata)
        : metadata(metadata) {};

    friend std::ostream& operator<<(
        std::ostream& os, const MetaDataStream& s)
    {
        os << "MetaData(";

        /* metadata: description */
        os << "desc=" << s.metadata.description().c_str() << ", ";

        /* metadata: derived_state */
        os << "DerivedState=" << upsf_derived_state_to_name(s.metadata.derived_state()) << ", ";

        /* metadata: created */
        os << "created=" << google::protobuf::util::TimeUtil::ToString(s.metadata.created()) << ", ";

        /* metadata: last_updated */
        os << "last_updated=" << google::protobuf::util::TimeUtil::ToString(s.metadata.last_updated());

        os << ")"; // Metadata

        return os;
    };
};

class MaintenanceStream {
    const wt474_messages::v1::Maintenance& maintenance;

public:
    MaintenanceStream(
        const wt474_messages::v1::Maintenance& maintenance)
        : maintenance(maintenance) {};

    friend std::ostream& operator<<(
        std::ostream& os, const MaintenanceStream& s)
    {
        os << "Maintenance(";

        /* maintenance: derived_state */
        os << "req=" << upsf_maintenance_req_to_name(s.maintenance.maintenance_req());

        os << ")"; // Metadata

        return os;
    };
};

class VtepStream {
    const wt474_messages::v1::Vtep& vtep;

public:
    VtepStream(
        const wt474_messages::v1::Vtep& vtep)
        : vtep(vtep) {};

    friend std::ostream& operator<<(
        std::ostream& os, const VtepStream& s)
    {
        os << "Vtep(";

        /* vtep: ip_address */
        os << "ip_address=" << s.vtep.ip_address().c_str() << ", ";

        /* vtep: udp_port */
        os << "udp_port=" << s.vtep.udp_port() << ", ";

        /* vtep: vni */
        os << "vni=" << s.vtep.vni();

        os << ")"; // Vtep

        return os;
    };
};

class PortVlanStream {
    const wt474_messages::v1::PortVlan& port_vlan;

public:
    PortVlanStream(
        const wt474_messages::v1::PortVlan& port_vlan)
        : port_vlan(port_vlan) {};

    friend std::ostream& operator<<(
        std::ostream& os, const PortVlanStream& s)
    {
        os << "PortVlan(";

        /* port_vlan: logical_port */
        os << "logical_port=" << s.port_vlan.logical_port().c_str() << ", ";

        /* port_vlan: svlan */
        os << "svlan=" << s.port_vlan.svlan() << ", ";

        /* port_vlan: cvlan */
        os << "cvlan=" << s.port_vlan.cvlan();

        os << ")"; // PortVlan

        return os;
    };
};

class L2vpnStream {
    const wt474_messages::v1::L2vpn& l2vpn;

public:
    L2vpnStream(
        const wt474_messages::v1::L2vpn& l2vpn)
        : l2vpn(l2vpn) {};

    friend std::ostream& operator<<(
        std::ostream& os, const L2vpnStream& s)
    {
        os << "L2vpn(";

        /* l2vpn: vpn_id */
        os << "vpn_id=" << s.l2vpn.vpn_id();

        os << ")"; // L2vpn

        return os;
    };
};

class NetworkConnectionSpecEndpointStream {
    const wt474_messages::v1::NetworkConnection::Spec::Endpoint& endpoint;

public:
    NetworkConnectionSpecEndpointStream(
        const wt474_messages::v1::NetworkConnection::Spec::Endpoint& endpoint)
        : endpoint(endpoint) {};

    friend std::ostream& operator<<(
        std::ostream& os, const NetworkConnectionSpecEndpointStream& s)
    {
        os << "Endpoint(";

        /* network_connection: spec: endpoint: endpoint_name */
        os << "name=" << s.endpoint.endpoint_name().c_str() << ", ";

        if (s.endpoint.has_vtep()) {
            /* network_connection: spec: endpoint: vtep */
            os << VtepStream(s.endpoint.vtep());
        } else if (s.endpoint.has_l2vpn()) {
            /* network_connection: spec: endpoint: l2vpn */
            os << L2vpnStream(s.endpoint.l2vpn());
        } else if (s.endpoint.has_port_vlan()) {
            /* network_connection: spec: endpoint: port_vlan */
            os << PortVlanStream(s.endpoint.port_vlan());
        }

        os << ")"; // NetworkConnectionSpecEndpoint

        return os;
    };
};

class NetworkConnectionSpecSsPtpSpecStream {
    const wt474_messages::v1::NetworkConnection::Spec::SsPtpSpec& spec;

public:
    NetworkConnectionSpecSsPtpSpecStream(
        const wt474_messages::v1::NetworkConnection::Spec::SsPtpSpec& spec)
        : spec(spec) {};

    friend std::ostream& operator<<(
        std::ostream& os, const NetworkConnectionSpecSsPtpSpecStream& s)
    {
        os << "SsPtpSpec(";

        /* network_connection: spec: ss_ptp_spec: sgup_endpoint */
        os << "SgUpEndpoints(";
        for (int i = 0; i < s.spec.sgup_endpoint_size(); i++) {
            os << "[" << i << "]" << NetworkConnectionSpecEndpointStream(s.spec.sgup_endpoint(i)) << ", ";
        }
        os << "), ";

        /* network_connection: spec: ss_ptp_spec: tsf_endpoint */
        os << "TsfEndpoint(" << NetworkConnectionSpecEndpointStream(s.spec.tsf_endpoint()) << ")";

        os << ")"; // end of SsPtpSpec

        return os;
    };
};

class NetworkConnectionSpecSsMptpSpecStream {
    const wt474_messages::v1::NetworkConnection::Spec::SsMptpSpec& spec;

public:
    NetworkConnectionSpecSsMptpSpecStream(
        const wt474_messages::v1::NetworkConnection::Spec::SsMptpSpec& spec)
        : spec(spec) {};

    friend std::ostream& operator<<(
        std::ostream& os, const NetworkConnectionSpecSsMptpSpecStream& s)
    {
        os << "SsMptpSpec(";

        /* network_connection: spec: ss_mptp_spec: sgup_endpoint */
        os << "SgUpEndpoints(";
        for (int i = 0; i < s.spec.sgup_endpoint_size(); i++) {
            os << "[" << i << "]" << NetworkConnectionSpecEndpointStream(s.spec.sgup_endpoint(i)) << ", ";
        }
        os << "), ";

        /* network_connection: spec: ss_mptp_spec: tsf_endpoint */
        os << "TsfEndpoints(";
        for (int i = 0; i < s.spec.tsf_endpoint_size(); i++) {
            os << "[" << i << "]" << NetworkConnectionSpecEndpointStream(s.spec.tsf_endpoint(i)) << ", ";
        }
        os << ")";

        os << ")"; // end of SsMptpSpec

        return os;
    };
};

class NetworkConnectionSpecMsPtpSpecStream {
    const wt474_messages::v1::NetworkConnection::Spec::MsPtpSpec& spec;

public:
    NetworkConnectionSpecMsPtpSpecStream(
        const wt474_messages::v1::NetworkConnection::Spec::MsPtpSpec& spec)
        : spec(spec) {};

    friend std::ostream& operator<<(
        std::ostream& os, const NetworkConnectionSpecMsPtpSpecStream& s)
    {
        os << "MsPtpSpec(";

        /* network_connection: spec: ms_ptp_spec: sgup_endpoint */
        os << "SgUpEndpoint(" << NetworkConnectionSpecEndpointStream(s.spec.sgup_endpoint()) << "), ";

        /* network_connection: spec: ms_ptp_spec: tsf_endpoint */
        os << "TsfEndpoint(" << NetworkConnectionSpecEndpointStream(s.spec.tsf_endpoint()) << ")";

        os << ")"; // end of MsPtpSpec

        return os;
    };
};

class NetworkConnectionSpecMsMptpSpecStream {
    const wt474_messages::v1::NetworkConnection::Spec::MsMptpSpec& spec;

public:
    NetworkConnectionSpecMsMptpSpecStream(
        const wt474_messages::v1::NetworkConnection::Spec::MsMptpSpec& spec)
        : spec(spec) {};

    friend std::ostream& operator<<(
        std::ostream& os, const NetworkConnectionSpecMsMptpSpecStream& s)
    {
        os << "MsMptpSpec(";

        /* network_connection: spec: ms_mptp_spec: sgup_endpoint */
        os << "SgUpEndpoint(" << NetworkConnectionSpecEndpointStream(s.spec.sgup_endpoint()) << "), ";

        /* network_connection: spec: ms_mptp_spec: tsf_endpoint */
        os << "TsfEndpoints(";
        for (int i = 0; i < s.spec.tsf_endpoint_size(); i++) {
            os << "[" << i << "]" << NetworkConnectionSpecEndpointStream(s.spec.tsf_endpoint(i)) << ", ";
        }
        os << ")";

        os << ")"; // end of MsMptpSpec

        return os;
    };
};

class NetworkConnectionSpecStream {
    const wt474_messages::v1::NetworkConnection::Spec& spec;

public:
    NetworkConnectionSpecStream(
        const wt474_messages::v1::NetworkConnection::Spec& spec)
        : spec(spec) {};

    friend std::ostream& operator<<(
        std::ostream& os, const NetworkConnectionSpecStream& s)
    {
        os << "Spec(";

        /* network_connection: spec: maximum_supported_quality */
        os << "max_supp_quality=" << s.spec.maximum_supported_quality() << ", ";

        if (s.spec.has_ss_ptp()) {
            /* network_connection: spec: ss_ptp_spec */
            os << "SsPtp=" << NetworkConnectionSpecSsPtpSpecStream(s.spec.ss_ptp());
        } else if (s.spec.has_ss_mptpc()) {
            /* network_connection: spec: ss_mptp_spec */
            os << "SsMptp=" << NetworkConnectionSpecSsMptpSpecStream(s.spec.ss_mptpc());
        } else if (s.spec.has_ms_ptp()) {
            /* network_connection: spec: ms_ptp_spec */
            os << "MsPtp=" << NetworkConnectionSpecMsPtpSpecStream(s.spec.ms_ptp());
        } else if (s.spec.has_ms_mptp()) {
            /* network_connection: spec: ms_mptp_spec */
            os << "MsMptp=" << NetworkConnectionSpecMsMptpSpecStream(s.spec.ms_mptp());
        }

        os << ")"; // NetworkConnectionSpec

        return os;
    };
};

class NetworkConnectionStatusStream {
    const wt474_messages::v1::NetworkConnection::Status& status;

public:
    NetworkConnectionStatusStream(
        const wt474_messages::v1::NetworkConnection::Status& status)
        : status(status) {};

    friend std::ostream& operator<<(
        std::ostream& os, const NetworkConnectionStatusStream& s)
    {
        os << "Status(";

        /* network_connection: status: nc_active */
        os << "nc_active(";
        for (auto it : s.status.nc_active()) {
            os << it.first.c_str() << ":" << it.second << ", ";
        }
        os << "), ";

        /* network_connection: status: allocated_shards */
        os << "allocated_shards=" << s.status.allocated_shards() << ", ";

        os << ")"; // NetworkConnectionStatus

        return os;
    };
};

class NetworkConnectionStream {
    const wt474_messages::v1::NetworkConnection& network_connection;

public:
    NetworkConnectionStream(
        const wt474_messages::v1::NetworkConnection& network_connection)
        : network_connection(network_connection) {};

    friend std::ostream& operator<<(
        std::ostream& os, const NetworkConnectionStream& s)
    {
        os << "NetworkConnection(";

        /* network_connection: name */
        os << "name=" << s.network_connection.name().c_str() << ", ";

        /* network_connection: metadata */
        os << MetaDataStream(s.network_connection.metadata()) << ", ";

        /* network_connection: spec */
        os << NetworkConnectionSpecStream(s.network_connection.spec()) << ", ";

        /* network_connection: status */
        os << NetworkConnectionStatusStream(s.network_connection.status());

        os << ")"; // NetworkConnection

        return os;
    };
};

class TrafficSteeringFunctionSpecStream {
    const wt474_messages::v1::TrafficSteeringFunction::Spec& spec;

public:
    TrafficSteeringFunctionSpecStream(
        const wt474_messages::v1::TrafficSteeringFunction::Spec& spec)
        : spec(spec) {};

    friend std::ostream& operator<<(
        std::ostream& os, const TrafficSteeringFunctionSpecStream& s)
    {
        os << "Spec(";

        /* traffic_steering_function: spec: default_endpoint */
        os << "default_endpoint=" << NetworkConnectionSpecEndpointStream(s.spec.default_endpoint());

        os << ")"; // TrafficSteeringFunctionSpec

        return os;
    };
};

class TrafficSteeringFunctionStream {
    const wt474_messages::v1::TrafficSteeringFunction& traffic_steering_function;

public:
    TrafficSteeringFunctionStream(
        const wt474_messages::v1::TrafficSteeringFunction& traffic_steering_function)
        : traffic_steering_function(traffic_steering_function) {};

    friend std::ostream& operator<<(
        std::ostream& os, const TrafficSteeringFunctionStream& s)
    {
        os << "TrafficSteeringFunction(";

        /* traffic_steering_function: name */
        os << "name=" << s.traffic_steering_function.name().c_str() << ", ";

        /* traffic_steering_function: metadata */
        os << MetaDataStream(s.traffic_steering_function.metadata()) << ", ";

        /* traffic_steering_function: spec */
        os << TrafficSteeringFunctionSpecStream(s.traffic_steering_function.spec());

        os << ")"; // TrafficSteeringFunction

        return os;
    };
};

class ServiceGatewayStream {
    const wt474_messages::v1::ServiceGateway& service_gateway;

public:
    ServiceGatewayStream(
        const wt474_messages::v1::ServiceGateway& service_gateway)
        : service_gateway(service_gateway) {};

    friend std::ostream& operator<<(
        std::ostream& os, const ServiceGatewayStream& s)
    {
        os << "ServiceGateway(";
        /* service_gateway: name */
        os << "name=" << s.service_gateway.name().c_str() << ", ";

        /* service_gateway: metadata */
        os << MetaDataStream(s.service_gateway.metadata());

        os << ")"; // ServiceGateway

        return os;
    };
};

class ServiceGatewayUserPlaneSpecStream {
    const wt474_messages::v1::ServiceGatewayUserPlane::Spec& spec;

public:
    ServiceGatewayUserPlaneSpecStream(
        const wt474_messages::v1::ServiceGatewayUserPlane::Spec& spec)
        : spec(spec) {};

    friend std::ostream& operator<<(
        std::ostream& os, const ServiceGatewayUserPlaneSpecStream& s)
    {
        os << "Spec(";

        /* service_gateway_user_plane: spec: max_session_count */
        os << "max_session_count=" << s.spec.max_session_count() << ", ";

        /* service_gateway_user_plane: spec: max_shards */
        os << "max_shards=" << s.spec.max_shards() << ", ";

        /* service_gateway_user_plane: spec: supported_service_group */
        os << "SupportedServiceGroup(";
        for (int i = 0; i < s.spec.supported_service_group_size(); i++) {
            os << "[" << i << "]" << s.spec.supported_service_group(i).c_str() << ", ";
        }
        os << "), ";

        /* service_gateway_user_plane: spec: default_endpoint */
        os << "default_endpoint=" << NetworkConnectionSpecEndpointStream(s.spec.default_endpoint());

        os << ")"; // ServiceGatewayUserPlaneSpec

        return os;
    };
};

class ServiceGatewayUserPlaneStatusStream {
    const wt474_messages::v1::ServiceGatewayUserPlane::Status& status;

public:
    ServiceGatewayUserPlaneStatusStream(
        const wt474_messages::v1::ServiceGatewayUserPlane::Status& status)
        : status(status) {};

    friend std::ostream& operator<<(
        std::ostream& os, const ServiceGatewayUserPlaneStatusStream& s)
    {
        os << "Status(";

        /* service_gateway_user_plane: status: allocated_session_count */
        os << "allocated_session_count=" << s.status.allocated_session_count() << ", ";

        /* service_gateway_user_plane: status: allocated_shards */
        os << "allocated_shards=" << s.status.allocated_shards();

        os << ")"; // ServiceGatewayUserPlaneStatus

        return os;
    };
};

class ServiceGatewayUserPlaneStream {
    const wt474_messages::v1::ServiceGatewayUserPlane& service_gateway_user_plane;

public:
    ServiceGatewayUserPlaneStream(
        const wt474_messages::v1::ServiceGatewayUserPlane& service_gateway_user_plane)
        : service_gateway_user_plane(service_gateway_user_plane) {};

    friend std::ostream& operator<<(
        std::ostream& os, const ServiceGatewayUserPlaneStream& s)
    {
        os << "ServiceGatewayUserPlane(";

        /* service_gateway_user_plane: name */
        os << "name=" << s.service_gateway_user_plane.name().c_str() << ", ";

        /* service_gateway_user_plane: service_gateway_name */
        os << "sg_name=" << s.service_gateway_user_plane.service_gateway_name().c_str() << ", ";

        /* service_gateway_user_plane: metadata */
        os << MetaDataStream(s.service_gateway_user_plane.metadata()) << ", ";

        /* service_gateway_user_plane: maintenance */
        os << MaintenanceStream(s.service_gateway_user_plane.maintenance()) << ", ";

        /* service_gateway_user_plane: spec */
        os << ServiceGatewayUserPlaneSpecStream(s.service_gateway_user_plane.spec()) << ", ";

        /* service_gateway_user_plane: status */
        os << ServiceGatewayUserPlaneStatusStream(s.service_gateway_user_plane.status());

        os << ")"; // ServiceGatewayUserPlane

        return os;
    };
};

class ShardSpecDesiredStateStream {
    const wt474_messages::v1::Shard::Spec::DesiredState& desired_state;

public:
    ShardSpecDesiredStateStream(
        const wt474_messages::v1::Shard::Spec::DesiredState& desired_state)
        : desired_state(desired_state) {};

    friend std::ostream& operator<<(
        std::ostream& os, const ShardSpecDesiredStateStream& s)
    {
        os << "DesiredState(";

        /* shard: spec: desired_state: service_gateway_user_plane */
        os << "sgup=" << s.desired_state.service_gateway_user_plane().c_str() << ", ";

        /* shard: spec: desired_state: network_connection */
        os << "NetworkConnection=";
        for (int i = 0; i < s.desired_state.network_connection_size(); i++) {
            os << "[" << i << "]" << s.desired_state.network_connection(i).c_str() << ", ";
        }
        os << ")";

        os << ")"; // DesiredState

        return os;
    };
};

class ShardSpecStream {
    const wt474_messages::v1::Shard::Spec& spec;

public:
    ShardSpecStream(
        const wt474_messages::v1::Shard::Spec& spec)
        : spec(spec) {};

    friend std::ostream& operator<<(
        std::ostream& os, const ShardSpecStream& s)
    {
        os << "Spec(";

        /* shard: spec: max_session_count */
        os << "max_session_count=" << s.spec.max_session_count() << ", ";

        /* shard: spec: virtual_mac */
        os << "virtual_mac=" << s.spec.virtual_mac().c_str() << ", ";

        /* shard: spec: desired_state */
        os << ShardSpecDesiredStateStream(s.spec.desired_state()) << ", ";

        /* shard: spec: prefix */
        os << "Prefix=";
        for (int i = 0; i < s.spec.prefix_size(); i++) {
            os << "[" << i << "]" << s.spec.prefix(i).c_str() << ", ";
        }
        os << ")";

        os << ")"; // Spec

        return os;
    };
};

class ShardStatusCurrentStateStream {
    const wt474_messages::v1::Shard::Status::CurrentState& current_state;

public:
    ShardStatusCurrentStateStream(
        const wt474_messages::v1::Shard::Status::CurrentState& current_state)
        : current_state(current_state) {};

    friend std::ostream& operator<<(
        std::ostream& os, const ShardStatusCurrentStateStream& s)
    {
        os << "CurrentState(";

        /* shard: status: current_state: service_gateway_user_plane */
        os << "sgup=" << s.current_state.service_gateway_user_plane().c_str() << ", ";

        /* shard: status: current_state: tsf_network_connection */
        os << "TsfNetworkConnection=";
        for (auto it : s.current_state.tsf_network_connection()) {
            os << it.first.c_str() << ":" << it.second.c_str() << ", ";
        }
        os << ")";

        os << ")"; // CurrentState

        return os;
    };
};

class ShardStatusStream {
    const wt474_messages::v1::Shard::Status& status;

public:
    ShardStatusStream(
        const wt474_messages::v1::Shard::Status& status)
        : status(status) {};

    friend std::ostream& operator<<(
        std::ostream& os, const ShardStatusStream& s)
    {
        os << "Status(";

        /* shard: status: allocated_session_count */
        os << "allocated_session_count=" << s.status.allocated_session_count() << ", ";

        /* shard: status: maximum_allocated_quality */
        os << "maximum_allocated_quality=" << s.status.maximum_allocated_quality() << ", ";

        /* shard: status: current_state */
        os << ShardStatusCurrentStateStream(s.status.current_state());

        os << ")"; // Status

        return os;
    };
};

class ShardMbbStream {
    const wt474_messages::v1::Shard::Mbb& mbb;

public:
    ShardMbbStream(
        const wt474_messages::v1::Shard::Mbb& mbb)
        : mbb(mbb) {};

    friend std::ostream& operator<<(
        std::ostream& os, const ShardMbbStream& s)
    {
        os << "Mbb(";

        /* shard: mbb: mbb_state */
        os << "State=" << upsf_mbb_state_to_name(s.mbb.mbb_state());

        os << ")"; // Mbb

        return os;
    };
};

class ShardStream {
    const wt474_messages::v1::Shard& shard;

public:
    ShardStream(
        const wt474_messages::v1::Shard& shard)
        : shard(shard) {};

    friend std::ostream& operator<<(
        std::ostream& os, const ShardStream& s)
    {
        os << "Shard(";

        /* shard: name */
        os << "name=" << s.shard.name().c_str() << ", ";

        /* shard: metadata */
        os << MetaDataStream(s.shard.metadata()) << ", ";

        /* shard: spec */
        os << ShardSpecStream(s.shard.spec()) << ", ";

        /* shard: status */
        os << ShardStatusStream(s.shard.status()) << ", ";

        /* shard: mbb */
        os << ShardMbbStream(s.shard.mbb());

        os << ")"; // Shard

        return os;
    };
};

class SessionFilterStream {
    const wt474_messages::v1::SessionFilter& session_filter;

public:
    SessionFilterStream(
        const wt474_messages::v1::SessionFilter& session_filter)
        : session_filter(session_filter) {};

    friend std::ostream& operator<<(
        std::ostream& os, const SessionFilterStream& s)
    {
        os << "SessionFilter(";

        /* session_filter: source_mac_address */
        os << "smac=" << s.session_filter.source_mac_address().c_str() << ", ";

        /* session_filter: svlan */
        os << "svlan=" << s.session_filter.svlan() << ", ";

        /* session_filter: cvlan */
        os << "cvlan=" << s.session_filter.cvlan();

        os << ")"; // SessionFilter

        return os;
    };
};

class SessionContextSpecDesiredStateStream {
    const wt474_messages::v1::SessionContext::Spec::DesiredState& desired_state;

public:
    SessionContextSpecDesiredStateStream(
        const wt474_messages::v1::SessionContext::Spec::DesiredState& desired_state)
        : desired_state(desired_state) {};

    friend std::ostream& operator<<(
        std::ostream& os, const SessionContextSpecDesiredStateStream& s)
    {
        os << "DesiredState(";

        /* session_context: spec: desired_state: shard */
        os << "shard=" << s.desired_state.shard().c_str() << ", ";

        os << ")"; // DesiredState

        return os;
    };
};

class SessionContextSpecStream {
    const wt474_messages::v1::SessionContext::Spec& spec;

public:
    SessionContextSpecStream(
        const wt474_messages::v1::SessionContext::Spec& spec)
        : spec(spec) {};

    friend std::ostream& operator<<(
        std::ostream& os, const SessionContextSpecStream& s)
    {
        os << "Spec(";

        /* session_context: spec: traffic_steering_function */
        os << "tsf=" << s.spec.traffic_steering_function().c_str() << ", ";

        /* session_context: spec: required_service_group */
        os << "RequiredServiceGroup(";
        for (int i = 0; i < s.spec.required_service_group_size(); i++) {
            os << "[" << i << "]" << s.spec.required_service_group(i).c_str() << ", ";
        }
        os << "), ";

        /* session_context: spec: required_quality */
        os << "required_quality=" << s.spec.required_quality() << ", ";

        /* session_context: spec: circuit_id */
        os << "circuit_id=" << s.spec.circuit_id().c_str() << ", ";

        /* session_context: spec: remote_id */
        os << "remote_id=" << s.spec.remote_id().c_str() << ", ";

        /* session_context: spec: session_filter */
        os << SessionFilterStream(s.spec.session_filter()) << ", ";

        /* session_context: spec: desired_state */
        os << SessionContextSpecDesiredStateStream(s.spec.desired_state());

        os << ")"; // Spec

        return os;
    };
};

class SessionContextStatusCurrentStateStream {
    const wt474_messages::v1::SessionContext::Status::CurrentState& current_state;

public:
    SessionContextStatusCurrentStateStream(
        const wt474_messages::v1::SessionContext::Status::CurrentState& current_state)
        : current_state(current_state) {};

    friend std::ostream& operator<<(
        std::ostream& os, const SessionContextStatusCurrentStateStream& s)
    {
        os << "CurrentState(";

        /* session_context: status: current_state: user_plane_shard */
        os << "up_shard=" << s.current_state.user_plane_shard().c_str() << ", ";

        /* session_context: status: current_state: tsf_shard */
        os << "tsf_shard=" << s.current_state.tsf_shard().c_str() << ", ";

        os << ")"; // CurrentState

        return os;
    };
};

class SessionContextStatusStream {
    const wt474_messages::v1::SessionContext::Status& status;

public:
    SessionContextStatusStream(
        const wt474_messages::v1::SessionContext::Status& status)
        : status(status) {};

    friend std::ostream& operator<<(
        std::ostream& os, const SessionContextStatusStream& s)
    {
        os << "Status(";

        /* session_context: status: current_state */
        os << SessionContextStatusCurrentStateStream(s.status.current_state());

        os << ")"; // Status

        return os;
    };
};

class SessionContextStream {
    const wt474_messages::v1::SessionContext& session_context;

public:
    SessionContextStream(
        const wt474_messages::v1::SessionContext& session_context)
        : session_context(session_context) {};

    friend std::ostream& operator<<(
        std::ostream& os, const SessionContextStream& s)
    {
        os << "SessionContext(";

        /* session_context: name */
        os << "name=" << s.session_context.name().c_str() << ", ";

        /* session_context: metadata */
        os << MetaDataStream(s.session_context.metadata()) << ", ";

        /* session_context: spec */
        os << SessionContextSpecStream(s.session_context.spec()) << ", ";

        /* session_context: status */
        os << SessionContextStatusStream(s.session_context.status());

        os << ")"; // SessionContext

        return os;
    };
};

}; // end namespace upsf

#endif
