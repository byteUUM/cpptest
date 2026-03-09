#include <etcd/Client.hpp>
#include <etcd/KeepAlive.hpp>
#include <etcd/Response.hpp>
#include <etcd/Watcher.hpp>
#include <etcd/Value.hpp>
#include <etcd/SyncClient.hpp>
#include <memory>
#include <functional>
class Registry
{
public:
    Registry(const std::string& host)
    :_client(std::make_shared<etcd::Client>(host))
    ,_keep_alive(_client->leasekeepalive(3).get())
    ,_lease_id(_keep_alive->Lease())
    {}
    bool registry(const std::string& key,const std::string& val)
    {
        auto rsp = _client->put(key,val,_lease_id,true).get();
        if(rsp.is_ok()==false)
        {
            //建议替换为日志输出
            std::cout<<key<<": "<<val<<"注册失败"<<std::endl;
            return false;
        }
        else return true;
    }
    ~Registry()
    {
        _client->Cancel();
    }
private:
    std::shared_ptr<etcd::Client> _client;
    std::shared_ptr<etcd::KeepAlive> _keep_alive;
    uint64_t _lease_id;
};
class Discovery
{
public:
    using NotifyCallback = std::function<void(std::string,std::string)>;
    Discovery(const std::string& host
        ,const std::string& basedir
        ,const NotifyCallback& put_cb
        ,const NotifyCallback& del_cb)
    :
    _client(std::make_shared<etcd::Client>(host))
    ,_put_cb(put_cb)
    ,_del_cb(del_cb)
    {
        auto rsp = _client->ls(basedir).get();
        if(rsp.is_ok()==false)
        {
            std::cout<<"获取数据失败 "<<rsp.error_message()<<std::endl;
        }
        int sz = rsp.keys().size();
        for(int i=0;i<sz;i++)
        {
            if(_del_cb)
                _put_cb(rsp.key(i),rsp.value(i).as_string());
        }
        _watcher = std::make_shared<etcd::Watcher>(*_client,basedir
            ,std::bind(&Discovery::callback,this, std::placeholders::_1),true);
    }
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
                std::cout << "上线服务：" << ev.kv().key() << "-" << ev.kv().as_string() << std::endl;
            }else if (ev.event_type() == etcd::Event::EventType::DELETE_) {
                std::cout << "下线服务：" << ev.kv().key() << "-" << ev.kv().as_string() << std::endl;
            }
        }
    }
    ~Discovery()
    {
        _watcher->Cancel();
    }
private:
    NotifyCallback _put_cb;
    NotifyCallback _del_cb;
    std::shared_ptr<etcd::Client> _client;
    std::shared_ptr<etcd::Watcher> _watcher;
};

