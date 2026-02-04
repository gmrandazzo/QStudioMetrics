/*
 * This project uses Qt under the GNU General Public License version 3.0 (GPL‑3.0).
 *
 * Header file for FindCorrelationWidget.
 *
 * Copyright (C) 2016-2026 designed, written and mantained by Giuseppe Marco Randazzo <gmrandazzo@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef FindCorrelationWidget_H
#define FindCorrelationWidget_H

#include <QStandardItemModel>
#include <QString>
#include <QStringList>
#include <QWidget>

#include "qsmdata.h"
#include "qstudiometricstypes.h"

#include <ui_FindCorrelationWidget.h>

class FindCorrelationWidget : public QWidget {
  Q_OBJECT

public:
  FindCorrelationWidget(matrix *cmx_, matrix *derr_, QStringList *varname_,
                        LABELS *varlabels_, int pid_, int mid_, int mtype_);
  ~FindCorrelationWidget();

private slots:
  void Find();
  void SelectAll();
  void UnselectAll();
  void InvertSelection();
  void PlotVarVSVar();
  void Higlight();

signals:
  void VVPlotSignal(vvplotSignal vvs);
  void HighlightSignal(highlightSignal hhs);
  void CloseFindCorrelationWidgetSignal();

private:
  Ui::FindCorrelationWidget ui;
  QStandardItemModel *model;
  QStringList varname;
  LABELS *varlabels;
  matrix *cmx, *derr;
  QList<CorrVarVarPlotSignal> corr_0, corr_180;
  int pid, mid, mtype;
};

#endif
