// Copyright (c) 2017-2018 The Multiverse developer xp

#include "dnseeddb.h"

using namespace multiverse::storage;
using namespace boost::asio::ip;

DNSeedDB::DNSeedDB()
{

}

DNSeedDB::~DNSeedDB()
{
    deinit();
}

bool DNSeedDB::init(const CMvDBConfig& config)
{
    if (!dbConn.Connect(config))
    {
        return false;
    }
    return CreateTable();
}

void DNSeedDB::deinit()
{
    dbConn.Disconnect();
}

bool DNSeedDB::insertNode(SeedNode &node)
{
    std::ostringstream oss;
    oss << "INSERT INTO dnseednode(address,port,score) "
              "VALUES("
        <<            "\'" << node._ep.address().to_v6().to_bytes().data() << "\',"
        <<            node._ep.port() << ","
        <<            node._score << ")";
    return dbConn.Query(oss.str());
}

bool DNSeedDB::deleteNode(SeedNode &node)
{
    std::ostringstream oss;
    oss << "DELETE FROM dnseednode"
        <<            "where address =\'" << node._ep.address().to_v6().to_bytes().data() << "\'";
    return dbConn.Query(oss.str());
}

bool DNSeedDB::updateNodeScore(SeedNode &node)
{
    std::ostringstream oss;
    oss << "UPDATE dnseednode SET score = " << node._score   
                << " WHERE address = " << "\'" << node._ep.address().to_v6().to_bytes().data() << "\'";
    return dbConn.Query(oss.str());
}

bool DNSeedDB::selectAllNode(std::vector<SeedNode> & nodeList)
{
    CMvDBRes res(dbConn,"SELECT id,address,port,score FROM dnseednode",true);
    while (res.GetRow())
    {
        SeedNode node;
        byte_string address;
        short port;
        if (!res.GetField(0,node._id) || !res.GetField(1,address) || !res.GetField(2,port) 
            || !res.GetField(3,node._score))
        {
            return false;
        }
        node._ep=tcp::endpoint(tcp::address.from_bytes(address),port);//
    }
    return true;
}

bool DNSeedDB::CreateTable()
{
        return dbConn.Query("CREATE TABLE IF NOT EXISTS dnseednode("
                          "id INT NOT NULL AUTO_INCREMENT PRIMARY KEY,"
                          "address BINARY(16) NOT NULL UNIQUE KEY,"
                          "port INT NOT NULL,"
                          "score INT NOT NULL)"
                       );
}

// void main(int argc, char const *argv[])
// {
//     /* code */
//     DNSeedDB db;
//     db.init();
//     tcp::endpoint ep(tcp::address.from_string("174.137.61.150"),1111);
//     SeedNode node(ep);
    
//     db.insertNode(node);
//     return 0;
// }
