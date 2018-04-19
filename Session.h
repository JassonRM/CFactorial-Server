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
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include "MemoryManager.h"

#ifndef CFACTORIAL_SERVER_SESSION_H
#define CFACTORIAL_SERVER_SESSION_H


using namespace boost::asio::ip;

/**
 * Sesion de transferencia en red
 */
class Session{
private:
    /**
     * Buffer de memoria
     */
    char bytes[4096];
    /**
     * Manejador de memoria de la sesion
     */
    MemoryManager *memoryManager;

public:
    /**
     * Socket de red al cual esta conectado
     */
    tcp::socket tcp_socket;
    /**
     * Constructor de la sesion
     * @param ioservice Servicio de entradas y salidas
     */
    Session(boost::asio::io_service *ioservice);
    /**
     * Inicia la recepcion de la informacion una vez conectado
     */
    void start();
    /**
     * Maneja la entrada de datos a traves del socket
     * @param ec Codigo de error
     * @param bytes_transferred Datos recibidos
     */
    void read_handler(const boost::system::error_code &ec, std::size_t bytes_transferred);
    /**
     * Maneja la salida de datos a traves del socket
     * @param ec Codigo de error
     * @param bytes_transferred Datos enviados
     */
    void write_handler(const boost::system::error_code &ec, std::size_t bytes_transferred);
};


#endif //CFACTORIAL_SERVER_SESSION_H
