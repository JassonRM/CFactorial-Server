//
// Created by Jasson Rodríguez Méndez on 1/4/18.
//

#include "Server.h"

using namespace boost::asio::ip;

Server::Server(const tcp::endpoint &endpoint, boost::asio::io_service *io_service) : ioservice(io_service), tcp_acceptor(*ioservice, endpoint) {
    start_accept();
}

void Server::start_accept() {
    Session *session(new Session(ioservice));
    tcp_acceptor.async_accept(session->tcp_socket, boost::bind(&Server::accept_handler, this, _1, session));
}

void Server::accept_handler(const boost::system::error_code &ec, Session *session) {
    if(!ec){
        session->start();
    }
    start_accept();
}



