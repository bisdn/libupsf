#include "upsf_c_mapping.hpp"

using namespace upsf;

bool
UpsfMapping::map(
    const wt474_messages::v1::ServiceGateway& from,
    upsf_service_gateway_t& to)
{
    /* service_gateway: init */
    memset(&to, 0, sizeof(to));

    /* service_gateway: name */
    strncpy(
        to.name.str,
        from.name().c_str(),
        sizeof(to.name.str) - 1);
    to.name.len = strlen(to.name.str);

    /* service_gateway: metadata: description */
    strncpy(
        to.metadata.description.str,
        from.metadata().description().c_str(),
        sizeof(to.metadata.description.str) - 1);
    to.metadata.description.len = strlen(to.metadata.description.str);

    /* service_gateway: metadata: derived_state */
    to.metadata.derived_state = (upsf_derived_state_t)from.metadata().derived_state();

    return true;
}

bool
UpsfMapping::map(
    const upsf_service_gateway_t& from,
    wt474_messages::v1::ServiceGateway& to)
{
    /* service_gateway: name */
    to.set_name(std::string(from.name.str));

    /* service_gateway: metadata: description */
    to.mutable_metadata()->set_description(
        std::string(
            from.metadata.description.str));

    /* service_gateway: metadata: derived_state */
    to.mutable_metadata()->set_derived_state(
        wt474_messages::v1::DerivedState(
            from.metadata.derived_state));

    return true;
}

bool
UpsfMapping::map(
    const wt474_messages::v1::ServiceGatewayUserPlane& from,
    upsf_service_gateway_user_plane_t& to)
{
    /* service_gateway_user_plane: init */
    memset(&to, 0, sizeof(to));

    /* service_gateway_user_plane: name */
    strncpy(
        to.name.str,
        from.name().c_str(),
        sizeof(to.name.str) - 1);
    to.name.len = strlen(to.name.str);

    /* service_gateway_user_plane: metadata: description */
    strncpy(
        to.metadata.description.str,
        from.metadata().description().c_str(),
        sizeof(to.metadata.description.str) - 1);
    to.metadata.description.len = strlen(to.metadata.description.str);

    /* service_gateway_user_plane: service_gateway_name */
    strncpy(
        to.service_gateway_name.str,
        from.service_gateway_name().c_str(),
        sizeof(to.service_gateway_name.str) - 1);
    to.service_gateway_name.len = strlen(to.service_gateway_name.str);

    /* service_gateway_user_plane: metadata: derived_state */
    to.metadata.derived_state = (upsf_derived_state_t)from.metadata().derived_state();

    /* service_gateway_user_plane: maintenance: derived_state */
    to.maintenance.maintenance_req = (upsf_maintenance_req_t)from.maintenance().maintenance_req();

    /* service_gateway_user_plane: spec */
    UpsfMapping::map(from.spec(), to.spec);

    /* service_gateway_user_plane: status */
    UpsfMapping::map(from.status(), to.status);

    return true;
}

bool
UpsfMapping::map(
    const upsf_service_gateway_user_plane_t& from,
    wt474_messages::v1::ServiceGatewayUserPlane& to)
{
    /* service_gateway_user_plane: name */
    to.set_name(std::string(from.name.str));

    /* service_gateway_user_plane: metadata: description */
    to.mutable_metadata()->set_description(
        std::string(
            from.metadata.description.str));

    /* service_gateway_user_plane: metadata: derived_state */
    to.mutable_metadata()->set_derived_state(
        wt474_messages::v1::DerivedState(
            from.metadata.derived_state));

    /* service_gateway_user_plane: service_gateway_name */
    to.set_service_gateway_name(std::string(from.service_gateway_name.str));

    /* service_gateway_user_plane: maintenance: maintenance_req */
    to.mutable_maintenance()->set_maintenance_req(
        wt474_messages::v1::Maintenance::MaintenanceReq(
            from.maintenance.maintenance_req));

    /* service_gateway_user_plane: spec */
    UpsfMapping::map(from.spec, *to.mutable_spec());

    /* service_gateway_user_plane: status */
    UpsfMapping::map(from.status, *to.mutable_status());

    return true;
}

bool
UpsfMapping::map(
    const wt474_messages::v1::ServiceGatewayUserPlane::Spec& from,
    upsf_service_gateway_user_plane_spec_t& to)
{
    /* service_gateway_user_plane: spec: init */
    memset(&to, 0, sizeof(to));
 
    /* service_gateway_user_plane: spec: max_session_count */
    to.max_session_count = from.max_session_count();

    /* service_gateway_user_plane: spec: max_shards */
    to.max_shards = from.max_shards();

    /* service_gateway_user_plane: spec: supported_service_group */
    to.supported_service_group_size = from.supported_service_group_size();
    for (int i = 0; i < to.supported_service_group_size; i++) {
        strncpy(
            to.supported_service_group[i].str,
            from.supported_service_group(i).c_str(),
            sizeof(to.supported_service_group[i].str) - 1);
        to.supported_service_group[i].len = strlen(to.supported_service_group[i].str);
    }

    /* service_gateway_user_plane: spec: default_endpoint: endpoint_name */
    strncpy(
        to.default_endpoint.endpoint_name.str,
        from.default_endpoint().endpoint_name().c_str(),
        sizeof(to.default_endpoint.endpoint_name.str) - 1);
    to.default_endpoint.endpoint_name.len = strlen(to.default_endpoint.endpoint_name.str);

    /* service_gateway_user_plane: spec: default_endpoint: ep_spec: vtep: udp_port */
    /* TODO: add all endpoint types*/
    to.default_endpoint.ep_spec.vtep.udp_port = from.default_endpoint().vtep().udp_port();

    /* service_gateway_user_plane: spec: default_endpoint: ep_spec: vtep: vni: */
    to.default_endpoint.ep_spec.vtep.vni = from.default_endpoint().vtep().vni();

    /* service_gateway_user_plane: spec: default_endpoint: ep_spec: vtep: ip_address: */
    strncpy(
        to.default_endpoint.ep_spec.vtep.ip_address.str,
        from.default_endpoint().vtep().ip_address().c_str(),
        sizeof(to.default_endpoint.ep_spec.vtep.ip_address.str) -1);
    to.default_endpoint.ep_spec.vtep.ip_address.len = strlen(to.default_endpoint.ep_spec.vtep.ip_address.str);

    return true;
}

