#include "qplotly.h"
#include <algorithm>
#include <QString>
#include <QStringList>
#include <QWebEngineView>
#include <QWebEngineProfile>
#include <QWebEngineFullScreenRequest>
#include <QUrl>
#include <QFile>
#include <QTextStream>
#include <QVBoxLayout>
#include <QWidget>
#include <QApplication>

#include <QDebug>

void QPlotlyWindow::Demo()
{
  code.clear();
  code += "<html>";
  code +="<head>";
  code +="<!-- Plotly.js -->";
  code +="<script src=\"https://cdn.plot.ly/plotly-latest.min.js\"></script>";
  code +="</head>";
  code +="<body>";
  code +="<div id=\"myDiv\"  style=\"height: 100%;width: 100%;\"><!-- Plotly chart will be drawn inside this DIV --></div>";
  code +="<script>";
  code +="var trace1 = {";
  code +="x: [1, 2, 3, 4],";
  code +="y: [10, 15, 13, 17],";
  code +="mode: 'markers'";
  code +="};";
  code +="var trace2 = {";
  code +="x: [2, 3, 4, 5],";
  code +="y: [16, 5, 11, 10],";
  code +="mode: 'lines'";
  code +="};";
  code +="var trace3 = {";
  code +="x: [1, 2, 3, 4],";
  code +="y: [12, 9, 15, 12],";
  code +="mode: 'lines+markers'";
  code +="};";
  code +="var data = [ trace1, trace2, trace3 ];";
  code +="var layout = {};";
  code +="var d3 = Plotly.d3;";
  code +="var gd3 = d3.select(\"div[id='myDiv']\");";
  code +="var gd = gd3.node();";
  code +="Plotly.newPlot('myDiv', data, layout);";
  code +="window.addEventListener('resize', function() { Plotly.Plots.resize(gd); });";
  code +="</script>";
  code +="</body>";
  code += "</html>";
}

QUrl QPlotlyWindow::WriteTemporaryPage(QString code)
{
  if (dir.isValid()) {
    #ifdef DEBUG
    printf("QPlotlyWindow::WriteTemporaryPage\n");
    qDebug() << "Path is" << dir.path();
    #endif

    dir.setAutoRemove(false);

    QString f_path = QString("%1/index.html").arg(dir.path());
    //qDebug() << f_path;
    QFile f(f_path);
    if(f.open(QIODevice::WriteOnly | QIODevice::Text)){
      QTextStream stream(&f);
      stream << code;
      f.close();
      return QUrl::fromLocalFile(f_path);
    }
    else{
      return QUrl();
    }
  }
  return QUrl();
}

void QPlotlyWindow::setXaxisName(QString xaxisname_)
{
  xaxisname = xaxisname_;
}

void QPlotlyWindow::setYaxisName(QString yaxisname_)
{
  yaxisname = yaxisname_;
}

void QPlotlyWindow::setZaxisName(QString zaxisname_)
{
  zaxisname = zaxisname_;
}

void QPlotlyWindow::setPlotTitle(QString plot_title_)
{
  plot_title = plot_title_;
}

void QPlotlyWindow::addPoint(qreal x, qreal y, QString name)
{
  #ifdef DEBUG
  printf("QPlotlyWindow::addPoint(qreal x, qreal y, QString name)\n");
  #endif
  p.append(new DataPoint(x, y, name));
}

void QPlotlyWindow::addPoint(qreal x, qreal y, QString name, QColor color, int radius)
{
  #ifdef DEBUG
  printf("QPlotlyWindow::addPoint(qreal x, qreal y, QString name, QColor color, int radius)\n");
  #endif
  p.append(new DataPoint(x, y, name));
  p.last()->setColor(color);
  p.last()->setRadius(radius);
}

void QPlotlyWindow::addPoint(qreal x, qreal y, qreal z, QString name)
{
  pt = _3D;
  #ifdef DEBUG
  printf("QPlotlyWindow::addPoint(qreal x, qreal y, QString name)\n");
  #endif
  p.append(new DataPoint(x, y, z, name));
}

