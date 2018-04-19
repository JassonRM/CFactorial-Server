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

/**
 * Servidor encargado de recibir conexiones
 */
class Server {
private:
    /**
     * Servicio de entradas y salidas
     */
    boost::asio::io_service  *ioservice;
    /**
     * Aceptor de conexiones TCP
     */
    tcp::acceptor tcp_acceptor;


public:
    /**
     * Constructor del servidor
     * @param endpoint Direccion ip en la que va a escuchar
     * @param io_service Servicio de entradas y salidas
     */
    Server(const tcp::endpoint &endpoint, boost::asio::io_service *io_service);
    /**
     * Inicia proceso de escucha
     */
    void start_accept();
    /**
     * Maneja las conexiones entrantes
     * @param ec Codigo de error en la transferencia d edatos
     * @param session Sesion de transferencia
     */
    void accept_handler(const boost::system::error_code &ec, Session *session);
};




#endif //CFACTORIAL_SERVER_SERVER_H
