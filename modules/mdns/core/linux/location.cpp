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

#include "dub/dub.h"
#include "mdns/Location.h"

#include <sstream>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

namespace mdns {

unsigned long Location::ip_from_hostname(const char *hostname) {
  struct addrinfo *result, *result0;
  unsigned long ip;
  int error;

  error = getaddrinfo(hostname, NULL, NULL, &result0);
  if (error) {
    throw dub::Exception("Could not resolve '%s' (%s).", hostname, gai_strerror(error));
    return Location::NO_IP;
  }

  for (result = result0; result; result = result->ai_next) {
    if (result->ai_family == AF_INET) {
      // only support AF_INET for now (no IPv6)

      // sockaddr is a generic raw bunch of bytes, we must cast it first to
      // an IPv4 struct:
      struct sockaddr_in *ipv4 = (sockaddr_in*) result->ai_addr;

      // ip is in sin_addr
      // we must convert the 32-bit IP from network to host
      ip = ntohl(ipv4->sin_addr.s_addr);
      break;
    }
  }

  freeaddrinfo(result0);
  return ip;
}

} // mdns
