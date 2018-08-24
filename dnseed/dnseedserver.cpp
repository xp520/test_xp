// Copyright (c) 2017-2018 The Multiverse developers xp@fnfn
#include "dnseedserver.h"


using namespace walleve;
using namespace multiverse;
using namespace multiverse::network;
using namespace multiverse::storage;

#define TIMING_FILTER_INTERVAL (24*60*60)
#define SEND_ADDRESS_LIMIT 50

bool DNSeedServer::init()
{
    //const CMvStorageConfig * config=dynamic_cast<const CMvStorageConfig *>(IWalleveBase::WalleveConfig());
    CMvDBConfig dbConfig("127.0.0.1",3306,"multiverse","multiverse","multiverse");
    if(!_db.init(dbConfig))
    {
        WalleveLog("Failed to initialize DNSeed database\n");
        return false;
    }
    //load _a

    return true;
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
    if(gettype==GET_ALL)
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
    for(size_t i=0;i<epList.size();i++)
    {
        SeedNode * sa=&epList[i];
        if(this->hasAddress(sa->_ep)) continue;
        else _nodeList.push_back(SeedNode(sa->_ep));
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

int DNSeedServer::test()
{
    /* code */
    DNSeedServer dnseed;
    DNSeedDB* db=dnseed.testGetDb();

    // boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address().from_string("174.137.61.150"),1111);
    // SeedNode node(ep);
    
    // bool rzt=db->insertNode(node);

    // node._score=999;
    // bool rzt1=db->updateNodeScore(node);

    std::vector<SeedNode> list;
    db->selectAllNode(list);
   SeedNode* a=db->findOneWithAddress("174.137.61.150");
    
    return 0;
}