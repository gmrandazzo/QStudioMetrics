#include "addfileDialog.h"
#include <QDialogButtonBox>
#include <QPushButton>
#include <QFileDialog>
#include <QStandardItemModel>
#include <QStringList>
#include <QTextStream>
#include <QMessageBox>
#include "qstudiometricstypes.h"

auto AddFileDialog::CountNumberColum(QString file_)
{
  ssize_t ncol = 0;
  QFile file(file_);
  file.open(QIODevice::ReadOnly | QIODevice::Text);
  QTextStream in(&file);
  while(!in.atEnd()){
    if(in.readLine().contains("#", Qt::CaseInsensitive) == true){ // skip line
      continue;
    }
    else{
      ncol = in.readLine().split(getSeparator()).size();
      break;
    }
  }
  file.close();
  return ncol;
}

auto AddFileDialog::CountNumberRow(QString file_)
{
  ssize_t nrow = 0;
  QFile file(file_);
  file.open(QIODevice::ReadOnly | QIODevice::Text);
  QTextStream in(&file);
  while(!in.atEnd()){
    QString line = in.readLine();
    if(line.contains("#", Qt::CaseInsensitive) == true){ // skip line
      continue;
    }
    else if(line.size() == 1 && line.compare("-", Qt::CaseInsensitive) == true){
      break;
    }
    else{
      nrow++;
    }
  }
  file.close();
  return nrow;
}

QStringList AddFileDialog::ListRead(QString file_)
{
  QStringList lst;
  QFile file(file_);
  file.open(QIODevice::ReadOnly | QIODevice::Text);
  QTextStream in(&file);
  while(!in.atEnd()){
    lst.append(in.readLine().trimmed().split(getSeparator(), Qt::SkipEmptyParts));
  }
  file.close();
  return lst;
}


void AddFileDialog::setSimpleOpen()
{
  ui.label->hide();
  ui.splitlineby->hide();
  ui.label_3->hide();
  ui.horizontalSpacer->changeSize(0,0, QSizePolicy::Fixed, QSizePolicy::Fixed);
  ui.horizontalSpacer_3->changeSize(0,0, QSizePolicy::Fixed, QSizePolicy::Fixed);
  ui.ignorelinebychar->hide();
  ui.file_varname->hide();
  ui.filevarname->hide();
  ui.openbutton->hide();
}

QString AddFileDialog::getSkipChar()
{
  return ui.ignorelinebychar->text();
}

QString AddFileDialog::getSeparator()
{
  if(ui.splitlineby->currentIndex() == 0) // tab
    return "\t";
  else if(ui.splitlineby->currentIndex() == 1) // space
    return " ";
  else if(ui.splitlineby->currentIndex() == 2) // ;
    return ";";
  else // PERSONAL
    return ui.splitlineby->currentText();

}

QString AddFileDialog::getLabel()
{
  if(ui.filename->text().compare("project") != 0) // project name is a reserved name in order to recognize a speicif kind of data in the qtreeview
    return ui.filename->text();
  else
    return ui.filename->text().append("_");
}

QString AddFileDialog::getFileName()
{
  return ui.file->text();
}

QString AddFileDialog::getFileObjName()
{
  if(ui.fileobjname->isChecked()){
    if(ui.file_objname->text().isEmpty()){
      return QString("");
    }
    else{
      return ui.file_objname->text();
    }
  }
  else{
    return QString("");
  }
}

QString AddFileDialog::getFileVarName()
{
  if(ui.filevarname->isChecked()){
    if(ui.file_varname->text().isEmpty()){
      return QString("");
    }
    else{
      return ui.file_varname->text();
    }
  }
  else{
    return QString("");
  }
}