void QPlotlyWindow::addPoint(qreal x, qreal y, qreal z, QString name, QColor color, int radius)
{
  pt = _3D;
  #ifdef DEBUG
  printf("QPlotlyWindow::addPoint(qreal x, qreal y, QString name, QColor color, int radius)\n");
  #endif
  p.append(new DataPoint(x, y, z, name));
  p.last()->setColor(color);
  p.last()->setRadius(radius);
}

void QPlotlyWindow::addCurve(QVector< QPointF > curve, QString name, QColor color)
{
  #ifdef DEBUG
  printf("QPlotlyWindow::addCurve\n");
  #endif
  c.append(DataCurve(curve, name, color));
}

void QPlotlyWindow::setCurveStyle(int indx, LTYPE cs){
  if(indx < c.size()){
    c[indx].setStyle(cs);
  }
}

void QPlotlyWindow::addBars(QStringList x, QVector<qreal> y, QStringList text, QColor color)
{
  #ifdef DEBUG
  printf("QPlotlyWindow::addBars(QStringList x, QVector<qreal> y, QStringList text)\n");
  #endif
  xtickangle = -45;
  b.append(new DataBar(x, y, text));
  b.last()->setColor(color);
}

int QPlotlyWindow::PointSize() const
{
  #ifdef DEBUG
  printf("QPlotlyWindow::PointSize\n");
  #endif
  return p.size();
}

DataPoint* QPlotlyWindow::getPoint(int id)
{
  #ifdef DEBUG
  printf("QPlotlyWindow::getPoint\n");
  #endif
  if(id < p.size())
    return p[id];
  else
    return 0;
}

void QPlotlyWindow::RemovePointAt(int id)
{
  #ifdef DEBUG
  printf("QPlotlyWindow::RemovePointAt\n");
  #endif
  if(id < p.size()){
    delete p[id];
    p.remove(id);
  }
}

void QPlotlyWindow::RemovePoint(DataPoint *dp)
{
  #ifdef DEBUG
  printf("QPlotlyWindow::RemovePoint\n");
  #endif
  int indx = p.indexOf(dp);
  if(indx > -1){
    delete p[indx];
    p.remove(indx);
  }
}

void QPlotlyWindow::RemoveAllPoints()
{
  #ifdef DEBUG
  printf("QPlotlyWindow::RemoveAllPoints\n");
  #endif
  for(int i = 0; i < p.size(); i++){
    delete p[i];
  }
  p.clear();
}


QVector< DataCurve > QPlotlyWindow::getCurves()
{
  #ifdef DEBUG
  printf("QPlotlyWindow::getCurves\n");
  #endif
  return c;
}

void QPlotlyWindow::RemoveCurveAt(int cid)
{
  #ifdef DEBUG
  printf("Chart::RemoveCurveAt\n");
  #endif
  c.remove(cid);
}

void QPlotlyWindow::RemoveAllCurves()
{
  #ifdef DEBUG
  printf("Chart::RemoveAllCurves\n");
  #endif
  c.clear();
}

void QPlotlyWindow::Refresh()
{
  // instead of  Plotly.update('graph', data_update) do a replot!
  Plot();
}

void QPlotlyWindow::SelectAll()
{
  selected_points.clear();
  for(int i = 0; i < p.size(); i++){
    p[i]->setSelection(true);
    selected_points << i;
  }
}

void QPlotlyWindow::ClearSelection()
{
  selected_points.clear();
}

void QPlotlyWindow::SaveAsImage(QString imgname)
{
  #ifdef DEBUG
  printf("Chart::SaveAsImage\n");
  #endif
  /*
  QByteArray bytes;
  QBuffer buffer(&bytes);
  buffer.open(QIODevice::WriteOnly);
  pixmap.save(&buffer, "PNG"); // writes pixmap into bytes in PNG format
  */
  /*
  if(imgname.contains(".png", Qt::CaseInsensitive) == true){
    pixmap.save(imgname, "PNG", 100);
  }
  else if(imgname.contains(".jpg", Qt::CaseInsensitive) == true ||
    imgname.contains(".jpeg", Qt::CaseInsensitive) == true){
    pixmap.save(imgname, "JPG", 100);
  }
  else if(imgname.contains(".pdf", Qt::CaseInsensitive) == true){
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(imgname);
    printer.setResolution(300); //override to 300dpi
    printer.setFontEmbeddingEnabled(true);
    printer.setColorMode(QPrinter::Color);
    printer.setOrientation(QPrinter::Portrait);
    printer.setPaperSize(size(), QPrinter::DevicePixel);
    printer.setFullPage(true);
    QPainter painter(&printer);
    painter.setRenderHint(QPainter::Antialiasing, antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing, antialiasing);
    painter.drawPixmap(rect(), pixmap);
  }
  else{
    pixmap.save(imgname+".png");
  }
   QMessageBox::information(this, "Plot Saved!", QString("The plot has been saved as %1").arg(imgname), QMessageBox::Ok);
   */
}

