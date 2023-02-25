#include "OpenCVExposureWidget.h"

OpenCVExposureWidget::OpenCVExposureWidget()
{
}

QImage OpenCVExposureWidget::adjust(QImage image, double exposure, double lightness, double contrast, double saturation)
{
    // Convert QImage to cv::Mat
    cv::Mat mat = QImageToMat(image);

    // Apply exposure compensation
    adjustExposure(mat, exposure);

    // Apply lightness adjustment
    adjustLightness(mat, lightness);

    // Apply contrast adjustment
    adjustContrast(mat, contrast);

    // Apply saturation adjustment
    adjustSaturation(mat, saturation);

    // Convert cv::Mat back to QImage
    QImage result = MatToQImage(mat);

    return result;
}

cv::Mat OpenCVExposureWidget::QImageToMat(const QImage &image)
{
    cv::Mat mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
    cv::cvtColor(mat, mat, cv::COLOR_BGRA2BGR);
    return mat;
}

QImage OpenCVExposureWidget::MatToQImage(const cv::Mat &mat)
{
    QImage result(mat.cols, mat.rows, QImage::Format_RGB888);

    if (mat.channels() == 1) {
        cv::cvtColor(mat, mat, cv::COLOR_GRAY2RGB);
    } else if (mat.channels() == 4) {
        cv::cvtColor(mat, mat, cv::COLOR_RGBA2RGB);
    }

    uchar *data = result.bits();
    int bytesPerLine = result.bytesPerLine();

    for (int row = 0; row < mat.rows; ++row) {
        const uchar *src = mat.ptr(row);
        uchar *dst = data + row * bytesPerLine;
        for (int col = 0; col < mat.cols; ++col) {
            dst[0] = src[0];
            dst[1] = src[1];
            dst[2] = src[2];
            src += 3;
            dst += 3;
        }
    }

    return result;
}

void OpenCVExposureWidget::adjustExposure(cv::Mat &image, double value)
{
    if (value == 0) {
        return;
    }

    if (value > 0) {
        value = 1.0 / (1.0
