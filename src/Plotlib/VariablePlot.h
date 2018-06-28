#ifndef VARIABLEPLOT_H
#define VARIABLEPLOT_H
#include "ScatterPlot.h"
#include "BarPlot.h"
#include "qsmdata.h"

#include <QList>
#include <QString>
#include <QStringList>
#include "qstudiometricstypes.h"

class VariablePlot
{
public:
  VariablePlot(PROJECTS *projects_);
  void setPlotType(int type_){ type = type_; } // MATRIXDATA, ARRATDATA
  void setPID(int pid_){ pid = pid_; }
  void setXHash(QStringList xhash_){ xhash = xhash_; }
  void setYHash(QStringList yhash_){ yhash = yhash_; }
  void setObjects(QList< QStringList > objects_){ objects = objects_; }
  void setObjLabels(QStringList selobjlabels_){ selobjlabels = selobjlabels_; }
  void setVarID1(int varid1_){ varid1 = varid1_; }
  void setVarID2(int varid2_){ varid2 = varid2_; }
  void setLayerID(int layerid_){ layerid = layerid_; }


  ScatterPlot *VariableVSVariable();
  BarPlot *VariableDistribution();
private:
  PROJECTS *projects;
  int type, pid, layerid, varid1, varid2;
  QStringList selobjlabels, xhash, yhash;
  QList< QStringList > objects;

};
#endif
