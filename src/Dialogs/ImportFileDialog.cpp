#include "ImportFileDialog.h"
#include "qstudiometricstypes.h"

#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QtCore/qvarlengtharray.h>
#include <QString>
#include <QTextStream>
#include <QtConcurrent/QtConcurrentRun>
#include <QFutureWatcher>
#include <QtConcurrent>

void ImportFileDialog::AssignName(QStringList &list, QString name)
{
  /*
   * Fast check if duplicate names in list!
   * Test resutls: works fast with 10.000.000 names!
   */
  try{
    int indx = rnames[name];
    if(indx == 0){
      list.append(name);
      rnames[name] = 1;
    }
    else{
      list.append(QString("%1_%2").arg(name).arg(QString::number(indx+1)));
      rnames[name]= indx+1;
    }
  }
  catch(...){
    list.append(name);
    rnames[name] = 1;
  }
}

int ImportFileDialog::getHeader(QStringList* header)
{
  QFileInfo info(ui.file->text());
  if(info.exists()){
    QFile f(ui.file->text());
    f.open(QIODevice::ReadOnly | QIODevice::Text);
    QString skipchar = getSkipChar();
    QString sep = getSeparator();
    QTextStream in(&f);
    size_t i = 0;
    while(!in.atEnd()){
      QString line = in.readLine();
      if(QString(line[0]).compare(skipchar, Qt::CaseInsensitive) == 0 || line.isEmpty()){ // skip line empty or starting with a skip char
        i++;
      }
      else{
        (*header) = line.split(sep);
        break;
      }
    }
    f.close();
    return i;
  }
  return 0;
}

QList<int> ImportFileDialog::getLineToSkip()
{
  QList<int> lskip;
  if(ui.ignore_lines_start_char->isChecked()){
    QFileInfo info(ui.file->text());
    if(info.exists()){
      QFile f(ui.file->text());
      f.open(QIODevice::ReadOnly | QIODevice::Text);
      QTextStream in(&f);

      QString skipchar = getSkipChar();
      size_t row = 0;
      std::string line;
      while(!in.atEnd()){
        QString line = in.readLine();
        if(QString(line).compare(skipchar, Qt::CaseInsensitive) == 0 || line.isEmpty()){ // skip line empty or starting with a skip char
          lskip.append(row);
          row++;
        }
        else{
          continue;
        }
      }
      f.close();
    }
    return lskip;
  }
  else{
    return lskip;
  }
}

//if(ui.firstrowvarname->isChecked() == true && ui.firstcolobjname->isChecked() == true)
void ImportFileDialog::ImportType0()
{
  QFileInfo info(ui.file->text());
  if(info.exists()){
    // get line to skip
    QList<int> lskip = getLineToSkip();
    // get separator
    QString sep = getSeparator();
    // Assign header to matrix
    Clean_rnames();
    AssignName(m->getVarName(), "Object Names");
    QStringList header;
    int header_line = getHeader(&header);
    for(int j = 1; j < header.size(); j++){
      AssignName(m->getVarName(), header[j]);
    }

    //add header line to the lines to skip
    lskip << header_line;
    // open the file
    Clean_rnames();
    QFile f(ui.file->text());
    f.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&f);

    size_t lnum = 0;
    size_t row = 0;
    while(!in.atEnd()){
      QString line = in.readLine();
      if(lskip.indexOf(lnum) > -1){
        lnum++;
      }
      else{
        // import data into the matrix
        QStringList items = line.split(sep);
        AssignName(m->getObjName(), items[0]);
        for(int j = 1; j < items.size(); j++){
          bool converted;
          double val = items[j].replace(",",".").toDouble(&converted);
          m->Matrix()->data[row][j-1] = (converted == true) ? val : DEFAULT_EMTPY_VALUE;
        }
        row++;
      }
      lnum++;
    }
    f.close();
  }
}

