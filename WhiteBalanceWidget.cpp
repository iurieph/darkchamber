#include "WhiteBalanceWidget.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QHBoxLayout>

WhiteBalanceWidget::WhiteBalanceWidget(QWidget *parent)
        : EffectGroupBox(tr("White balance"), parent)
{
    m_titleLabel = new QLabel("White Balance", this);
    m_redSlider = new QSlider(Qt::Horizontal, this);
    m_greenSlider = new QSlider(Qt::Horizontal, this);
    m_blueSlider = new QSlider(Qt::Horizontal, this);

    connect(m_redSlider, &QSlider::valueChanged, this, &WhiteBalanceWidget::onRedSliderValueChanged);
    connect(m_greenSlider, &QSlider::valueChanged, this, &WhiteBalanceWidget::onGreenSliderValueChanged);
    connect(m_blueSlider, &QSlider::valueChanged, this, &WhiteBalanceWidget::onBlueSliderValueChanged);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(m_titleLabel);

    QHBoxLayout *redLayout = new QHBoxLayout;
    QLabel *redLabel = new QLabel("Red", this);
    redLayout->addWidget(redLabel);
    redLayout->addWidget(m_redSlider);
    layout->addLayout(redLayout);

    QHBoxLayout *greenLayout = new QHBoxLayout;
    QLabel *greenLabel = new QLabel("Green", this);
    greenLayout->addWidget(greenLabel);
    greenLayout->addWidget(m_greenSlider);
    layout->addLayout(greenLayout);

    QHBoxLayout *blueLayout = new QHBoxLayout;
    QLabel *blueLabel = new QLabel("Blue", this);
    blueLayout->addWidget(blueLabel);
    blueLayout->addWidget(m_blueSlider);
    layout->addLayout(blueLayout);

    setLayout(layout);
}

WhiteBalanceWidget::~WhiteBalanceWidget()
{
    delete m_titleLabel;
    delete m_redSlider;
    delete m_greenSlider;
    delete m_blueSlider;
}

void WhiteBalanceWidget::onRedSliderValueChanged(int value)
{
    emit whiteBalanceChanged(value, m_greenSlider->value(), m_blueSlider->value());
}

void WhiteBalanceWidget::onGreenSliderValueChanged(int value)
{
    emit whiteBalanceChanged(m_redSlider->value(), value, m_blueSlider->value());
}

void WhiteBalanceWidget::onBlueSliderValueChanged(int value)
{
    emit whiteBalanceChanged(m_redSlider->value(), m_greenSlider->value(), value);
}
