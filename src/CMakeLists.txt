set(SRC_LIST 
    mprpcapplication.cc 
    mprpcconfig.cc
    rpcprovider.cc)
add_library(mprpc ${SRC_LIST})
target_link_libraries(mprpc muduo_net muduo_base pthread)