bool
UpsfMapping::map(
    const upsf_service_gateway_user_plane_spec_t& from,
    wt474_messages::v1::ServiceGatewayUserPlane::Spec& to)
{
    /* service_gateway_user_plane: spec: max_session_count */
    to.set_max_session_count(from.max_session_count);

    /* service_gateway_user_plane: spec: max_shards */
    to.set_max_shards(from.max_shards);

    /* service_gateway_user_plane: spec: supported_service_group */
    for (int i = 0; i < from.supported_service_group_size; i++) {
        to.add_supported_service_group(
            std::string(
                from.supported_service_group[i].str));
    }

    /* service_gateway_user_plane: spec: default_endpoint: endpoint_name */
    to.mutable_default_endpoint()->set_endpoint_name(
        from.default_endpoint.endpoint_name.str);

    /* service_gateway_user_plane: spec: default_endpoint: ep_spec: vtep: ip_address: */
    /* TODO: add all endpoint types*/
    to.mutable_default_endpoint()->mutable_vtep()->set_ip_address(
        from.default_endpoint.ep_spec.vtep.ip_address.str);

    /* service_gateway_user_plane: spec: default_endpoint: ep_spec: vtep: udp_port */
    to.mutable_default_endpoint()->mutable_vtep()->set_udp_port(
        from.default_endpoint.ep_spec.vtep.udp_port);

    /* service_gateway_user_plane: spec: default_endpoint: ep_spec: vtep: vni: */
    to.mutable_default_endpoint()->mutable_vtep()->set_vni(
      from.default_endpoint.ep_spec.vtep.vni);

    return true;
}

bool
UpsfMapping::map(
    const wt474_messages::v1::ServiceGatewayUserPlane::Status& from,
    upsf_service_gateway_user_plane_status_t& to)
{
    /* service_gateway_user_plane: status: init */
    memset(&to, 0, sizeof(to));
 
    /* service_gateway_user_plane: status: allocated_session_count */
    to.allocated_session_count = from.allocated_session_count();

    /* service_gateway_user_plane: status: allocated_shards */
    to.allocated_shards = from.allocated_shards();

    return true;
}

bool
UpsfMapping::map(
    const upsf_service_gateway_user_plane_status_t& from,
    wt474_messages::v1::ServiceGatewayUserPlane::Status& to)
{
    /* service_gateway_user_plane: status: allocated_session_count */
    to.set_allocated_session_count(
        from.allocated_session_count);

    /* service_gateway_user_plane: status: allocated_shards */
    to.set_allocated_shards(
        from.allocated_shards);

    return true;
}

bool
UpsfMapping::map(
    const wt474_messages::v1::TrafficSteeringFunction& from,
    upsf_traffic_steering_function_t& to)
{
    /* traffic_steering_function: init */
    memset(&to, 0, sizeof(to));

    /* traffic_steering_function: name */
    strncpy(
        to.name.str,
        from.name().c_str(),
        sizeof(to.name.str) - 1);
    to.name.len = strlen(to.name.str);

    /* traffic_steering_function: metadata: description */
    strncpy(
        to.metadata.description.str,
        from.metadata().description().c_str(),
        sizeof(to.metadata.description.str) - 1);
    to.metadata.description.len = strlen(to.metadata.description.str);

    /* traffic_steering_function: metadata: derived_state */
    to.metadata.derived_state = (upsf_derived_state_t)from.metadata().derived_state();

    /* traffic_steering_function: spec */
    UpsfMapping::map(from.spec(), to.spec);

    return true;
}

bool
UpsfMapping::map(
    const upsf_traffic_steering_function_t& from,
    wt474_messages::v1::TrafficSteeringFunction& to)
{
    /* traffic_steering_function: name */
    to.set_name(std::string(from.name.str));

    /* traffic_steering_function: metadata: description */
    to.mutable_metadata()->set_description(
        std::string(
            from.metadata.description.str));

    /* traffic_steering_function: metadata: derived_state */
    to.mutable_metadata()->set_derived_state(
        wt474_messages::v1::DerivedState(
            from.metadata.derived_state));

    /* traffic_steering_function: spec */
    UpsfMapping::map(from.spec, *to.mutable_spec());

    return true;
}

