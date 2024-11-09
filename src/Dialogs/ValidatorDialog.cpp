#include "ValidatorDialog.h"
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QModelIndex>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QStandardPaths>
#include <QString>
#include <QTextStream>
#ifdef DEBUG
#include <QDebug>
#endif

#include "ClassDialog.h"

void ValidatorDialog::setYSCramblingModels() {
  n_yscrambling = ui.n_yscrambling->value();
}

void ValidatorDialog::setYScrambling() {
  if (ui.YScramblingGroupBox->isChecked()) {
    yscrambling = true;
  } else {
    yscrambling = false;
  }
}

void ValidatorDialog::setNIterations() { niter = ui.iterations->value(); }

void ValidatorDialog::setNGroup() { ngroup = ui.groupnumber->value(); }

int ValidatorDialog::ClassNameContains(QString name) {
  for (int i = 0; i < kfc.size(); i++) {
    if (kfc[i].name.compare(name) == 0) {
      return i;
    } else {
      continue;
    }
  }
  return -1;
}

QString getSeparator(QString line) {
  QStringList seps;
  seps << ";"
       << ","
       << "\t"
       << " ";
  for (int i = 0; i < seps.size(); i++) {
    if (line.contains(seps[i]) == true) {
      return seps[i];
    }
  }
  return seps[0];
}

void ValidatorDialog::setKFoldClass() {
  QString homeLocation = QStandardPaths::locate(
      QStandardPaths::HomeLocation, QString(), QStandardPaths::LocateDirectory);

  QString csv = QString::fromUtf8(
      QFileDialog::getOpenFileName(this, tr("Open CSV Groups"), homeLocation,
                                   tr("CSV File(*.csv)"), 0,
                                   QFileDialog::DontUseNativeDialog)
          .toUtf8());
  QFile file(csv);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    return;

  QTextStream in(&file);
  kfc.clear();

  int cid = 0;
  while (!in.atEnd()) {
    QString line = in.readLine();
    kfc << LABEL();
    kfc.last().name = QString::number(cid);
    kfc.last().objects << QString(line).split(getSeparator(line));
    cid++;
  }
}

void ValidatorDialog::setValidationType() {
  if (ui.leaveoneout->isChecked()) {
    validtype = LOO_; // cross validation
  } else if (ui.kfoldcrossvalid->isChecked()) {
    validtype = KFOLDCV_;
  } else {
    validtype = BOOTSTRAPRGCV_; // cross validation
  }
}

void ValidatorDialog::setModelID(QModelIndex current) {
  if (current.isValid() &&
      projects_->keys().contains(selectedproject_) == true &&
      selectedproject_ > -1) {
    modelid = mids[current.row()];
  }
}

void ValidatorDialog::setProject(QModelIndex current) {
  if (current.isValid()) {
    // set project
    selectedproject_ = pids[current.row()];

    // create the value for the second tab
    tab2->clear();
    mids.clear();
    if (projects_->keys().contains(selectedproject_) == true) {
      if (type == PLSValidation) {
        for (int i = 0; i < projects_->value(selectedproject_)->PLSCount();
             i++) {
          QList<QStandardItem *> row;
          row.append(new QStandardItem(
              projects_->value(selectedproject_)->getPLSModelAt(i)->getName()));
          mids.append(projects_->value(selectedproject_)
                          ->getPLSModelAt(i)
                          ->getModelID());
          tab2->appendRow(row);
        }
      }
      else if (type == MLRValidation) {
        for (int i = 0; i < projects_->value(selectedproject_)->MLRCount();
             i++) {
          QList<QStandardItem *> row;
          row.append(new QStandardItem(
              projects_->value(selectedproject_)->getMLRModelAt(i)->getName()));
          mids.append(projects_->value(selectedproject_)
                          ->getMLRModelAt(i)
                          ->getModelID());
          tab2->appendRow(row);
        }
      } else if (type == LDAValidation) {
        for (int i = 0; i < projects_->value(selectedproject_)->LDACount();
             i++) {
          QList<QStandardItem *> row;
          row.append(new QStandardItem(
              projects_->value(selectedproject_)->getLDAModelAt(i)->getName()));
          mids.append(projects_->value(selectedproject_)
                          ->getLDAModelAt(i)
                          ->getModelID());
          tab2->appendRow(row);
        }
      }
    }
  }
}

