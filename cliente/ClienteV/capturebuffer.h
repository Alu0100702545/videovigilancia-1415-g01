#ifndef CAPTUREBUFFER_H
#define CAPTUREBUFFER_H

#include <QAbstractVideoSurface>
#include <QDebug>

class CaptureBuffer : public QAbstractVideoSurface
{
    Q_OBJECT

public:
    int id=0;

    QList<QVideoFrame::PixelFormat> supportedPixelFormats(
            QAbstractVideoBuffer::HandleType handleType =
            QAbstractVideoBuffer::NoHandle) const
    {
        // A través de este método nos preguntan que formatos de
        // vídeo soportamos. Como vamos a guardar los frames en
        // objetos QImage nos sirve cualquiera de los formatos
        // soportados por dicha clase: http://kcy.me/z6pa
        Q_UNUSED(handleType);
        QList<QVideoFrame::PixelFormat> formats;
        formats << QVideoFrame::Format_ARGB32;
        formats << QVideoFrame::Format_ARGB32_Premultiplied;
        formats << QVideoFrame::Format_RGB32;
        formats << QVideoFrame::Format_RGB24;
        formats << QVideoFrame::Format_RGB565;
        formats << QVideoFrame::Format_RGB555;
        return formats;
    }

    bool present(const QVideoFrame &frame)
    {
        //qDebug() << "Capture buffer";
        QVideoFrame frame2(frame);

        frame2.map(QAbstractVideoBuffer::ReadOnly);

        QImage frameAsImage = QImage(frame2.bits(), frame2.width(),frame2.height(), frame2.bytesPerLine(),
                                       QVideoFrame::imageFormatFromPixelFormat(frame2.pixelFormat()));

        frameAsImage.copy();
        emit s_image(frameAsImage,id);
        frame2.unmap();
        //qDebug() << "Mostramos desde capture";
        return true;
    }

signals:
  void s_image(const QImage&, const int&);
public slots:

};

#endif // CAPTUREBUFFER_H