bool
UpsfMapping::map(
    const wt474_messages::v1::TrafficSteeringFunction::Spec& from,
    upsf_traffic_steering_function_spec_t& to)
{
    /* traffic_steering_function: spec: init */
    memset(&to, 0, sizeof(to));

    /* traffic_steering_function: spec: default_endpoint: endpoint_name */
    strncpy(
        to.default_endpoint.endpoint_name.str,
        from.default_endpoint().endpoint_name().c_str(),
        sizeof(to.default_endpoint.endpoint_name.str) - 1);
    to.default_endpoint.endpoint_name.len = strlen(to.default_endpoint.endpoint_name.str);

    /* traffic_steering_function: spec: default_endpoint: ep_spec: vtep: udp_port */
    /* TODO: add all endpoint types*/
    to.default_endpoint.ep_spec.vtep.udp_port = from.default_endpoint().vtep().udp_port();

    /* traffic_steering_function: spec: default_endpoint: ep_spec: vtep: vni: */
    to.default_endpoint.ep_spec.vtep.vni = from.default_endpoint().vtep().vni();

    /* traffic_steering_function: spec: default_endpoint: ep_spec: vtep: ip_address: */
    strncpy(
        to.default_endpoint.ep_spec.vtep.ip_address.str,
        from.default_endpoint().vtep().ip_address().c_str(),
        sizeof(to.default_endpoint.ep_spec.vtep.ip_address.str) -1);
    to.default_endpoint.ep_spec.vtep.ip_address.len = strlen(to.default_endpoint.ep_spec.vtep.ip_address.str);

    return true;
}

bool
UpsfMapping::map(
    const upsf_traffic_steering_function_spec_t& from,
    wt474_messages::v1::TrafficSteeringFunction::Spec& to)
{
    /* traffic_steering_function: spec: default_endpoint: endpoint_name */
    to.mutable_default_endpoint()->set_endpoint_name(
        from.default_endpoint.endpoint_name.str);

    /* traffic_steering_function: spec: default_endpoint: ep_spec: vtep: ip_address: */
    /* TODO: add all endpoint types*/
    to.mutable_default_endpoint()->mutable_vtep()->set_ip_address(
        from.default_endpoint.ep_spec.vtep.ip_address.str);

    /* traffic_steering_function: spec: default_endpoint: ep_spec: vtep: udp_port */
    to.mutable_default_endpoint()->mutable_vtep()->set_udp_port(
        from.default_endpoint.ep_spec.vtep.udp_port);

    /* traffic_steering_function: spec: default_endpoint: ep_spec: vtep: vni: */
    to.mutable_default_endpoint()->mutable_vtep()->set_vni(
      from.default_endpoint.ep_spec.vtep.vni);

    return true;
}

bool
UpsfMapping::map(
    const wt474_messages::v1::NetworkConnection& from,
    upsf_network_connection_t& to)
{
    /* network_connection: init */
    memset(&to, 0, sizeof(to));

    /* network_connection: name */
    strncpy(
        to.name.str,
        from.name().c_str(),
        sizeof(to.name.str) - 1);
    to.name.len = strlen(to.name.str);

    /* network_connection: metadata: description */
    strncpy(
        to.metadata.description.str,
        from.metadata().description().c_str(),
        sizeof(to.metadata.description.str) - 1);
    to.metadata.description.len = strlen(to.metadata.description.str);

    /* network_connection: metadata: derived_state */
    to.metadata.derived_state = (upsf_derived_state_t)from.metadata().derived_state();

    /* network_connection: maintenance: maintenance_req */
    to.maintenance.maintenance_req = (upsf_maintenance_req_t)from.maintenance().maintenance_req();

    /* network_connection: spec */
    UpsfMapping::map(from.spec(), to.spec);

    /* network_connection: status */
    UpsfMapping::map(from.status(), to.status);

    return true;
}

bool
UpsfMapping::map(
    const upsf_network_connection_t& from,
    wt474_messages::v1::NetworkConnection& to)
{
    /* network_connection: name */
    to.set_name(std::string(from.name.str));

    /* network_connection: metadata: description */
    to.mutable_metadata()->set_description(
        std::string(
            from.metadata.description.str));

    /* network_connection: metadata: derived_state */
    to.mutable_metadata()->set_derived_state(
        wt474_messages::v1::DerivedState(
            from.metadata.derived_state));

    /* network_connection: maintenance: maintenance_req */
    to.mutable_maintenance()->set_maintenance_req(
        wt474_messages::v1::Maintenance::MaintenanceReq(
            from.maintenance.maintenance_req));

    /* network_connection: spec */
    UpsfMapping::map(from.spec, *to.mutable_spec());

    /* network_connection: status */
    UpsfMapping::map(from.status, *to.mutable_status());

    return true;
}

