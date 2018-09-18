#include "iostream"
#include <string>
#include <boost/ref.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>




int main(int argc,char **argv)
{
    //创建一个测试节点,连接到DNseed服务
    std::cout<<"Hello cmake"<<std::endl;
    
    boost::asio::io_service io_service_;
    boost::asio::ip::tcp::socket sock_(io_service_);
    sock_.open(boost::asio::ip::tcp::v4());
    boost::asio::ip::tcp::endpoint endpoint_1(boost::asio::ip::tcp::v4(),6816);
    boost::asio::ip::address addr = boost::asio::ip::address::from_string("113.105.146.22");
    endpoint_1.address(addr);
    boost::system::error_code err;
    sock_.connect(endpoint_1, err);

    //读取消息

    //发送握手消息

    while(true)
    {
        
    }

    return 0;
}