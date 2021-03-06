#include "qplotly.h"

#include <algorithm>

#include <QApplication>
#include <QFile>
#include <QString>
#include <QStringList>
#include <QTextStream>
#include <QThread>
#include <QtConcurrent/QtConcurrent>
#include <QUrl>
#include <QVBoxLayout>
#include <QWidget>

#ifdef WEBKIT
#include <QWebView>
#include <QSettings>
#else
#include <QWebEngineView>
#include <QWebEngineProfile>
#include <QWebEngineFullScreenRequest>
#endif


#include <QDebug>

#ifdef WEBKIT
CustomNetworkCookieJar::CustomNetworkCookieJar( QObject * p_pParent )
    : QNetworkCookieJar( p_pParent )
{
}

CustomNetworkCookieJar::~CustomNetworkCookieJar()
{
}

bool CustomNetworkCookieJar::setCookiesFromUrl(const QList<QNetworkCookie> & p_grCookieList, const QUrl &p_grUrl)
{
    for(int i = 0; i < p_grCookieList.size(); i++){
       QString cookie = QString("%1=%2").arg(QString(p_grCookieList[i].name())).arg(QString(p_grCookieList[i].value()));
       emit cookiesChanged(cookie);
    }
    return true;
}
#endif


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
    //#ifdef DEBUG
    //printf("QPlotlyWindow::WriteTemporaryPage\n");
    qDebug() << "Path is" << dir.path();
    //#endif

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

void QPlotlyWindow::ModifyTemporaryPage(QString code, QList<int> line_ids)
{
  QString f_path = QString("%1/index.html").arg(dir.path());
  QFile f(f_path);
  for(int i = 0; i < line_ids.size(); i++){
    return;
  }
  return;
}

void QPlotlyWindow::setAxisValueSize(int size)
{
  return;
}

void QPlotlyWindow::setXLabelSize(int size)
{
  return;
}

void QPlotlyWindow::setXminXmaxXTick(double xmin, double xmax, int xtick)
{
  return;
}

void QPlotlyWindow::getXminXmaxXTick(double *xmin, double *xmax, int *xtick)
{
  return;
}

void QPlotlyWindow::setYLabelSize(int size)
{
  return;
}

void QPlotlyWindow::setYminYmaxYTick(double ymin, double ymax, int ytick)
{
  return;
}