// if(ui.firstrowvarname->isChecked() == true && ui.firstcolobjname->isChecked() == false)
void ImportFileDialog::ImportType1()
{
  QFileInfo info(ui.file->text());
  if(info.exists()){

    // get line to skip
    QList<int> lskip = getLineToSkip();
    // get separator
    QString sep = getSeparator();
    // Assign header to matrix
    Clean_rnames();
    AssignName(m->getVarName(), "Object Names");
    QStringList header;
    int header_line = getHeader(&header);
    for(int j = 0; j < header.size(); j++){
      AssignName(m->getVarName(), header[j]);
    }
    //add header line to the lines to skip
    lskip << header_line;
    // open the file
    Clean_rnames();

    QFile f(ui.file->text());
    if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
      return;

    QTextStream in(&f);
    size_t lnum = 0;
    size_t row = 0;
    while(!in.atEnd()){
      QString line = in.readLine();
      if(lskip.indexOf(lnum) > -1){
        lnum++;
      }
      else{
        QStringList items = line.split(sep);
        for(int j = 0; j < items.size(); j++){
          bool converted;
          double val = items[j].replace(",",".").toDouble(&converted);
          m->Matrix()->data[row][j] = (converted == true) ? val : DEFAULT_EMTPY_VALUE;
        }
        row++;
      }
    }
    f.close();
  }
}

//else if(ui.firstrowvarname->isChecked() == false && ui.firstcolobjname->isChecked() == true){
void ImportFileDialog::ImportType2()
{
  QFileInfo info(ui.file->text());
  if(info.exists()){
    // get line to skip
    QList<int> lskip = getLineToSkip();
    // get separator
    QString sep = getSeparator();

    QFile f(ui.file->text());
    if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
      return;

    Clean_rnames();
    AssignName(m->getVarName(), "Object Names");
    size_t lnum = 0;
    size_t row = 0;
    QTextStream in(&f);
    // open the file
    Clean_rnames();
    while(!in.atEnd()){
      QString line = in.readLine();
      if(lskip.indexOf(lnum) > -1){
        lnum++;
      }
      else{
        QStringList items = line.split(sep);
        AssignName(m->getObjName(), items[0]);
        for(int j = 1; j < items.size(); j++){
          bool converted;
          double val = items[j].replace(",",".").toDouble(&converted);
          m->Matrix()->data[row][j-1] = (converted == true) ? val : DEFAULT_EMTPY_VALUE;
        }
        row++;
      }
    }
    f.close();
  }
}

// ui.firstrowvarname->isChecked() == false && ui.firstcolobjname->isChecked() == false
void ImportFileDialog::ImportType3()
{
  QFileInfo info(ui.file->text());
  if(info.exists()){
    // get line to skip
    QList<int> lskip = getLineToSkip();
    // get separator
    QString sep = getSeparator();

    QFile f(ui.file->text());
    if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
      return;

    Clean_rnames();
    AssignName(m->getVarName(), "Object Names");
    size_t row = 0;
    size_t lnum = 0;
    QTextStream in(&f);
    // open the file
    Clean_rnames();
    while(!in.atEnd()){
      QString line = in.readLine();
      if(lskip.indexOf(lnum) > -1){
        lnum++;
      }
      else{
        QStringList items = line.split(sep);
        for(int j = 0; j < items.size(); j++){
          bool converted;
          double val = items[j].replace(",",".").toDouble(&converted);
          m->Matrix()->data[row][j] = (converted == true) ? val : DEFAULT_EMTPY_VALUE;
        }
        row++;
      }
    }
    f.close();
  }
}

