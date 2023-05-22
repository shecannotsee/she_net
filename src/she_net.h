//
// Created by shecannotsee on 23-5-18.
//

#ifndef SHE_NET_SRC_SHE_NET_H_
#define SHE_NET_SRC_SHE_NET_H_

#include <make_unique.h>
#include <sheNetException/sheNetException.h>

#include <transport_address_type.h>
#include <four_tuple.h>
#include <basic_io_operations/basic_io_operations.h>
#include <basic_socket_operations/basic_socket_operations.h>

#include <select_wrapper/select_wrapper.h>
#include <poll_wrapper/poll_wrapper.h>
#include <epoll_wrapper/epoll_wrapper.h>

#include <basic_tcp/tcp_client.h>
#include <basic_tcp/tcp_server.h>
#include <basic_udp/udp_client.h>
#include <basic_udp/udp_server.h>

#include <stream_data_parsing/base64.h>
#include <stream_data_parsing/json.h>
#include <stream_data_parsing/xml.h>
#include <stream_data_parsing/length_prefixed_messages.h>
#include <stream_data_parsing/fixed_length_messages.h>
// will drop
#include <high_coupling_model/socket.h>
#include <high_coupling_model/message.h>
#include <stream_data_parsing/packageInStream.h>
#include <stream_data_parsing/packageInStreamBase64.h>

#endif //SHE_NET_SRC_SHE_NET_H_