void QPlotlyWindow::getYminYmaxYTick(double *ymin, double *ymax, int *ytick)
{
  return;
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

void QPlotlyWindow::setPlotTitleSize(int size)
{
  return;
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
  printf("QPlotlyWindow::RemoveCurveAt\n");
  #endif
  c.remove(cid);
}

void QPlotlyWindow::RemoveAllCurves()
{
  #ifdef DEBUG
  printf("QPlotlyWindow::RemoveAllCurves\n");
  #endif
  c.clear();
}

void QPlotlyWindow::Refresh()
{
  //printf("Refresh\n");
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
  #ifdef DEBUG
  printf("QPlotlyWindow::ClearSelection");
  #endif
  for(int i = 0; i < selected_points.size(); i++){
    p[selected_points[i]]->setSelection(false);
  }
  selected_points.clear();
}

void QPlotlyWindow::SaveAsImage(QString imgname)
{
  #ifdef DEBUG
  printf("QPlotlyWindow::SaveAsImage\n");
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

QStringList QPlotlyWindow::JSonScatterWorker(int from, int to)
{
  //qDebug() << "Run from " << from << " to " << to;
  //Simple visible point
  QString x = "", y = "", text = "", color = "", msize = "", msymbol = "";
  //Simple point marked with label
  QString xm = "", ym = "", textm = "", colorm = "", msizem = "", msymbolm = "";
  //Simple point selected
  QString xs = "", ys = "", texts = "", colors = "", msizes = "", msymbols = "";

  if(to > p.size()-1)
    to = p.size()-1;

  for(int i = from; i < to; i++){
    int r_, g_, b_, a_;
    r_ = p[i]->getColor().red();
    g_ = p[i]->getColor().green();
    b_ = p[i]->getColor().blue();
    a_ = p[i]->getColor().alpha();

    MarkerType mtype = p[i]->marker();

    if(p[i]->isVisible() == true){
      if(p[i]->isLabelVisible() == true){
        xm += QString("%1,").arg(p[i]->x());
        ym += QString("%1,").arg(p[i]->y());
        textm += QString("'%1',").arg(p[i]->getName());
        colorm += QString("'rgba(%1, %2, %3, %4)',").arg(r_).arg(g_).arg(b_).arg(a_);
        msizem += QString("%1,").arg(p[i]->radius());
        if(mtype == CIRCLE){
          msymbolm += QString("'circle',");
        }
        else if(mtype == SQUARE){
          msymbolm += QString("'square',");
        }
        else{
          msymbolm += QString("'triangle-down',");
        }
      }
      else{
        if(p[i]->isSelected() == true || selected_points.contains(i) == true){
          xs += QString("%1,").arg(p[i]->x());
          ys += QString("%1,").arg(p[i]->y());
          texts += QString("'%1',").arg(p[i]->getName());
          colors += QString("'rgba(%1, %2, %3, %4)',").arg(r_).arg(g_).arg(b_).arg(a_);
          msizes += QString("%1,").arg(p[i]->radius());
          if(mtype == CIRCLE){
            msymbols += QString("'circle',");
          }
          else if(mtype == SQUARE){
            msymbols += QString("'square',");
          }
          else{
            msymbols += QString("'triangle-down',");
          }
        }
        else{
          x += QString("%1,").arg(p[i]->x());
          y += QString("%1,").arg(p[i]->y());
          text += QString("'%1',").arg(p[i]->getName());
          color += QString("'rgba(%1, %2, %3, %4)',").arg(r_).arg(g_).arg(b_).arg(a_);
          msize += QString("%1,").arg(p[i]->radius());
          if(mtype == CIRCLE){
            msymbol += QString("'circle',");
          }
          else if(mtype == SQUARE){
            msymbol += QString("'square',");
          }
          else{
            msymbol += QString("'triangle-down',");
          }
        }
      }
    }
    else{
      continue;
    }
  }

  QStringList rval;
  rval.append(x);
  rval.append(y);
  rval.append(text);
  rval.append(color);
  rval.append(msize);
  rval.append(msymbol);

  rval.append(xm);
  rval.append(ym);
  rval.append(textm);
  rval.append(colorm);
  rval.append(msizem);
  rval.append(msymbolm);

  rval.append(xs);
  rval.append(ys);
  rval.append(texts);
  rval.append(colors);
  rval.append(msizes);
  rval.append(msymbols);
  return rval;
}

QString QPlotlyWindow::genJSONScatter()
{
  QString json;
  //Simple visible point
  QString x = "x: [", y = "y: [", text = "text: [", color = "color: [", msize = "size: [", msymbol = "symbol: [";
  //Simple point marked with label
  QString xm = "x: [", ym = "y: [", textm = "text: [", colorm = "color: [", msizem = "size: [", msymbolm = "symbol: [";
  //Simple point selected
  QString xs = "x: [", ys = "y: [", texts = "text: [", colors = "color: [", msizes = "size: [", msymbols = "symbol: [";

  int nth = QThread::idealThreadCount();
  int step = (int)ceil((p.size()-1)/(double)nth);

  if(step == 0){
    nth = 1;
    step = p.size()-1;
  }

  QList<QFuture<QStringList>> futures;
  int from = 0, to = step;
  for(int i = 0; i < nth; i++){
    futures.append(QtConcurrent::run(this, &QPlotlyWindow::JSonScatterWorker, from, to));
    from = to;
    if(from+step > p.size()-1){
      to = p.size()-1;
    }
    else{
      to+=step;
    }
  }


  for(int i = 0; i < futures.size(); i++){
    QStringList r = futures[i].result();
    x += r[0];
    y += r[1];
    text += r[2];
    color += r[3];
    msize += r[4];
    msymbol += r[5];
    xm += r[6];
    ym += r[7];
    textm += r[8];
    colorm += r[9];
    msizem += r[10];
    msymbolm += r[11];
    xs += r[12];
    ys += r[13];
    texts += r[14];
    colors += r[15];
    msizes += r[16];
    msymbols += r[17];
  }

  /* SINGLE THREAD IMPLEMENTATION
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
        if(p[i]->isSelected() == true){
          xs += QString("%1,").arg(p[i]->x());
          ys += QString("%1,").arg(p[i]->y());
          texts += QString("'%1',").arg(p[i]->getName());
          colors += QString("'rgba(%1, %2, %3, %4)',").arg(r_).arg(g_).arg(b_).arg(a_);
          msizes += QString("%1,").arg(p[i]->radius());
        }
        else{
          x += QString("%1,").arg(p[i]->x());
          y += QString("%1,").arg(p[i]->y());
          text += QString("'%1',").arg(p[i]->getName());
          color += QString("'rgba(%1, %2, %3, %4)',").arg(r_).arg(g_).arg(b_).arg(a_);
          msize += QString("%1,").arg(p[i]->radius());
        }
      }
    }
    else{
      continue;
    }
  }*/

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
      msizem += QString("%1]").arg(p[last_id]->radius());

      if(p[last_id]->marker() == CIRCLE){
        msymbolm += QString("'circle']");
      }
      else if(p[last_id]->marker() == SQUARE){
        msymbolm += QString("'square']");
      }
      else{
        msymbolm += QString("'triangle-down']");
      }

      x += QString("]");
      y += QString("]");
      text += QString("'']");
      color += QString("]");
      msize += QString("]");
      msymbol += QString("]");
      xs += QString("]");
      ys += QString("]");
      texts += QString("'']");
      colors += QString("]");
      msizes += QString("]");
      msymbols += QString("]");
    }
    else{
      if(p[last_id]->isSelected() == true || selected_points.contains(last_id) == true){
        xs += QString("%1]").arg(p[last_id]->x());
        ys += QString("%1]").arg(p[last_id]->y());
        texts += QString("'%1']").arg(p[last_id]->getName());
        colors += QString("'rgba(%1, %2, %3, %4)']").arg(r_).arg(g_).arg(b_).arg(a_);
        msizes += QString("%1]").arg(p[last_id]->radius());
        if(p[last_id]->marker() == CIRCLE){
          msymbols += QString("'circle']");
        }
        else if(p[last_id]->marker() == SQUARE){
          msymbols += QString("'square']");
        }
        else{
          msymbols += QString("'triangle-down']");
        }
        x += QString("]");
        y += QString("]");
        text += QString("'']");
        color += QString("]");
        msize += QString("]");
        msymbol += QString("]");
        xm += QString("]");
        ym += QString("]");
        textm += QString("'']");
        colorm += QString("]");
        msizem += QString("]");
        msymbolm += QString("]");
      }
      else{
        x += QString("%1]").arg(p[last_id]->x());
        y += QString("%1]").arg(p[last_id]->y());
        text += QString("'%1']").arg(p[last_id]->getName());
        color += QString("'rgba(%1, %2, %3, %4)']").arg(r_).arg(g_).arg(b_).arg(a_);
        msize += QString("%1]").arg(p[last_id]->radius());
        if(p[last_id]->marker() == CIRCLE){
          msymbol += QString("'circle']");
        }
        else if(p[last_id]->marker() == SQUARE){
          msymbol += QString("'square']");
        }
        else{
          msymbol += QString("'triangle-down']");
        }
        xm += QString("]");
        ym += QString("]");
        textm += QString("'']");
        colorm += QString("]");
        msizem += QString("]");
        msymbolm += QString("]");
        xs += QString("]");
        ys += QString("]");
        texts += QString("'']");
        colors += QString("]");
        msizes += QString("]");
        msymbols += QString("]");
      }
    }
  }
  else{
    x += QString("]");
    y += QString("]");
    text += QString("'']");
    color += QString("]");
    msize += QString("]");
    msymbols += QString("]");
    xm += QString("]");
    ym += QString("]");
    textm += QString("'']");
    colorm += QString("]");
    msizem += QString("]");
    msymbolm += QString("]");
    xs += QString("]");
    ys += QString("]");
    texts += QString("'']");
    colors += QString("]");
    msizes += QString("]");
    msymbols += QString("]");
  }

  QString scatter_type;
  if(p.size() > 10000){
    scatter_type = "scattergl";
  }
  else{
    scatter_type = "scatter";
  }

  json += QString("var id%1 = { %2, %3, %4, mode: 'markers', marker: { %5, opacity: 0.9, %6, %7 }, type: '%8' };").arg(trace_id).arg(x).arg(y).arg(text).arg(msize).arg(color).arg(msymbol).arg(scatter_type);
  trace_id++;
  json += QString("var id%1 = { %2, %3, %4, mode: 'markers+text', textposition: 'top center', textfont: { family: 'Raleway, sans-serif' }, marker: { %5, opacity: 0.9, %6, %7 }, type: '%8' };").arg(trace_id).arg(xm).arg(ym).arg(textm).arg(msizem).arg(colorm).arg(msymbolm).arg(scatter_type);
  trace_id++;
  json += QString("var id%1 = { %2, %3, %4, mode: 'markers', marker: { %5, opacity: 0.9, %6, %7, line: { color: 'rgb(231, 99, 250)', width: 1} }, type: '%8' };").arg(trace_id).arg(xs).arg(ys).arg(texts).arg(msizes).arg(colors).arg(msymbols).arg(scatter_type);
  trace_id++;
  return json;
}