void QPlotlyWindow::close() {}

QString QPlotlyWindow::genJSONScatter()
{
  QString json;
  size_t i;
  QString x = "x: [", y = "y: [", text = "text: [", color = "color: [", msize = "size: [";
  QString xm = "x: [", ym = "y: [", textm = "text: [", colorm = "color: [", msizem = "size: [";

  for(i = 0; i < p.size()-1; i++){
    int r_, g_, b_, a_;
    r_ = p[i]->getColor().red();
    g_ = p[i]->getColor().green();
    b_ = p[i]->getColor().blue();
    a_ = p[i]->getColor().alpha();

    if(p[i]->isVisible() == true){
      if(p[i]->isLabelVisible() == true){
        xm += QString("%1,").arg(p[i]->x());
        ym += QString("%1,").arg(p[i]->y());
        textm += QString("'%1',").arg(p[i]->getName());
        colorm += QString("'rgba(%1, %2, %3, %4)',").arg(r_).arg(g_).arg(b_).arg(a_);
        msizem += QString("%1,").arg(p[i]->radius());
      }
      else{
        x += QString("%1,").arg(p[i]->x());
        y += QString("%1,").arg(p[i]->y());
        text += QString("'%1',").arg(p[i]->getName());
        color += QString("'rgba(%1, %2, %3, %4)',").arg(r_).arg(g_).arg(b_).arg(a_);
        msize += QString("%1,").arg(p[i]->radius());
      }
    }
    else{
      continue;
    }
  }

  int last_id = p.size()-1;

  if(p[last_id]->isVisible() == true){
    int r_, g_, b_, a_;
    r_ = p[last_id]->getColor().red();
    g_ = p[last_id]->getColor().green();
    b_ = p[last_id]->getColor().blue();
    a_ = p[last_id]->getColor().alpha();

    if(p[last_id]->isLabelVisible() == true){
      xm += QString("%1]").arg(p[last_id]->x());
      ym += QString("%1]").arg(p[last_id]->y());
      textm += QString("'%1']").arg(p[last_id]->getName());
      colorm += QString("'rgba(%1, %2, %3, %4)']").arg(r_).arg(g_).arg(b_).arg(a_);
      msizem += QString("%1]").arg(p[i]->radius());
      x += QString("]");
      y += QString("]");
      text += QString("'']");
      color += QString("]");
      msize += QString("]");
    }
    else{
      x += QString("%1]").arg(p[last_id]->x());
      y += QString("%1]").arg(p[last_id]->y());
      text += QString("'%1']").arg(p[last_id]->getName());
      color += QString("'rgba(%1, %2, %3, %4)']").arg(r_).arg(g_).arg(b_).arg(a_);
      msize += QString("%1]").arg(p[last_id]->radius());
      xm += QString("]");
      ym += QString("]");
      textm += QString("'']");
      colorm += QString("]");
      msizem += QString("]");
    }
  }
  else{
    x += QString("]");
    y += QString("]");
    text += QString("'']");
    color += QString("]");
    msize += QString("]");
    xm += QString("]");
    ym += QString("]");
    textm += QString("'']");
    colorm += QString("]");
    msizem += QString("]");
  }

  QString scatter_type = "scatter";
  if(p.size() > 10000){
    scatter_type = "scattergl";
  }

  json += QString("var id%1 = { %2, %3, %4, mode: 'markers', marker: { %5, opacity: 0.9, %6 }, type: '%7' };").arg(trace_id).arg(x).arg(y).arg(text).arg(msize).arg(color).arg(scatter_type);
  trace_id++;
  json += QString("var id%1 = { %2, %3, %4, mode: 'markers+text', textposition: 'top center', textfont: { family: 'Raleway, sans-serif' }, marker: { %5, opacity: 0.9, %6 }, type: '%7' };").arg(trace_id).arg(xm).arg(ym).arg(textm).arg(msizem).arg(colorm).arg(scatter_type);
  trace_id++;
  return json;
}

