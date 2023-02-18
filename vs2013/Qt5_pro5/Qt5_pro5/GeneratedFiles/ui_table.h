/********************************************************************************
** Form generated from reading UI file 'table.ui'
**
** Created by: Qt User Interface Compiler version 5.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TABLE_H
#define UI_TABLE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TableClass
{
public:
    QVBoxLayout *verticalLayout;
    QTableWidget *tableWidget;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QLineEdit *lineEdit;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_2;
    QLineEdit *lineEdit_2;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_3;
    QLineEdit *lineEdit_3;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;

    void setupUi(QWidget *TableClass)
    {
        if (TableClass->objectName().isEmpty())
            TableClass->setObjectName(QStringLiteral("TableClass"));
        TableClass->resize(694, 547);
        verticalLayout = new QVBoxLayout(TableClass);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        tableWidget = new QTableWidget(TableClass);
        tableWidget->setObjectName(QStringLiteral("tableWidget"));

        verticalLayout->addWidget(tableWidget);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label = new QLabel(TableClass);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout_2->addWidget(label);

        lineEdit = new QLineEdit(TableClass);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));

        horizontalLayout_2->addWidget(lineEdit);


        verticalLayout_2->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label_2 = new QLabel(TableClass);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout_3->addWidget(label_2);

        lineEdit_2 = new QLineEdit(TableClass);
        lineEdit_2->setObjectName(QStringLiteral("lineEdit_2"));

        horizontalLayout_3->addWidget(lineEdit_2);


        verticalLayout_2->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        label_3 = new QLabel(TableClass);
        label_3->setObjectName(QStringLiteral("label_3"));

        horizontalLayout_4->addWidget(label_3);

        lineEdit_3 = new QLineEdit(TableClass);
        lineEdit_3->setObjectName(QStringLiteral("lineEdit_3"));

        horizontalLayout_4->addWidget(lineEdit_3);


        verticalLayout_2->addLayout(horizontalLayout_4);


        verticalLayout->addLayout(verticalLayout_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        pushButton = new QPushButton(TableClass);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        horizontalLayout->addWidget(pushButton);

        pushButton_2 = new QPushButton(TableClass);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));

        horizontalLayout->addWidget(pushButton_2);

        pushButton_3 = new QPushButton(TableClass);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));

        horizontalLayout->addWidget(pushButton_3);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(TableClass);

        QMetaObject::connectSlotsByName(TableClass);
    } // setupUi

    void retranslateUi(QWidget *TableClass)
    {
        TableClass->setWindowTitle(QApplication::translate("TableClass", "Table", Q_NULLPTR));
        label->setText(QApplication::translate("TableClass", "\345\247\223\345\220\215\357\274\232", Q_NULLPTR));
        label_2->setText(QApplication::translate("TableClass", "\346\200\247\345\210\253\357\274\232", Q_NULLPTR));
        label_3->setText(QApplication::translate("TableClass", "\347\217\255\347\272\247\357\274\232", Q_NULLPTR));
        pushButton->setText(QApplication::translate("TableClass", "\345\242\236\345\212\240\345\255\246\347\224\237\344\277\241\346\201\257", Q_NULLPTR));
        pushButton_2->setText(QApplication::translate("TableClass", "\345\210\240\351\231\244\345\255\246\347\224\237\344\277\241\346\201\257", Q_NULLPTR));
        pushButton_3->setText(QApplication::translate("TableClass", "\346\233\264\346\224\271\345\255\246\347\224\237\344\277\241\346\201\257", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class TableClass: public Ui_TableClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TABLE_H