QStringList QPlotlyWindow::JSon3DScatterWorker(int from, int to)
{
  QString x = "", y = "", z = "", text = "", color = "", symbol = "";
  for(int i = from; i < to; i++){
    if(p[i]->isVisible() == true){
      int r_, g_, b_, a_;
      r_ = p[i]->getColor().red();
      g_ = p[i]->getColor().green();
      b_ = p[i]->getColor().blue();
      a_ = p[i]->getColor().alpha();
      MarkerType mtype = p[i]->marker();
      if(p[i]->isLabelVisible() == true){
        //json += QString("var id%1 = { x: [%2], y: [%3], text: ['%4'], mode: 'markers+text', textposition: 'top center', textfont: { family: 'Raleway, sans-serif' }, marker: { size: %5, opacity: 0.9, color: 'rgba(%6, %7, %8, %9)' }, type: 'scatter' };").arg(trace_id).arg(p[i]->x()).arg(p[i]->y()).arg(p[i]->getName()).arg(p[i]->radius()).arg(r_).arg(g_).arg(b_).arg(a_);
        //json += QString("var id%1 = { x: [%2], y: [%3], z: [%4] text: ['%5'], mode: 'markers+text', textposition: 'top center', textfont: { family: 'Raleway, sans-serif' }, marker: { size: %6, opacity: 0.9, color: 'rgba(%7, %8, %9, %10)', symbol: 'circle', }, type: 'scatter3d' };").arg(trace_id).arg(p[i]->x()).arg(p[i]->y()).arg(p[i]->z()).arg(p[i]->getName()).arg(p[i]->radius()).arg(r_).arg(g_).arg(b_).arg(a_);
        x+=QString("%1,").arg(p[i]->x());
        y+=QString("%1,").arg(p[i]->y());
        z+=QString("%1,").arg(p[i]->z());
        text+=QString("'%1',").arg(p[i]->getName());
        color+=QString("'rgba(%1, %2, %3, %4)',").arg(r_).arg(g_).arg(b_).arg(a_);
        if(mtype == CIRCLE){
          symbol += QString("'circle',");
        }
        else if(mtype == SQUARE){
          symbol += QString("'square',");
        }
        else{
          symbol += QString("'cross',");
        }
      }
      else{
        //json += QString("var id%1 = { x: [%2], y: [%3], text: ['%4'], mode: 'markers', marker: { size: %5, opacity: 0.9, color: 'rgba(%6, %7, %8, %9)' }, type: 'scatter' };").arg(trace_id).arg(p[i]->x()).arg(p[i]->y()).arg(p[i]->getName()).arg(p[i]->radius()).arg(r_).arg(g_).arg(b_).arg(a_);
        //json += QString("var id%1 = { x: [%2], y: [%3], z: [%4], text: ['%5'], mode: 'markers', marker: { size: %6, opacity: 0.9, color: 'rgba(%7, %8, %9, %10)', symbol: 'circle', }, type: 'scatter3d' };").arg(trace_id).arg(p[i]->x()).arg(p[i]->y()).arg(p[i]->z()).arg(p[i]->getName()).arg(p[i]->radius()).arg(r_).arg(g_).arg(b_).arg(a_);
        x+=QString("%1,").arg(p[i]->x());
        y+=QString("%1,").arg(p[i]->y());
        z+=QString("%1,").arg(p[i]->z());
        text+=QString("'%1',").arg(p[i]->getName());
        color+=QString("'rgba(%1, %2, %3, %4)',").arg(r_).arg(g_).arg(b_).arg(a_);
        if(mtype == CIRCLE){
          symbol += QString("'circle',");
        }
        else if(mtype == SQUARE){
          symbol += QString("'square',");
        }
        else{
          symbol += QString("'cross',");
        }
      }
      //trace_id++;
    }
    else{
      continue;
    }
  }

  QStringList res;
  res.append(x);
  res.append(y);
  res.append(z);
  res.append(text);
  res.append(color);
  res.append(symbol);
  return res;
}

