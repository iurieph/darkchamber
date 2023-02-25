#include "DehazeWidget.h"

#include <QSlider>
#include <QLabel>
#include <QVBoxLayout>

DehazeWidget::DehazeWidget(QWidget *parent)
        : EffectGroupBox(tr("Haze Removal"))
{
    setupUI();

    // Set default values
    m_dehazeSlider->setValue(0);
}

void DehazeWidget::setupUI()
{
    m_dehazeSlider = new QSlider(Qt::Horizontal, this);
    m_dehazeLabel = new QLabel(tr("Dehaze: %1").arg(m_dehazeSlider->value()), this);

    connect(m_dehazeSlider, &QSlider::valueChanged, [this](int value) {
        m_dehazeLabel->setText(tr("Dehaze: %1").arg(value));
        emit dehazeChanged(value);
    });

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_dehazeLabel);
    layout->addWidget(m_dehazeSlider);
}
