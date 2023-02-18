/********************************************************************************
** Form generated from reading UI file 'qt5_pro1.ui'
**
** Created by: Qt User Interface Compiler version 5.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QT5_PRO1_H
#define UI_QT5_PRO1_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Qt5_pro1Class
{
public:
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout_2;
    QLineEdit *lineEdit;
    QGridLayout *gridLayout;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;
    QPushButton *pushButton_8;
    QPushButton *pushButton_7;
    QPushButton *pushButton_9;
    QPushButton *pushButton_11;
    QPushButton *pushButton_6;
    QPushButton *pushButton_5;
    QPushButton *pushButton_12;
    QPushButton *pushButton_10;
    QPushButton *pushButton_13;
    QPushButton *pushButton_14;
    QPushButton *pushButton_15;
    QPushButton *pushButton_16;

    void setupUi(QWidget *Qt5_pro1Class)
    {
        if (Qt5_pro1Class->objectName().isEmpty())
            Qt5_pro1Class->setObjectName(QStringLiteral("Qt5_pro1Class"));
        Qt5_pro1Class->resize(600, 600);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Qt5_pro1Class->sizePolicy().hasHeightForWidth());
        Qt5_pro1Class->setSizePolicy(sizePolicy);
        Qt5_pro1Class->setMinimumSize(QSize(600, 600));
        Qt5_pro1Class->setMaximumSize(QSize(600, 600));
        verticalLayoutWidget = new QWidget(Qt5_pro1Class);
        verticalLayoutWidget->setObjectName(QStringLiteral("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(10, 10, 571, 581));
        verticalLayout_2 = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(10, 10, 10, 10);
        lineEdit = new QLineEdit(verticalLayoutWidget);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setMinimumSize(QSize(0, 60));

        verticalLayout_2->addWidget(lineEdit);

        gridLayout = new QGridLayout();
        gridLayout->setSpacing(10);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        gridLayout->setContentsMargins(10, 10, 10, 10);
        pushButton = new QPushButton(verticalLayoutWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(pushButton->sizePolicy().hasHeightForWidth());
        pushButton->setSizePolicy(sizePolicy1);
        pushButton->setMinimumSize(QSize(0, 50));

        gridLayout->addWidget(pushButton, 0, 0, 1, 1);

        pushButton_2 = new QPushButton(verticalLayoutWidget);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        sizePolicy1.setHeightForWidth(pushButton_2->sizePolicy().hasHeightForWidth());
        pushButton_2->setSizePolicy(sizePolicy1);
        pushButton_2->setMinimumSize(QSize(0, 50));

        gridLayout->addWidget(pushButton_2, 1, 0, 1, 1);

        pushButton_3 = new QPushButton(verticalLayoutWidget);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));
        sizePolicy1.setHeightForWidth(pushButton_3->sizePolicy().hasHeightForWidth());
        pushButton_3->setSizePolicy(sizePolicy1);
        pushButton_3->setMinimumSize(QSize(0, 50));

        gridLayout->addWidget(pushButton_3, 0, 1, 1, 1);

        pushButton_4 = new QPushButton(verticalLayoutWidget);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));
        sizePolicy1.setHeightForWidth(pushButton_4->sizePolicy().hasHeightForWidth());
        pushButton_4->setSizePolicy(sizePolicy1);
        pushButton_4->setMinimumSize(QSize(0, 50));

        gridLayout->addWidget(pushButton_4, 1, 1, 1, 1);

        pushButton_8 = new QPushButton(verticalLayoutWidget);
        pushButton_8->setObjectName(QStringLiteral("pushButton_8"));
        sizePolicy1.setHeightForWidth(pushButton_8->sizePolicy().hasHeightForWidth());
        pushButton_8->setSizePolicy(sizePolicy1);
        pushButton_8->setMinimumSize(QSize(0, 50));

        gridLayout->addWidget(pushButton_8, 2, 1, 1, 1);

        pushButton_7 = new QPushButton(verticalLayoutWidget);
        pushButton_7->setObjectName(QStringLiteral("pushButton_7"));
        sizePolicy1.setHeightForWidth(pushButton_7->sizePolicy().hasHeightForWidth());
        pushButton_7->setSizePolicy(sizePolicy1);
        pushButton_7->setMinimumSize(QSize(0, 50));

        gridLayout->addWidget(pushButton_7, 2, 0, 1, 1);

        pushButton_9 = new QPushButton(verticalLayoutWidget);
        pushButton_9->setObjectName(QStringLiteral("pushButton_9"));
        sizePolicy1.setHeightForWidth(pushButton_9->sizePolicy().hasHeightForWidth());
        pushButton_9->setSizePolicy(sizePolicy1);
        pushButton_9->setMinimumSize(QSize(0, 50));

        gridLayout->addWidget(pushButton_9, 2, 2, 1, 1);

        pushButton_11 = new QPushButton(verticalLayoutWidget);
        pushButton_11->setObjectName(QStringLiteral("pushButton_11"));
        sizePolicy1.setHeightForWidth(pushButton_11->sizePolicy().hasHeightForWidth());
        pushButton_11->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(pushButton_11, 1, 3, 1, 1);

        pushButton_6 = new QPushButton(verticalLayoutWidget);
        pushButton_6->setObjectName(QStringLiteral("pushButton_6"));
        sizePolicy1.setHeightForWidth(pushButton_6->sizePolicy().hasHeightForWidth());
        pushButton_6->setSizePolicy(sizePolicy1);
        pushButton_6->setMinimumSize(QSize(0, 50));

        gridLayout->addWidget(pushButton_6, 0, 2, 1, 1);

        pushButton_5 = new QPushButton(verticalLayoutWidget);
        pushButton_5->setObjectName(QStringLiteral("pushButton_5"));
        sizePolicy1.setHeightForWidth(pushButton_5->sizePolicy().hasHeightForWidth());
        pushButton_5->setSizePolicy(sizePolicy1);
        pushButton_5->setMinimumSize(QSize(0, 50));

        gridLayout->addWidget(pushButton_5, 1, 2, 1, 1);

        pushButton_12 = new QPushButton(verticalLayoutWidget);
        pushButton_12->setObjectName(QStringLiteral("pushButton_12"));
        sizePolicy1.setHeightForWidth(pushButton_12->sizePolicy().hasHeightForWidth());
        pushButton_12->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(pushButton_12, 0, 3, 1, 1);

        pushButton_10 = new QPushButton(verticalLayoutWidget);
        pushButton_10->setObjectName(QStringLiteral("pushButton_10"));
        sizePolicy1.setHeightForWidth(pushButton_10->sizePolicy().hasHeightForWidth());
        pushButton_10->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(pushButton_10, 2, 3, 1, 1);

        pushButton_13 = new QPushButton(verticalLayoutWidget);
        pushButton_13->setObjectName(QStringLiteral("pushButton_13"));
        sizePolicy1.setHeightForWidth(pushButton_13->sizePolicy().hasHeightForWidth());
        pushButton_13->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(pushButton_13, 3, 0, 1, 1);

        pushButton_14 = new QPushButton(verticalLayoutWidget);
        pushButton_14->setObjectName(QStringLiteral("pushButton_14"));
        sizePolicy1.setHeightForWidth(pushButton_14->sizePolicy().hasHeightForWidth());
        pushButton_14->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(pushButton_14, 3, 1, 1, 1);

        pushButton_15 = new QPushButton(verticalLayoutWidget);
        pushButton_15->setObjectName(QStringLiteral("pushButton_15"));
        sizePolicy1.setHeightForWidth(pushButton_15->sizePolicy().hasHeightForWidth());
        pushButton_15->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(pushButton_15, 3, 2, 1, 1);

        pushButton_16 = new QPushButton(verticalLayoutWidget);
        pushButton_16->setObjectName(QStringLiteral("pushButton_16"));
        sizePolicy1.setHeightForWidth(pushButton_16->sizePolicy().hasHeightForWidth());
        pushButton_16->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(pushButton_16, 3, 3, 1, 1);


        verticalLayout_2->addLayout(gridLayout);


        retranslateUi(Qt5_pro1Class);

        pushButton->setDefault(false);


        QMetaObject::connectSlotsByName(Qt5_pro1Class);
    } // setupUi

    void retranslateUi(QWidget *Qt5_pro1Class)
    {
        Qt5_pro1Class->setWindowTitle(QApplication::translate("Qt5_pro1Class", "\350\256\241\347\256\227\345\231\250", Q_NULLPTR));
        pushButton->setText(QApplication::translate("Qt5_pro1Class", "7", Q_NULLPTR));
        pushButton_2->setText(QApplication::translate("Qt5_pro1Class", "4", Q_NULLPTR));
        pushButton_3->setText(QApplication::translate("Qt5_pro1Class", "8", Q_NULLPTR));
        pushButton_4->setText(QApplication::translate("Qt5_pro1Class", "5", Q_NULLPTR));
        pushButton_8->setText(QApplication::translate("Qt5_pro1Class", "2", Q_NULLPTR));
        pushButton_7->setText(QApplication::translate("Qt5_pro1Class", "1", Q_NULLPTR));
        pushButton_9->setText(QApplication::translate("Qt5_pro1Class", "3", Q_NULLPTR));
        pushButton_11->setText(QApplication::translate("Qt5_pro1Class", "*", Q_NULLPTR));
        pushButton_6->setText(QApplication::translate("Qt5_pro1Class", "9", Q_NULLPTR));
        pushButton_5->setText(QApplication::translate("Qt5_pro1Class", "6", Q_NULLPTR));
        pushButton_12->setText(QApplication::translate("Qt5_pro1Class", "/", Q_NULLPTR));
        pushButton_10->setText(QApplication::translate("Qt5_pro1Class", "-", Q_NULLPTR));
        pushButton_13->setText(QApplication::translate("Qt5_pro1Class", "0", Q_NULLPTR));
        pushButton_14->setText(QApplication::translate("Qt5_pro1Class", ".", Q_NULLPTR));
        pushButton_15->setText(QApplication::translate("Qt5_pro1Class", "=", Q_NULLPTR));
        pushButton_16->setText(QApplication::translate("Qt5_pro1Class", "+", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Qt5_pro1Class: public Ui_Qt5_pro1Class {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QT5_PRO1_H
