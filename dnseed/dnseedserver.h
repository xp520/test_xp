// Copyright (c) 2017-2018 The Multiverse developers xp
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#ifndef __DNSEED_SERVER__
#define __DNSEED_SERVER__

#include "walleve/walleve.h"
#include "dnseeddb.h"


namespace multiverse 
{

namespace network
{


class DNSeedServer:public walleve::IWalleveBase 
{
public: 
    DNSeedServer(){
        _isDNSeedServerNode=false;
        _initTime=walleve::GetTime();
        _runTime=0;
        init();
    } 
    ~DNSeedServer(){}
public:
    enum GetNodeWay{
        GET_ALL,
        GET_A_LOT
    };
    void getAddressList(std::vector<storage::SeedNode> & list,GetNodeWay gettype=GET_A_LOT);
    bool add2list(boost::asio::ip::tcp::endpoint newep);
    void switchAddressList(std::vector<storage::SeedNode> epList);

    static int test();
    storage::DNSeedDB * testGetDb(){return &_db;}
private:
    bool init();
    void filterAddressList();
    bool hasAddress(boost::asio::ip::tcp::endpoint ep);
    bool updateScore(storage::SeedNode node);

private:
    std::vector<storage::SeedNode> _nodeList;
    bool _isDNSeedServerNode;

    multiverse::storage::DNSeedDB _db;
    //timer
    int64 _initTime;
    int64 _runTime;
//advanced
    //定时任务

};
}
}
#endif