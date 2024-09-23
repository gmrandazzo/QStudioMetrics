#include "LabelDialog.h"
#include <QMessageBox>
#include <qdial.h>

#include "ImportFileDialog.h"
#include "ImportLabelListDialog.h"
#include "addLabelDialog.h"
#include "qstudiometricstypes.h"

#ifdef DEBUG
#include <QDebug>
#endif

int LabelDialog::ntablabels() {
  int n = 0;
  for (int i = 0; i < lidlst.size(); i++) {
    if (lidlst[i].type == TABLABELST)
      n++;
    else
      continue;
  }
  return n;
}

int LabelDialog::nlabels() {
  int n = 0;
  for (int i = 0; i < lidlst.size(); i++) {
    if (lidlst[i].type == OBJLABELST || lidlst[i].type == VARLABELST)
      n++;
    else
      continue;
  }
  return n;
}

bool LabelDialog::ObjlabelContains(QString name) {
  for (int i = 0; i < (*labels).size(); i++) {
    if ((*labels)[i].name.compare(name, Qt::CaseSensitive) == 0) {
      return true;
    } else {
      continue;
    }
  }
  return false;
}

void LabelDialog::AddLabel() {
  addLabelDialog add("Insert Label");
  if (add.exec() == QDialog::Accepted) {
    if (ObjlabelContains(add.getLabel()) == false) {
      QList<QStandardItem *> label;
      label.append(new QStandardItem(add.getLabel()));
      tab2->appendRow(label);
      labels->append(LABEL());
      labels->last().name = add.getLabel();
      lidlst.append(labtype());
      lidlst.last().id = nlabels();

      if (windowtype == OBJLABELS)
        lidlst.last().type = OBJLABELST;
      else if (windowtype == VARLABELS)
        lidlst.last().type = VARLABELST;
      else
        lidlst.last().type = TABLABELST;
    } else {
      QMessageBox::warning(this, tr("Warning!"), tr("Duplicated Label Name!\n"),
                           QMessageBox::Close);
    }
  }
}

void LabelDialog::ImportTable() {
  ImportFileDialog iffd;
  iffd.setPath(lastpath);
  if (iffd.exec() == QDialog::Accepted) {
    QList<QStandardItem *> label;
    label.append(new QStandardItem(iffd.getMatrix()->getName()));
    tab2->appendRow(label);
    tablabels->append(new TABLABEL());
    tablabels->last()->setName(iffd.getMatrix()->getName());
    matrix *msrc = iffd.getMatrix()->Matrix();
    matrix *mdst = tablabels->last()->getMatrix();
    MatrixCopy(msrc, &mdst);
    tablabels->last()->getObjectsName().append(iffd.getMatrix()->getObjName());
    tablabels->last()->getFeaturesName().append(iffd.getMatrix()->getVarName());

    lidlst.append(labtype());
    lidlst.last().id = ntablabels();
    lidlst.last().type = TABLABELST;

    lastpath = iffd.getLastPath();
  }
}

void LabelDialog::ImportLabelList() {
  ImportLabelListDialog illdialog(lastpath);
  if (illdialog.exec() == QDialog::Accepted) {
    if (ObjlabelContains(illdialog.getLabel()) == false) {

      QList<QStandardItem *> label;
      label.append(new QStandardItem(illdialog.getLabel()));
      tab2->appendRow(label);
      labels->append(LABEL());
      labels->last().name = illdialog.getLabel();
      DATA::ImportRows(illdialog.getFileName(), labels->last().objects);

      lidlst.append(labtype());
      lidlst.last().id = nlabels();

      if (windowtype == OBJLABELS)
        lidlst.last().type = OBJLABELST;
      else if (windowtype == VARLABELS)
        lidlst.last().type = VARLABELST;
      else
        lidlst.last().type = TABLABELST;
    } else {
      QMessageBox::warning(this, tr("Warning!"), tr("Duplicated Label Name!\n"),
                           QMessageBox::Close);
    }
  }
}

void LabelDialog::Show() {
  if (pid > -1 && lid > -1 && lid < lidlst.size()) {
    lsig_.pid = pid;
    lsig_.id = lidlst[lid].id;
    lsig_.type = lidlst[lid].type;
    emit ShowList(lsig_);
  } else {
    return;
  }
}

void LabelDialog::RemoveLabel() {
  if (lid != -1 && lid < lidlst.size()) {
    if (lidlst[lid].type == OBJLABELST || lidlst[lid].type == VARLABELST) {
#ifdef DEBUG
      qDebug() << "REMOVE LABEL";
#endif
      labels->removeAt(lidlst[lid].id);
    } else {
#ifdef DEBUG
      qDebug() << "REMOVE TABLABEL";
#endif
      delete tablabels->at(lidlst[lid].id);
      tablabels->removeAt(lidlst[lid].id);
    }
    tab2->removeRow(lid);
  }
}

void LabelDialog::OK() {
  if (lid != -1 && lid < lidlst.size()) {
    if (lidlst[lid].type == OBJLABELST) {
#ifdef DEBUG
      qDebug() << "OBJLABELST";
#endif
    } else if (lidlst[lid].type == VARLABELST) {
#ifdef DEBUG
      qDebug() << "VARLABELST";
#endif
    } else {
#ifdef DEBUG
      qDebug() << "STICAZZI";
#endif
    }
#ifdef DEBUG
    qDebug() << lidlst[lid].id << (*labels).size();
#endif
    if ((lidlst[lid].type == OBJLABELST || lidlst[lid].type == VARLABELST) &&
        lidlst[lid].id < (*labels).size()) {
      (*labels)[lidlst[lid].id].objects.append(selectedobject);
      accept();
    } else {
      return;
    }
  } else {
    return;
  }
}

