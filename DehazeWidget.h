#ifndef DEHAZEWIDGET_H
#define DEHAZEWIDGET_H

#include "EffectGroupBox.h"

class QSlider;
class QLabel;
class QVBoxLayout;

class DehazeWidget : public EffectGroupBox
{
    Q_OBJECT
public:
    explicit DehazeWidget(QWidget *parent = nullptr);

signals:
    void dehazeChanged(int value);

private:
    QSlider *m_dehazeSlider;
    QLabel *m_dehazeLabel;

    void setupUI();
};

#endif // DEHAZEWIDGET_H