void AddFileDialog::Open()
{
  if(path.isEmpty()){
    ui.file->setText(QFileDialog::getOpenFileName( this, tr("Open File"), QDir::currentPath(), tr("Text Files (*.txt *.csv);;All files (*.*)"), 0, QFileDialog::DontUseNativeDialog ));
    QFileInfo last(ui.file->text());
    path = last.absoluteFilePath();
  }
  else{
    ui.file->setText(QFileDialog::getOpenFileName( this, tr("Open File"), path, tr("Text Files (*.txt *.csv);;All files (*.*)"), 0, QFileDialog::DontUseNativeDialog ));
    QFileInfo last(ui.file->text());
    path = last.absoluteFilePath();
  }

  QStringList list = ui.file->text().split("/", Qt::SkipEmptyParts);
  if(list.size() > 0){
    list.last().remove(".txt", Qt::CaseInsensitive);
    list.last().remove(".csv", Qt::CaseInsensitive);
    ui.filename->setText(list.last());
  }
}

void AddFileDialog::OpenFileObjName()
{
  if(path.isEmpty()){
    ui.file_objname->setText(QFileDialog::getOpenFileName( this, tr("Open File"), QDir::currentPath(), tr("Text Files (*.txt *.csv);;All files (*.*)"), 0, QFileDialog::DontUseNativeDialog ));
    QFileInfo last(ui.file_objname->text());
    path = last.absoluteFilePath();
  }
  else{
    ui.file_objname->setText(QFileDialog::getOpenFileName( this, tr("Open File"), path, tr("Text Files (*.txt *.csv);;All files (*.*)"), 0, QFileDialog::DontUseNativeDialog ));
    QFileInfo last(ui.file_objname->text());
    path = last.absoluteFilePath();
  }
}

void AddFileDialog::OpenFileVarName()
{
  if(path.isEmpty()){
    ui.file_varname->setText(QFileDialog::getOpenFileName( this, tr("Open File"), QDir::currentPath(), tr("Text Files (*.txt *.csv);;All files (*.*)"), 0, QFileDialog::DontUseNativeDialog ));
    QFileInfo last(ui.file_varname->text());
    path = last.absoluteFilePath();
  }
  else{
    ui.file_varname->setText(QFileDialog::getOpenFileName( this, tr("Open File"), path, tr("Text Files (*.txt *.csv);;All files (*.*)"), 0, QFileDialog::DontUseNativeDialog ));
    QFileInfo last(ui.file_varname->text());
    path = last.absoluteFilePath();
  }
}

void AddFileDialog::Accept()
{
  if(!ui.file->text().isEmpty())
    accept();
  else
    reject();
}


