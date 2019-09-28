#include "imagecomposerthread.hpp"
#include "common.hpp"
#include "log/logger.h"

QMutex mutex;

ImageComposerThread::ImageComposerThread() : QThread() {
  m_result = new QImage(640, 480, QImage::Format_ARGB32_Premultiplied);
  m_rgb = new QImage();
  m_thermal = new QImage();
  m_mode = QPainter::CompositionMode_Overlay;
#if LOGGER
  LOG(INFO, "ctor ImageComposerThread class")
#endif
}

ImageComposerThread::~ImageComposerThread() {
  if (m_rgb) {
    delete m_rgb;
  }
  if (m_result) {
    delete m_result;
  }
  if (m_thermal) {
    delete m_thermal;
  }
#if LOGGER
  LOG(INFO, "dtor ImageComposerThread class")
#endif
}

void ImageComposerThread::run() {
  usleep(200000);
  while (true) {
    usleep(200);
    recalculateResult();
  }
}

void ImageComposerThread::recalculateResult() {
  // m_rgb->scaled(640, 480, Qt::KeepAspectRatio);
  m_thermal->scaled(640, 480, Qt::KeepAspectRatio);
  m_thermal->
  QPainter painter(m_result);
  painter.setCompositionMode(QPainter::CompositionMode_Source);
  painter.fillRect(m_result->rect(), Qt::transparent);
  painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
  painter.drawImage(0, 0, *m_thermal);
  painter.setCompositionMode(m_mode);
  painter.drawImage(0, 0, *m_rgb);
  painter.setCompositionMode(QPainter::CompositionMode_DestinationOver);
  painter.fillRect(m_result->rect(), Qt::white);
  painter.end();
  // update signal
  emit updateImage(*m_result);
}

void ImageComposerThread::setMode(int index) {
  m_mode = static_cast<QPainter::CompositionMode>(index);
  this->start();
}

void ImageComposerThread::setRGBImage(QImage img) {
  *m_rgb = img;
  *m_rgb = img.scaled(640, 480, Qt::KeepAspectRatio);
#if LOGGER
  LOG(INFO, "update rgb image")
#endif
}

void ImageComposerThread::setThermalImage(QImage img) {
  *m_thermal = img;
  *m_thermal = img.scaled(640, 480, Qt::KeepAspectRatio);
#if LOGGER
  LOG(INFO, "update thermal image")
#endif
}