void ImportFileDialog::BuildMatrix()
{
  // Get the file size and build the empty matrix
  FSIZE sz = GetSize();
  ResizeMatrix(&m->Matrix(), sz.row, sz.col);
  // Assign the matrix name
  m->setName(ui.filename->text());

  // Import the matrix
  if(ui.firstrowvarname->isChecked() == true && ui.firstcolobjname->isChecked() == true){
    QFuture<void> future = QtConcurrent::run([this]{ ImportFileDialog::ImportType0(); });
    //QFuture<void> future = QtConcurrent::run(this, &ImportFileDialog::ImportType0);
    future.waitForFinished();
    //ImportType0();
  }
  else if(ui.firstrowvarname->isChecked() == true && ui.firstcolobjname->isChecked() == false){
    QFuture<void> future = QtConcurrent::run([this]{ ImportFileDialog::ImportType1(); });
    //QFuture<void> future = QtConcurrent::run(this, &ImportFileDialog::ImportType1);
    future.waitForFinished();
    //ImportType1();
  }
  else if(ui.firstrowvarname->isChecked() == false && ui.firstcolobjname->isChecked() == true){
    QFuture<void> future = QtConcurrent::run([this]{ ImportFileDialog::ImportType2(); });
    //QFuture<void> future = QtConcurrent::run(this, &ImportFileDialog::ImportType2);
    future.waitForFinished();
    //ImportType2();
  }
  else{ //ui.firstrowvarname->isChecked() == false && ui.firstcolobjname->isChecked() == false
    QFuture<void> future = QtConcurrent::run([this] { ImportFileDialog::ImportType3(); });
    //QFuture<void> future = QtConcurrent::run(this, &ImportFileDialog::ImportType3);
    future.waitForFinished();
    //ImportType3();
  }

  // verify the variable names
  if(m->getVarName().size() == 1){
    for(size_t i = 0; i < m->Matrix()->col; i++)
      m->getVarName().append(QString("Var %1").arg(i+1));
  }

  // verify the object names
  if(m->getObjName().size() == 0){
    for(size_t i = 0; i < m->Matrix()->row; i++)
      m->getObjName().append(QString("Objects %1").arg(i+1));
  }

  // Do some other operation
  if(ui.transpose->isChecked()){
    matrix *mt;
    NewMatrix(&mt, m->Matrix()->col, m->Matrix()->row);
    MatrixTranspose(m->Matrix(), mt);
    MatrixCopy(mt, &m->Matrix());
    DelMatrix(&mt);
    QStringList tmp = m->getObjName();
    m->getVarName().removeFirst();
    m->getObjName() = m->getVarName();
    m->getVarName().clear();
    m->getVarName().append("Object Names");
    m->getVarName().append(tmp);
  }

  if(ui.fix_empty_values->isChecked()){
    // Check if there are nan value and substitute with the average.
    for(size_t j = 0; j < m->Matrix()->col; j++){
      double mean = 0.f;
      int empty = 0;
      for(int i = 0; i < (int)m->Matrix()->row; i++){
        if(FLOAT_EQ(getMatrixValue(m->Matrix(), i, j), DEFAULT_EMTPY_VALUE, EPSILON)){
          empty++;
        }
        else{
          mean += getMatrixValue(m->Matrix(), i, j);
        }
      }

      mean /= (m->Matrix()->row - empty);

      if(_isnan_(mean))
        mean = 0.f;

      for(int i = 0; i < (int)m->Matrix()->row; i++){
        if(FLOAT_EQ(getMatrixValue(m->Matrix(), i, j), DEFAULT_EMTPY_VALUE, EPSILON)){
          setMatrixValue(m->Matrix(), i, j, mean);
        }
        else{
          continue;
        }
      }
    }
  }
}

