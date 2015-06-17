// Copyright (c) 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef StreamSocketImpl_h
#define StreamSocketImpl_h

#include "net/socket/stream_socket.h"

namespace net {

class StreamSocketImpl : public StreamSocket {
 public:
  StreamSocketImpl();
  virtual ~StreamSocketImpl();

  // Called to establish a connection.  Returns OK if the connection could be
  // established synchronously.  Otherwise, ERR_IO_PENDING is returned and the
  // given callback will run asynchronously when the connection is established
  // or when an error occurs.  The result is some other error code if the
  // connection could not be established.
  //
  // The socket's Read and Write methods may not be called until Connect
  // succeeds.
  //
  // It is valid to call Connect on an already connected socket, in which case
  // OK is simply returned.
  //
  // Connect may also be called again after a call to the Disconnect method.
  //
  virtual int Connect(const CompletionCallback& callback) override {
    fprintf(stderr, "SteamSocketImpl\n");
    return -1;
  }

  // Called to disconnect a socket.  Does nothing if the socket is already
  // disconnected.  After calling Disconnect it is possible to call Connect
  // again to establish a new connection.
  //
  // If IO (Connect, Read, or Write) is pending when the socket is
  // disconnected, the pending IO is cancelled, and the completion callback
  // will not be called.
  virtual void Disconnect() override {
    fprintf(stderr, "SteamSocketImpl\n");
  }

  // Called to test if the connection is still alive.  Returns false if a
  // connection wasn't established or the connection is dead.
  virtual bool IsConnected() const override {
    fprintf(stderr, "SteamSocketImpl\n");
    return false;
  }

  // Called to test if the connection is still alive and idle.  Returns false
  // if a connection wasn't established, the connection is dead, or some data
  // have been received.
  virtual bool IsConnectedAndIdle() const override {
    fprintf(stderr, "SteamSocketImpl\n");
    return false;
  }

  // Copies the peer address to |address| and returns a network error code.
  // ERR_SOCKET_NOT_CONNECTED will be returned if the socket is not connected.
  virtual int GetPeerAddress(IPEndPoint* address) const override {
    fprintf(stderr, "SteamSocketImpl\n");
    return -1;
  }

  // Copies the local address to |address| and returns a network error code.
  // ERR_SOCKET_NOT_CONNECTED will be returned if the socket is not bound.
  virtual int GetLocalAddress(IPEndPoint* address) const override {
    fprintf(stderr, "SteamSocketImpl\n");
    return -1;
  }

  // Gets the NetLog for this socket.
  virtual const BoundNetLog& NetLog() const override {
    fprintf(stderr, "SteamSocketImpl\n");
    void* x = nullptr;
    return *reinterpret_cast<BoundNetLog*>(x);
  }

  // Set the annotation to indicate this socket was created for speculative
  // reasons.  This call is generally forwarded to a basic TCPClientSocket*,
  // where a UseHistory can be updated.
  virtual void SetSubresourceSpeculation() override {
    fprintf(stderr, "SteamSocketImpl\n");
  }

  virtual void SetOmniboxSpeculation() override {
    fprintf(stderr, "SteamSocketImpl\n");
  }

  // Returns true if the socket ever had any reads or writes.  StreamSockets
  // layered on top of transport sockets should return if their own Read() or
  // Write() methods had been called, not the underlying transport's.
  virtual bool WasEverUsed() const override {
    fprintf(stderr, "SteamSocketImpl\n");
    return false;
  }

  // TODO(jri): Clean up -- remove this method.
  // Returns true if the underlying transport socket is using TCP FastOpen.
  // TCP FastOpen is an experiment with sending data in the TCP SYN packet.
  virtual bool UsingTCPFastOpen() const override {
    fprintf(stderr, "SteamSocketImpl\n");
    return false;
  }

  // TODO(jri): Clean up -- rename to a more general EnableAutoConnectOnWrite.
  // Enables use of TCP FastOpen for the underlying transport socket.
  virtual void EnableTCPFastOpenIfSupported() {}

  // Returns true if NPN was negotiated during the connection of this socket.
  virtual bool WasNpnNegotiated() const override {
    fprintf(stderr, "SteamSocketImpl\n");
    return false;
  }

  // Returns the protocol negotiated via NPN for this socket, or
  // kProtoUnknown will be returned if NPN is not applicable.
  virtual NextProto GetNegotiatedProtocol() const override {
    fprintf(stderr, "SteamSocketImpl\n");
    return kProtoUnknown;
}

  // Gets the SSL connection information of the socket.  Returns false if
  // SSL was not used by this socket.
  virtual bool GetSSLInfo(SSLInfo* ssl_info) override {
    fprintf(stderr, "SteamSocketImpl\n");
    return false;
  }

  // Overwrites |out| with the connection attempts made in the process of
  // connecting this socket.
  virtual void GetConnectionAttempts(ConnectionAttempts* out) const override {
    fprintf(stderr, "SteamSocketImpl\n");
  }

  // Clears the socket's list of connection attempts.
  virtual void ClearConnectionAttempts() override {
    fprintf(stderr, "SteamSocketImpl\n");
  }

  // Adds |attempts| to the socket's list of connection attempts.
  virtual void AddConnectionAttempts(const ConnectionAttempts& attempts) override {
    fprintf(stderr, "SteamSocketImpl\n");
  }





  virtual int Read(IOBuffer* buf, int buf_len,
                   const CompletionCallback& callback) override {
    fprintf(stderr, "StreamSocketImpl\n");
    return -1;
  }

  // Writes data, up to |buf_len| bytes, to the socket.  Note: data may be
  // written partially.  The number of bytes written is returned, or an error
  // is returned upon failure.  ERR_SOCKET_NOT_CONNECTED should be returned if
  // the socket is not currently connected.  The return value when the
  // connection is closed is undefined, and may be OS dependent.  ERR_IO_PENDING
  // is returned if the operation could not be completed synchronously, in which
  // case the result will be passed to the callback when available.  If the
  // operation is not completed immediately, the socket acquires a reference to
  // the provided buffer until the callback is invoked or the socket is
  // closed.  Implementations of this method should not modify the contents
  // of the actual buffer that is written to the socket.  If the socket is
  // Disconnected before the write completes, the callback will not be invoked.
  virtual int Write(IOBuffer* buf, int buf_len,
                    const CompletionCallback& callback) override {
    fprintf(stderr, "StreamSocketImpl\n");
    return -1;
  }

  // Set the receive buffer size (in bytes) for the socket.
  // Note: changing this value can affect the TCP window size on some platforms.
  // Returns a net error code.
  virtual int SetReceiveBufferSize(int32 size) override {
    fprintf(stderr, "StreamSocketImpl\n");
    return -1;
  }

  // Set the send buffer size (in bytes) for the socket.
  // Note: changing this value can affect the TCP window size on some platforms.
  // Returns a net error code.
  virtual int SetSendBufferSize(int32 size) override {
    fprintf(stderr, "StreamSocketImpl\n");
    return -1;
  }


 private:
  DISALLOW_COPY_AND_ASSIGN(StreamSocketImpl);
};

}  // namespace net

#endif  // StreamSocketImpl_h
