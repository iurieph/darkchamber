#include "NoiseReductionWidget.h"

#include <QVBoxLayout>

NoiseReductionWidget::NoiseReductionWidget(QWidget *parent)
        : EffectGroupBox(tr("Noise reduction"), parent)
{
    m_titleLabel = new QLabel("Noise Reduction", this);
    m_slider = new QSlider(Qt::Horizontal, this);

    connect(m_slider, &QSlider::valueChanged, this, &NoiseReductionWidget::onSliderValueChanged);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(m_titleLabel);
    layout->addWidget(m_slider);
    setLayout(layout);
}

NoiseReductionWidget::~NoiseReductionWidget()
{
    delete m_titleLabel;
    delete m_slider;
}

void NoiseReductionWidget::onSliderValueChanged(int value)
{
    emit noiseReductionChanged(value);
}
