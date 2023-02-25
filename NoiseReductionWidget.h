#ifndef NOISEREDUCTIONWIDGET_H
#define NOISEREDUCTIONWIDGET_H

#include "EffectGroupBox.h"

#include <QLabel>
#include <QSlider>

class NoiseReductionWidget : public EffectGroupBox
{
    Q_OBJECT
public:
    explicit NoiseReductionWidget(QWidget *parent = nullptr);
    ~NoiseReductionWidget();

signals:
    void noiseReductionChanged(int value);

private slots:
    void onSliderValueChanged(int value);

private:
    QLabel *m_titleLabel;
    QSlider *m_slider;
};

#endif // NOISEREDUCTIONWIDGET_H
