// Copyright (c) 2017-2018 The Multiverse developer xp
#ifndef __DNSEED_DB__
#define __DNSEED_DB__

#include "dbconn.h"
#include <boost/asio.hpp>
#include <boost/any.hpp>

namespace multiverse
{
namespace storage
{

class SeedNode{
public:
    SeedNode(){}
    SeedNode(boost::asio::ip::tcp::endpoint ep,int score=0)
    {
        this->_ep=ep;
        this->_score=score;
    }
public:
    int _id;
    boost::asio::ip::tcp::endpoint _ep;
    int _score;
};

class DNSeedDB
{
public:
    DNSeedDB();
    ~DNSeedDB();

    bool insertNode(SeedNode &node);
    bool deleteNode(SeedNode &node);
    bool updateNodeScore(SeedNode &node);
    bool selectAllNode(std::vector<SeedNode> & nodeList);
private:
    bool init(const CMvDBConfig& config);
    void deinit();
    bool CreateTable();
protected:
    CMvDBConn dbConn;
};

}
}
#endif 