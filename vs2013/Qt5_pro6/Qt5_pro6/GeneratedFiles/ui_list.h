/********************************************************************************
** Form generated from reading UI file 'list.ui'
**
** Created by: Qt User Interface Compiler version 5.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LIST_H
#define UI_LIST_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ListClass
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QListWidget *listWidget;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QLineEdit *lineEdit;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_2;
    QLineEdit *lineEdit_2;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_3;
    QLineEdit *lineEdit_3;
    QHBoxLayout *horizontalLayout_5;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *ListClass)
    {
        if (ListClass->objectName().isEmpty())
            ListClass->setObjectName(QStringLiteral("ListClass"));
        ListClass->resize(791, 648);
        centralWidget = new QWidget(ListClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
        centralWidget->setSizePolicy(sizePolicy);
        verticalLayout_2 = new QVBoxLayout(centralWidget);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        listWidget = new QListWidget(centralWidget);
        listWidget->setObjectName(QStringLiteral("listWidget"));

        horizontalLayout->addWidget(listWidget);


        verticalLayout_2->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout_2->addWidget(label);

        lineEdit = new QLineEdit(centralWidget);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));

        horizontalLayout_2->addWidget(lineEdit);


        verticalLayout_2->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout_3->addWidget(label_2);

        lineEdit_2 = new QLineEdit(centralWidget);
        lineEdit_2->setObjectName(QStringLiteral("lineEdit_2"));

        horizontalLayout_3->addWidget(lineEdit_2);


        verticalLayout_2->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QStringLiteral("label_3"));

        horizontalLayout_4->addWidget(label_3);

        lineEdit_3 = new QLineEdit(centralWidget);
        lineEdit_3->setObjectName(QStringLiteral("lineEdit_3"));

        horizontalLayout_4->addWidget(lineEdit_3);


        verticalLayout_2->addLayout(horizontalLayout_4);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        horizontalLayout_5->addWidget(pushButton);

        pushButton_2 = new QPushButton(centralWidget);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));

        horizontalLayout_5->addWidget(pushButton_2);

        pushButton_3 = new QPushButton(centralWidget);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));

        horizontalLayout_5->addWidget(pushButton_3);


        verticalLayout_2->addLayout(horizontalLayout_5);

        ListClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(ListClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 791, 26));
        ListClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(ListClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        ListClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(ListClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        ListClass->setStatusBar(statusBar);

        retranslateUi(ListClass);

        QMetaObject::connectSlotsByName(ListClass);
    } // setupUi

    void retranslateUi(QMainWindow *ListClass)
    {
        ListClass->setWindowTitle(QApplication::translate("ListClass", "List", Q_NULLPTR));
        label->setText(QApplication::translate("ListClass", "\345\247\223\345\220\215\357\274\232", Q_NULLPTR));
        label_2->setText(QApplication::translate("ListClass", "\345\271\264\351\276\204\357\274\232", Q_NULLPTR));
        label_3->setText(QApplication::translate("ListClass", "\347\217\255\347\272\247\357\274\232", Q_NULLPTR));
        pushButton->setText(QApplication::translate("ListClass", "\346\267\273\345\212\240\344\277\241\346\201\257", Q_NULLPTR));
        pushButton_2->setText(QApplication::translate("ListClass", "\345\210\240\351\231\244\344\277\241\346\201\257", Q_NULLPTR));
        pushButton_3->setText(QApplication::translate("ListClass", "\344\277\256\346\224\271\344\277\241\346\201\257", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class ListClass: public Ui_ListClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LIST_H
