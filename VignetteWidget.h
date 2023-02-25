#ifndef VIGNETTEWIDGET_H
#define VIGNETTEWIDGET_H

#include "EffectGroupBox.h"

class QSlider;
class QLabel;
class QVBoxLayout;
class QHBoxLayout;
class QComboBox;

class VignetteWidget : public EffectGroupBox
{
    Q_OBJECT
public:
    explicit VignetteWidget(QWidget *parent = nullptr);

signals:
    void vignetteChanged(int value);
    void shapeChanged(const QString &shape);

private:
    QSlider *m_vignetteSlider;
    QLabel *m_vignetteLabel;
    QLabel *m_shapeLabel;
    QComboBox *m_shapeComboBox;

    void setupUI();
};

#endif // VIGNETTEWIDGET_H
