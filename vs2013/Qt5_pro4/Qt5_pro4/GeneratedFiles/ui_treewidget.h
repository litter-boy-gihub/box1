/********************************************************************************
** Form generated from reading UI file 'treewidget.ui'
**
** Created by: Qt User Interface Compiler version 5.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TREEWIDGET_H
#define UI_TREEWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_treeWidgetClass
{
public:
    QVBoxLayout *verticalLayout;
    QTreeWidget *tree;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_1;
    QLineEdit *lineEdit_1;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_2;
    QLineEdit *lineEdit_2;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_3;
    QLineEdit *lineEdit_3;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;

    void setupUi(QWidget *treeWidgetClass)
    {
        if (treeWidgetClass->objectName().isEmpty())
            treeWidgetClass->setObjectName(QStringLiteral("treeWidgetClass"));
        treeWidgetClass->resize(727, 485);
        verticalLayout = new QVBoxLayout(treeWidgetClass);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        tree = new QTreeWidget(treeWidgetClass);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QStringLiteral("1"));
        tree->setHeaderItem(__qtreewidgetitem);
        tree->setObjectName(QStringLiteral("tree"));

        verticalLayout->addWidget(tree);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label_1 = new QLabel(treeWidgetClass);
        label_1->setObjectName(QStringLiteral("label_1"));

        horizontalLayout_2->addWidget(label_1);

        lineEdit_1 = new QLineEdit(treeWidgetClass);
        lineEdit_1->setObjectName(QStringLiteral("lineEdit_1"));

        horizontalLayout_2->addWidget(lineEdit_1);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label_2 = new QLabel(treeWidgetClass);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout_3->addWidget(label_2);

        lineEdit_2 = new QLineEdit(treeWidgetClass);
        lineEdit_2->setObjectName(QStringLiteral("lineEdit_2"));

        horizontalLayout_3->addWidget(lineEdit_2);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        label_3 = new QLabel(treeWidgetClass);
        label_3->setObjectName(QStringLiteral("label_3"));

        horizontalLayout_4->addWidget(label_3);

        lineEdit_3 = new QLineEdit(treeWidgetClass);
        lineEdit_3->setObjectName(QStringLiteral("lineEdit_3"));

        horizontalLayout_4->addWidget(lineEdit_3);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        pushButton_2 = new QPushButton(treeWidgetClass);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));

        horizontalLayout->addWidget(pushButton_2);

        pushButton_3 = new QPushButton(treeWidgetClass);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));

        horizontalLayout->addWidget(pushButton_3);

        pushButton_4 = new QPushButton(treeWidgetClass);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));

        horizontalLayout->addWidget(pushButton_4);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(treeWidgetClass);

        QMetaObject::connectSlotsByName(treeWidgetClass);
    } // setupUi

    void retranslateUi(QWidget *treeWidgetClass)
    {
        treeWidgetClass->setWindowTitle(QApplication::translate("treeWidgetClass", "treeWidget", Q_NULLPTR));
        label_1->setText(QApplication::translate("treeWidgetClass", "\345\247\223\345\220\215\357\274\232", Q_NULLPTR));
        label_2->setText(QApplication::translate("treeWidgetClass", "\346\200\247\345\210\253\357\274\232", Q_NULLPTR));
        label_3->setText(QApplication::translate("treeWidgetClass", "\347\217\255\347\272\247\357\274\232", Q_NULLPTR));
        pushButton_2->setText(QApplication::translate("treeWidgetClass", "\345\242\236\345\212\240\345\255\246\347\224\237\344\277\241\346\201\257", Q_NULLPTR));
        pushButton_3->setText(QApplication::translate("treeWidgetClass", "\345\210\240\351\231\244\345\255\246\347\224\237\344\277\241\346\201\257", Q_NULLPTR));
        pushButton_4->setText(QApplication::translate("treeWidgetClass", "\344\277\256\346\224\271\345\255\246\347\224\237\344\277\241\346\201\257", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class treeWidgetClass: public Ui_treeWidgetClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TREEWIDGET_H
