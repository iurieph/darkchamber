/**
 * File name: ExposureWidget.cpp
 * Project: DarkChamber (A photo management software)
 *
 * Copyright (C) 2023 Iurie Nistor
 *
 * This file is part of DarkChamber.
 *
 * GeonKick is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include "ExposureWidget.h"

#include <QSlider>
#include <QLabel>
#include <QVBoxLayout>
#include <QComboBox>

ExposureWidget::ExposureWidget(QWidget *parent)
        : EffectGroupBox(tr("Exposure"), parent)
{
    createLayout();
    m_exposureSlider->setValue(0);
    m_lightnessSlider->setValue(0);
    m_contrastSlider->setValue(0);
    m_saturationSlider->setValue(0);
}

void ExposureWidget::createLayout()
{
        m_exposureSlider = new QSlider(Qt::Horizontal, this);
        m_exposureSlider->setRange(-100, 100);

        m_exposureLabel = new QLabel(tr("Exposure: 0"), this);
        m_exposureLabel->setAlignment(Qt::AlignCenter);

        m_lightnessSlider = new QSlider(Qt::Horizontal, this);
        m_lightnessSlider->setRange(-100, 100);

        m_lightnessLabel = new QLabel(tr("Lightness: 0"), this);
        m_lightnessLabel->setAlignment(Qt::AlignCenter);

        m_contrastSlider = new QSlider(Qt::Horizontal, this);
        m_contrastSlider->setRange(-100, 100);

        m_contrastLabel = new QLabel(tr("Contrast: 0"), this);
        m_contrastLabel->setAlignment(Qt::AlignCenter);

        m_saturationSlider = new QSlider(Qt::Horizontal, this);
        m_saturationSlider->setRange(-100, 100);

        m_saturationLabel = new QLabel(tr("Saturation: 0"), this);
        m_saturationLabel->setAlignment(Qt::AlignCenter);

        m_colorModelComboBox = new QComboBox(this);
        m_colorModelComboBox->addItems({"RGB", "HSV", "HSL"});
        m_colorModelComboBox->setToolTip(tr("Color model"));

        connect(m_exposureSlider, &QSlider::valueChanged, [this](int value) {
                m_exposureLabel->setText(tr("Exposure: %1").arg(value));
                emit exposureChanged(value);
        });

        connect(m_lightnessSlider, &QSlider::valueChanged, [this](int value) {
                m_lightnessLabel->setText(tr("Lightness: %1").arg(value));
                emit lightnessChanged(value);
        });

        connect(m_contrastSlider, &QSlider::valueChanged, [this](int value) {
                m_contrastLabel->setText(tr("Contrast: %1").arg(value));
                emit contrastChanged(value);
        });

        connect(m_saturationSlider, &QSlider::valueChanged, [this](int value) {
                m_saturationLabel->setText(tr("Saturation: %1").arg(value));
                //                emit effectUpdated(std::move(ExposureWidget::getEffect()));
        });

        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->addWidget(m_exposureLabel);
        layout->addWidget(m_exposureSlider);
        layout->addWidget(m_lightnessLabel);
        layout->addWidget(m_lightnessSlider);
        layout->addWidget(m_contrastLabel);
        layout->addWidget(m_contrastSlider);
        layout->addWidget(m_saturationLabel);
        layout->addWidget(m_saturationSlider);
        layout->addWidget(m_colorModelComboBox); 
}

/*std::unique_ptr<ImageEffect> ExposureWidget::getEffect() const
{
        return std::make_unique<ExposureEffect>(m_exposureSlider->value(),
                                                m_lightnessSlider->value(),
                                                m_contrastSlider->value(),
                                                m_saturationSlider->value());
                                                }*/
