#include "ModelInfo.h"
#include <QApplication>
#include "qstudiometricstypes.h"

void ModelInfo::setVariableSelectionOptions(QString varseloption)
{
  ui.varseloptions->clear();
  QStringList list = varseloption.split(";");
  for(int i = 0; i < list.size(); i++)
    ui.varseloptions->append(list[i].trimmed());
}

void ModelInfo::setVariableSelectionAlgorithm(QString varselalgorithm)
{
  ui.varselalgorithm->setText(varselalgorithm);
  ui.modvalidtypelabel->hide(); ui.modvalidtype->hide();
}

void ModelInfo::setModelValidationType(int modvalidtype_)
{
  if(modvalidtype_ == 0){
    ui.modvalidtype->setText("Model Not Validated");
  }
  else if(modvalidtype_ == LOO){
    ui.modvalidtype->setText("LOO Validation");
  }
  else if(modvalidtype_ == RANDOMGROUP){
    ui.modvalidtype->setText("Random Group Validation");
  }
  else{
    ui.modvalidtype->setText("Error!!! Validation not recognized!!!");
  }
}

void ModelInfo::setModelYScalingType(int modyscalingtype_)
{
  if(modyscalingtype_ == 0){
    ui.modyscaling->setText("No Scaling");
  }
  else if(modyscalingtype_ == 1){
    ui.modyscaling->setText("SDEV Column Scaling");
  }
  else  if(modyscalingtype_ == 2){
    ui.modyscaling->setText("RMS Column Scaling");
  }
  else  if(modyscalingtype_ == 3){
    ui.modyscaling->setText("Pareto Column Scaling");
  }
  else  if(modyscalingtype_ == 4){
    ui.modyscaling->setText("Range Column Scaling");
  }
  else  if(modyscalingtype_ == 5){
    ui.modyscaling->setText("Level Column Scaling");
  }
  else{
    ui.modyscaling->setText("Error!!! Scaling not recongized!!!");
  }
}


void ModelInfo::setModelXScalingType(int modxscalingtype_)
{
  if(modxscalingtype_ == 0){
    ui.modxscaling->setText("No Scaling");
  }
  else if(modxscalingtype_ == 1){
    ui.modxscaling->setText("SDEV Column Scaling");
  }
  else  if(modxscalingtype_ == 2){
    ui.modxscaling->setText("RMS Column Scaling");
  }
  else  if(modxscalingtype_ == 3){
    ui.modxscaling->setText("Pareto Column Scaling");
  }
  else  if(modxscalingtype_ == 4){
    ui.modxscaling->setText("Range Column Scaling");
  }
  else  if(modxscalingtype_ == 5){
    ui.modxscaling->setText("Level Column Scaling");
  }
  else{
    ui.modxscaling->setText("Error!!! Scaling not recongized!!!");
  }
}

void ModelInfo::setYDataOrigin(QString modydataorig_)
{
  ui.modyorig->setText(modydataorig_);
}

void ModelInfo::setXDataOrigin(QString modxdataorig_)
{
  ui.modxorig->setText(modxdataorig_);
}

void ModelInfo::setNY(int ny)
{
  ui.nyvariables->setText(QString::number(ny));
}

void ModelInfo::setNVariables(int nvar)
{
  ui.nvariables->setText(QString::number(nvar));
}

void ModelInfo::setNObjects(int nobj)
{
  ui.nobjects->setText(QString::number(nobj));
}

void ModelInfo::setModelComponentNumber(int modnpc_)
{
  ui.modnpc->setText(QString::number(modnpc_));
}

void ModelInfo::setModelType(QString modtype_)
{
  if(modtype_.compare("PCA Model") == 0 || modtype_.compare("UPCA Model") == 0){
    ui.modyoriglabel->hide(); ui.modyorig->hide();
    ui.modyscalinglabel->hide(); ui.modyscaling->hide();
    ui.modvalidtypelabel->hide(); ui.modvalidtype->hide();
    ui.varseltypelabel->hide(); ui.varselalgorithm->hide();
    ui.varseloptionslabel->hide(); ui.varseloptions->hide();
    ui.nyvariables->hide();
    resize(minimumSize());
  }
  else if(modtype_.compare("PLS Model") == 0 || modtype_.compare("UPLS Model") == 0){
    ui.varseltypelabel->hide(); ui.varselalgorithm->hide();
    ui.varseloptionslabel->hide(); ui.varseloptions->hide();
    resize(minimumSize());
  }
  ui.modtype->setText(modtype_);
  
}

void ModelInfo::setModelName(QString modname_)
{
  ui.modname->setText(modname_);  
}

void ModelInfo::Adjust()
{
  resize(minimumSize());
//   adjustSize();
}

ModelInfo::ModelInfo(QWidget* parent): QWidget(parent)
{
  ui.setupUi(this);
}
