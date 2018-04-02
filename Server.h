//
// Created by Jasson Rodríguez Méndez on 1/4/18.
//

#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio/read.hpp>
#include <boost/bind.hpp>

#include "Session.h"

#ifndef CFACTORIAL_SERVER_SERVER_H
#define CFACTORIAL_SERVER_SERVER_H

using namespace boost::asio::ip;

class Server {
private:
    boost::asio::io_service  *ioservice;
    tcp::acceptor tcp_acceptor;


public:
    Server(const tcp::endpoint &endpoint, boost::asio::io_service *io_service);
    void start_accept();
    void accept_handler(const boost::system::error_code &ec, Session *session);

};




#endif //CFACTORIAL_SERVER_SERVER_H
