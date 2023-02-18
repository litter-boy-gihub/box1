/********************************************************************************
** Form generated from reading UI file 'painter.ui'
**
** Created by: Qt User Interface Compiler version 5.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PAINTER_H
#define UI_PAINTER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_painterClass
{
public:
    QWidget *Canvas;

    void setupUi(QMainWindow *painterClass)
    {
        if (painterClass->objectName().isEmpty())
            painterClass->setObjectName(QStringLiteral("painterClass"));
        painterClass->resize(803, 682);
        Canvas = new QWidget(painterClass);
        Canvas->setObjectName(QStringLiteral("Canvas"));
        painterClass->setCentralWidget(Canvas);

        retranslateUi(painterClass);

        QMetaObject::connectSlotsByName(painterClass);
    } // setupUi

    void retranslateUi(QMainWindow *painterClass)
    {
        painterClass->setWindowTitle(QApplication::translate("painterClass", "painter", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class painterClass: public Ui_painterClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PAINTER_H
