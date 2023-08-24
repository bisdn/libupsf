# libupsf

libupsf is a C/C++ client implementation of the Broadband Forum's (BBF)
Subscriber Session Steering (SSS) protocol defined by the Access and Transport
Architecture (ATA) group in WT-474.

<table>
  <tr>
    <td><b>Homepage:</b></td>
    <td><a href="https://github.com/bisdn/libupsf">https://github.com/bisdn/libupsf</a></td>
  </tr>
</table>

It is used for communication to the User Plane Selection Function (UPSF) in
a distributed Broadband Network Gateway (BNG) environment. See the associated
BBF documents for details.

# Getting started: building and installing

The library's build system is based on cmake and makes use of pkg-config. 
The list of dependencies includes:

<table>
  <tr>
    <th>Name</th>
    <th>Project page</th>
  </tr>
  <tr>
    <td><b>gRPC framework</b></td>
    <td><a href="https://grpc.io">https://grpc.io</a></td>
  </tr>
  <tr>
    <td><b>Protocol buffers</b></td>
    <td><a href="https://protobuf.dev">https://protobuf.dev</a></td>
  </tr>
  <tr>
    <td><b>Google logging library</b></td>
    <td><a href="https://github.com/google/glog">https://github.com/google/glog</a></td>
  </tr>
  <tr>
    <td><b>Google gflags library</b></td>
    <td><a href="https://github.com/gflags/gflags">https://github.com/gflags/gflags</a></td>
  </tr>
</table>

On Ubuntu 22.04 LTS install the gRPC framework and the associated protobuf
compiler:
```
sh# sudo apt install libgrpc++-dev protobuf-compiler-grpc libgflags-dev
```

libupsf needs version 0.7.0 or better of Google's logging library. For a manual
installation clone the associated repository for building and installing the library:

```
sh# git clone https://github.com/google/glog
sh# mkdir -p glog/build && cd glog/build
sh# cmake -DCMAKE_INSTALL_PREFIX=<path> -DCMAKE_BUILD_TYPE=Release ..
sh# make && make install
```

Adjust the PKG_CONFIG_PATH environment variable according to the chosen
installation path, e.g.:

```
sh# export PKG_CONFIG_PATH=<path>/lib/pkgconfig:${PKG_CONFIG_PATH}
```

Enter libupsf's build directory and run cmake for configuring the project. 
Start the build process and install the resulting library:

```
sh# cd libupsf/build
sh# cmake -DCMAKE_INSTALL_PREFIX=<path> ..
sh# make && make install

sh# pkg-config --cflags upsf++
-I<path>/include

sh# pkg-config --libs upsf++
-L<path>/lib -lupsf++
```

# For developers: using libupsf within your project

libupsf provides a C++ and C interface. The latter is a thin wrapper for
using the library in a C environment, but linking libupsf to your project
still requires a C++ linker. Here a list of source files:

<table>
  <tr>
    <th>File</th>
    <th>Description</th>
  </tr>
  <tr>
    <td>upsf.h</td>
    <td>C header file</td>
  </tr>
  <tr>
    <td>upsf.hpp</td>
    <td>C++ header file</td>
  </tr>
  <tr>
    <td>upsf_c_wrapper.cpp</td>
    <td>UPSF C wrapper functions</td>
  </tr>
  <tr>
    <td>upsf_c_mapping.hpp</td>
    <td>functions for bidirectional mapping C to C++ representation</td>
  </tr>
  <tr>
    <td>upsf_c_mapping.cpp</td>
    <td>functions for bidirectional mapping C to C++ representation</td>
  </tr>
  <tr>
    <td>upsf_stream.hpp</td>
    <td>helper output operators for C++ upsf classes</td>
  </tr>
  <tr>
    <td>upsf++.pc.in</td>
    <td>pkg-config template for libupsf</td>
  </tr>
</table>

## A C based example

Please see file <a
href="./examples/c/upsf_c_example.c">upsf_c_example.c</a> as a starting point
for your own project.

