#ifndef WHITEBALANCEWIDGET_H
#define WHITEBALANCEWIDGET_H

#include "EffectGroupBox.h"

#include <QLabel>
#include <QSlider>

class WhiteBalanceWidget : public EffectGroupBox
{
    Q_OBJECT
public:
    explicit WhiteBalanceWidget(QWidget *parent = nullptr);
    ~WhiteBalanceWidget();

signals:
    void whiteBalanceChanged(int redValue, int greenValue, int blueValue);

private slots:
    void onRedSliderValueChanged(int value);
    void onGreenSliderValueChanged(int value);
    void onBlueSliderValueChanged(int value);

private:
    QLabel *m_titleLabel;
    QSlider *m_redSlider;
    QSlider *m_greenSlider;
    QSlider *m_blueSlider;
};

#endif // WHITEBALANCEWIDGET_H