QString QPlotlyWindow::genJSON3DScatter()
{
  QString json;
  QString x = "x:[", y = "y:[", z = "z:[", text = "text:[", color = "color:[", symbol = "symbol: [";


  int nth = QThread::idealThreadCount();
  int step = (int)ceil((p.size()-1)/(double)nth);

  if(step == 0){
    nth = 1;
    step = p.size()-1;
  }

  QList<QFuture<QStringList>> futures;
  int from = 0, to = step;
  for(int i = 0; i < nth; i++){
    futures.append(QtConcurrent::run(this, &QPlotlyWindow::JSon3DScatterWorker, from, to));
    from = to;
    if(from+step > p.size()-1){
      to = p.size()-1;
    }
    else{
      to+=step;
    }
  }


  for(int i = 0; i < futures.size(); i++){
    QStringList r = futures[i].result();
    x += r[0];
    y += r[1];
    z += r[2];
    text += r[3];
    color += r[4];
    symbol += r[5];
  }

  /* SINGLE THREAD IMPLEMENTATION
  for(int i = 0; i < p.size()-1; i++){
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
  }*/

  size_t last_id = p.size()-1;
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
  if(p[last_id]->marker() == CIRCLE){
    symbol +=QString("'circle']");
  }
  else if(p[last_id]->marker() == SQUARE){
    symbol +=QString("'square']");
  }
  else{
    symbol +=QString("'cross']");
  }


  json += QString("var id%1 = { %2, %3, %4, %5, mode: 'markers', marker: { size: 5, opacity: 0.9, %6, %7 }, type: 'scatter3d' };").arg(trace_id).arg(x).arg(y).arg(z).arg(text).arg(color).arg(symbol);
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
  int i, j;
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
    QString plotly_file = QString("%1/plotly-latest.min.js").arg(qApp->applicationDirPath());
    QFile file(plotly_file);
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
    page = WriteTemporaryPage(code);
    wview->load(page);
    //wview->load(QUrl(QStringLiteral("https://www.google.com")));
    //nam->cookieJar()->cookiesForUrl(QUrl(QStringLiteral("https://www.google.com")));
  }
}

