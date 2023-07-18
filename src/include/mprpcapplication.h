#pragma once
#include <proc_service.h>
#include "mprpcconfig.h"
class MprpcApplication
{
private:
    static MprpcConfig m_config;

    MprpcApplication() {}
    MprpcApplication(MprpcApplication &&) = delete;
    MprpcApplication(const MprpcApplication &) = delete;

public:
    static void Init(int argc, char **argv);
    static MprpcApplication &GetInstance();
    static MprpcConfig &GetConfig();
};