void ImportFileDialog::Preview()
{
  QList < QStringList > fpreview;

  QFile f(ui.file->text());
  if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
    return;

  QStringList objnames, varnames;
  varnames << "Object Names";

  size_t row = 0, max_row = 10;
  QTextStream in(&f);
  while(!in.atEnd()){

    if(row == max_row){
      break;
    }

    QString line = in.readLine();
    /*
     * Preview should work for any text file and should
     * allow to visualize anykind of text, also the ones not 
     * usefull for this software. These lines fix an unespected
     * application crash while opening any txt/ascii file.
     */
    if(line.isEmpty() == true){
      continue;
    }
    else{
      if(QString(line[0]).compare(getSkipChar(), Qt::CaseInsensitive) == 0 || line.isEmpty()){ // skip line starting with
        continue;
      }
      else{
        QStringList items = line.split(getSeparator());
        if(row == 0){
          if(ui.firstrowvarname->isChecked() == true){
            if(ui.firstcolobjname->isChecked() == true){
              items.removeFirst();
            }
            for(int i = 0; i < items.size(); i++){
              AssignName(varnames, items[i]);
            }
          }
          else{
            if(ui.firstcolobjname->isChecked() == true){
              AssignName(objnames, items.first());
              fpreview.append(QStringList());
              for(int j = 1; j < items.size(); j++){
                fpreview.last().append(items[j]);
              }
            }
            else{
              fpreview.append(QStringList());
              for(int j = 0; j < items.size(); j++){
                fpreview.last().append(items[j]);
              }
            }
          }
        }
        else{
          if(ui.firstcolobjname->isChecked() == true){
            AssignName(objnames, items.first());
            fpreview.append(QStringList());
            for(int j = 1; j < items.size(); j++){
              fpreview.last().append(items[j]);
            }
          }
          else{
            fpreview.append(QStringList());
            for(int j = 0; j < items.size(); j++){
              fpreview.last().append(items[j]);
            }
          }
        }
        row++;
      }
    }
  }
  f.close();

  if(varnames.size() == 1 && fpreview.size() > 0){
    for(int i = 0; i < fpreview.first().size(); i++)
      varnames.append(QString("Var %1").arg(i+1));
  }

  if(objnames.size() == 0){
    for(int i = 0; i < fpreview.size(); i++)
      objnames.append(QString("Objects %1").arg(i+1));
  }

  if(ui.transpose->isChecked() && fpreview.size() > 0){
    QList<QStringList> fpreview_t;
    // deep copy
    for(int j = 0; j < fpreview.first().size(); j++){
      fpreview_t.append(QStringList());
      for(int i = 0; i < fpreview.size(); i++){
        if(j < fpreview[i].size()){
          fpreview_t.last().append(fpreview[i][j]);
        }
        else{
          continue;
        }
      }
    }
    fpreview.clear();
    fpreview = fpreview_t;

    QStringList tmp = objnames;
    varnames.removeFirst();
    objnames = varnames;
    varnames.clear();
    varnames.append("Object Names");
    varnames.append(tmp);
  }

  model->clear();

  model->setHorizontalHeaderLabels(varnames);
  for(int i = 0; i < fpreview.size(); i++){
    QList<QStandardItem*> row;
    row.append(new QStandardItem(objnames[i]));
    for(int j = 0; j < fpreview[i].size(); j++){
      row.append(new QStandardItem(fpreview[i][j]));
    }
    model->appendRow(row);
  }

  if(row == max_row && fpreview.size() > 0){
    QList<QStandardItem*> row;
    row.append(new QStandardItem("...."));
    for(int j = 0; j < fpreview.last().size(); j++){
      row.append(new QStandardItem("...."));
    }
  }
}

