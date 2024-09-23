#include "DoClusterAnalysisDialog.h"
#include "qsmdata.h"
#include "qstudiometricstypes.h"

#include <QMessageBox>
#include <QStringListModel>

int DoClusterAnalysisDialog::getMaxClustersNumber() {
  return ui.maxnclustersBox->value();
}

int DoClusterAnalysisDialog::getNGroups() { return ui.groupBox->value(); }

int DoClusterAnalysisDialog::getNIterations() {
  return ui.iterationBox->value();
}

int DoClusterAnalysisDialog::getVaidationType() {
  if (ui.jumpMethodButton->isChecked()) {
    return JUMPMETHOD;
  } else {
    return BOOTSTRAPRGCV_;
  }
}

bool DoClusterAnalysisDialog::ValidateCluster() {
  if (ui.validationClustersButton->isChecked()) {
    return true;
  } else {
    return false;
  }
}

bool DoClusterAnalysisDialog::SaveClusterLabels() {
  if (ui.saveClusterLabelBox->isChecked()) {
    return true;
  } else {
    return false;
  }
}

int DoClusterAnalysisDialog::getNMaxObjects() {
  return ui.maxnobjectBox->value();
}

int DoClusterAnalysisDialog::getExtractObjects() {
  if (ui.extractObjectBox->isChecked()) {
    if (ui.nearobjcentroidButton->isChecked()) {
      return NEAROBJECTS;
    } else {
      return FAROBJECTS;
    }
  } else {
    return ALLOBJECTS;
  }
}

int DoClusterAnalysisDialog::getNumberOfClusters() {
  return ui.nclusterBox->value();
}

int DoClusterAnalysisDialog::getAlgorithmType() {
  if (ui.kmeansrandom->isChecked()) {
    return KMEANSRANDOM;
  } else if (ui.kmeanspp->isChecked()) {
    return KMEANSPP;
  } else if (ui.kmeansmdc->isChecked()) {
    return KMEANSMDC;
  } else if (ui.kmeansmaxmindis->isChecked()) {
    return KMEANSMAXMINDIS;
  } else if (ui.hclsinglelink->isChecked()) {
    return HCLSINGLELINK;
  } else if (ui.hclcompletelink->isChecked()) {
    return HCLCOMPLETELINK;
  } else if (ui.hclaveragelink->isChecked()) {
    return HCLAVERAGELINK;
  } else {
    return HCLWARDLINK;
  }
}

QString DoClusterAnalysisDialog::getClusterLabelSufix() {
  return ui.lineEdit->text();
}

int DoClusterAnalysisDialog::getDataType() {
  if (ui.plotcoordinatesButton->isChecked()) {
    return 0;
  } else {
    return 1;
  }
}

QString DoClusterAnalysisDialog::getDataHash() { return selectedhash; }

int DoClusterAnalysisDialog::getLayerID() {
  return ui.layerlist->currentIndex();
}

void DoClusterAnalysisDialog::EnableDisable() {
  if (ui.plotcoordinatesButton->isChecked())
    ui.listView->setEnabled(false);
  else
    ui.listView->setEnabled(true);
}

void DoClusterAnalysisDialog::GenLayerList() {
  selectedhash = hash[ui.listView->currentIndex().row()];
  int id = -1;

  for (int i = 0; i < arlst.size(); i++) {
    if (selectedhash.compare(arlst[i]->getHash()) == 0) {
      id = i;
      break;
    } else {
      continue;
    }
  }

  if (id != -1) {
    for (uint i = 0; i < arlst[id]->Array()->order; i++) {
      ui.layerlist->addItem(QString::number(i + 1));
    }
  } else {
    ui.layerlist->clear();
  }
}

void DoClusterAnalysisDialog::OK() {
  if (ui.saveClusterLabelBox->isChecked() && ui.lineEdit->text().isEmpty()) {
    QMessageBox::warning(this, tr("Warning!"), tr("Please set Model Name.\n"),
                         QMessageBox::Close);
  } else {
    return accept();
  }
}

DoClusterAnalysisDialog::DoClusterAnalysisDialog(QList<MATRIX *> *mxlst_,
                                                 QList<ARRAY *> *arlst_,
                                                 int windowtype_)
    : QDialog(0) {
  ui.setupUi(this);
  windowtype = windowtype_;

  ui.jumpMethodButton->setEnabled(false);

  if (mxlst_ != NULL)
    mxlst = (*mxlst_);

  if (arlst_ != NULL)
    arlst = (*arlst_);

  QStringList model;

  for (int i = 0; i < mxlst.size(); i++) {
    hash.append(mxlst[i]->getHash());
    model.append(mxlst[i]->getName());
  }

  for (int i = 0; i < arlst.size(); i++) {
    hash.append(arlst[i]->getHash());
    model.append(arlst[i]->getName());
  }

  ui.listView->setModel(new QStringListModel(model));

  EnableDisable();
  connect(ui.listView->selectionModel(),
          SIGNAL(selectionChanged(QItemSelection, QItemSelection)),
          SLOT(GenLayerList()));
  connect(ui.plotcoordinatesButton, SIGNAL(clicked(bool)),
          SLOT(EnableDisable()));
  connect(ui.sourcedataButton, SIGNAL(clicked(bool)), SLOT(EnableDisable()));
  connect(ui.cancelButton, SIGNAL(clicked(bool)), SLOT(reject()));
  connect(ui.okButton, SIGNAL(clicked(bool)), SLOT(OK()));
}
