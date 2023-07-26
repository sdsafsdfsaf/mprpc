#include "mprpcapplication.h"
#include "mprpcchannel.h"
#include "mprpcconfig.h"
#include "../user.pb.h"
#include <iostream>

int main(int argc, char **argv)
{
    MprpcApplication::Init(argc, argv);
    fixbug::UserServiceRpc_Stub stub(new MprpcChannel());

    fixbug::LoginRequest request;
    request.set_name("zhang san");
    request.set_pwd("123456");
    fixbug::LoginResponse response;

    stub.Login(nullptr, &request, &response, nullptr);
    if (0 == response.result().errcode())
    {
        std::cout << "rpc login response success:" << response.sucess() << std::endl;
    }
    else
    {
        std::cout << "rpc login response error : " << response.result().errmsg() << std::endl;
    }
}