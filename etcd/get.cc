#include <etcd/Client.hpp>
#include <etcd/KeepAlive.hpp>
#include <etcd/Response.hpp>
#include <etcd/Watcher.hpp>
#include <etcd/Value.hpp>
#include <etcd/SyncClient.hpp>

void callback(const etcd::Response& rsp)
{
    if(rsp.is_ok()==false)
    {
        std::cout<<"错误事件通知"<<rsp.error_message()<<std::endl;
        return;
    }
    for(auto const& ev: rsp.events())
    {
        if (ev.event_type() == etcd::Event::EventType::PUT) {
            std::cout << "服务信息发生了改变:\n" ;
            std::cout << "当前的值：" << ev.kv().key() << "-" << ev.kv().as_string() << std::endl;
            std::cout << "原来的值：" << ev.prev_kv().key() << "-" << ev.prev_kv().as_string() << std::endl;
        }else if (ev.event_type() == etcd::Event::EventType::DELETE_) {
            std::cout << "服务信息下线被删除:\n";
            std::cout << "当前的值：" << ev.kv().key() << "-" << ev.kv().as_string() << std::endl;
            std::cout << "原来的值：" << ev.prev_kv().key() << "-" << ev.prev_kv().as_string() << std::endl;
        }
    }
}

int main(int argc,char* argv[])
{
    //创建客户端
    etcd::Client client("http://127.0.0.1:2379");
    //获取数据
    auto rsp = client.ls("/source").get();
    if(rsp.is_ok()==false)
    {
        std::cout<<"数据获取失败"<<std::endl;
        return -1;
    }
    int n = rsp.keys().size();
    for(int i = 0;i<n;i++)
    {
        std::cout<<rsp.value(i).as_string()<<":"<<rsp.key(i)<<std::endl;
    }
    auto watcher = etcd::Watcher(client,"/source",callback,true);
    watcher.Wait();
    return 0;
}

