#ifndef VIGNETTERUNNABLE_H
#define VIGNETTERUNNABLE_H

#include <QRunnable>
#include <QImage>
#include <QColor>
#include <opencv2/core/core.hpp>

class VignetteRunnable : public QRunnable
{
public:
    VignetteRunnable(QImage *image, QColor color, qreal strength);

    void run() override;

private:
    QImage *m_image;
    QColor m_color;
    qreal m_strength;
};

#endif // VIGNETTERUNNABLE_H
