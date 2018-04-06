//
// Created by Jasson Rodríguez Méndez on 2/4/18.
//

#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio/read.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <rapidjson/document.h>

#ifndef CFACTORIAL_SERVER_SESSION_H
#define CFACTORIAL_SERVER_SESSION_H


using namespace boost::asio::ip;

class Session{
private:
    std::array<char, 4096> bytes;
public:
    tcp::socket tcp_socket;

    Session(boost::asio::io_service *ioservice);
    void start();
    void read_handler(const boost::system::error_code &ec, std::size_t bytes_transferred);
    void write_handler(const boost::system::error_code &ec, std::size_t bytes_transferred);
};


#endif //CFACTORIAL_SERVER_SESSION_H
