#include "ImportLabelListDialog.h"
#include <QFileDialog>
#include <QFileInfo>

QString ImportLabelListDialog::getFileName()
{
  return ui.file->text();
}

QString ImportLabelListDialog::getLabel()
{
  return ui.labelname->text();
}

void ImportLabelListDialog::Open(){
  #ifdef WIN32
  if(path.isEmpty()){
    ui.file->setText(QFileDialog::getOpenFileName( this, tr("Open File"), QDir::currentPath(), tr("Text Files (*.txt *.csv *.lst *.list);;All files (*.*)")));
    QFileInfo last(ui.file->text());
    path = last.absoluteFilePath();
  }
  else{
    ui.file->setText(QFileDialog::getOpenFileName( this, tr("Open File"), path, tr("Text Files (*.txt *.csv *.lst *.list);;All files (*.*)")));
    QFileInfo last(ui.file->text());
    path = last.absoluteFilePath();
  }
  
  QStringList list = ui.file->text().split("/", QString::SkipEmptyParts);
  if(list.size() > 0){
    list.last().remove(".txt");
    list.last().remove(".csv");
    ui.labelname->setText(list.last());
  }
  #else
  if(path.isEmpty()){
    ui.file->setText(QFileDialog::getOpenFileName( this, tr("Open File"), QDir::currentPath(), tr("Text Files (*.txt *.csv *.lst *.list);;All files (*.*)"), 0, QFileDialog::DontUseNativeDialog ));
    QFileInfo last(ui.file->text());
    path = last.absoluteFilePath();
  }
  else{
    ui.file->setText(QFileDialog::getOpenFileName( this, tr("Open File"), path, tr("Text Files (*.txt *.csv *.lst *.list);;All files (*.*)"), 0, QFileDialog::DontUseNativeDialog ));
    QFileInfo last(ui.file->text());
    path = last.absoluteFilePath();
  }
  
  QStringList list = ui.file->text().split("/", QString::SkipEmptyParts);
  if(list.size() > 0){
    list.last().remove(".txt", Qt::CaseInsensitive);
    list.last().remove(".csv", Qt::CaseInsensitive);
    ui.labelname->setText(list.last());
  }
  #endif
}

ImportLabelListDialog::ImportLabelListDialog(QString path_)
{
  ui.setupUi(this);
  path = path_;
  connect(ui.openButton, SIGNAL(clicked(bool)), this, SLOT(Open()));
  connect(ui.okButton, SIGNAL(clicked(bool)), SLOT(accept()));
  connect(ui.cancelButton, SIGNAL(clicked(bool)), SLOT(reject()));
}
