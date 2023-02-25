#ifndef SHADOWSANDHIGHLIGHTSWIDGET_H
#define SHADOWSANDHIGHLIGHTSWIDGET_H

#include "EffectGroupBox.h"

class QSlider;
class QLabel;
class QVBoxLayout;
class QHBoxLayout;

class ShadowsAndHighlightsWidget : public EffectGroupBox
{
    Q_OBJECT
public:
    explicit ShadowsAndHighlightsWidget(QWidget *parent = nullptr);

signals:
    void shadowsChanged(int value);
    void highlightsChanged(int value);

private:
    QSlider *m_shadowsSlider;
    QLabel *m_shadowsLabel;
    QSlider *m_highlightsSlider;
    QLabel *m_highlightsLabel;

    void setupUI();
};

#endif // SHADOWSANDHIGHLIGHTSWIDGET_H
