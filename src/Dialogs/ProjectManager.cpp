#include "ProjectManager.h"
#include "addfileDialog.h"
#include "qsmdata.h"
// #include "ImportFromFileDialog.h"
#include "ImportFileDialog.h"
#include "qstudiometricstypes.h"
#include <QFileDialog>
#include <QItemSelectionModel>
#include <QMessageBox>
#include <qdial.h>

#include <QDir>

#ifdef DEBUG
#include <QDebug>
#endif

void ProjectManager::Finish() {
  if (appendstate == true) {
    /*if there are some image in list then append this to the project...*/
    for (int i = 0; i < imagenamelst.size(); i++) {
      for (int j = 0; j < imagenamelst[i].size(); j++) {
        /* check if already exist a file with this name....*/
        QFileInfo finfo(imagenamelst[i][j]);
        bool duplicate = false;
        for (int k = 0; k < data->getImages().size(); k++) {
          if (data->getImages()[k].name.compare(finfo.baseName()) == 0) {
            duplicate = true;
            break;
          } else {
            continue;
          }
        }

        if (duplicate == false) {
          data->getImages().append(IMAGE());
          QString imgname = finfo.fileName();
          imgname.remove(".jpg");
          imgname.remove(".jpeg");
          imgname.remove(".png");
          imgname.remove(".bmp");
          imgname.remove(".ppm");
          imgname.remove(".xbm");
          imgname.remove(".xpm");
          data->getImages().last().name = imgname;
          data->getImages().last().filepath = finfo.absoluteFilePath();
          data->getImages().last().image = QPixmap(finfo.absoluteFilePath());
        } else {
          continue;
        }
      }
    }
    accept();
  } else {
    if (ui.lineEdit->text().isEmpty()) {
      QMessageBox::warning(this, tr("Warning"),
                           tr("Please, specify the project name.\n"),
                           QMessageBox::Ok);
    } else {
      for (int i = 0; i < imagenamelst.size(); i++) {
        for (int j = 0; j < imagenamelst[i].size(); j++) {
          /* check if already exist a file with this name....*/
          QFileInfo finfo(imagenamelst[i][j]);
          bool duplicate = false;
          for (int k = 0; k < data->getImages().size(); k++) {
            if (data->getImages()[k].name.compare(finfo.fileName()) == 0) {
              duplicate = true;
              break;
            } else {
              continue;
            }
          }

          if (duplicate == false) {
            data->getImages().append(IMAGE());
            QString imgname = finfo.fileName();
            imgname.remove(".jpg");
            imgname.remove(".jpeg");
            imgname.remove(".png");
            imgname.remove(".bmp");
            imgname.remove(".ppm");
            imgname.remove(".xbm");
            imgname.remove(".xpm");
            data->getImages().last().name = imgname;
            data->getImages().last().filepath = finfo.absoluteFilePath();
            data->getImages().last().image = QPixmap(finfo.absoluteFilePath());
          } else {
            continue;
          }
        }
      }
      accept();
    }
  }
}

void ProjectManager::Remove() {
  QModelIndexList indexes = ui.tableView->selectionModel()->selectedRows();
  if (indexes.size() < 1)
    return;

  QModelIndex current;
  Q_FOREACH (current, indexes) {
    if (table->index(current.row(), 2)
            .data(Qt::DisplayRole)
            .toString()
            .compare("Matrix") == 0) {
      data->delMatrixAt(matrixid[current.row()]);
    } else if (table->index(current.row(), 2)
                   .data(Qt::DisplayRole)
                   .toString()
                   .compare("Array") == 0) {
      data->delArrayAt(arrayid[current.row()]);
    } else {
      imagenamelst.removeAt(current.row());
    }
    table->removeRow(current.row());
  }
}