QString QPlotlyWindow::genJSON3DScatter()
{
  QString json;
  size_t i;
  QString x = "x:[", y = "y:[", z = "z:[", text = "text:[", color = "color:[";

  for(i = 0; i < p.size()-1; i++){
    if(p[i]->isVisible() == true){
      int r_, g_, b_, a_;
      r_ = p[i]->getColor().red();
      g_ = p[i]->getColor().green();
      b_ = p[i]->getColor().blue();
      a_ = p[i]->getColor().alpha();
      if(p[i]->isLabelVisible() == true){
        //json += QString("var id%1 = { x: [%2], y: [%3], text: ['%4'], mode: 'markers+text', textposition: 'top center', textfont: { family: 'Raleway, sans-serif' }, marker: { size: %5, opacity: 0.9, color: 'rgba(%6, %7, %8, %9)' }, type: 'scatter' };").arg(trace_id).arg(p[i]->x()).arg(p[i]->y()).arg(p[i]->getName()).arg(p[i]->radius()).arg(r_).arg(g_).arg(b_).arg(a_);
        //json += QString("var id%1 = { x: [%2], y: [%3], z: [%4] text: ['%5'], mode: 'markers+text', textposition: 'top center', textfont: { family: 'Raleway, sans-serif' }, marker: { size: %6, opacity: 0.9, color: 'rgba(%7, %8, %9, %10)', symbol: 'circle', }, type: 'scatter3d' };").arg(trace_id).arg(p[i]->x()).arg(p[i]->y()).arg(p[i]->z()).arg(p[i]->getName()).arg(p[i]->radius()).arg(r_).arg(g_).arg(b_).arg(a_);
        x+=QString("%1,").arg(p[i]->x());
        y+=QString("%1,").arg(p[i]->y());
        z+=QString("%1,").arg(p[i]->z());
        text+=QString("'%1',").arg(p[i]->getName());
        color+=QString("'rgba(%1, %2, %3, %4)',").arg(r_).arg(g_).arg(b_).arg(a_);
      }
      else{
        //json += QString("var id%1 = { x: [%2], y: [%3], text: ['%4'], mode: 'markers', marker: { size: %5, opacity: 0.9, color: 'rgba(%6, %7, %8, %9)' }, type: 'scatter' };").arg(trace_id).arg(p[i]->x()).arg(p[i]->y()).arg(p[i]->getName()).arg(p[i]->radius()).arg(r_).arg(g_).arg(b_).arg(a_);
        //json += QString("var id%1 = { x: [%2], y: [%3], z: [%4], text: ['%5'], mode: 'markers', marker: { size: %6, opacity: 0.9, color: 'rgba(%7, %8, %9, %10)', symbol: 'circle', }, type: 'scatter3d' };").arg(trace_id).arg(p[i]->x()).arg(p[i]->y()).arg(p[i]->z()).arg(p[i]->getName()).arg(p[i]->radius()).arg(r_).arg(g_).arg(b_).arg(a_);
        x+=QString("%1,").arg(p[i]->x());
        y+=QString("%1,").arg(p[i]->y());
        z+=QString("%1,").arg(p[i]->z());
        text+=QString("'%1',").arg(p[i]->getName());
        color+=QString("'rgba(%1, %2, %3, %4)',").arg(r_).arg(g_).arg(b_).arg(a_);
      }
      //trace_id++;
    }
    else{
      continue;
    }
  }
  int last_id = p.size()-1;
  int r_, g_, b_, a_;
  r_ = p[last_id]->getColor().red();
  g_ = p[last_id]->getColor().green();
  b_ = p[last_id]->getColor().blue();
  a_ = p[last_id]->getColor().alpha();

  x+=QString("%1]").arg(p[last_id]->x());
  y+=QString("%1]").arg(p[last_id]->y());
  z+=QString("%1]").arg(p[last_id]->z());
  text+=QString("'%1']").arg(p[last_id]->getName());
  color+=QString("'rgba(%1, %2, %3, %4)']").arg(r_).arg(g_).arg(b_).arg(a_);

  json += QString("var id%1 = { %2, %3, %4, %5, mode: 'markers', marker: { size: 5, opacity: 0.9, %6 }, type: 'scatter3d' };").arg(trace_id).arg(x).arg(y).arg(z).arg(text).arg(color);
  trace_id++;

  return json;
}

