/*
  ==============================================================================

   This file is part of the LUBYK project (http://lubyk.org)
   Copyright (c) 2007-2011 by Gaspard Bucher (http://teti.ch).

  ------------------------------------------------------------------------------

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.

  ==============================================================================
*/
#ifndef LUBYK_INCLUDE_RK_SOCKET_H_
#define LUBYK_INCLUDE_RK_SOCKET_H_

#include "lubyk.h"
#include "lubyk/msgpack.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>  // getaddrinfo
//#include <arpa/inet.h> // inet_ntop

#include <errno.h>  // errno
#include <string.h> // strerror

#include <string>

#include "lua_cpp_helper.h"

// How many pending connections should wait for 'accept'.
#define BACKLOG 10

using namespace lubyk;

typedef int LuaStackSize;

namespace lk {

/** Listen for incoming messages on a given port.
 *
 * @dub lib_name:'Socket_core'
 *      string_format:'%%s:%%d --> %%s:%%d'
 *      string_args:'(*userdata)->local_host(), (*userdata)->local_port(), (*userdata)->remote_host(), (*userdata)->remote_port()'
 */
class Socket : public LuaCallback
{
  Thread thread_;
  int socket_fd_;
  int socket_type_;
  std::string local_host_;
  int local_port_;
  std::string remote_host_;
  int remote_port_;
public:

  enum SocketType {
    TCP = SOCK_STREAM,
    UDP = SOCK_DGRAM,
  };

  Socket(lubyk::Worker *worker, int socket_type)
    : LuaCallback(worker),
      socket_fd_(-1),
      socket_type_(socket_type),
      local_host_("*"),
      local_port_(-1),
      remote_host_("?"),
      remote_port_(-1) {
  }

  ~Socket() {
    kill();
    if (socket_fd_ != -1) {
      printf("close(%i)\n", socket_fd_);
      close(socket_fd_);
    }
  }

  /** Bind socket to a specific interface.
   * @return bound port
   */
  int bind(const char *localhost = NULL, int port = 0) {
    char port_str[10];
    snprintf(port_str, 10, "%i", port);

    if (localhost) {
      local_host_ = localhost;
      if (local_host_ == "" || local_host_ == "*") {
        // bind to any interface
        localhost = NULL;
        local_host_ = "*";
      }
    }

    struct addrinfo hints, *res;

    memset(&hints, 0, sizeof(hints));

    // we do not care if we get an IPv4 or IPv6 address
    hints.ai_family = AF_UNSPEC;
    // TCP
    hints.ai_socktype = SOCK_STREAM;
    // fill our own IP
    hints.ai_flags = AI_PASSIVE;

    if (getaddrinfo(NULL, port_str, &hints, &res)) {
      throw Exception("Could not getaddrinfo for %s:%i.", local_host_.c_str(), port);
    }

    if (socket_fd_ != -1) {
      close(socket_fd_);
    }

    // we use getaddrinfo to stay IPv4/IPv6 agnostic
    socket_fd_ = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    printf("socket --> %i\n", socket_fd_);
    if (socket_fd_ == -1) {
      throw Exception("Could not create socket for %s:%i (%s).", local_host_.c_str(), port, strerror(errno));
    }

    // bind to port
    if (::bind(socket_fd_, res->ai_addr, res->ai_addrlen)) {
      throw Exception("Could not bind socket to %s:%i (%s).", local_host_.c_str(), port, strerror(errno));
    }

    if (port == 0) {
      local_port_ = get_port(socket_fd_);
    } else {
      local_port_ = port;
    }

    freeaddrinfo(res);

    return local_port_;
  }

  void connect(const char *host, int port) {
    char port_str[10];
    snprintf(port_str, 10, "%i", port);
    struct addrinfo hints, *res;

    memset(&hints, 0, sizeof(hints));

    // we do not care if we get an IPv4 or IPv6 address
    hints.ai_family = AF_UNSPEC;
    // TCP
    hints.ai_socktype = SOCK_STREAM;

    int status;
    if ( (status = getaddrinfo(host, port_str, &hints, &res)) ) {
      throw Exception("Could not getaddrinfo for %s:%i (%s).", host, port, gai_strerror(status));
    }

    if (socket_fd_ != -1) {
      printf("close(%i)\n", socket_fd_);
      close(socket_fd_);
    }

    // we use getaddrinfo to stay IPv4/IPv6 agnostic
    socket_fd_ = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    printf("socket --> %i\n", socket_fd_);
    if (socket_fd_ == -1) {
      throw Exception("Could not create socket for %s:%i (%s).", host, port, strerror(errno));
    }

    // connect
    if (::connect(socket_fd_, res->ai_addr, res->ai_addrlen)) {
      throw Exception("Could not connect socket to %s:%i (%s).", host, port, strerror(errno));
    }

    // char info[INET6_ADDRSTRLEN];
    // if (res->ai_family == AF_INET) {
    //   inet_ntop(res->ai_family, &(((struct sockaddr_in*)res)->sin_addr), info, sizeof(info));
    // } else {
    //   inet_ntop(res->ai_family, &(((struct sockaddr_in6*)res)->sin6_addr), info, sizeof(info));
    // }
    // printf("Connected to %s:%i (%s)\n", host, port, info);

    freeaddrinfo(res);

    remote_host_ = host;
    remote_port_ = port;
  }

