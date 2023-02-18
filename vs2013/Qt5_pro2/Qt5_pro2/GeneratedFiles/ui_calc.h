/********************************************************************************
** Form generated from reading UI file 'calc.ui'
**
** Created by: Qt User Interface Compiler version 5.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CALC_H
#define UI_CALC_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CalcClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *CalcClass)
    {
        if (CalcClass->objectName().isEmpty())
            CalcClass->setObjectName(QStringLiteral("CalcClass"));
        CalcClass->resize(600, 400);
        menuBar = new QMenuBar(CalcClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        CalcClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(CalcClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        CalcClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(CalcClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        CalcClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(CalcClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        CalcClass->setStatusBar(statusBar);

        retranslateUi(CalcClass);

        QMetaObject::connectSlotsByName(CalcClass);
    } // setupUi

    void retranslateUi(QMainWindow *CalcClass)
    {
        CalcClass->setWindowTitle(QApplication::translate("CalcClass", "Calc", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class CalcClass: public Ui_CalcClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CALC_H