QString QPlotlyWindow::genJSONCurve()
{
  QString json;
  for(int j = 0; j < c.size(); j++){
    json += QString("var id%1 = {").arg(QString::number(trace_id));
    QString x, y, name;
    x += "x: [";
    y += "y: [";
    QVector <QPointF> cp = c[j].getPoints();
    for(int i = 0; i < cp.size()-1; i++){
      x += QString("%1,").arg(cp[i].x());
      y += QString("%1,").arg(cp[i].y());
    }

    x += QString("%1],").arg(cp.last().x());
    y += QString("%1],").arg(cp.last().y());

    json += x;
    json += y;
    if (c[j].getStyle() == L)
      json += "mode: 'lines',";
    else
      json += "mode: 'lines+markers',";

    int r_, g_, b_, a_;
    c[j].color().getRgb(&r_, &g_, &b_, &a_);
    if (c[j].getStyle() == LM)
      json += QString("marker: { size: %1, color: 'rgb(%2, %3, %4, %5)', opacity: 0.8 },").arg(QString::number(c[j].radius())).arg(r_).arg(g_).arg(b_).arg(a_);
    json += QString("line: { color: 'rgb(%1, %2, %3, %4)', width: %5, opacity: 0.8 }").arg(r_).arg(g_).arg(b_).arg(a_).arg(c[j].width());
    json += "};";
    trace_id++;
  }
  return json;
}


QString QPlotlyWindow::genJSONBar()
{
  QString json;
  size_t i, j;
  for(i = 0; i < b.size(); i++){
    if(b[i]->isVisible() == true){
      int r_, g_, b_, a_;
      r_ = b[i]->getColor().red();
      g_ = b[i]->getColor().green();
      b_ = b[i]->getColor().blue();
      a_ = b[i]->getColor().alpha();
      QString x = "[";
      QString y = "[";
      QString text = "[";
      for(j = 0; j < b[i]->x().size()-1; j++){
        x+= QString("'%1',").arg(b[i]->x()[j]);
        y+= QString("%1,").arg(b[i]->y()[j]);
        text+= QString("'%1',").arg(b[i]->getText()[j]);
      }
      int last_id = b[i]->x().size()-1;
      x+= QString("'%1']").arg(b[i]->x()[last_id]);
      y+=QString("%1]").arg(b[i]->y()[last_id]);
      text+=QString("'%1']").arg(b[i]->getText()[last_id]);

      if(b[i]->isTextVisible() == true){
        //json += QString("var id%1 = { x: ['%2'], y: [%3], width: ['%4'], text: ['%5'], mode: 'markers+text', textposition: 'top center', textfont: { family: 'Raleway, sans-serif' }, marker: { opacity: 0.9, color: 'rgba(%6, %7, %8, %9)' }, type: 'bar' };").arg(trace_id).arg(b[i]->x()).arg(b[i]->y()).arg(b[i]->width()).arg(b[i]->getText()).arg(r_).arg(g_).arg(b_).arg(a_);
        json += QString("var id%1 = { x: %2, y: %3, text: %4, mode: 'markers+text', textposition: 'top center', textfont: { family: 'Raleway, sans-serif' }, marker: { opacity: 0.9, color: 'rgba(%5, %6, %7, %8)' }, type: 'bar' };").arg(trace_id).arg(x).arg(y).arg(text).arg(r_).arg(g_).arg(b_).arg(a_);
      }
      else{
        json += QString("var id%1 = { x: %2, y: %3, text: %4, mode: 'markers', marker: { opacity: 0.9, color: 'rgba(%5, %6, %7, %8)' }, type: 'bar' };").arg(trace_id).arg(x).arg(y).arg(text).arg(r_).arg(g_).arg(b_).arg(a_);
      }
      trace_id++;
    }
    else{
      continue;
    }
  }
  return json;
}

