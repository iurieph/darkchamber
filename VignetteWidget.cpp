#include "VignetteWidget.h"

#include <QSlider>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>

VignetteWidget::VignetteWidget(QWidget *parent) : EffectGroupBox(tr("Vignette"), parent)
{
    setupUI();

    // Set default values
    m_vignetteSlider->setValue(0);
    m_shapeComboBox->setCurrentIndex(0);
}

void VignetteWidget::setupUI()
{
    m_vignetteSlider = new QSlider(Qt::Horizontal, this);
    m_vignetteLabel = new QLabel(tr("Vignette: %1").arg(m_vignetteSlider->value()), this);
    m_shapeLabel = new QLabel(tr("Shape:"), this);
    m_shapeComboBox = new QComboBox(this);
    m_shapeComboBox->addItems({"Circular", "Rectangular"});
    m_shapeComboBox->setToolTip(tr("Vignette shape"));

    connect(m_vignetteSlider, &QSlider::valueChanged, [this](int value) {
        m_vignetteLabel->setText(tr("Vignette: %1").arg(value));
        emit vignetteChanged(value);
    });

    //    connect(m_shapeComboBox, QOverload<const QString &>::of(&QComboBox::currentIndexChanged), [this](const QString &shape) {
    //     emit shapeChanged(shape);
    // });

    QHBoxLayout *shapeLayout = new QHBoxLayout();
    shapeLayout->addWidget(m_shapeLabel);
    shapeLayout->addWidget(m_shapeComboBox);
    shapeLayout->addStretch();

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_vignetteLabel);
    layout->addWidget(m_vignetteSlider);
    layout->addLayout(shapeLayout);
}
