#include "upsf.hpp"

int main(int argc, char** argv) {

  upsf::UpsfClient client(
      grpc::CreateChannel("localhost:50051",
                          grpc::InsecureChannelCredentials()));

  bbf::sss::ServiceGatewayResp resp;
  bbf::sss::UpdateServiceGatewayReq req;

  req.mutable_service_gateway()->mutable_metadata()->set_name(std::string("up1"));

  if (!client.UpdateServiceGateway(req, resp)) {
    std::cerr << "failed to call UpdateServiceGateway" << std::endl;
  }

  std::cout << resp.result() << std::endl;
  std::cout << resp.service_gateway().id() << std::endl;

  return 0;
};
