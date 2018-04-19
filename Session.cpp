//
// Created by Jasson Rodríguez Méndez on 2/4/18.
//

#include <iostream>
#include "Session.h"
#include "MemoryManager.h"


Session::Session(boost::asio::io_service *ioservice) : tcp_socket(*ioservice){
    memoryManager = new MemoryManager(10240);
}

void Session::start() {
    std::cout<<"Conectado"<<std::endl;
    tcp_socket.async_read_some(boost::asio::buffer(bytes), boost::bind(&Session::read_handler, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void Session::read_handler(const boost::system::error_code &ec, std::size_t bytes_transferred) {
    std::string request = "";
    rapidjson::Document jsonRequest;
    request.append(bytes);
    std::cout<<"Request: " << bytes<<std::endl;
    jsonRequest.Parse(request.data());
    if(!ec){
        if(jsonRequest.IsObject()) {
            rapidjson::Document* response = memoryManager->request(&jsonRequest);


            rapidjson::StringBuffer buffer;
            rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
            response->Accept(writer);

            std::string strResponse = buffer.GetString();

            boost::asio::async_write(tcp_socket, boost::asio::buffer(strResponse),
                                     boost::bind(&Session::write_handler, this, boost::asio::placeholders::error,
                                                 boost::asio::placeholders::bytes_transferred));
        }
        else{
            tcp_socket.async_read_some(boost::asio::buffer(bytes), boost::bind(&Session::read_handler, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
        }
    }else{
        tcp_socket.async_read_some(boost::asio::buffer(bytes), boost::bind(&Session::read_handler, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
    }
}

void Session::write_handler(const boost::system::error_code &ec, std::size_t bytes_transferred) {
    if(!ec){
        std::cout<<"Request finalizado"<<std::endl;
        bytes[0] = '\0';
        tcp_socket.async_read_some(boost::asio::buffer(bytes), boost::bind(&Session::read_handler, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
    }else{
        std::cout<<"Error de escritura: "<<ec<<std::endl;
    }
}