void ProjectManager::Add() {
  //   ImportFromFileDialog iffd;
  ImportFileDialog iffd;
  iffd.setPath(lastpath);
  if (iffd.exec() == QDialog::Accepted) {
    //     if(iffd.getDataType() == MATRIXDATA){
    QList<QStandardItem *> row;
    row.append(new QStandardItem(iffd.getFileName()));
    row.append(new QStandardItem(iffd.getMatrix()->getName()));
    row.append(new QStandardItem("Matrix"));
    table->appendRow(row);

    data->addMatrix();
    matrix *_m_ = data->getMATRIXList().last()->Matrix();
    MatrixCopy(iffd.getMatrix()->Matrix(), &_m_);

    for (int i = 0; i < iffd.getMatrix()->getObjName().size(); i++) {
      data->getMATRIXList().last()->getObjName().append(
          iffd.getMatrix()->getObjName()[i]);
    }

    for (int i = 0; i < iffd.getMatrix()->getVarName().size(); i++) {
      data->getMATRIXList().last()->getVarName().append(
          iffd.getMatrix()->getVarName()[i]);
    }

    data->getMATRIXList().last()->setName(iffd.getMatrix()->getName());
    matrixid.append(data->MatrixCount() - 1);
    /*}
    else{
      QList<QStandardItem*> row;
      row.append(new QStandardItem(iffd.getFileName()));
      row.append(new QStandardItem(iffd.getArray()->getName()));
      row.append(new QStandardItem("Array"));

      table->appendRow(row);

      data->addArray();
      array *_a_ = data->getARRAYList().last()->Array();
      for(int i = 0; i < (int)iffd.getArray()->Array()->order; i++){
        ArrayAppendMatrix(&_a_, iffd.getArray()->Array()->m[i]);
      }

      for(int i = 0; i < iffd.getArray()->getObjName().size(); i++){
        data->getARRAYList().last()->getObjName().append(iffd.getArray()->getObjName()[i]);
      }

      for(int i = 0; i < iffd.getArray()->getVarName().size(); i++){
        data->getARRAYList().last()->getVarName().append(iffd.getArray()->getVarName()[i]);
      }

      data->getARRAYList().last()->setName(iffd.getArray()->getName());
      arrayid.append(data->ArrayCount()-1);
    }
    */
    lastpath = iffd.getLastPath();
  }
}

void ProjectManager::AddImagesAsRGB() {
  QFileDialog fimgdialog;

  fimgdialog.setFileMode(QFileDialog::Directory);
  fimgdialog.setOptions(QFileDialog::ShowDirsOnly);
  fimgdialog.setDirectory(lastpath);
  fimgdialog.setWindowTitle(tr("Open Image Directory"));

  //   QString dirname = QFileDialog::getExistingDirectory(this, tr("Open
  //   Directory"), lastpath, QFileDialog::ShowDirsOnly |
  //   QFileDialog::DontResolveSymlinks);
  if (fimgdialog.exec()) {
    QString dirname = fimgdialog.selectedFiles().first();
    QDir dir = QDir(dirname);
    if (dir.exists() && !dir.entryInfoList().isEmpty()) {
      QFileInfo dirinfo(dir.absolutePath());
      QList<QStandardItem *> row;
      row.append(new QStandardItem(dirinfo.filePath()));
      row.append(new QStandardItem(dirinfo.fileName()));
      row.append(new QStandardItem("Matrix"));
      table->appendRow(row);

      QList<QList<float>> imgmx;
      QStringList imgname;
      int colsize = 999999999;
      QFileInfoList list = dir.entryInfoList();
      for (int i = 0; i < list.size(); i++) {
        QFileInfo fileInfo = list.at(i);
        QStringList split = fileInfo.fileName().split(".", Qt::SkipEmptyParts);
        if (!split.isEmpty()) {
          if (split.last().toLower().compare("jpg") == 0 ||
              split.last().toLower().compare("jpeg") == 0 ||
              split.last().toLower().compare("png") == 0 ||
              split.last().toLower().compare("bmp") == 0 ||
              split.last().toLower().compare("ppm") == 0 ||
              split.last().toLower().compare("xbm") == 0 ||
              split.last().toLower().compare("xpm") == 0) {

            imgmx.append(QList<float>());

            QImage img(fileInfo.absoluteFilePath());

            QString rowname = fileInfo.absoluteFilePath();
            rowname.remove(".jpg");
            rowname.remove(".jpeg");
            rowname.remove(".png");
            rowname.remove(".bmp");
            rowname.remove(".ppm");
            rowname.remove(".xbm");
            rowname.remove(".xpm");
            imgname.append(rowname);

            for (int row = 1; row < img.height() + 1; ++row) {
              for (int col = 1; col < img.width() + 1; ++col) {
                QColor clrCurrent(img.pixel(row, col));
                imgmx.last().append(clrCurrent.red() + clrCurrent.green() +
                                    clrCurrent.blue());
              }
            }

            if (imgmx.last().size() < colsize)
              colsize = imgmx.last().size();

          } else {
            continue;
          }
        } else {
          continue;
        }
      }

      if (imgname.size() > 0) {
        data->addMatrix();
        matrix *_m_ = data->getMATRIXList().last()->Matrix();
        ResizeMatrix(_m_, imgmx.size(), colsize);

        for (int i = 0; i < imgmx.size(); i++) {
          QFileInfo finfo(imgname[i]);
          data->getMATRIXList().last()->getObjName().append(finfo.fileName());
          for (int j = 0; j < colsize; j++) {
            _m_->data[i][j] = imgmx[i][j];
          }
        }
        data->getMATRIXList().last()->getVarName().append(firstcol_name);
        for (uint i = 0; i < _m_->col; i++) {
          data->getMATRIXList().last()->getVarName().append(
              QString("pixel%1").arg(i + 1));
        }

        data->getMATRIXList().last()->setName(dirinfo.fileName());
        matrixid.append(data->MatrixCount() - 1);
      }
    } else {
      QMessageBox::warning(this, tr("Warning"),
                           tr("No such directory or could be empty.\n"),
                           QMessageBox::Ok);
    }
  }
}