void AddFileDialog::Preview()
{
  if(!ui.file->text().isEmpty()){

    model->clear();

    QFile file(ui.file->text());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
      return;

    auto file_nrow = CountNumberRow(ui.file->text());
    auto file_ncol = CountNumberColum(ui.file->text());

    QStringList objname;
    if(!getFileObjName().isEmpty()){
      objname = ListRead(getFileObjName());

      if(file_nrow != objname.size()){
        QMessageBox::warning(this, tr("Import Warning!!"), tr("Object name size and data row differ.\n Please check your data."), QMessageBox::Ok);
      }
    }

    int row = 0;
    QTextStream in(&file);
    while(!in.atEnd()){
      QString line_ = in.readLine().trimmed();
      if(line_.toStdString().find(getSkipChar().toStdString()) == 0 || line_.isEmpty()){
        continue;
      }
      else if(line_.compare("-", Qt::CaseInsensitive) == 0){
        break;
      }
      else{
        QStringList value =  line_.split(getSeparator(), Qt::SkipEmptyParts);
        if(row == 0){
          if(ui.filevarname->isChecked() && !ui.file_varname->text().isEmpty()){
            model->setHorizontalHeaderItem(0, new QStandardItem(firstcol_name));
            QStringList varname = ListRead(getFileVarName());

            if(file_ncol != varname.size()){
              QMessageBox::warning(this, tr("Import Warning!!"), tr("Variable name size and data column differ.\n Please check your data or variable file."), QMessageBox::Ok);
            }

            if(varname.size() < 10){ // is the max number of column
              for(int i = 0; i < varname.size(); i++){
                model->setHorizontalHeaderItem(i+1, new QStandardItem(varname[i]));
              }
            }
            else{
              for(int i = 0; i < 10; i++){
                model->setHorizontalHeaderItem(i+1, new QStandardItem(varname[i]));
              }
              model->setHorizontalHeaderItem(11, new QStandardItem("..."));
            }
          }
          else{
            model->setHorizontalHeaderItem(0, new QStandardItem(firstcol_name));

            if(value.size() < 10){ // is the max number of column
              for(int c = 0; c < value.size(); c++){
                model->setHorizontalHeaderItem(c+1, new QStandardItem(QString("Column %0" ).arg(c+1)));
              }
            }
            else{
              for(int c = 0; c < 10; c++){
                model->setHorizontalHeaderItem(c+1, new QStandardItem(QString("Column %0" ).arg(c+1)));
              }
              model->setHorizontalHeaderItem(11, new QStandardItem("..."));
            }
          }
          QList<QStandardItem*> line;

          if(objname.isEmpty()){
            line.append(new QStandardItem(QString("obj%0" ).arg(row+1)));
          }
          else{
            if(row < objname.size()){
              line.append(new QStandardItem(objname[row]));
            }
            else{
              line.append(new QStandardItem(QString("obj%0" ).arg(row+1)));
            }
          }

          if(value.size() < 10){
            for(int c = 0; c < value.size(); c++){
              line.append(new QStandardItem(value[c]));
            }
          }
          else{
            for(int c = 0; c < 10; c++){
              line.append(new QStandardItem(value[c]));
            }
            line.append(new QStandardItem("..."));
          }
          model->appendRow(line);
        }
        else if(row > 5){
          QList<QStandardItem*> line;
          line.append(new QStandardItem(QString("...")));
          if(value.size() < 10){
            for(int c = 0; c < value.size(); c++){
              line.append(new QStandardItem(value[c]));
            }
          }
          else{
            for(int c = 0; c < 10; c++){
              line.append(new QStandardItem(value[c]));
            }
            line.append(new QStandardItem("..."));
          }
          model->appendRow(line);
          break;
        }
        else{
          QList<QStandardItem*> line;
          if(objname.isEmpty()){
            line.append(new QStandardItem(QString("obj%0" ).arg(row+1)));
          }
          else{
            if(row < objname.size()){
              line.append(new QStandardItem(objname[row]));
            }
            else{
              line.append(new QStandardItem(QString("obj%0" ).arg(row+1)));
            }
          }

          if(value.size() < 10){
            for(int c = 0; c < value.size(); c++){
              line.append(new QStandardItem(value[c]));
            }
          }
          else{
            for(int c = 0; c < 10; c++){
              line.append(new QStandardItem(value[c]));
            }
            line.append(new QStandardItem("..."));
          }
          model->appendRow(line);
        }
        row++;
      }
    }
    file.close();
  }

}

AddFileDialog::AddFileDialog()
{
  ui.setupUi(this);

  model = new QStandardItemModel();

  ui.tableView->setModel(model);

  connect(ui.ok, SIGNAL(clicked(bool)), SLOT(Accept()));
  connect(ui.cancel, SIGNAL(clicked(bool)), SLOT(reject()));
  connect(ui.buttonBox->button(QDialogButtonBox::Open), SIGNAL(clicked(bool)), this, SLOT(Open()));
  connect(ui.openbutton, SIGNAL(clicked(bool)), this, SLOT(OpenFileObjName()));
  connect(ui.openbutton_2, SIGNAL(clicked(bool)), this, SLOT(OpenFileVarName()));

  connect(ui.file, SIGNAL(textChanged(QString)), SLOT(Preview()));
  connect(ui.splitlineby, SIGNAL(currentIndexChanged(int)), SLOT(Preview()));
  connect(ui.ignorelinebychar, SIGNAL(textChanged(QString)), SLOT(Preview()));
  connect(ui.file_varname, SIGNAL(textChanged(QString)), SLOT(Preview()));
  connect(ui.filevarname, SIGNAL(clicked(bool)), SLOT(Preview()));
  connect(ui.fileobjname, SIGNAL(clicked(bool)), SLOT(Preview()));
  connect(ui.file_objname, SIGNAL(textChanged(QString)), SLOT(Preview()));
}

AddFileDialog::~AddFileDialog()
{
  delete model;
}