void QPlotlyWindow::Plot()
{
  if(p.size() > 0 || c.size() > 0 || b.size() > 0){
    trace_id = 1;
    code.clear();
    code += "<html>";
    code +="<head>";

    code +="<!-- Plotly.js -->";
    QFile file("plotly-latest.min.js");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
      code +="<script src=\"https://cdn.plot.ly/plotly-latest.min.js\"></script>";
    }
    else{
      code +="<script>";
      QByteArray barr = file.readAll();
      file.close();
      code += QString(barr);
      code +="</script>";
    }
    // insert a script to read/write txt files
    code +="</head>";
    code +="<body>";

    //code +="<p id='selid' hidden></p>";
    code +="<div id=\"myDiv\"  style=\"height: 100%;width: 100%;\"><!-- Plotly chart will be drawn inside this DIV --></div>";
    code +="<script>";

    if(b.size() == 0){
      if(p.size() > 0){
        if(pt == _2D){
          // Prepare code for scatter plot
          code += genJSONScatter();
        }
        else{
          code += genJSON3DScatter();
        }
      }

      if(c.size() > 0 && pt == _2D){
        // Prepare code for curve plot)
        code += genJSONCurve();
      }
    }
    else{
      // Prepare code for bar plot
      code += genJSONBar();
    }

    code +="var data = [";
    for(int i = 1; i < trace_id-1; i++)
      code += QString(" id%1,").arg(i);
    code += QString(" id%1];").arg(trace_id-1);


    //code +="var layout = {};";
    if(pt == _3D){
      code += QString("var layout = { showlegend: false, title: '%1', xaxis: { title: '%2'}, yaxis: { title: '%3'}, zaxis: { title: '%4'}, margin: {l: 0, r: 0, b:0, t: 0}};").arg(plot_title).arg(xaxisname).arg(yaxisname).arg(zaxisname);
    }
    else{
      code += QString("var layout = { showlegend: false, title: '%1', xaxis: { title: '%2', tickangle: '%3'}, yaxis: { title: '%4', tickangle: '%5'}, barmode: 'group' };").arg(plot_title).arg(xaxisname).arg(xtickangle).arg(yaxisname).arg(ytickangle);
    }

    code +="var d3 = Plotly.d3;";
    code +="var gd3 = d3.select(\"div[id='myDiv']\");";
    code +="var gd = gd3.node();";
    code +="Plotly.newPlot('myDiv', data, layout);";

    // Add plot interactions
    code += "var myPlot = document.getElementById('myDiv');";
    code += "myPlot.on('plotly_click', function() {";
    //code += "alert('You clicked this Plotly chart!');";
    code += "});";
    //Point selection. all selected points will be sent to cookie. Then
    code += "myPlot.on('plotly_selected', function(eventData) {";
    //code += "alert('You selected some points in this Plotly chart!');";
    code += "if (eventData.points.length > 0){";
    code += "document.cookie = 'selpnt=start';";
    code += "eventData.points.forEach(function(pt) {";
    //code += "console.log(pt);";
    code += "document.cookie = 'point=x: '+pt.x+' y: '+pt.y +' z: '+pt.z + ' name: '+ pt.text;";
    //code += "alert('x: '+pt.x+' y: '+pt.y + ' name: '+ pt.text);";
      //x.push(pt.x);
      //y.push(pt.y);
      //code += "colors[pt.pointNumber] = color1;";
    code += "});";
    code += "document.cookie = 'selpnt=end';";
    //code += "return points;";
    code += "}";
    code += "else { return undefined; }";
    code += "});";

    // Add resize event
    code +="window.addEventListener('resize', function() { Plotly.Plots.resize(gd); });";
    code +="</script>";
    code +="</body>";
    code += "</html>";
  }

  /*if(wview != NULL){
    delete wview;
  }

  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  QVBoxLayout *mainLayout = new QVBoxLayout;
  wview = new QWebEngineView(this);
  if(code.size() > 0){
    QUrl page = WriteTemporaryPage(code);
    wview->load(page);
    //wview->load(QUrl(QStringLiteral("https://www.qt.io")));//
    this->setMinimumSize(this->size());
    wview->resize(this->size());
    //wview->page()->profile()->setPersistentCookiesPolicy(QWebEngineProfile::ForcePersistentCookies);
    mainLayout->addWidget(wview);
    this->setLayout(mainLayout);

    cookie_store = wview->page()->profile()->cookieStore();
    connect(cookie_store, &QWebEngineCookieStore::cookieAdded, this, &QPlotlyWindow::handleCookieAdded);
  }
  */
  if(code.size() > 0){
    QUrl page = WriteTemporaryPage(code);
    wview->load(page);
    //wview->load(QUrl(QStringLiteral("https://www.qt.io")));//
  }
}