void ProjectManager::AddImageList() {
  QFileDialog fimgdialog;

  fimgdialog.setFileMode(QFileDialog::Directory);
  fimgdialog.setOptions(QFileDialog::ShowDirsOnly);
  fimgdialog.setDirectory(lastpath);
  fimgdialog.setWindowTitle(tr("Open Image Directory"));

  //   QString dirname = QFileDialog::getExistingDirectory(this, tr("Open
  //   Directory"), lastpath, QFileDialog::ShowDirsOnly |
  //   QFileDialog::DontResolveSymlinks);
  if (fimgdialog.exec()) {
    QString dirname = fimgdialog.selectedFiles().first();
    QDir dir = QDir(dirname);
    if (dir.exists() && !dir.entryInfoList().isEmpty()) {
      QFileInfo dirinfo(dir.absolutePath());
      QList<QStandardItem *> row;
      row.append(new QStandardItem(dirinfo.filePath()));
      row.append(new QStandardItem(dirinfo.fileName()));
      row.append(new QStandardItem("Image List"));
      table->appendRow(row);
      imagenamelst.append(QStringList());

      QFileInfoList list = dir.entryInfoList();
      for (int i = 0; i < list.size(); i++) {
        QFileInfo fileInfo = list.at(i);
        QStringList split = fileInfo.fileName().split(".", Qt::SkipEmptyParts);
        if (!split.isEmpty()) {
          if (split.last().toLower().compare("jpg") == 0 ||
              split.last().toLower().compare("jpeg") == 0 ||
              split.last().toLower().compare("png") == 0 ||
              split.last().toLower().compare("bmp") == 0 ||
              split.last().toLower().compare("ppm") == 0 ||
              split.last().toLower().compare("xbm") == 0 ||
              split.last().toLower().compare("xpm") == 0) {
            imagenamelst.last().append(fileInfo.absoluteFilePath());
          } else {
            continue;
          }
        } else {
          continue;
        }
      }
    } else {
      QMessageBox::warning(this, tr("Warning"),
                           tr("No such directory or could be empty.\n"),
                           QMessageBox::Ok);
    }
  }
}

void ProjectManager::setAppend(bool appendstate_) {
  appendstate = appendstate_;
  if (appendstate == true) {
    ui.lineEdit->setText(data->getProjectName());
    ui.lineEdit->setEnabled(false);
  } else {
    ui.lineEdit->setEnabled(true);
    ui.lineEdit->clear();
  }
}

void ProjectManager::setWindowName(QString name_) { setWindowTitle(name_); }

ProjectManager::ProjectManager(DATA *data_) {
  ui.setupUi(this);

  data = data_;
  appendstate = false;
  table = new QStandardItemModel(0, 3);

  QStringList labels;
  labels << "File"
         << "Data Name"
         << "Type";
  table->setHorizontalHeaderLabels(labels);

  ui.tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
  ui.tableView->setModel(table);

  /*ui.addImageAsRGB->hide();
  #ifdef DEBUG
  ui.addImageAsRGB->show();
  #endif
  */
  connect(ui.addFileButton, SIGNAL(clicked(bool)), SLOT(Add()));
  connect(ui.addImageAsRGB, SIGNAL(clicked(bool)), SLOT(AddImagesAsRGB()));
  connect(ui.addImageListButton, SIGNAL(clicked(bool)), SLOT(AddImageList()));
  connect(ui.removeButton, SIGNAL(clicked(bool)), SLOT(Remove()));
  connect(ui.finish, SIGNAL(clicked(bool)), SLOT(Finish()));
  connect(ui.cancel, SIGNAL(clicked(bool)), SLOT(reject()));
}

ProjectManager::~ProjectManager() { delete table; }
