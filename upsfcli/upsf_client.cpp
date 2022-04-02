#include "upsf.hpp"

int main(int argc, char** argv) {

  upsf::UpsfClient client(
      grpc::CreateChannel("localhost:50051",
                          grpc::InsecureChannelCredentials()));

  bbf::sss::UpdateServiceGatewayReq req;
  bbf::sss::ServiceGatewayResp resp;

  if (!client.UpdateServiceGateway(req, resp)) {
    std::cerr << "failed to call UpdateServiceGateway" << std::endl;
  }

  return 0;
};
