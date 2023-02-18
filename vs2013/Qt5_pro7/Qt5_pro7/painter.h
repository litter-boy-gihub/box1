#ifndef PAINTER_H
#define PAINTER_H
   
#include <QtWidgets/QMainWindow>
#include "ui_painter.h"
#include <QEvent>
class painter : public QMainWindow
{
    Q_OBJECT

public:
    painter(QWidget *parent = 0);
    ~painter();

protected:
    void keyPressEvent( QKeyEvent *event ) override;
    void mousePressEvent( QMouseEvent *event ) override;
    void mouseMoveEvent( QMouseEvent *event ) override;
    void paintEvent( QPaintEvent *event ) override;

private:
    Ui::painterClass ui;
    QImage *image;
    QPoint pre_point;
    QPoint cur_point;
    QPoint letf_pre_point;
    QPoint letf_cur_point;

    double rotae;
    QPair<double , double> scale;

    QPointF off_point;
};

#endif // PAINTER_H