int QPlotlyWindow::FindPoint(qreal x, qreal y, qreal z, QString name_)
{
  DataPoint f(x, y, z, name_);
  //return p.indexOf(&f);
  for(int i = 0; i < p.size(); i++){
    if(p[i]->compare(f) == true){
      return i;
    }
    else{
      continue;
    }
  }
  return -1;
  /*auto it = std::find(p.begin(), p.end(), &f);
  if (it == p.end()){
    return -1;
  }
  else{
    return (int)std::distance(p.begin(), it);
  }*/
}

void QPlotlyWindow::handleCookieAdded(const QNetworkCookie &cookie)
{
  QString cookie_(cookie.toRawForm());
  #ifdef DEBUG
  qDebug() << cookie_ << "\n";
  #endif

  if(cookie_.contains("selpnt=start") == true){
    for(int i = 0; i < selected_points.size(); i++){
      p[selected_points[i]]->setSelection(false);
    }
    selected_points.clear();
  }
  else if(cookie_.contains("point=") == true){
    #ifdef DEBUG
    qDebug() << cookie_.split(";");
    #endif
    QStringList vars = cookie_.split(";")[0].split(" ");
    //point=x: 2 y: 2 z: 2 name: due; path=/var/folders/1z/j2xq6qss7s93bmf23nzcskp00000gp/T/testqplotly-YlDhEE"
    qreal x = 0.f, y = 0.f, z = 0.f;
    x = atof(vars[1].toStdString().c_str());
    y = atof(vars[3].toStdString().c_str());
    if(vars[5].compare("undefined") != 0)
      z = atof(vars[5].toStdString().c_str());
    QString name = vars[7];
    int id = FindPoint(x, y, z, name);
    if(id > -1){
      selected_points << id;
      p[id]->setSelection(true);
      //qDebug() << "id Nr. " << selected_points.last();
    }
    else{
      return;
    }
  }
  else if(cookie_.contains("selpnt=end") == true){
    /* Selection END!
     for(int i = 0; i < selected_points.size(); i++){
      qDebug() << selected_points[i] << "\n";
    }*/
    return;
  }
  return;
}

QPlotlyWindow::QPlotlyWindow(QWidget *parent) : QWidget(parent)
{
  pt = _2D;
  xaxisname = "X axis";
  yaxisname = "Y axis";
  zaxisname = "Z axis";
  plot_title = "Title";
  trace_id = 1;
  xtickangle = ytickangle = 0;

  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  QVBoxLayout *mainLayout = new QVBoxLayout;
  wview = new QWebEngineView(this);
  this->setMinimumSize(this->size());
  wview->resize(this->size());
  //wview->page()->profile()->setPersistentCookiesPolicy(QWebEngineProfile::ForcePersistentCookies);
  mainLayout->addWidget(wview);
  this->setLayout(mainLayout);
  cookie_store = wview->page()->profile()->cookieStore();
  connect(cookie_store, &QWebEngineCookieStore::cookieAdded, this, &QPlotlyWindow::handleCookieAdded);

}

QPlotlyWindow::~QPlotlyWindow()
{
  // Free the memory!
  dir.remove();
  delete wview;
  for(int i = 0; i < p.size(); i++){
    delete p[i];
  }
  for(int i = 0; i < b.size(); i++){
    delete b[i];
  }
}
