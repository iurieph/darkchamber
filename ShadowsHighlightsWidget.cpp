#include "ShadowsHighlightsWidget.h"

#include <QSlider>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>

ShadowsAndHighlightsWidget::ShadowsAndHighlightsWidget(QWidget *parent)
        : EffectGroupBox(tr("Shadows and Highlights"), parent)
{
    setupUI();

    // Set default values
    m_shadowsSlider->setValue(0);
    m_highlightsSlider->setValue(0);
}

void ShadowsAndHighlightsWidget::setupUI()
{
    m_shadowsSlider = new QSlider(Qt::Horizontal, this);
    m_shadowsLabel = new QLabel(tr("Shadows: %1").arg(m_shadowsSlider->value()), this);
    m_highlightsSlider = new QSlider(Qt::Horizontal, this);
    m_highlightsLabel = new QLabel(tr("Highlights: %1").arg(m_highlightsSlider->value()), this);

    connect(m_shadowsSlider, &QSlider::valueChanged, [this](int value) {
        m_shadowsLabel->setText(tr("Shadows: %1").arg(value));
        emit shadowsChanged(value);
    });

    connect(m_highlightsSlider, &QSlider::valueChanged, [this](int value) {
        m_highlightsLabel->setText(tr("Highlights: %1").arg(value));
        emit highlightsChanged(value);
    });

    QHBoxLayout *shadowsLayout = new QHBoxLayout();
    shadowsLayout->addWidget(m_shadowsLabel);
    shadowsLayout->addWidget(m_shadowsSlider);
    shadowsLayout->addStretch();

    QHBoxLayout *highlightsLayout = new QHBoxLayout();
    highlightsLayout->addWidget(m_highlightsLabel);
    highlightsLayout->addWidget(m_highlightsSlider);
    highlightsLayout->addStretch();

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addLayout(shadowsLayout);
    layout->addLayout(highlightsLayout);
}
