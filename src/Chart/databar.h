#ifndef DATABAR_H
#define DATABAR_H

#include <QtGlobal>
#include <QColor>
#include <QList>
#include <QVector>
#include <QStringList>
#include <QMetaType>


class DataBar
{
public:
  DataBar();
  DataBar(QStringList x, QVector<qreal> y);
  DataBar(QStringList x,  QVector<qreal> y, QStringList text);

  void setX(QStringList x);
  void setY(QVector<qreal> y);
  void setText(QStringList text);
  QStringList &getText();
  void setColor(const QColor &color);
  QColor &getColor();
  void setWidth(int width);

  void setSelection(bool selection);
  void setVisible(bool visible);
  void setTextVisible(bool tvisible);

  QStringList x() const;
  QVector<qreal> y() const;
  QStringList text() const;
  bool isSelected() const;
  bool isVisible() const;
  bool isTextVisible() const;

  QColor color() const;
  int width() const;

private:
  QStringList m_x;
  QVector<qreal> m_y;
  QStringList m_text;
  QColor m_color;
  int m_width;
  bool m_selection;
  bool m_visible, m_tvisible;
};

#endif // DATABAR_H