The library is thread-aware, i.e., a separate gRPC connection will be
used per execution thread. You must call function upsf_open() once in
each thread explicitly for specifying UPSF host and port to connect
to, though. Changing the associated UPSF host/port requires calling
function upsf_close() first before reestablishing the connection. A
helper function named upsf_exists() is providing a simple check to verify
existence of an active gRPC connection in the calling thread context.

Class UpsfSlot serves as container for all thread local variables including
an instance of class UpsfClient (see the C++ related section below).

### SSS gRPC item representation

All items are represented by an associated C structure as shown below:

<table>
  <tr>
    <th>C structure</th>
    <th>SSS gRPC item</th>
  </tr>
  <tr>
    <td><a href="./upsf/upsf.h?ref_type=heads#L244">upsf_service_gateway_t</a></td>
    <td>Service gateway</td>
  </tr>
  <tr>
    <td><a href="./upsf/upsf.h?ref_type=heads#L250">upsf_service_gateway_user_plane_t</a></td>
    <td>Service gateway user plane</td>
  </tr>
  <tr>
    <td><a href="./upsf/upsf.h?ref_type=heads#L265">upsf_traffic_steering_function_t</a></td>
    <td>Traffic steering function</td>
  </tr>
  <tr>
    <td><a href="./upsf/upsf.h?ref_type=heads#L309">upsf_shard_t</a></td>
    <td>Subscriber group (shard)</td>
  </tr>
  <tr>
    <td><a href="./upsf/upsf.h?ref_type=heads#L354">upsf_session_context_t</a></td>
    <td>Session context</td>
  </tr>
  <tr>
    <td><a href="./upsf/upsf.h?ref_type=heads#L220">upsf_network_connection_t</a></td>
    <td>Network connection</td>
  </tr>
</table>

Various C sub-structures exist for the various SSS protobuf
definitions, see upsf.h for details.

Arrays are limited to a maximum size of 16 elements: adjust the
constants defined in upsf.h to suit your needs in case these
limitations do not meet your project's requirements.

Strings defined in the SSS gRPC protobufs are stored in upsf_string_t
providing maximum space of 64 bytes for the string and an associated
length field indicating the string's actual length.

### Create a service gateway user plane

Here an example for creating an SGUP item in the UPSF.

```
    /* SGUP C structure */
    upsf_service_gateway_user_plane_t item;
    memset(&item, 0, sizeof(item));

    /* populate mandatory fields */
    snprintf(item.name.str, sizeof(item.name.str), "my-up");
    snprintf(item.service_gateway_name.str, UPSF_MAX_STRING_SIZE, "my-sg");
    item.spec.max_session_count = 1024;
    item.spec.max_shards = 128;

    /* create service gateway user plane */
    if (upsf_create_service_gateway_user_plane(&item) == NULL) {
        return -1;
    }
    /* item contains updated SGUP received from UPSF */
    ...
```

### Update a service gateway user plane

Here an example for updating an SGUP item in the UPSF.

```
    /* SGUP C structure */
    upsf_service_gateway_user_plane_t item;
    memset(&item, 0, sizeof(item));

    /* populate mandatory fields */
    snprintf(item.name.str, sizeof(item.name.str), "my-up");
    snprintf(item.service_gateway_name.str, UPSF_MAX_STRING_SIZE, "my-sg");
    item.spec.max_session_count = 1024;
    item.spec.max_shards = 128;
    item.status.allocated_session_count = 7;
    item.status.allocated_shards = 1;

    /* specify SGUP default endpoint */
    item.spec.default_endpoint.ep_type = UPSF_EP_TYPE_VTEP;
    snprintf(item.spec.default_endpoint.endpoint_name.str, UPSF_MAX_STRING_SIZE, "my-up-ep1");
    snprintf(item.spec.default_endpoint.ep_spec.vtep.ip_address.str, UPSF_MAX_STRING_SIZE, "1.2.3.4");
    item.spec.default_endpoint.ep_spec.vtep.udp_port = 4789;
    item.spec.default_endpoint.ep_spec.vtep.vni = 1234;

    /* create service gateway user plane */
    if (upsf_create_service_gateway_user_plane(&item) == NULL) {
        return -1;
    }
    /* item contains updated SGUP received from UPSF */
    ...
```

