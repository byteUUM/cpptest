#include <etcd/Client.hpp>
#include <etcd/KeepAlive.hpp>
#include <etcd/Response.hpp>
#include <etcd/Watcher.hpp>
#include <thread>
int main(int argc,char* argv[])
{
    //创建客户端
    etcd::Client client("http://127.0.0.1:2379");
    //指定租约并获取租约保活对象
    auto keep_alive = client.leasekeepalive(3).get();
    //获取租约id
    auto lease_id = keep_alive->Lease();
    //向etcd新增数据
    auto rsp = client.put("/source/user","127.0.0.1:6000",lease_id).get();
    if(rsp.is_ok()==false)
        std::cout<<"新增数据失败"<<std::endl;
    rsp = client.put("/source/file","127.0.0.1:6001",lease_id).get();
    if(rsp.is_ok()==false)
        std::cout<<"新增数据失败"<<std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(10));
    return 0;
}

