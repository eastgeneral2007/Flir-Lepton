#include "tcpthread.hpp"

#include "instrumentor.h"

TCPThread::TCPThread(int nsocket) {
  socket = new QTcpSocket();
  if (!socket->setSocketDescriptor(nsocket)) {
    emit Disconnect();
    return;
  }
}

void TCPThread::on_newImg(QByteArray img) {
  PROFILE_FUNCTION();
  if (socket->state() == QTcpSocket::ConnectedState) {
    socket->write(img);
  } else {
    emit Disconnect();
    this->deleteLater();
    this->exit();
  }
}