bool
UpsfMapping::map(
    const upsf_network_connection_spec_t& from,
    wt474_messages::v1::NetworkConnection::Spec& to)
{
    /* network_connection: spec: maximum_supported_quality */
    to.set_maximum_supported_quality(
        from.maximum_supported_quality);

    /* network_connection: spec: nc_spec:  */
    switch (from.nc_spec_type) {
    case UPSF_NC_SPEC_TYPE_SS_PTP:
        /* network_connection: spec: ss_ptp */

        /* network_connection: spec: ss_ptp: sgup_endpoint */
        for (int i = 0; i < from.nc_spec.ss_ptp.sgup_endpoint_size; i++) {
            UpsfMapping::map(
                from.nc_spec.ss_ptp.sgup_endpoint[i],
                *to.mutable_ss_ptp()->add_sgup_endpoint());
        }

        /* network_connection: spec: ss_ptp: tsf_endpoint */
        UpsfMapping::map(
            from.nc_spec.ss_ptp.tsf_endpoint,
            *to.mutable_ss_ptp()->mutable_tsf_endpoint());

        break;
    case UPSF_NC_SPEC_TYPE_SS_MPTP:
        /* network_connection: spec: ss_mptp */

        /* network_connection: spec: ss_mptp: sgup_endpoint */
        for (int i = 0; i < from.nc_spec.ss_mptp.sgup_endpoint_size; i++) {
            UpsfMapping::map(
                from.nc_spec.ss_ptp.sgup_endpoint[i],
                *to.mutable_ss_mptpc()->add_sgup_endpoint());
        }

        /* network_connection: spec: ss_mptp: tsf_endpoint */
        for (int i = 0; i < from.nc_spec.ss_mptp.tsf_endpoint_size; i++) {
            UpsfMapping::map(
                from.nc_spec.ss_mptp.tsf_endpoint[i],
                *to.mutable_ss_mptpc()->add_tsf_endpoint());
        }

        break;
    case UPSF_NC_SPEC_TYPE_MS_PTP:
        /* network_connection: spec: ms_ptp */

        /* network_connection: spec: ms_ptp: sgup_endpoint */
        UpsfMapping::map(
            from.nc_spec.ms_ptp.sgup_endpoint,
            *to.mutable_ms_ptp()->mutable_sgup_endpoint());

        /* network_connection: spec: ms_ptp: tsf_endpoint */
        UpsfMapping::map(
            from.nc_spec.ms_ptp.tsf_endpoint,
            *to.mutable_ms_ptp()->mutable_tsf_endpoint());

        break;
    case UPSF_NC_SPEC_TYPE_MS_MPTP:
        /* network_connection: spec: ms_mptp */

        /* network_connection: spec: ms_mptp: sgup_endpoint */
        UpsfMapping::map(
            from.nc_spec.ms_mptp.sgup_endpoint,
            *to.mutable_ms_mptp()->mutable_sgup_endpoint());

        /* network_connection: spec: ms_mptp: tsf_endpoint */
        for (int i = 0; i < from.nc_spec.ms_mptp.tsf_endpoint_size; i++) {
            UpsfMapping::map(
                from.nc_spec.ms_mptp.tsf_endpoint[i],
                *to.mutable_ms_mptp()->add_tsf_endpoint());
        }

        break;
    default:
        /* network_connection: spec: unknown */
        return false;
    }

    return true;
}

bool
UpsfMapping::map(
    const wt474_messages::v1::NetworkConnection::Spec& from,
    upsf_network_connection_spec_t& to)
{
    /* network_connection: init */
    memset(&to, 0, sizeof(to));

    /* network_connection: spec: maximum_supported_quality */
    to.maximum_supported_quality = from.maximum_supported_quality();

    /* network_connection: spec: ss_ptp */
    if (from.has_ss_ptp()) {

        to.nc_spec_type = UPSF_NC_SPEC_TYPE_SS_PTP;

        /* network_connection: spec: ss_ptp: sgup_endpoint */
        to.nc_spec.ss_ptp.sgup_endpoint_size = from.ss_ptp().sgup_endpoint_size();
        for (int i = 0; i < to.nc_spec.ss_ptp.sgup_endpoint_size; i++) {
            UpsfMapping::map(
                from.ss_ptp().sgup_endpoint(i),
                to.nc_spec.ss_ptp.sgup_endpoint[i]);
        }

        /* network_connection: spec: ss_ptp: tsf_endpoint */
        UpsfMapping::map(
            from.ss_ptp().tsf_endpoint(),
            to.nc_spec.ss_ptp.tsf_endpoint);

    /* network_connection: spec: ss_mptp */
    } else if (from.has_ss_mptpc()) {

        to.nc_spec_type = UPSF_NC_SPEC_TYPE_SS_MPTP;

        /* network_connection: spec: ss_mptp: sgup_endpoint */
        to.nc_spec.ss_mptp.sgup_endpoint_size = from.ss_mptpc().sgup_endpoint_size();
        for (int i = 0; i < to.nc_spec.ss_mptp.sgup_endpoint_size; i++) {
            UpsfMapping::map(
                from.ss_mptpc().sgup_endpoint(i),
                to.nc_spec.ss_mptp.sgup_endpoint[i]);
        }

        /* network_connection: spec: ss_mptp: tsf_endpoint */
        to.nc_spec.ss_mptp.tsf_endpoint_size = from.ss_mptpc().tsf_endpoint_size();
        for (int i = 0; i < to.nc_spec.ss_mptp.tsf_endpoint_size; i++) {
            UpsfMapping::map(
                from.ss_mptpc().tsf_endpoint(i),
                to.nc_spec.ss_mptp.tsf_endpoint[i]);
        }

    /* network_connection: spec: ms_ptp */
    } else if (from.has_ms_ptp()) {

        to.nc_spec_type = UPSF_NC_SPEC_TYPE_MS_PTP;

        /* network_connection: spec: ms_ptp: sgup_endpoint */
        UpsfMapping::map(
            from.ms_ptp().sgup_endpoint(),
            to.nc_spec.ms_ptp.sgup_endpoint);

        /* network_connection: spec: ms_ptp: tsf_endpoint */
        UpsfMapping::map(
            from.ms_ptp().tsf_endpoint(),
            to.nc_spec.ms_ptp.tsf_endpoint);

    /* network_connection: spec: ms_mptp */
    } else if (from.has_ms_mptp()) {

        to.nc_spec_type = UPSF_NC_SPEC_TYPE_MS_MPTP;

        /* network_connection: spec: ms_mptp: sgup_endpoint */
        UpsfMapping::map(
            from.ms_mptp().sgup_endpoint(),
            to.nc_spec.ms_mptp.sgup_endpoint);

        /* network_connection: spec: ms_mptp: tsf_endpoint */
        to.nc_spec.ms_mptp.tsf_endpoint_size = from.ms_mptp().tsf_endpoint_size();
        for (int i = 0; i < to.nc_spec.ms_mptp.tsf_endpoint_size; i++) {
            UpsfMapping::map(
                from.ms_mptp().tsf_endpoint(i),
                to.nc_spec.ms_mptp.tsf_endpoint[i]);
        }
    }

    return true;
}

