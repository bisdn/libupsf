/* upsf_cpp_example
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

#ifndef UPSF_CPP_EXAMPLE_HPP
#define UPSF_CPP_EXAMPLE_HPP

#include <map>
#include <string>

#include "upsf.hpp"
#include "upsf_stream.hpp"

class UpsfCppExample {
private:
    /* server address */
    std::string srvaddr;
    /* service gateway: name */
    std::string sg_name;
    /* service gateway: description */
    std::string sg_desc;
    /* service gateway user plane: name:description */
    std::map<std::string, std::string> up_names;
    /* subscriber group: name */
    std::string sgrp_name;
    /* subscriber group: description */
    std::string sgrp_desc;
    /* session context: name */
    std::string sctx_name;
    /* session context: description */
    std::string sctx_desc;
    /* traffic steering function: name */
    std::string tsf_name;
    /* traffic steering function: description */
    std::string tsf_desc;
    /* network connection: name */
    std::string nc_name;
    /* network connection: description */
    std::string nc_desc;
    /* libupsf client */
    upsf::UpsfClient client;

public:
    UpsfCppExample(
        const std::string& srvaddr = std::string("127.0.0.1:50051"),
        const std::string& sg_name = std::string("my-sg"),
        const std::string& tsf_name = std::string("my-tsf"),
        const std::string& sgrp_name = std::string("my-sgrp"),
        const std::string& sctx_name = std::string("my-sctx"),
        const std::string& nc_name = std::string("my-nc"),
        const std::string& sg_desc = std::string("this is my service gateway"),
        const std::string& tsf_desc = std::string("this is my traffic steering function"),
        const std::string& sgrp_desc = std::string("this is my subscriber group"),
        const std::string& sctx_desc = std::string("this is my session context"),
        const std::string& nc_desc = std::string("this is my network connection"))
        : srvaddr(srvaddr)
        , sg_name(sg_name)
        , tsf_name(tsf_name)
        , sgrp_name(sgrp_name)
        , nc_name(nc_name)
        , sctx_name(sctx_name)
        , sg_desc(sg_desc)
        , tsf_desc(tsf_desc)
        , sgrp_desc(sgrp_desc)
        , sctx_desc(sctx_desc)
        , nc_desc(nc_desc)
        , client(
              grpc::CreateChannel(srvaddr, grpc::InsecureChannelCredentials()))
    {
        up_names["my-up-a"]
            = std::string("this is my service gateway user plane A");
        up_names["my-up-b"]
            = std::string("this is my service gateway user plane B");
    };

    virtual ~UpsfCppExample() {};

    void sg_create(const std::string& sg_name);
    void sg_delete(const std::string& sg_name);
    void sg_read(const std::string& sg_name);
    bool sg_exists(const std::string& sg_name);
    void sg_list();

    void up_create(const std::string& up_name, const std::string& up_desc);
    void up_delete(const std::string& up_name);
    void up_read(const std::string& up_name);
    bool up_exists(const std::string& up_name);
    void up_list();

    void tsf_create(const std::string& tsf_name);
    void tsf_delete(const std::string& tsf_name);
    void tsf_read(const std::string& tsf_name);
    bool tsf_exists(const std::string& tsf_name);
    void tsf_list();

    void sgrp_create(const std::string& sgrp_name);
    void sgrp_delete(const std::string& sgrp_name);
    void sgrp_read(const std::string& sgrp_name);
    bool sgrp_exists(const std::string& sgrp_name);
    void sgrp_list();

    void sctx_create(const std::string& sctx_name);
    void sctx_delete(const std::string& sctx_name);
    void sctx_read(const std::string& sctx_name);
    bool sctx_exists(const std::string& sctx_name);
    void sctx_list();

    void nc_create(const std::string& nc_name);
    void nc_delete(const std::string& nc_name);
    void nc_read(const std::string& nc_name);
    bool nc_exists(const std::string& nc_name);
    void nc_list();

public:
    void create_items();
    void delete_items();
};

#endif