void ValidatorDialog::OK() {
  if (selectedproject_ == -1 || modelid == -1) {
    QMessageBox::warning(
        this, tr("Warning!"),
        tr("Please select a project and a model to validate.\n"),
        QMessageBox::Close);
  } else {
    if (ui.averageCrule->isChecked()) {
      crule = Averaging;
    } else {
      crule = Median;
    }
    compute_ = true;
    accept();
  }
}

ValidatorDialog::ValidatorDialog(PROJECTS *projects, int type_) {
  ui.setupUi(this);

  type = type_;

  projects_ = projects;

  if (type == PLSValidation) {
    setWindowTitle("Compute PLS Validation");
    ui.groupBox->hide();
  } else if (type == MLRValidation) {
    setWindowTitle("Compute MLR Validation");
    ui.groupBox->hide();
  } else if (type == LDAValidation) {
    setWindowTitle("Compute LDA Validation");
    ui.groupBox->hide();
    ui.YScramblingGroupBox->hide();
    ui.label->hide();
    ui.n_yscrambling->hide();
  }

  if (projects_->size() > 0) {
    selectedproject_ = 0;
  } else {
    selectedproject_ = -1;
  }

  modelid = -1;
  niter = ui.iterations->value();
  ngroup = ui.groupnumber->value();
  yscrambling = false;
  n_yscrambling = ui.n_yscrambling->value();

  setValidationType();
  compute_ = false;

  tab1 = new QStandardItemModel();
  tab2 = new QStandardItemModel();

  ui.listView->setModel(tab1);
  ui.listView_2->setModel(tab2);

  // Fill the table with data
  QList<QStandardItem *> projectsname;
  for (int i = 0; i < projects_->keys().size(); i++) {
    int pid = projects_->keys()[i];
    projectsname.append(
        new QStandardItem(projects_->value(pid)->getProjectName()));
    pids.append(pid);
  }
  tab1->appendColumn(projectsname);

  connect(ui.loadclassButton, SIGNAL(clicked()), SLOT(setKFoldClass()));
  connect(ui.cancelButton, SIGNAL(clicked()), SLOT(reject()));
  connect(ui.okButton, SIGNAL(clicked()), SLOT(OK()));

  connect(ui.leaveoneout, SIGNAL(clicked(bool)), SLOT(setValidationType()));
  connect(ui.kfoldcrossvalid, SIGNAL(clicked(bool)), SLOT(setValidationType()));
  connect(ui.crossvalid, SIGNAL(clicked(bool)), SLOT(setValidationType()));

  connect(ui.listView->selectionModel(),
          SIGNAL(currentChanged(QModelIndex, QModelIndex)),
          SLOT(setProject(QModelIndex)));
  connect(ui.listView_2->selectionModel(),
          SIGNAL(currentChanged(QModelIndex, QModelIndex)),
          SLOT(setModelID(QModelIndex)));

  connect(ui.groupnumber, SIGNAL(valueChanged(int)), SLOT(setNGroup()));
  connect(ui.iterations, SIGNAL(valueChanged(int)), SLOT(setNIterations()));

  connect(ui.YScramblingGroupBox, SIGNAL(clicked(bool)),
          SLOT(setYScrambling()));
  connect(ui.n_yscrambling, SIGNAL(valueChanged(int)),
          SLOT(setYSCramblingModels()));

  adjustSize();
  ui.listView->selectionModel()->clear();
  ui.listView_2->selectionModel()->clear();
}

ValidatorDialog::~ValidatorDialog() {
  delete tab1;
  delete tab2;
}
