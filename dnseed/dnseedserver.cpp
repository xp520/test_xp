// Copyright (c) 2017-2018 The Multiverse developers xp@fnfn
#include "dnseedserver.h"

using namespace walleve;
using namespace multiverse::network;

#define TIMING_FILTER_INTERVAL (24*60*60)
#define SEND_ADDRESS_LIMIT 50

void DNSeedServer::init()
{
    storage::CMvDBConfig dbConfig(StorageConfig()->strDBHost,StorageConfig()->nDBPort,
                                  StorageConfig()->strDBName,StorageConfig()->strDBUser,StorageConfig()->strDBPass);
    if(!_db.init(dbConfig))
    {
        WalleveLog("Failed to initialize DNSeed database\n");
        return false;
    }
    //load _a
}

bool DNSeedServer::add2list(boost::asio::ip::tcp::endpoint newep)
{
    //过滤局域网地址
    if(!IsRoutable(newep.address())) return false;
    if(this->hasAddress(newep)) return false;

    _nodeList.push_back(SeedNode(newep));
    //TODO 入库
    
    return true;
}

void DNSeedServer::getAddressList(std::vector<SeedNode> & list,GetNodeWay gettype)
{
    if(gettype==GetNodeWay::GET_ALL)
    {
        list=_nodeList;
    }
   
    for(size_t i=0;i<_nodeList.size()&&list.size()<SEND_ADDRESS_LIMIT;i++)
    {
        //TODO choose role
        list.push_back(_nodeList[i]);
    }
    
}

bool DNSeedServer::hasAddress(boost::asio::ip::tcp::endpoint ep)
{
    for(size_t i=0;i<_nodeList.size();i++)
    {
        if(_nodeList[i]._ep==ep) return true;
    }
    return false;
}

void DNSeedServer::switchAddressList(std::vector<SeedNode> epList)
{
    for(SeedNode sa :epList)
    {
        if(this->hasAddress(sa._ep)) continue;
        else _nodeList.push_back(SeedNode(sa._ep));
    }
}

void DNSeedServer::filterAddressList()
{
    //建立连接
    //检查高度
    //断开连接

}

bool DNSeedServer::updateScore(SeedNode node)
{
    
}