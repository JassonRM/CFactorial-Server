//
// Created by Jasson Rodríguez Méndez on 2/4/18.
//

#include "Session.h"


Session::Session(boost::asio::io_service *ioservice) : tcp_socket(*ioservice){}

void Session::start() {
    std::string data = "wow que cool\n";
    boost::asio::async_write(tcp_socket, boost::asio::buffer(data), boost::bind(&Session::write_handler, this));
    //boost::asio::async_read(tcp_socket, boost::asio::buffer(bytes), read_handler));
}

void Session::read_handler(const boost::system::error_code &ec) {
    if(!ec){
        // Aqui procesa la peticion del cliente
    }
}

void Session::write_handler() {
    // Aqui responde
}