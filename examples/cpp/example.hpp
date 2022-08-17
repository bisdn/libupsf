#ifndef EXAMPLE_HPP
#define EXAMPLE_HPP

#include <string>
#include <map>

#include "upsf.hpp"
#include "upsf_stream.hpp"

class Example {
  private:
    /* server address */
    std::string srvaddr;
    /* service gateway: name */
    std::string sg_name;
    /* service gateway: description */
    std::string sg_desc;
    /* service gateway user plane: name:description */
    std::map<std::string, std::string> up_names;
    /* shard: name */
    std::string shard_name;
    /* shard: description */
    std::string shard_desc;
    /* session_context: name */
    std::string session_context_name;
    /* session_context: description */
    std::string session_context_desc;
    /* traffic steering function: name */
    std::string tsf_name;
    /* traffic steering function: description */
    std::string tsf_desc;
    /* service gateway: name */
    std::string nc_name;
    /* service gateway: description */
    std::string nc_desc;
    /* libupsf client */
    upsf::UpsfClient client;

  public:
    Example(
      const std::string& srvaddr = std::string("127.0.0.1:50051"),
      const std::string& sg_name = std::string("sg-test"),
      const std::string& sg_desc = std::string("sg-test description"),
      const std::string& shard_name = std::string("shard-test"),
      const std::string& shard_desc = std::string("shard-test description"),
      const std::string& session_context_name = std::string("session-context-test"),
      const std::string& session_context_desc = std::string("session-context-test description"),
      const std::string& tsf_name = std::string("tsf-test"),
      const std::string& tsf_desc = std::string("tsf-test description"),
      const std::string& nc_name = std::string("nc-test"),
      const std::string& nc_desc = std::string("nc-test description")) :
        srvaddr(srvaddr),
        sg_name(sg_name),
        sg_desc(sg_desc),
        shard_name(shard_name),
        shard_desc(shard_desc),
        session_context_name(session_context_name),
        session_context_desc(session_context_desc),
        tsf_name(tsf_name),
        tsf_desc(tsf_desc),
        nc_name(nc_name),
        nc_desc(nc_desc),
        client(
          grpc::CreateChannel(
            srvaddr,
            grpc::InsecureChannelCredentials()))
    {
      up_names["up-a"] = std::string("this is UP-A");
      up_names["up-b"] = std::string("this is UP-B");
      setup();
    };

    virtual
    ~Example()
    {
      release();
    };

    void
    sg_create();

    void
    sg_delete();

    void
    sg_list();

    void
    sg_read();

    void
    up_create(
      const std::string& up_name,
      const std::string& up_desc);

    void
    up_delete(
      const std::string& up_name);

    void
    up_list();

    void
    up_read(
      const std::string& up_name);

    void
    shard_create();

    void
    shard_delete();

    void
    shard_list();

    void
    shard_read();

    void
    session_context_create();

    void
    session_context_delete();

    void
    session_context_list();

    void
    session_context_read();

    void
    tsf_create();

    void
    tsf_delete();

    void
    tsf_list();

    void
    tsf_read();

    void
    nc_create();

    void
    nc_delete();

    void
    nc_list();

    void
    nc_read();
 
  private:
    void
    setup();

    void
    release();
};

#endif
