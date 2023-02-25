#ifndef DEFRINGEWIDGET_H
#define DEFRINGEWIDGET_H

#include "EffectGroupBox.h"

class QSlider;
class QLabel;
class QVBoxLayout;

class DefringeWidget : public EffectGroupBox
{
    Q_OBJECT
public:
    explicit DefringeWidget(QWidget *parent = nullptr);

signals:
    void radiusChanged(int value);
    void amountChanged(int value);

private:
    QSlider *m_radiusSlider;
    QLabel *m_radiusLabel;
    QSlider *m_amountSlider;
    QLabel *m_amountLabel;

    void setupUI();
};

#endif // DEFRINGEWIDGET_H
