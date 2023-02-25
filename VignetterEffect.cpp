#include "VignetteRunnable.h"
#include <opencv2/imgproc.hpp>

VignetteRunnable::VignetteRunnable(QImage *image, QColor color, qreal strength)
    : m_image(image), m_color(color), m_strength(strength)
{

}

void VignetteRunnable::run()
{
    cv::Mat srcMat(m_image->height(), m_image->width(), CV_8UC4, m_image->bits(), m_image->bytesPerLine());
    cv::Mat dstMat;

    cv::Scalar color(m_color.blue(), m_color.green(), m_color.red());
    cv::Point2f center(m_image->width() / 2.0f, m_image->height() / 2.0f);
    float maxDistance = qSqrt(qPow(m_image->width(), 2) + qPow(m_image->height(), 2)) / 2.0f;
    float minDistance = maxDistance * (1.0f - m_strength);

    cv::Mat mask(m_image->height(), m_image->width(), CV_8U);
    cv::Point2f roiCenter(center.x, center.y);
    cv::Size2f roiSize(m_image->width(), m_image->height());
    cv::ellipse(mask, roiCenter, roiSize, 0, 0, 360, cv::Scalar(255), -1);

    cv::Mat distance(m_image->height(), m_image->width(), CV_32F);
    cv::Point2f p;
    for (int r = 0; r < m_image->height(); r++) {
        for (int c = 0; c < m_image->width(); c++) {
            p = cv::Point2f(c, r);
            distance.at<float>(r, c) = cv::norm(p - center);
        }
    }

    cv::normalize(distance, distance, 0.0f, 1.0f, cv::NORM_MINMAX);
    distance = 1 - distance;
    distance = cv::max(distance - minDistance / maxDistance, 0.0f) / (1.0f - minDistance / maxDistance);
    cv::cvtColor(distance, distance, cv::COLOR_GRAY2BGR);

    cv::multiply(srcMat, distance, dstMat, 1.0 / 255.0);
    cv::addWeighted(dstMat, 1.0, cv::Mat(m_image->height(), m_image->width(), CV_8UC4, color), m_strength, 0.0, dstMat);

    cv::cvtColor(dstMat, dstMat, cv::COLOR_BGR2RGBA);
    QImage outImage(dstMat.data, dstMat.cols, dstMat.rows, dstMat.step, QImage::Format_RGBA8888);

    *m_image = outImage.copy();
}
