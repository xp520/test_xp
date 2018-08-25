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

    address_v4::bytes_type bt=node._ep.address().to_v4().to_bytes();
    oss << "INSERT INTO dnseednode(address,port,score) "
              "VALUES("
        <<            "\'" << dbConn.ToEscString(&bt[0],bt.size()) << "\',"
        <<            node._ep.port() << ","
        <<            node._score << ")";
    std::string str=oss.str();
    return dbConn.Query(oss.str());
}

bool DNSeedDB::deleteNode(SeedNode &node)
{
    std::ostringstream oss;
    address_v4::bytes_type bt=node._ep.address().to_v4().to_bytes();
    oss << "DELETE FROM dnseednode"
        <<            "where address =\'" << dbConn.ToEscString(&bt[0],bt.size()) << "\'";
    return dbConn.Query(oss.str());
}

SeedNode* DNSeedDB::findOneWithAddress(std::string ip)
{
    tcp::endpoint ep(address::from_string(ip),123);
    
    address_v4::bytes_type bt=ep.address().to_v4().to_bytes();
    std::ostringstream oss;
    oss<<"SELECT id,address,port,score FROM dnseednode where address = \'"
    << dbConn.ToEscString(&bt[0],bt.size())<<"\'";
    std::string str=oss.str();
    CMvDBRes res(dbConn,str,true);
    while(res.GetRow())
    {
        SeedNode* node=new SeedNode();
        std::vector<unsigned char>  addbyte;
        short port;
        if (!res.GetField(0,node->_id) || !res.GetField(1,addbyte) || !res.GetField(2,port) 
            || !res.GetField(3,node->_score))
        {
            return 0;
        }
        address_v4::bytes_type byte;
        for(size_t i=0;i<4;i++)
        {
            byte[i]=addbyte[i];
        }
        node->_ep=tcp::endpoint(address(address_v4(byte)),port);
        return node;
    }
    return 0;
}

bool DNSeedDB::updateNodeScore(SeedNode &node)
{
    std::ostringstream oss;
    address_v4::bytes_type bt=node._ep.address().to_v4().to_bytes();
    oss << "UPDATE dnseednode SET score = " << node._score   
                << " WHERE address = " << "\'" << dbConn.ToEscString(&bt[0],bt.size()) << "\'"
                << "or id="<<18;
    std::string str=oss.str();
    return dbConn.Query(oss.str());
}

bool DNSeedDB::selectAllNode(std::vector<SeedNode> & nodeList)
{
    CMvDBRes res(dbConn,"SELECT id,address,port,score FROM dnseednode limit 100",true);
    while (res.GetRow())
    {
       
        SeedNode node;
        std::vector<unsigned char>  addbyte;
        short port;
        if (!res.GetField(0,node._id) || !res.GetField(1,addbyte) || !res.GetField(2,port) 
            || !res.GetField(3,node._score))
        {
            return false;
        }
        address_v4::bytes_type byte;
        for(size_t i=0;i<4;i++)
        {
            byte[i]=addbyte[i];
        }
        node._ep=tcp::endpoint(address(address_v4(byte)),port);
        nodeList.push_back(node);
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

