/********************************************************************************
** Form generated from reading UI file 'q8.ui'
**
** Created by: Qt User Interface Compiler version 5.6.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_Q8_H
#define UI_Q8_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Q8Class
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;

    void setupUi(QMainWindow *Q8Class)
    {
        if (Q8Class->objectName().isEmpty())
            Q8Class->setObjectName(QStringLiteral("Q8Class"));
        Q8Class->resize(835, 652);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Q8Class->sizePolicy().hasHeightForWidth());
        Q8Class->setSizePolicy(sizePolicy);
        Q8Class->setMinimumSize(QSize(835, 600));
        Q8Class->setMaximumSize(QSize(16777215, 16777215));
        centralWidget = new QWidget(Q8Class);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        sizePolicy.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
        centralWidget->setSizePolicy(sizePolicy);
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(0);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        Q8Class->setCentralWidget(centralWidget);

        retranslateUi(Q8Class);

        QMetaObject::connectSlotsByName(Q8Class);
    } // setupUi

    void retranslateUi(QMainWindow *Q8Class)
    {
        Q8Class->setWindowTitle(QApplication::translate("Q8Class", "Q8", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Q8Class: public Ui_Q8Class {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_Q8_H