class DataPointIsEqual {
  DataPoint p1;
public:
  DataPointIsEqual(DataPoint &p):p1(p){}
  bool operator()(DataPoint *p2) const
  {
    //QEPSILON and FLOAT_EQ DEFINED IN DATAPOINT.H
    if(FLOAT_EQ(p1.x(), p2->x(), QEPSILON) &&
       FLOAT_EQ(p1.y(), p2->y(), QEPSILON) &&
       FLOAT_EQ(p1.z(), p2->z(), QEPSILON)){
      if(p1.getName().compare(p2->getName()) == 0){
        return true;
      }
      else{
        return false;
      }
    }
    else{
      return false;
    }
  }
};

int QPlotlyWindow::FindPoint(qreal x, qreal y, qreal z, QString name_)
{
  DataPoint f(x, y, z, name_);
  //int indx = std::find(p.begin(), p.end(), &f) - p.begin();
  int indx = std::find_if(p.begin(), p.end(), DataPointIsEqual(f)) - p.begin();
  if(indx == p.size()){
    return -1;
  }
  else{
    return indx;
  }
  //return p.indexOf(&f);
  /*for(int i = 0; i < p.size(); i++){
    if(p[i]->compare(f) == true){
      return i;
    }
    else{
      continue;
    }
  }
  return -1;*/
  /*auto it = std::find(p.begin(), p.end(), &f);
  if (it == p.end()){
    return -1;
  }
  else{
    return (int)std::distance(p.begin(), it);
  }*/
}

