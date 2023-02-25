#ifndef OPENCVEXPOSUREWIDGET_H
#define OPENCVEXPOSUREWIDGET_H

#include <QImage>
#include <opencv2/opencv.hpp>

class OpenCVExposureWidget
{
public:
    explicit OpenCVExposureWidget();

    QImage adjust(QImage image, double exposure, double lightness, double contrast, double saturation);

private:
    cv::Mat QImageToMat(const QImage &image);
    QImage MatToQImage(const cv::Mat &mat);
};

#endif // OPENCVEXPOSUREWIDGET_H