### Read a service gateway user plane

Here an example for reading an SGUP item from the UPSF.

```
    /* SGUP C structure */
    upsf_service_gateway_user_plane_t item;
    memset(&item, 0, sizeof(item));

    /* populate mandatory fields */
    snprintf(item.name.str, sizeof(item.name.str), "my-up");

    /* read service gateway user plane */
    if (upsf_get_service_gateway_user_plane(&item) < 0) {
        return -1;
    }
    /* item contains updated SGUP received from UPSF */
    ...
```

### Delete a service gateway user plane

Here an example for deleting an SGUP item from UPSF.

```
    /* SGUP C structure */
    upsf_service_gateway_user_plane_t item;
    memset(&item, 0, sizeof(item));

    /* populate mandatory fields */
    snprintf(item.name.str, sizeof(item.name.str), "my-up");

    /* delete service gateway user plane */
    if (upsf_delete_service_gateway_user_plane(&item) < 0) {
        return -1;
    }
```

## A C++ based example

Please see file <a
href="./examples/cpp/upsf_cpp_example.cpp">upsf_cpp_example.cpp</a>
as a starting point for your own project.

Class UpsfClient serves as container for all thread local variables
including the gRPC channel and associated protocol stub. Please find
below a simple example for wrapping UpsfClient in another class:

```
    #include <upsf.hpp>

    class UpsfExample {
        private:
            upsf::UpsfClient client;


        public:
            UpsfExample(const std::string &srv_addr = std::string("127.0.0.1:50051")) :
                client(
                    grpc::CreateChannel(
                        srv_addr,
                        grpc::InsecureChannelCredentials()
                    )
                )
            { ... };
    }
```

### Create a service gateway user plane

Here an example for creating an SGUP item in the UPSF.

```
    std::string srv_addr("127.0.0.1:50051");

    upsf::UpsfClient client(
        grpc::CreateChannel(
            srv_addr, 
            grpc::InsecureChannelCredentials()
        )
    );

    wt474_messages::v1::ServiceGatewayUserPlane request;
    wt474_messages::v1::ServiceGatewayUserPlane reply;

    /* populate mandatory fields */
    request.set_name(std::string("my-up"));
    request.set_service_gateway_name(std::string("my-sg"));
    request.mutable_spec()->set_max_session_count(1024);
    request.mutable_spec()->set_max_shards(128);

    /* create service gateway user plane, returns 0 in case of failure */
    if (!client.CreateV1(request, reply)) {
        // handle error
        return;
    }
    // reply contains SGUP received from UPSF
```

### Update a service gateway user plane

Here an example for updating an SGUP item in the UPSF.

```
    std::string srv_addr("127.0.0.1:50051");

    upsf::UpsfClient client(
        grpc::CreateChannel(
            srv_addr, 
            grpc::InsecureChannelCredentials()
        )
    );

    wt474_messages::v1::ServiceGatewayUserPlane request;
    wt474_messages::v1::ServiceGatewayUserPlane reply;
    wt474_upsf_service::v1::UpdateReq::UpdateOptions options;

    /* populate mandatory fields */
    request.set_name(std::string("my-up"));
    request.set_service_gateway_name(std::string("my-sg"));
    request.mutable_spec()->add_supported_service_group("ssg-1");
    request.mutable_spec()->add_supported_service_group("ssg-2");
    request.mutable_spec()->mutable_default_endpoint()->set_endpoint_name("my-up-ep1");
    request.mutable_spec()->mutable_default_endpoint()->mutable_vtep()->set_ip_address("1.2.3.4");
    request.mutable_spec()->mutable_default_endpoint()->mutable_vtep()->set_udp_port(4789);
    request.mutable_spec()->mutable_default_endpoint()->mutable_vtep()->set_vni(12345);
    request.mutable_status()->set_allocated_session_count(7);
    request.mutable_status()->set_allocated_shards(1);

    /* update service gateway user plane, returns 0 in case of failure */
    if (!client.UpdateV1(request, reply, options)) {
        // handle error
        return;
    }
    // reply contains SGUP received from UPSF
```