#ifdef WEBKIT
void QPlotlyWindow::WebKithandleCookieAdded(QString cookie)
{
  //qDebug() << cookie;
  CookieParser(cookie);
}
#else
void QPlotlyWindow::WebEnginehandleCookieAdded(const QNetworkCookie &cookie)
{
  QString cookie_(cookie.toRawForm());
  CookieParser(cookie_);
  #ifdef DEBUG
  qDebug() << cookie_ << "\n";
  #endif
}
#endif

void QPlotlyWindow::CookieParser(QString cookie)
{
  if(cookie.contains("selpnt=start") == true){
    #ifdef DEBUG
    qDebug() << "Clear previous selection";
    #endif
    for(int i = 0; i < selected_points.size(); i++){
      p[selected_points[i]]->setSelection(false);
    }
    selected_points.clear();
  }
  else if(cookie.contains("point=") == true){
    #ifdef DEBUG
    qDebug() << cookie.split(";");
    #endif
    QStringList vars = cookie.split(";")[0].split(" ");
    //point=x: 2 y: 2 z: 2 name: due; path=/var/folders/1z/j2xq6qss7s93bmf23nzcskp00000gp/T/testqplotly-YlDhEE"
    qreal x = 0.f, y = 0.f, z = 0.f;
    x = atof(vars[1].toStdString().c_str());
    y = atof(vars[3].toStdString().c_str());
    if(vars[5].compare("undefined") != 0)
      z = atof(vars[5].toStdString().c_str());
    QString name = vars[7];
    int id = FindPoint(x, y, z, name);
    #ifdef DEBUG
    qDebug() << "Find Point Result: " << id;
    #endif

    if(id > -1){
      selected_points << id;
      p[id]->setSelection(true);
      //qDebug() << "id Nr. " << selected_points.last();
    }
    else{
      //Refresh();
      update();
    }
  }
  else if(cookie.contains("selpnt=end") == true){
    #ifdef DEBUG
    qDebug() << "End point selection";
    #endif
    /* Selection END!
     for(size_t i = 0; i < selected_points.size(); i++){
      qDebug() << selected_points[i] << "\n";
    }*/
    Refresh(); // OK THIS
    update();
  }
  //Refresh();
  update();
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
  #ifdef WEBKIT
  wview = new QWebView(this);
  #else
  wview = new QWebEngineView(this);
  #endif
  this->setMinimumSize(this->size());
  wview->resize(this->size());
  //wview->page()->profile()->setPersistentCookiesPolicy(QWebEngineProfile::ForcePersistentCookies);
  mainLayout->addWidget(wview);
  this->setLayout(mainLayout);
  //cookie halding
  #ifdef WEBKIT
  /* 1) get QWebPage and grap the underlying QNetworkAccessManager
   * 2) Set a custom cookieJar that overrides the functions to set cookies.
   * 3) When a cookie is added emit a signal and get it in a format that you need
   */
  cookieJar = new CustomNetworkCookieJar();

  nam = new QNetworkAccessManager();
  nam->setCookieJar(cookieJar);
  wview->page()->setNetworkAccessManager(nam);
  connect(nam->cookieJar(), SIGNAL(cookiesChanged(QString)), this, SLOT(WebKithandleCookieAdded(QString)));
  #else
  cookie_store = wview->page()->profile()->cookieStore();
  connect(cookie_store, &QWebEngineCookieStore::cookieAdded, this, &QPlotlyWindow::WebEnginehandleCookieAdded);
  #endif
}

QPlotlyWindow::~QPlotlyWindow()
{
  // Free the memory!
  dir.remove();
  delete wview;

  #ifdef WEBKIT
  delete cookieJar;
  delete nam;
  #endif

  for(int i = 0; i < p.size(); i++){
    delete p[i];
  }
  for(int i = 0; i < b.size(); i++){
    delete b[i];
  }
}