bool
UpsfMapping::map(
    const wt474_messages::v1::NetworkConnection::Status& from,
    upsf_network_connection_status_t& to)
{
    /* network_connection: init */
    memset(&to, 0, sizeof(to));

    /* network_connection: status: nc_active */
    to.nc_active_size = from.nc_active_size();
    int i = 0;
    for (auto it : from.nc_active()) {
        /* network_connection: status: nc_active[i] */
        strncpy(
            to.nc_active[i].key.str,
            it.first.c_str(),
            sizeof(to.nc_active[i].key.str) - 1);
        to.nc_active[i].key.len = strlen(to.nc_active[i].key.str);

        /* network_connection: status: nc_active[i] */
        if (it.second) {
            strncpy(
                to.nc_active[i].value.str,
                "1",
                sizeof(to.nc_active[i].value.str) - 1);
        } else {
            strncpy(
                to.nc_active[i].value.str,
                "0",
                sizeof(to.nc_active[i].value.str) - 1);
        }
        to.nc_active[i].value.len = strlen(to.nc_active[i].value.str);

        i++;
    }

    return true;
}

bool
UpsfMapping::map(
    const upsf_network_connection_status_t& from,
    wt474_messages::v1::NetworkConnection::Status& to)
{
    /* network_connection: status: nc_active */
    for (int i = 0; i < from.nc_active_size; i++) {
        (*to.mutable_nc_active())[from.nc_active[i].key.str] = from.nc_active[i].value.str;
    }

    /* network_connection: status: nc_active */
    to.set_allocated_shards(from.allocated_shards);

    return true;
}

bool
UpsfMapping::map(
    const wt474_messages::v1::Shard& from,
    upsf_shard_t& to)
{
    /* shard: init */
    memset(&to, 0, sizeof(to));

    /* shard: name */
    strncpy(
        to.name.str,
        from.name().c_str(),
        sizeof(to.name.str) - 1);
    to.name.len = strlen(to.name.str);

    /* shard: metadata: description */
    strncpy(
        to.metadata.description.str,
        from.metadata().description().c_str(),
        sizeof(to.metadata.description.str) - 1);
    to.metadata.description.len = strlen(to.metadata.description.str);

    /* shard: metadata: derived_state */
    to.metadata.derived_state = (upsf_derived_state_t)from.metadata().derived_state();

    /* shard: spec */
    UpsfMapping::map(from.spec(), to.spec);

    /* shard: status */
    UpsfMapping::map(from.status(), to.status);

    /* shard: mbb */
    UpsfMapping::map(from.mbb(), to.mbb);

    return true;
}

bool
UpsfMapping::map(
    const upsf_shard_t& from,
    wt474_messages::v1::Shard& to)
{
    /* shard: name */
    to.set_name(std::string(from.name.str));

    /* shard: metadata: description */
    to.mutable_metadata()->set_description(
        std::string(
            from.metadata.description.str));

    /* shard: metadata: derived_state */
    to.mutable_metadata()->set_derived_state(
        wt474_messages::v1::DerivedState(
            from.metadata.derived_state));

    /* shard: spec */
    UpsfMapping::map(from.spec, *to.mutable_spec());

    /* shard: status */
    UpsfMapping::map(from.status, *to.mutable_status());

    /* shard: mbb */
    UpsfMapping::map(from.mbb, *to.mutable_mbb());

    return true;
}

bool
UpsfMapping::map(
    const wt474_messages::v1::Shard::Spec& from,
    upsf_shard_spec_t& to)
{
    /* shard: init */
    memset(&to, 0, sizeof(to));

    /* shard: spec: max_session_count */
    to.max_session_count = from.max_session_count();

    /* shard: spec: virtual_mac */
    strncpy(
        to.virtual_mac.str,
        from.virtual_mac().c_str(),
        sizeof(to.virtual_mac.str) - 1);
    to.virtual_mac.len = strlen(to.virtual_mac.str);

    /* shard: spec: desired_state */
    UpsfMapping::map(from.desired_state(), to.desired_state);

    /* shard: spec: prefix */
    for (int i = 0; i < from.prefix_size(); i++) {
        strncpy(
            to.prefix[i].str,
            from.prefix(i).c_str(),
            sizeof(to.prefix[i].str) - 1);
        to.prefix[i].len = strlen(to.prefix[i].str);
    }

    return true;
}

bool
UpsfMapping::map(
    const upsf_shard_spec_t& from,
    wt474_messages::v1::Shard::Spec& to)
{
    /* shard: spec: max_session_count */
    to.set_max_session_count(
        from.max_session_count);

    /* shard: spec: virtual_mac */
    to.set_virtual_mac(
        std::string(
            from.virtual_mac.str));

    /* shard: spec: desired_state */
    UpsfMapping::map(from.desired_state, *to.mutable_desired_state());

    /* shard: spec: prefix */
    for (int i = 0; i < from.prefix_size; i++) {
        to.add_prefix(
            std::string(
                from.prefix[i].str));
    }

    return true;
}

bool
UpsfMapping::map(
    const wt474_messages::v1::Shard::Spec::DesiredState& from,
    upsf_shard_spec_desired_state_t& to)
{
    /* shard: init */
    memset(&to, 0, sizeof(to));

    /* shard: spec: desired_state: service_gateway_user_plane */
    strncpy(
        to.service_gateway_user_plane.str,
        from.service_gateway_user_plane().c_str(),
        sizeof(to.service_gateway_user_plane.str) - 1);
    to.service_gateway_user_plane.len = strlen(to.service_gateway_user_plane.str);

    /* shard: spec: desired_state: network_connection */
    for (int i = 0; i < from.network_connection_size(); i++) {
        strncpy(
            to.network_connection[i].str,
            from.network_connection(i).c_str(),
            sizeof(to.network_connection[i].str) - 1);
        to.network_connection[i].len = strlen(to.network_connection[i].str);
    }

    return true;
}

