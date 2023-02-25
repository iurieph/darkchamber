/**
 * File name: EffectsWidget.h
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

#include "EffectsWidget.h"
#include "ExposureWidget.h"
#include "DehazeWidget.h"
#include "ShadowsHighlightsWidget.h"
#include "VignetteWidget.h"
#include "NoiseReductionWidget.h"
#include "DefringeWidget.h"
#include "WhiteBalanceWidget.h"

#include <QVBoxLayout>
#include <QWidgetList>
#include <QLabel>
#include <QScrollArea>

EffectsWidget::EffectsWidget(QWidget *parent)
        : QDockWidget(tr("Effects"), parent)
        , m_exposureWidget{new ExposureWidget}
        , m_dehazeWidget{new DehazeWidget(this)}
        , m_shadowsAndHighlightsWidget{new ShadowsAndHighlightsWidget(this)}
        , m_vignetteWidget{new VignetteWidget(this)}
        , m_noiseReductionWidget{new NoiseReductionWidget(this)}
        , m_defringeWidget{new DefringeWidget(this)}
        , m_whiteBalanceWidget{new WhiteBalanceWidget(this)}
{
              auto scrollArea = new QScrollArea(this);
        scrollArea->setWidgetResizable(true);
        auto containerWidget = new QWidget(scrollArea);
        
        auto containerWidgetLayout = new QVBoxLayout;
        containerWidgetLayout->addWidget(m_exposureWidget);
        //        containerWidgetLayout->addWidget(createSeparator());
        containerWidgetLayout->addWidget(m_dehazeWidget);
        //        containerWidgetLayout->addWidget(createSeparator());
        containerWidgetLayout->addWidget(m_shadowsAndHighlightsWidget);
        //        containerWidgetLayout->addWidget(createSeparator());
        containerWidgetLayout->addWidget(m_vignetteWidget);
        //        containerWidgetLayout->addWidget(createSeparator());
        containerWidgetLayout->addWidget(m_noiseReductionWidget);
        //        containerWidgetLayout->addWidget(createSeparator());
        containerWidgetLayout->addWidget(m_defringeWidget);
        //        containerWidgetLayout->addWidget(createSeparator());
        containerWidgetLayout->addWidget(m_whiteBalanceWidget);
        containerWidget->setLayout(containerWidgetLayout);        
        scrollArea->setWidget(containerWidget);
        setWidget(scrollArea);
}

QWidget* EffectsWidget::createSeparator()
{
    auto separator = new QFrame;
    separator->setFrameShape(QFrame::HLine);
    separator->setFrameShadow(QFrame::Sunken);
    separator->setLineWidth(1);
    separator->setMidLineWidth(0);
    return separator;
}

