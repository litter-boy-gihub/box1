/********************************************************************************
** Form generated from reading UI file 'nodepad.ui'
**
** Created by: Qt User Interface Compiler version 5.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NODEPAD_H
#define UI_NODEPAD_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_NodepadClass
{
public:
    QAction *actionOpen;
    QAction *actionSave;
    QAction *actionEcit;
    QAction *actionAll;
    QAction *actionDel;
    QAction *actionHelp;
    QAction *actionAbout;
    QWidget *centralWidget;
    QTextEdit *textEdit;
    QMenuBar *menuBar;
    QMenu *menu;
    QMenu *menu_2;
    QMenu *menu_3;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *NodepadClass)
    {
        if (NodepadClass->objectName().isEmpty())
            NodepadClass->setObjectName(QStringLiteral("NodepadClass"));
        NodepadClass->resize(600, 400);
        NodepadClass->setMinimumSize(QSize(600, 400));
        NodepadClass->setMaximumSize(QSize(600, 400));
        actionOpen = new QAction(NodepadClass);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        actionSave = new QAction(NodepadClass);
        actionSave->setObjectName(QStringLiteral("actionSave"));
        actionEcit = new QAction(NodepadClass);
        actionEcit->setObjectName(QStringLiteral("actionEcit"));
        actionAll = new QAction(NodepadClass);
        actionAll->setObjectName(QStringLiteral("actionAll"));
        actionDel = new QAction(NodepadClass);
        actionDel->setObjectName(QStringLiteral("actionDel"));
        actionHelp = new QAction(NodepadClass);
        actionHelp->setObjectName(QStringLiteral("actionHelp"));
        actionAbout = new QAction(NodepadClass);
        actionAbout->setObjectName(QStringLiteral("actionAbout"));
        centralWidget = new QWidget(NodepadClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        textEdit = new QTextEdit(centralWidget);
        textEdit->setObjectName(QStringLiteral("textEdit"));
        textEdit->setGeometry(QRect(0, 0, 590, 300));
        textEdit->setMinimumSize(QSize(590, 300));
        textEdit->setMaximumSize(QSize(590, 300));
        NodepadClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(NodepadClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 26));
        menu = new QMenu(menuBar);
        menu->setObjectName(QStringLiteral("menu"));
        menu_2 = new QMenu(menuBar);
        menu_2->setObjectName(QStringLiteral("menu_2"));
        menu_3 = new QMenu(menuBar);
        menu_3->setObjectName(QStringLiteral("menu_3"));
        NodepadClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(NodepadClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        NodepadClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        NodepadClass->insertToolBarBreak(mainToolBar);
        statusBar = new QStatusBar(NodepadClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        NodepadClass->setStatusBar(statusBar);

        menuBar->addAction(menu->menuAction());
        menuBar->addAction(menu_2->menuAction());
        menuBar->addAction(menu_3->menuAction());
        menu->addAction(actionOpen);
        menu->addAction(actionSave);
        menu->addAction(actionEcit);
        menu_2->addAction(actionAll);
        menu_2->addAction(actionDel);
        menu_3->addAction(actionHelp);
        menu_3->addAction(actionAbout);
        mainToolBar->addAction(actionOpen);
        mainToolBar->addAction(actionSave);
        mainToolBar->addAction(actionEcit);

        retranslateUi(NodepadClass);

        QMetaObject::connectSlotsByName(NodepadClass);
    } // setupUi

    void retranslateUi(QMainWindow *NodepadClass)
    {
        NodepadClass->setWindowTitle(QApplication::translate("NodepadClass", "Nodepad", Q_NULLPTR));
        actionOpen->setText(QApplication::translate("NodepadClass", "\346\211\223\345\274\200", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        actionOpen->setStatusTip(QApplication::translate("NodepadClass", "\346\211\223\345\274\200", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        actionSave->setText(QApplication::translate("NodepadClass", "\344\277\235\345\255\230", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        actionSave->setStatusTip(QApplication::translate("NodepadClass", "\344\277\235\345\255\230", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        actionEcit->setText(QApplication::translate("NodepadClass", "\351\200\200\345\207\272", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        actionEcit->setStatusTip(QApplication::translate("NodepadClass", "\351\200\200\345\207\272", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        actionAll->setText(QApplication::translate("NodepadClass", "\345\205\250\351\200\211", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        actionAll->setStatusTip(QApplication::translate("NodepadClass", "\345\205\250\351\200\211", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        actionDel->setText(QApplication::translate("NodepadClass", "\345\210\240\351\231\244", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        actionDel->setStatusTip(QApplication::translate("NodepadClass", "\345\210\240\351\231\244", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        actionHelp->setText(QApplication::translate("NodepadClass", "\345\270\256\345\212\251", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        actionHelp->setStatusTip(QApplication::translate("NodepadClass", "\345\270\256\345\212\251", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        actionAbout->setText(QApplication::translate("NodepadClass", "\345\205\263\344\272\216", Q_NULLPTR));
#ifndef QT_NO_STATUSTIP
        actionAbout->setStatusTip(QApplication::translate("NodepadClass", "\345\205\263\344\272\216", Q_NULLPTR));
#endif // QT_NO_STATUSTIP
        menu->setTitle(QApplication::translate("NodepadClass", "\346\226\207\344\273\266", Q_NULLPTR));
        menu_2->setTitle(QApplication::translate("NodepadClass", "\347\274\226\350\276\221", Q_NULLPTR));
        menu_3->setTitle(QApplication::translate("NodepadClass", "\345\270\256\345\212\251", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class NodepadClass: public Ui_NodepadClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NODEPAD_H
