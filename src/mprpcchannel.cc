#include "mprpcchannel.h"
#include "rpcheader.pb.h"
#include "mprpcapplication.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>
#include <unistd.h>
#include "zookeeperutil.h"

void MprpcChannel::CallMethod(const google::protobuf::MethodDescriptor *method,
                              google::protobuf::RpcController *controller,
                              const google::protobuf::Message *request,
                              google::protobuf::Message *response,
                              google::protobuf::Closure *done)
{
    const google::protobuf::ServiceDescriptor *sd = method->service();
    std::string service_name = sd->name();
    std::string method_name = method->name();

    uint32_t args_size;
    std::string args_str;
    if (request->SerializeToString(&args_str))
    {
        args_size = args_str.size();
    }

    mprpc::RpcHeader rpcHeader;
    rpcHeader.set_service_name(service_name);
    rpcHeader.set_method_name(method_name);
    rpcHeader.set_args_size(args_size);
    uint32_t header_size;
    std::string header;
    if (rpcHeader.SerializeToString(&header))
    {
        header_size = header.size();
    }

    std::string send_rpc_str;
    send_rpc_str.insert(0, std::string((char *)&header_size, 4)); // ÓÐ´ýÀí½â
    send_rpc_str.append(header);
    send_rpc_str.append(args_str);

    int clientfd = socket(AF_INET, SOCK_STREAM, 0);
    if (clientfd == -1)
    {
        // TODO
    }

    // std::string ip = MprpcApplication::GetInstance().GetConfig().Load("rpcserverip");
    // uint16_t port = atoi(MprpcApplication::GetInstance().GetConfig().Load("rpcserverport").c_str());
    std::string method_path = "/" + service_name + "/" + method_name;
    ZkClient zkCli;
    zkCli.Start();
    std::string host_data = zkCli.GetData(method_path.c_str());
    int idx = host_data.find(":");
    std::string ip = host_data.substr(0, idx);
    uint16_t port = atoi(host_data.substr(idx + 1, host_data.size() - idx).c_str());

    struct sockaddr_in sever_addr;
    sever_addr.sin_family = AF_INET;
    sever_addr.sin_addr.s_addr = inet_addr(ip.c_str());
    sever_addr.sin_port = htons(port);

    if (-1 == connect(clientfd, (sockaddr *)&sever_addr, sizeof(sever_addr)))
    {
        // TODO
    }
    if (-1 == send(clientfd, (void *)send_rpc_str.c_str(), send_rpc_str.size(), 0))
    {
        // TODO
    }
    char recv_buf[1024] = {0};
    int recv_size = 0;
    if (-1 == (recv_size = recv(clientfd, recv_buf, 1024, 0)))
    {
        // TODO
    }
    if (!response->ParseFromArray(recv_buf, recv_size))
    {
        // TODO
    }
    close(clientfd);
}