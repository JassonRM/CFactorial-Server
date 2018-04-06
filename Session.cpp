//
// Created by Jasson Rodríguez Méndez on 2/4/18.
//

#include <iostream>
#include "Session.h"


Session::Session(boost::asio::io_service *ioservice) : tcp_socket(*ioservice){}

void Session::start() {
    std::cout<<"Conectado"<<std::endl;
    tcp_socket.async_read_some(boost::asio::buffer(bytes), boost::bind(&Session::read_handler, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void Session::read_handler(const boost::system::error_code &ec, std::size_t bytes_transferred) {
    std::string request = "";
    rapidjson::Document jsonRequest;
    request.append(bytes.data());
    jsonRequest.Parse(request.data());
    if(!ec){
        if(jsonRequest.IsObject()) {
            //std::string response = memorymanager.analyze(jsonRequest);
            std::string response = "Se ha recibido el mensaje: " + request;
            boost::asio::async_write(tcp_socket, boost::asio::buffer(response),
                                     boost::bind(&Session::write_handler, this, boost::asio::placeholders::error,
                                                 boost::asio::placeholders::bytes_transferred));
        }
        else{
            tcp_socket.async_read_some(boost::asio::buffer(bytes), boost::bind(&Session::read_handler, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
        }
    }else{
        std::cout<<"Datos recibidos formateados incorrectamente"<<std::endl;
    }
}

void Session::write_handler(const boost::system::error_code &ec, std::size_t bytes_transferred) {
    if(!ec){
        std::cout<<"Escrito correctamente"<<std::endl;
    }else{
        std::cout<<"Error de escritura: "<<ec<<std::endl;
    }
}