void LabelDialog::Cancel() { reject(); }

void LabelDialog::Close() { accept(); }

void LabelDialog::GenLabelView(QModelIndex current) {
  if (current.isValid()) {
    pid = projects->keys()[current.row()];
    lidlst.clear();

    if (pid != -1) {
      tab2->clear();

      if (windowtype == OBJLABELS) {
        labels = &(projects->value(pid)->getObjectLabels());
        tablabels = 0;
      } else {
        labels = &(projects->value(pid)->getVariableLabels());
        tablabels = &(projects->value(pid)->getVariableTabLabels());
      }

      if (!(*labels).isEmpty()) {
        QList<QStandardItem *> labellist;
        for (int i = 0; i < (*labels).size(); i++) {
          lidlst.append(labtype());
          lidlst.last().id = i;
          if (windowtype == OBJLABELS)
            lidlst.last().type = OBJLABELST;
          else
            lidlst.last().type = VARLABELST;

          labellist.append(new QStandardItem((*labels)[i].name));
        }
        tab2->appendColumn(labellist);
      }

      if (tablabels != 0) {
        if (!(*tablabels).isEmpty()) {
          QList<QStandardItem *> labellist;
          for (int i = 0; i < (*tablabels).size(); i++) {
            lidlst.append(labtype());
            lidlst.last().id = i;
            lidlst.last().type = TABLABELST;
            labellist.append(new QStandardItem((*tablabels)[i]->getName()));
          }
          tab2->appendRow(labellist);
        }
      }
    }
  }
}

void LabelDialog::setLabelId(QModelIndex current) {
  if (current.isValid()) {
    lid = current.row();
  }
}

LabelDialog::LabelDialog(PROJECTS *projects_, int windowtype_) : QDialog() {
  ui.setupUi(this);

  windowtype = windowtype_;

  if (windowtype == OBJLABELS || windowtype == SHOWOBJLABELS)
    ui.importtabButton->hide();

  ui.okButton->hide();
  ui.cancelButton->hide();
  tab1 = new QStandardItemModel();
  ui.projectView->setModel(tab1);
  tab2 = new QStandardItemModel();
  ui.labelView->setModel(tab2);
  adjustSize();

  projects = projects_;
  pid = lid = -1;

  // Fill the table with data
  QList<QStandardItem *> projectsname;
  for (int i = 0; i < projects_->keys().size(); i++) {
    pid = projects_->keys()[i];
    projectsname.append(
        new QStandardItem(projects_->value(pid)->getProjectName()));
  }
  tab1->appendColumn(projectsname);

  connect(ui.closeButton, SIGNAL(clicked(bool)), SLOT(Close()));
  connect(ui.addButton, SIGNAL(clicked(bool)), SLOT(AddLabel()));
  connect(ui.importlistButton, SIGNAL(clicked(bool)), SLOT(ImportLabelList()));
  connect(ui.importtabButton, SIGNAL(clicked(bool)), SLOT(ImportTable()));
  connect(ui.showButton, SIGNAL(clicked(bool)), SLOT(Show()));
  connect(ui.removeButton, SIGNAL(clicked(bool)), SLOT(RemoveLabel()));
  connect(ui.projectView->selectionModel(),
          SIGNAL(currentChanged(QModelIndex, QModelIndex)),
          SLOT(GenLabelView(QModelIndex)));
  connect(ui.labelView->selectionModel(),
          SIGNAL(currentChanged(QModelIndex, QModelIndex)),
          SLOT(setLabelId(QModelIndex)));
}

LabelDialog::LabelDialog(LABELS *labels_, QStringList &selectedobject_,
                         int windowtype_)
    : QDialog() {
  ui.setupUi(this);
  ui.importlistButton->hide();
  ui.projectgroupBox->hide();
  ui.showButton->hide();
  ui.closeButton->hide();
  ui.importtabButton->hide();

  windowtype = windowtype_;

  tab1 = 0;
  tab2 = new QStandardItemModel();
  ui.labelView->setModel(tab2);
  adjustSize();

  labels = labels_;
  selectedobject = selectedobject_;

  pid = lid = -1;

  if (!labels->isEmpty()) {
    QList<QStandardItem *> labellist;
    for (int i = 0; i < labels->size(); i++) {
      labellist.append(new QStandardItem((*labels)[i].name));

      lidlst.append(labtype());
      lidlst.last().id = i;
      if (windowtype == OBJLABELS)
        lidlst.last().type = OBJLABELST;
      else
        lidlst.last().type = VARLABELST;
    }
    tab2->appendColumn(labellist);
  }

  connect(ui.okButton, SIGNAL(clicked(bool)), SLOT(OK()));
  connect(ui.cancelButton, SIGNAL(clicked(bool)), SLOT(Cancel()));
  connect(ui.addButton, SIGNAL(clicked(bool)), SLOT(AddLabel()));
  connect(ui.removeButton, SIGNAL(clicked(bool)), SLOT(RemoveLabel()));
  connect(ui.labelView->selectionModel(),
          SIGNAL(currentChanged(QModelIndex, QModelIndex)),
          SLOT(setLabelId(QModelIndex)));
}

LabelDialog::~LabelDialog() {
  if (tab1 != 0) {
    delete tab1;
  }
  delete tab2;
}