### Read a service gateway user plane

Here an example for reading a single SGUP item from the UPSF.

```
    std::string srv_addr("127.0.0.1:50051");

    upsf::UpsfClient client(
        grpc::CreateChannel(
            srv_addr, 
            grpc::InsecureChannelCredentials()
        )
    );

    wt474_messages::v1::ServiceGatewayUserPlane reply;

    /* read service gateway user plane, returns 0 in case of failure */
    if (!client.ReadV1(std::string("my-up"), reply)) {
        // handle error
        return;
    }
    // reply contains SGUP received from UPSF
```

And for reading the entire list of SGUP instances stored in UPSF:

```
    std::string srv_addr("127.0.0.1:50051");

    upsf::UpsfClient client(
        grpc::CreateChannel(
            srv_addr, 
            grpc::InsecureChannelCredentials()
        )
    );

    std::vector<wt474_messages::v1::ServiceGatewayUserPlane> items;

    /* read service gateway user plane, returns 0 in case of failure */
    if (!client.ReadV1(items)) {
        // handle error
        return;
    }
    // items contains all SGUP instances received from UPSF
```

### Delete a service gateway user plane

Here an example for deleting an SGUP item from UPSF.

```
    std::string srv_addr("127.0.0.1:50051");

    upsf::UpsfClient client(
        grpc::CreateChannel(
            srv_addr, 
            grpc::InsecureChannelCredentials()
        )
    );

    std::string reply;

    /* delete service gateway user plane, returns 0 in case of failure */
    if (!client.DeleteV1(std::string("my-up"), reply)) {
        // handle error
        return;
    }
    // reply contains status message received from UPSF
```

## Subscribing to UPSF emitted notifications

The SSS gRPC protobuf definition includes a mechanism for receiving
a continuous stream of events for certain types of items stored in
the UPSF database. The following sub-sections cover this subscription
process for both C and C++ based applications.

### C subscriber example

Main entry point for subscribing to UPSF emitted notifications is 
function <a href="./upsf/upsf.h#L509">upsf_subscribe()</a>. This 
function blocks and never returns upon reception of notifications
so it is best run within its own background thread. It accepts a set
of parameters including:

<table>
  <tr>
    <th>Parameter</th>
    <th>Example value</th>
    <th>Description</th>
  </tr>
  <tr>
    <td>upsf_host</td>
    <td>127.0.0.1</td>
    <td>UPSF server host to connect to</td>
  </tr>
  <tr>
    <td>upsf_port</td>
    <td>50051</td>
    <td>UPSF server port to connect to</td>
  </tr>
  <tr>
    <td>userdata</td>
    <td>...</td>
    <td>An opaque void pointer to arbitrary user data used by callbacks and ignored by libupsf</td>
  </tr>
</table>

In addition a set of callbacks must be specified for those items
one is interested in (set the associcated callback to nullptr for
skipping certain item types).

See next table for their prototype definitions:

<table>
  <tr>
    <th>SSS item type</th>
    <th>Callback prototype</th>
  </tr>
  <tr>
    <td><a href="./upsf/upsf.h#L369">Service gateway</a></td>
    <td>typedef int (*upsf_service_gateway_cb_t)(upsf_service_gateway_t* service_gateway, void* userdata);</td>
  </tr>
  <tr>
    <td><a href="./upsf/upsf.h#L367">Service gateway user plane</a></td>
    <td>typedef int (*upsf_service_gateway_user_plane_cb_t)(upsf_service_gateway_user_plane_t* service_gateway_user_plane, void* userdata);</td>
  </tr>
  <tr>
    <td><a href="./upsf/upsf.h#L368">Traffic steering function</a></td>
    <td>typedef int (*upsf_traffic_steering_function_cb_t)(upsf_traffic_steering_function_t* traffic_steering_function, void* userdata);</td>
  </tr>
  <tr>
    <td><a href="./upsf/upsf.h#L364">Subscriber group (shard)</a></td>
    <td>typedef int (*upsf_shard_cb_t)(upsf_shard_t* shard, void* userdata);</td>
  </tr>
  <tr>
    <td><a href="./upsf/upsf.h#L365">Session context</a></td>
    <td>typedef int (*upsf_session_context_cb_t)(upsf_session_context_t* session_context, void* userdata);</td>
  </tr>
  <tr>
    <td><a href="./upsf/upsf.h#L369">Network connection</a></td>
    <td>typedef int (*upsf_network_connection_cb_t)(upsf_network_connection_t* network_connection, void* userdata);</td>
  </tr>
