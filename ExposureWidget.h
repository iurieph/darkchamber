/**
 * File name: ExposureWidget.h
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

#ifndef EXPOSURE_WIDGET_H
#define EXPOSURE_WIDGET_H

#include "EffectGroupBox.h"

class QSlider;
class QLabel;
class QComboBox;

class ExposureWidget : public EffectGroupBox
{
    Q_OBJECT

public:
    explicit ExposureWidget(QWidget *parent = nullptr);
    std::unique_ptr<ImageEffect> getEffect() const;

signals:
    void exposureChanged(int value);
    void lightnessChanged(int value);
    void contrastChanged(int value);
    void saturationChanged(int value);

private:
    QSlider *m_exposureSlider;
    QLabel *m_exposureLabel;
    QSlider *m_lightnessSlider;
    QLabel *m_lightnessLabel;
    QSlider *m_contrastSlider;
    QLabel *m_contrastLabel;
    QSlider *m_saturationSlider;
    QLabel *m_saturationLabel;
    QComboBox *m_colorModelComboBox;

    void createLayout();
};

#endif // EXPOSURE_WIDGET_H
