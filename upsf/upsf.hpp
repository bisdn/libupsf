#ifndef _UPSF_HPP_
#define _UPSF_HPP_

#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>

#include "sssupsf.grpc.pb.h"

// using bbf::sss::sssUpsf::ServiceGatewayReq;
// using bbf::sss::sssUpsf::ServiceGatewayResp;

// rpc UpdateServiceGateway (UpdateServiceGatewayReq) returns
// (ServiceGatewayResp) {}

namespace upsf {
class UpsfClient {
public:
  /**
   * constructor
   */
  UpsfClient(std::shared_ptr<grpc::Channel> channel)
      : channel(channel), stub_(bbf::sss::sssUpsf::NewStub(channel)) {}

  /**
   * destructor
   */
  virtual ~UpsfClient() {}

public:
  /**
   * UpdateServiceGateway
   */
  bool UpdateServiceGateway(const bbf::sss::UpdateServiceGatewayReq &req,
                            bbf::sss::ServiceGatewayResp &resp) {
    grpc::ClientContext context;
    grpc::Status status = stub_->UpdateServiceGateway(&context, req, &resp);
    if (!status.ok()) {
      std::cerr << "UpdateServiceGateway rpc failed." << std::endl;
      return false;
    }
    return true;
  };

private:
  std::shared_ptr<grpc::Channel> channel;
  std::unique_ptr<bbf::sss::sssUpsf::Stub> stub_;
};
} // namespace upsf

#endif
