//
// connection.hpp
// ~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2018 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef DAPP_CONNECTION_HPP
#define DAPP_CONNECTION_HPP

#include <array>
#include <memory>
#include <boost/asio.hpp>
#include "TcpSplitPkt.h"
#include "reply.hpp"
#include "request_handler.hpp"

namespace dapp {
namespace server {

class connection_manager;

/// Represents a single connection from a client.
class connection
  : public std::enable_shared_from_this<connection>
{
public:
  connection(const connection&) = delete;
  connection& operator=(const connection&) = delete;

  /// Construct a connection with the given socket.
  explicit connection(boost::asio::ip::tcp::socket socket, connection_manager& manager);

  /// Start the first asynchronous operation for the connection.
  void  start();

  /// Stop all asynchronous operations associated with the connection.
  void  stop();
  int   request_handle( unsigned char * pData,int pktlen);

private:
  /// Perform an asynchronous read operation.
  void do_read();

  /// Perform an asynchronous write operation.
  void do_write();

  /// Socket for the connection.
  boost::asio::ip::tcp::socket socket_;

  /// The manager for this connection.
  connection_manager& connection_manager_;
   
  request_handler  request_handler_;

  /// Buffer for incoming data.
  std::array<char, 8192> buffer_;
  reply  reply_;  
  CTcpSplitPkt  splitPkt_;

};

typedef std::shared_ptr<connection> connection_ptr;

} // namespace server
} // namespace http

#endif // DAPP_CONNECTION_HPP