</table>

Here an example for a UPSF notification service:

```
    int service_gateway_cb(upsf_service_gateway_t* data, void* userdata)
    {
        /* use data */
        return 1; /* continue subscribe */
    }
    
    int service_gateway_user_plane_cb(upsf_service_gateway_user_plane_t* data, void* userdata)
    {
        /* use data */
        return 1; /* continue subscribe */
    }
    
    int shard_cb(upsf_shard_t* data, void* userdata)
    {
        /* use data */
        return 1; /* continue subscribe */
    }
    
    int session_context_cb(upsf_session_context_t* data, void* userdata)
    {
        /* use data */
        return 1; /* continue subscribe */
    }
    
    int network_connection_cb(upsf_network_connection_t* data, void* userdata)
    {
        /* use data */
        return 1; /* continue subscribe */
    }
    
    int traffic_steering_function_cb(upsf_traffic_steering_function_t* data, void* userdata)
    {
        /* use data */
        return 1; /* continue subscribe */
    }
    
    int subscribe()
    {
        /* opaque pointer to arbitrary user data */
        void* userdata = NULL;
    
        /* never returns */
        if (upsf_subscribe("127.0.0.1", 50051, userdata,
            &shard_cb,
            &session_context_cb,
            &network_connection_cb,
            &service_gateway_user_plane_cb,
            &traffic_steering_function_cb,
            &service_gateway_cb) < 0) 
        {
            return -1;
        }
    
        return 0;
    }
```

### C++ subscriber example

For subscribing to UPSF emitted notifications main entry point is class <a
href="./upsf/upsf.hpp#L57">UpsfSubscriber</a>. It contains the following 
parameters:

<table>
  <tr>
    <th>Parameter</th>
    <th>Type</th>
    <th>Description</th>
  </tr>
  <tr>
    <td>watch</td>
    <td>bool</td>
    <td>Boolean value for continuous reception</td>
  </tr>
  <tr>
    <td>item_types</td>
    <td>std::vector&lt;wt474_messages::v1::ItemType&gt;&</td>
    <td>Vector of SSS item types</td>
  </tr>
  <tr>
    <td>derived_states</td>
    <td>std::vector&lt;wt474_messages::v1::DerivedState&gt;&</td>
    <td>Vector of SSS derived states</td>
  </tr>
  <tr>
    <td>parents</td>
    <td>std::vector&lt;std::string&gt;></td>
    <td>Vector of item parents to watch</td>
  </tr>
  <tr>
    <td>names</td>
    <td>std::vector&lt;std::string&gt;></td>
    <td>Vector of item names to watch</td>
  </tr>
</table>

Set watch to true to receive continuous notifications, its default is false!

Overwrite the associated notification methods defined in class UpsfSubscriber
to receive notifications for a specific item type:

```
class UpsfExample : public upsf::UpsfSubscriber
{
public:
    virtual void notify(
        const wt474_messages::v1::Shard& shard);

    virtual void notify(
        const wt474_messages::v1::SessionContext& session_context);

    virtual void notify(
        const wt474_messages::v1::NetworkConnection& network_connection);

    virtual void notify(
        const wt474_messages::v1::ServiceGatewayUserPlane& service_gateway_user_plane);

    virtual void notify(
        const wt474_messages::v1::TrafficSteeringFunction& traffic_steering_function);

    virtual void notify(
        const wt474_messages::v1::ServiceGateway& service_gateway);
}
```
