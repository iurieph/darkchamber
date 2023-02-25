#include "DefringeWidget.h"

#include <QSlider>
#include <QLabel>
#include <QVBoxLayout>

DefringeWidget::DefringeWidget(QWidget *parent)
        : EffectGroupBox(tr("Defringe"), parent)
{
    setupUI();

    // Set default values
    m_radiusSlider->setValue(0);
    m_amountSlider->setValue(0);
}

void DefringeWidget::setupUI()
{
    m_radiusSlider = new QSlider(Qt::Horizontal, this);
    m_radiusLabel = new QLabel(tr("Radius: %1").arg(m_radiusSlider->value()), this);
    m_amountSlider = new QSlider(Qt::Horizontal, this);
    m_amountLabel = new QLabel(tr("Amount: %1").arg(m_amountSlider->value()), this);

    connect(m_radiusSlider, &QSlider::valueChanged, [this](int value) {
        m_radiusLabel->setText(tr("Radius: %1").arg(value));
        emit radiusChanged(value);
    });

    connect(m_amountSlider, &QSlider::valueChanged, [this](int value) {
        m_amountLabel->setText(tr("Amount: %1").arg(value));
        emit amountChanged(value);
    });

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_radiusLabel);
    layout->addWidget(m_radiusSlider);
    layout->addWidget(m_amountLabel);
    layout->addWidget(m_amountSlider);
}