bool
UpsfMapping::map(
    const upsf_shard_spec_desired_state_t& from,
    wt474_messages::v1::Shard::Spec::DesiredState& to)
{
    /* shard: spec: desired_state: service_gateway_user_plane */
    to.set_service_gateway_user_plane(
        std::string(
            from.service_gateway_user_plane.str));

    /* shard: spec: desired_state: network_connection */
    for (int i = 0; i < from.network_connection_size; i++) {
        to.add_network_connection(
            std::string(
                from.network_connection[i].str));
    }

    return true;
}

bool
UpsfMapping::map(
    const wt474_messages::v1::Shard::Status& from,
    upsf_shard_status_t& to)
{
    /* shard: init */
    memset(&to, 0, sizeof(to));

    /* shard: status: allocated_session_count */
    to.allocated_session_count = from.allocated_session_count();

    /* shard: status: maximum_allocated_quality */
    to.maximum_allocated_quality = from.maximum_allocated_quality();

    /* shard: status: current_state */
    UpsfMapping::map(from.current_state(), to.current_state);

    return true;
}

bool
UpsfMapping::map(
    const upsf_shard_status_t& from,
    wt474_messages::v1::Shard::Status& to)
{
    /* shard: status: allocated_session_count */
    to.set_allocated_session_count(
        from.allocated_session_count);

    /* shard: status: maximum_allocated_quality */
    to.set_maximum_allocated_quality(
        from.maximum_allocated_quality);

    /* shard: status: current_state */
    UpsfMapping::map(from.current_state, *to.mutable_current_state());

    return true;
}

bool
UpsfMapping::map(
    const wt474_messages::v1::Shard::Status::CurrentState& from,
    upsf_shard_status_current_state_t& to)
{
    /* shard: init */
    memset(&to, 0, sizeof(to));

    /* shard: status: current_state: service_gateway_user_plane */
    strncpy(
        to.service_gateway_user_plane.str,
        from.service_gateway_user_plane().c_str(),
        sizeof(to.service_gateway_user_plane.str) - 1);
    to.service_gateway_user_plane.len = strlen(to.service_gateway_user_plane.str);

    /* shard: status: current_state: tsf_network_connection */
    int index = 0;
    for (auto it : from.tsf_network_connection()) {
        strncpy(
            to.tsf_network_connection[index].key.str,
            it.first.c_str(),
            sizeof(to.tsf_network_connection[index].key.str) - 1);
        to.tsf_network_connection[index].key.len = strlen(to.tsf_network_connection[index].key.str);

        strncpy(
            to.tsf_network_connection[index].value.str,
            it.second.c_str(),
            sizeof(to.tsf_network_connection[index].value.str) - 1);
        to.tsf_network_connection[index].value.len = strlen(to.tsf_network_connection[index].value.str);

        index++;
    }

    return true;
}

bool
UpsfMapping::map(
    const upsf_shard_status_current_state_t& from,
    wt474_messages::v1::Shard::Status::CurrentState& to)
{
    /* shard: status: current_state: service_gateway_user_plane */
    to.set_service_gateway_user_plane(
        std::string(
            from.service_gateway_user_plane.str));

    /* shard: status: current_state: tsf_network_connection */
    for (int i = 0; i < from.tsf_network_connection_size; i++) {
        (*to.mutable_tsf_network_connection())[
            from.tsf_network_connection[i].key.str] = 
                from.tsf_network_connection[i].value.str;
    }

    return true;
}

bool
UpsfMapping::map(
    const wt474_messages::v1::Shard::Mbb& from,
    upsf_shard_mbb_t& to)
{
    /* shard: init */
    memset(&to, 0, sizeof(to));

    /* shard: mbb: mbb_state */
    to.mbb_state = (upsf_mbb_state_t)from.mbb_state();

    return true;
}

bool
UpsfMapping::map(
    const upsf_shard_mbb_t& from,
    wt474_messages::v1::Shard::Mbb& to)
{
    /* shard: mbb: mbb_state */
    to.set_mbb_state(
        wt474_messages::v1::Shard::Mbb::MbbState(
            from.mbb_state));

    return true;
}

bool
UpsfMapping::map(
    const wt474_messages::v1::NetworkConnection::Spec::Endpoint& from,
    upsf_network_connection_spec_endpoint_t& to)
{
    /* endpoint: init */
    memset(&to, 0, sizeof(to));

    /* endpoint: endpoint_name */
    strncpy(
        to.endpoint_name.str,
        from.endpoint_name().c_str(),
        sizeof(to.endpoint_name.str) - 1);
    to.endpoint_name.len = strlen(to.endpoint_name.str);

    /* endpoint: vtep */
    if (from.has_vtep()) {

        UpsfMapping::map(from.vtep(), to.ep_spec.vtep);
        to.ep_type = UPSF_EP_TYPE_VTEP;

    /* endpoint: l2vpn */
    } else if (from.has_l2vpn()) {

        UpsfMapping::map(from.l2vpn(), to.ep_spec.l2vpn);
        to.ep_type = UPSF_EP_TYPE_L2VPN;

    /* endpoint: port_vlan */
    } else if (from.has_port_vlan()) {

        UpsfMapping::map(from.port_vlan(), to.ep_spec.port_vlan);
        to.ep_type = UPSF_EP_TYPE_PORT_VLAN;

    /* endpoint: unknown */
    } else {

        return false;
    }

    return true;
}