FSIZE ImportFileDialog::GetSize()
{
  FSIZE sz;
  sz.row = sz.col = sz.linelenght = 0;
  QFileInfo info(ui.file->text());
  if(info.exists()){

    QFile f(ui.file->text());
    if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
      return sz;

    QList<int> lskip = getLineToSkip();
    QString sep = getSeparator();

    QTextStream in(&f);
    if(lskip.size() > 0){
      size_t lnum = 0;
      while(!in.atEnd()){
        QString line = in.readLine();
        if(lskip.indexOf(lnum) > -1){
          lnum++;
        }
        else{
          sz.row++;
          size_t col = line.split(sep).size();
          size_t linelenght = line.size();
          if(col > sz.col)
            sz.col = col;

          if(linelenght+3 > sz.linelenght)
            sz.linelenght = linelenght+3;
          lnum++;
        }
      }
    }
    else{
      while(!in.atEnd()){
        QString line = in.readLine();
        sz.row++;
        size_t col = line.split(sep).size();
        size_t linelenght = line.size();
        if(col > sz.col)
          sz.col = col;

        if(linelenght+3 > sz.linelenght)
          sz.linelenght = linelenght+3;
      }
    }
    f.close();
  }

  if(ui.firstcolobjname->isChecked() == true)
    sz.col -= 1;

  if(ui.firstrowvarname->isChecked() == true)
    sz.row -= 1;

  return sz;
}

QString ImportFileDialog::getFileName()
{
  return ui.file->text();
}

QString ImportFileDialog::getLabel()
{
  if(ui.filename->text().compare("project") != 0) // project name is a reserved name in order to recognize a speicific kind of data in the qtreeview
    return ui.filename->text();
  else
    return ui.filename->text().append("_");
}

QString ImportFileDialog::getSeparator()
{
  if(ui.splitlineby->currentIndex() == 0) // tab
    return "\t";
  else if(ui.splitlineby->currentIndex() == 1) // space
    return " ";
  else if(ui.splitlineby->currentIndex() == 2) // ,
    return ",";
  else if(ui.splitlineby->currentIndex() == 3) // ;
    return ";";
  else{ // PERSONAL
    return ui.splitlineby->currentText();
  }
}

QString ImportFileDialog::getSkipChar()
{
  return ui.ignorelinebychar->text();
}

void ImportFileDialog::Cancel()
{
  return reject();
}

void ImportFileDialog::OK()
{
  if(!ui.file->text().isEmpty()){
    QFileInfo info(ui.file->text());
    if(info.exists() == true){
      /*QFutureWatcher<void> futureWatcher;
      connect(&futureWatcher, SIGNAL(finished()), this,SLOT(accept()));
      futureWatcher.setFuture(QtConcurrent::run(this, &ImportFileDialog::BuildMatrix));
      futureWatcher.waitForFinished();*/

      BuildMatrix();
      accept();
    }
    else{
      QMessageBox::warning(this, tr("Warnig"), tr("The selected file could not be used because the file does not exist. Please select a file.."), QMessageBox::Ok);
      return;
    }
  }
  else{
    QMessageBox::warning(this, tr("Warnig"), tr("Please select a file.."), QMessageBox::Ok);
  }
}

void ImportFileDialog::Open()
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

  Clean_rnames();
  Preview();
}

void ImportFileDialog::Clean_rnames()
{
  //qDeleteAll(rnames.begin(), rnames.end());
  rnames.clear();
}

ImportFileDialog::ImportFileDialog(): QDialog()
{
  ui.setupUi(this);

  m = new MATRIX;
  a = new ARRAY;

  model = new QStandardItemModel;
  ui.tableView->setModel(model);

  connect(ui.okButton, SIGNAL(clicked(bool)), SLOT(OK()));
  connect(ui.cancelButton, SIGNAL(clicked(bool)), SLOT(reject()));
  connect(ui.openButton, SIGNAL(clicked(bool)), this, SLOT(Open()));
  connect(ui.firstcolobjname, SIGNAL(clicked(bool)), SLOT(Preview()));
  connect(ui.firstrowvarname, SIGNAL(clicked(bool)), SLOT(Preview()));
  connect(ui.transpose, SIGNAL(clicked(bool)), SLOT(Preview()));
  connect(ui.splitlineby, SIGNAL(currentIndexChanged(int)), SLOT(Preview()));
  connect(ui.ignorelinebychar, SIGNAL(textChanged(QString)), SLOT(Preview()));
}

ImportFileDialog::~ImportFileDialog()
{
  delete m;
  delete a;
  delete model;
}
