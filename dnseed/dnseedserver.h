// Copyright (c) 2017-2018 The Multiverse developers xp
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#ifndef __DNSEED_SERVER__
#define __DNSEED_SERVER__

#include "walleve/walleve.h"
#include "dnseeddb.h"

//class multiverse::storage::DNSeedDB;
namespace multiverse 
{

namespace network
{

// struct SeedNode{
//     SeedNode(){}
//     SeedNode(boost::asio::ip::tcp::endpoint ep,int score=0)
//     {
//         this->_ep=ep;
//         this->_score=score;
//     }

//     int _id;
//     boost::asio::ip::tcp::endpoint _ep;
//     int _score;
// };

class DNSeedServer:public IWalleveBase{
public: 
    DNSeedServer(){
        _initTime=walleve::GetTime();
    } 
    ~DNSeedServer(){}
public:
    enum GetNodeWay{
        GET_ALL,
        GET_A_LOT
    };
    void getAddressList(std::vector<SeedNode> & list,GetNodeWay gettype=GET_A_LOT);
    bool add2list(boost::asio::ip::tcp::endpoint newep);
    void switchAddressList(std::vector<SeedNode> epList);
private:
    void init();
    void filterAddressList();
    bool hasAddress(boost::asio::ip::tcp::endpoint ep);
    bool updateScore(SeedNode node);
private:
    std::vector<SeedNode> _nodeList;
    bool _isDNSeedServerNode=false;

    storage::DNSeedDB _db;
    //timer
    int64 _initTime;
    int64 _runTime=0;
//advanced
    //定时任务

};
}
}
#endif