bool
UpsfMapping::map(
    const upsf_network_connection_spec_endpoint_t& from,
    wt474_messages::v1::NetworkConnection::Spec::Endpoint& to)
{
    /* endpoint: endpoint_name */
    to.set_endpoint_name(std::string(from.endpoint_name.str));

    switch (from.ep_type) {
    case UPSF_EP_TYPE_VTEP:
        /* endpoint: vtep */
        UpsfMapping::map(from.ep_spec.vtep, *to.mutable_vtep());
        break;
    case UPSF_EP_TYPE_L2VPN:
        /* endpoint: l2vpn */
        UpsfMapping::map(from.ep_spec.l2vpn, *to.mutable_l2vpn());
        break;
    case UPSF_EP_TYPE_PORT_VLAN:
        /* endpoint: port_vlan */
        UpsfMapping::map(from.ep_spec.port_vlan, *to.mutable_port_vlan());
        break;
    default:
        /* endpoint: unknown */
        return false;
    }

    return true;
}

bool
UpsfMapping::map(
    const wt474_messages::v1::Vtep& from,
    upsf_vtep_t& to)
{
    /* vtep: init */
    memset(&to, 0, sizeof(to));

    /* vtep: ip_address */
    strncpy(
        to.ip_address.str,
        from.ip_address().c_str(),
        sizeof(to.ip_address.str) - 1);
    to.ip_address.len = strlen(to.ip_address.str);

    /* vtep: udp_port */
    to.udp_port = from.udp_port();

    /* vtep: vni */
    to.vni = from.vni();

    return true;
}

bool
UpsfMapping::map(
    const upsf_vtep_t& from,
    wt474_messages::v1::Vtep& to)
{
    /* vtep: ip_address */
    to.set_ip_address(std::string(from.ip_address.str));

    /* vtep: udp_port */
    to.set_udp_port(from.udp_port);

    /* vtep: vni */
    to.set_vni(from.vni);

    return true;
}

bool
UpsfMapping::map(
    const wt474_messages::v1::L2vpn& from,
    upsf_l2vpn_t& to)
{
    /* l2vpn: init */
    memset(&to, 0, sizeof(to));

    /* l2vpn: vpn_id */
    to.vpn_id = from.vpn_id();

    return true;
}

bool
UpsfMapping::map(
    const upsf_l2vpn_t& from,
    wt474_messages::v1::L2vpn& to)
{
    /* l2vpn: vpn_id */
    to.set_vpn_id(from.vpn_id);

    return true;
}

bool
UpsfMapping::map(
    const wt474_messages::v1::PortVlan& from,
    upsf_port_vlan_t& to)
{
    /* port_vlan: init */
    memset(&to, 0, sizeof(to));

    /* port_vlan: logical_port */
    strncpy(
        to.logical_port.str,
        from.logical_port().c_str(),
        sizeof(to.logical_port.str) - 1);
    to.logical_port.len = strlen(to.logical_port.str);

    /* port_vlan: svlan */
    to.svlan = from.svlan();

    /* port_vlan: cvlan */
    to.cvlan = from.cvlan();

    return true;
}

bool
UpsfMapping::map(
    const upsf_port_vlan_t& from,
    wt474_messages::v1::PortVlan& to)
{
    /* port_vlan: logical_port */
    to.set_logical_port(std::string(from.logical_port.str));

    /* port_vlan: svlan */
    to.set_svlan(from.svlan);

    /* port_vlan: cvlan */
    to.set_cvlan(from.cvlan);

    return true;
}

bool
UpsfMapping::map(
    const wt474_messages::v1::SessionContext& from,
    upsf_session_context_t& to)
{
    /* session_context: init */
    memset(&to, 0, sizeof(to));

    /* session_context: name */
    strncpy(
        to.name.str,
        from.name().c_str(),
        sizeof(to.name.str) - 1);
    to.name.len = strlen(to.name.str);

    /* session_context: metadata: description */
    strncpy(
        to.metadata.description.str,
        from.metadata().description().c_str(),
        sizeof(to.metadata.description.str) - 1);
    to.metadata.description.len = strlen(to.metadata.description.str);

    /* session_context: metadata: derived_state */
    to.metadata.derived_state = (upsf_derived_state_t)from.metadata().derived_state();

    /* session_context: spec */
    UpsfMapping::map(from.spec(), to.spec);

    /* session_context: status */
    UpsfMapping::map(from.status(), to.status);

    return true;
}

bool
UpsfMapping::map(
    const upsf_session_context_t& from,
    wt474_messages::v1::SessionContext& to)
{
    /* session_context: name */
    to.set_name(std::string(from.name.str));

    /* session_context: metadata: description */
    to.mutable_metadata()->set_description(
        std::string(
            from.metadata.description.str));

    /* session_context: metadata: derived_state */
    to.mutable_metadata()->set_derived_state(
        wt474_messages::v1::DerivedState(
            from.metadata.derived_state));

    /* session_context: spec */
    UpsfMapping::map(from.spec, *to.mutable_spec());

    /* session_context: status */
    UpsfMapping::map(from.status, *to.mutable_status());

    return true;
}

