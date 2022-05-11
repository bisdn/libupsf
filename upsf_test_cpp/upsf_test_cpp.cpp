#include "upsf.hpp"

#include <string>
#include <gflags/gflags.h>
#include <google/protobuf/util/time_util.h>

DEFINE_string(upsfhost, "localhost", "UPSF host");
DEFINE_int32(upsfport, 50051, "UPSF port");

/**
 *
 */
int list_service_gateways(upsf::UpsfClient &client) {

  bbf::sss::ListServiceGatewayResp lresp;
  if (!client.ListServiceGateways(lresp)) {
    LOG(ERROR) << "failure: ListServiceGateways" << std::endl;
    return -1;
  }

  if (lresp.result() > 0) {
    LOG(ERROR) << "failure: ListServiceGateways, result: " << lresp.result() << std::endl;
    return -1;
  }

  for (int i = 0; i < lresp.service_gateway_size(); i++) {
    LOG(INFO) << "success: ListServiceGateways, "
              << "id[" << i << "]:" << lresp.service_gateway(i).id() << " "
              << "name:" << lresp.service_gateway(i).metadata().name() << " "
              << "status:" << lresp.service_gateway(i).metadata().item_status() << " "
              << "created:" << google::protobuf::util::TimeUtil::ToString(lresp.service_gateway(i).metadata().created()) << " "
              << "updated:" << google::protobuf::util::TimeUtil::ToString(lresp.service_gateway(i).metadata().last_updated()) << " "
              << "delete_requested:" << google::protobuf::util::TimeUtil::ToString(lresp.service_gateway(i).metadata().delete_requested()) << " "
              << "deleted:" << google::protobuf::util::TimeUtil::ToString(lresp.service_gateway(i).metadata().deleted()) << " "
              << std::endl;
  }
  return 0;
}

int main(int argc, char** argv) {

  gflags::ParseCommandLineFlags(&argc, &argv, true);
  google::InitGoogleLogging(argv[0]);

  VLOG(1) << "UPSF host: " << FLAGS_upsfhost << std::endl;
  VLOG(1) << "UPSF port: " << FLAGS_upsfport << std::endl;
  VLOG(1) << "logtostderr: " << FLAGS_logtostderr << std::endl;

  // upsf address
  std::stringstream upsfaddr;
  upsfaddr << FLAGS_upsfhost << ":" << FLAGS_upsfport;

  // upsf client
  upsf::UpsfClient client(
      grpc::CreateChannel(upsfaddr.str(),
                          grpc::InsecureChannelCredentials()));


  // list service gateways
  list_service_gateways(client);

  // update service gateway
  bbf::sss::ServiceGatewayResp uresp;
  if (!client.UpdateServiceGateway(std::string("up1"), bbf::sss::ItemStatus::updating, uresp)) {
    LOG(ERROR) << "failure: UpdateServiceGateway" << std::endl;
    return -1;
  }

  if (uresp.result() > 0) {
    LOG(ERROR) << "failure: UpdateServiceGateway, result: " << uresp.result() << std::endl;
    return -1;
  }

  LOG(INFO) << "success: UpdateServiceGateway, id:" << uresp.service_gateway().id() << std::endl;

  // list service gateways
  list_service_gateways(client);


  // get service gateway
  bbf::sss::ServiceGatewayResp gresp;
  if (!client.GetServiceGateway(uresp.service_gateway().id(), gresp)) {
    LOG(ERROR) << "failure: GetServiceGateway" << std::endl;
    return -1;
  }

  if (gresp.result() > 0) {
    LOG(ERROR) << "failure: GetServiceGateway, result: " << gresp.result() << std::endl;
    return -1;
  }

  LOG(INFO) << "success: GetServiceGateway, id:" << gresp.service_gateway().id() << std::endl;


  // delete service gateway
  bbf::sss::ServiceGatewayResp dresp;
  if (!client.DeleteServiceGateway(uresp.service_gateway().id(), dresp)) {
    LOG(ERROR) << "failure: DeleteServiceGateway" << std::endl;
    return -1;
  }

  if (dresp.result() > 0) {
    LOG(ERROR) << "failure: DeleteServiceGateway, result: " << dresp.result() << std::endl;
    return -1;
  }

  LOG(INFO) << "success: DeleteServiceGateway, id:" << dresp.service_gateway().id() << std::endl;

  // list service gateways
  list_service_gateways(client);


  return 0;
};