  /** Start listening for incoming connections.
   */
  void listen() {
    if (local_port_ == -1)
      throw Exception("Listen called before bind.");

    if (::listen(socket_fd_, BACKLOG)) {
      throw Exception("Could not listen (%s).", strerror(errno));
    }
  }

  /** Accept a new incomming connection.
   * @return a new lk.Socket connected to the remote end.
   */
  LuaStackSize accept(lua_State *L) {

    if (local_port_ == -1)
      throw Exception("Accept called before bind.");

    struct sockaddr sa;
    memset(&sa, 0, sizeof(struct sockaddr));
    // length has to be in a variable
    socklen_t sa_len = sizeof(sa);

    int fd;

    { ScopedUnlock unlock(worker_);
      fd = ::accept(socket_fd_, &sa, &sa_len);

      printf("accept(%i) --> %i\n", socket_fd_, fd);
    }

    if (fd == -1) {
      throw Exception("Error while accepting connection (%s).", strerror(errno));
    }

    // get remote name / port
    int remote_port;
    if (sa.sa_family == AF_INET) {
      remote_port = ntohs(((struct sockaddr_in *)&sa)->sin_port);
    } else {
      remote_port = ntohs(((struct sockaddr_in6 *)&sa)->sin6_port);
    }

    char remote_host[NI_MAXHOST];

    if (getnameinfo(&sa, sizeof(sa), remote_host, sizeof(remote_host), NULL, 0, 0)) {
      throw Exception("Could not get remote host name (%s).", strerror(errno));
    }

    Socket *new_socket = new Socket(worker_, fd, local_host_.c_str(), remote_host, remote_port);

    lua_pushclass<Socket>(L, new_socket, "lk.Socket");
    return 1;
  }

  /** Receive a message (blocks).
   * For a server, this should typically be used inside the loop.
   * We pass the lua_State to avoid mixing thread contexts.
   */
  LuaStackSize recv(lua_State *L) {
    char buffer[1024]; // fix make this part of the Socket and tune size from data..
    int len;
    // unlock while waiting
    { lubyk::ScopedUnlock unlock(worker_);
      len = ::recv(socket_fd_, &buffer, 1023, 0);
      if (len == 0) {
        return 0; // connection closed
      } else if (len < 0) {
        throw Exception("Could not receive (%s).", strerror(errno));
      }
    }

    int arg_size = msgpack_bin_to_lua(L, buffer, len);
    return arg_size;
  }

  /** Send a message packed with msgpack.
   * Varying parameters.
   */
  void send(lua_State *L) {
    msgpack_sbuffer* buffer;
    msgpack_lua_to_bin(L, &buffer, 1);
    if (::send(socket_fd_, buffer->data, buffer->size, 0)) {
      throw Exception("Could not send message (%s).", strerror(errno));
    }
    free_msgpack_msg(NULL, buffer);
  }

  /** Request = remote call.
   */
  LuaStackSize request(lua_State *L) {
    send(L);
    return recv(L);
  }

  /** Execute a loop in a new thread.
   */
  void loop(lua_State *L) {
    set_lua_callback(L);
    thread_.startThread<Socket, &Socket::run>(this, NULL);
  }

  /** Halt loop.
   */
  void quit() {
    // sets should_run to false but does not interrupt
    thread_.quit();
  }

  /** Interrupt loop.
   */
  void kill() {
    // get out of blocking recv
    thread_.send_signal(SIGINT);
  }

  const char *local_host() const {
    return local_host_.c_str();
  }

  int local_port() const {
    return local_port_;
  }

  const char *remote_host() const {
    return remote_host_.c_str();
  }

  int remote_port() const {
    return remote_port_;
  }

  int port() const {
    return local_port_;
  }

private:
   /** Create a socket with an existing file descriptor.
    * This is used as the result of an 'accept()' call.
    */
   Socket(lubyk::Worker *worker, int fd, const char *local_host, const char *remote_host, int remote_port)
    : LuaCallback(worker),
      socket_fd_(fd),
      local_host_(local_host),
      local_port_(get_port(fd)),
      remote_host_(remote_host),
      remote_port_(remote_port) {

  }

  static int get_port(int fd) {
    // get bound port
    struct sockaddr sa;
    memset(&sa, 0, sizeof(struct sockaddr));
    // length has to be in a variable
    socklen_t sa_len = sizeof(sa);
    if (getsockname(fd, &sa, &sa_len)) {
      throw Exception("Could not get bound port (%s).", strerror(errno));
    }
    if (sa.sa_family == AF_INET) {
      return ntohs(((struct sockaddr_in *)&sa)->sin_port);
    } else {
      return ntohs(((struct sockaddr_in6 *)&sa)->sin6_port);
    }
  }

  void run(Thread *runner) {
    // lua_ = LuaCallback's lua thread context
    runner->thread_ready();

    // trigger callback

    lubyk::ScopedLock lock(worker_);

    push_lua_callback();

    // lua_ = LuaCallback's thread state
    // first argument is self
    int status = lua_pcall(lua_, 1, 0, 0);

    if (status) {
      printf("Error in Socket callback: %s\n", lua_tostring(lua_, -1));
      return;
    }
  }
};
} // lk

#endif // LUBYK_INCLUDE_RK_SOCKET_H_