bool
UpsfMapping::map(
    const wt474_messages::v1::SessionContext::Spec& from,
    upsf_session_context_spec_t& to)
{
    /* session_context: spec: init */
    memset(&to, 0, sizeof(to));

    /* session_context: spec: traffic_steering_function */
    strncpy(
        to.traffic_steering_function.str,
        from.traffic_steering_function().c_str(),
        sizeof(to.traffic_steering_function.str) - 1);
    to.traffic_steering_function.len = strlen(to.traffic_steering_function.str);

    /* session_context: spec: required_service_group */
    int i = 0;
    for (auto it : from.required_service_group()) {
        /* session_context: spec: required_service_group */
        strncpy(
            to.required_service_group[i].str,
            it.c_str(),
            sizeof(to.required_service_group[i].str) - 1);
        to.required_service_group[i].len = strlen(to.required_service_group[i].str);
        i++;
    }

    /* session_context: spec: required_quality */
    to.required_quality = from.required_quality();

    /* session_context: spec: circuit_id */
    strncpy(
        to.circuit_id.str,
        from.circuit_id().c_str(),
        sizeof(to.circuit_id.str) - 1);
    to.circuit_id.len = strlen(to.circuit_id.str);

    /* session_context: spec: remote_id */
    strncpy(
        to.remote_id.str,
        from.remote_id().c_str(),
        sizeof(to.remote_id.str) - 1);
    to.remote_id.len = strlen(to.remote_id.str);

    /* session_context: spec: session_filter */
    UpsfMapping::map(from.session_filter(), to.session_filter); 

    /* session_context: spec: desired_state */
    UpsfMapping::map(from.desired_state(), to.desired_state); 

    return true;
}

bool
UpsfMapping::map(
    const upsf_session_context_spec_t& from,
    wt474_messages::v1::SessionContext::Spec& to)
{
    /* session_context: spec: traffic_steering_function */
    to.set_traffic_steering_function(std::string(from.traffic_steering_function.str));

    /* session_context: spec: traffic_steering_function */
    for (int i = 0; i < from.required_service_group_size; i++) {
        to.add_required_service_group(
            from.required_service_group[i].str);
    }

    /* session_context: spec: required_quality */
    to.set_required_quality(from.required_quality);

    /* session_context: spec: circuit_id */
    to.set_circuit_id(std::string(from.circuit_id.str));

    /* session_context: spec: remote_id */
    to.set_remote_id(std::string(from.remote_id.str));

    /* session_context: spec: session_filter */
    UpsfMapping::map(from.session_filter, *to.mutable_session_filter()); 

    /* session_context: spec: desired_state */
    UpsfMapping::map(from.desired_state, *to.mutable_desired_state()); 

    return true;
}

bool
UpsfMapping::map(
    const wt474_messages::v1::SessionContext::Spec::DesiredState& from,
    upsf_session_context_spec_desired_state_t& to)
{
    /* session_context: spec: desired_state: init */
    memset(&to, 0, sizeof(to));

    /* session_context: spec: desired_state: shard */
    strncpy(
        to.shard.str,
        from.shard().c_str(),
        sizeof(to.shard.str) - 1);
    to.shard.len = strlen(to.shard.str);

    return true;
}

bool
UpsfMapping::map(
    const upsf_session_context_spec_desired_state_t& from,
    wt474_messages::v1::SessionContext::Spec::DesiredState& to)
{
    /* session_context: spec: desired_state: shard */
    to.set_shard(std::string(from.shard.str));

    return true;
}

bool
UpsfMapping::map(
    const wt474_messages::v1::SessionFilter& from,
    upsf_session_filter_t& to)
{
    /* session_filter: init */
    memset(&to, 0, sizeof(to));

    /* session_filter: source_mac_address */
    strncpy(
        to.source_mac_address.str,
        from.source_mac_address().c_str(),
        sizeof(to.source_mac_address.str) - 1);
    to.source_mac_address.len = strlen(to.source_mac_address.str);

    /* session_filter: svlan */
    to.svlan = from.svlan();

    /* session_filter: cvlan */
    to.cvlan = from.cvlan();

    return true;
}

bool
UpsfMapping::map(
    const upsf_session_filter_t& from,
    wt474_messages::v1::SessionFilter& to)
{
    /* session_filter: source_mac_address */
    to.set_source_mac_address(std::string(from.source_mac_address.str));

    /* session_filter: svlan */
    to.set_svlan(from.svlan);

    /* session_filter: cvlan */
    to.set_cvlan(from.cvlan);

    return true;
}

bool
UpsfMapping::map(
    const wt474_messages::v1::SessionContext::Status& from,
    upsf_session_context_status_t& to)
{
    /* session_context: status: init */
    memset(&to, 0, sizeof(to));

    /* session_context: status: current_state */
    UpsfMapping::map(from.current_state(), to.current_state); 

    return true;
}

bool
UpsfMapping::map(
    const upsf_session_context_status_t& from,
    wt474_messages::v1::SessionContext::Status& to)
{
    /* session_context: status: current_state */
    UpsfMapping::map(from.current_state, *to.mutable_current_state()); 

    return true;
}

bool
UpsfMapping::map(
    const wt474_messages::v1::SessionContext::Status::CurrentState& from,
    upsf_session_context_status_current_state_t& to)
{
    /* session_context: status: current_state: init */
    memset(&to, 0, sizeof(to));

    /* session_context: status: current_state: user_plane_shard */
    strncpy(
        to.user_plane_shard.str,
        from.user_plane_shard().c_str(),
        sizeof(to.user_plane_shard.str) - 1);
    to.user_plane_shard.len = strlen(to.user_plane_shard.str);

    /* session_context: status: current_state: tsf_shard */
    strncpy(
        to.tsf_shard.str,
        from.tsf_shard().c_str(),
        sizeof(to.tsf_shard.str) - 1);
    to.tsf_shard.len = strlen(to.tsf_shard.str);

    return true;
}

bool
UpsfMapping::map(
    const upsf_session_context_status_current_state_t& from,
    wt474_messages::v1::SessionContext::Status::CurrentState& to)
{
    /* session_context: status: current_state: user_plane_shard */
    to.set_user_plane_shard(std::string(from.user_plane_shard.str));

    /* session_context: status: current_state: tsf_shard */
    to.set_tsf_shard(std::string(from.tsf_shard.str));

    return true;
}

