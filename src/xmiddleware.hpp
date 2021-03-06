/***************************************************************************
* Copyright (c) 2018, Johan Mabille, Sylvain Corlay and Martin Renou       *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/

#ifndef XEUS_MIDDLEWARE_HPP
#define XEUS_MIDDLEWARE_HPP

#include <string>

namespace xeus
{
    std::string get_controller_end_point(const std::string& channel);
    std::string get_publisher_end_point();

    std::string get_end_point(const std::string& transport,
                              const std::string& ip,
                              const std::string& port);

    int get_socket_linger();

    void init_socket(zmq::socket_t& socket,
                     const std::string& transport,
                     const std::string& ip,
                     const std::string& port);
    void init_socket(zmq::socket_t& socket, const std::string& end_point);
    std::string get_socket_port(const zmq::socket_t& socket);
}

#endif
