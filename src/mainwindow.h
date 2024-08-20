//
// Created by nelos on 26.11.2023.
//

#ifndef QRDECODER_MAINWINDOW_H
#define QRDECODER_MAINWINDOW_H

#include <QGraphicsColorizeEffect>
#include <QKeyEvent>
#include <QMainWindow>
#include <QMenu>
#include <QSpacerItem>
#include <QString>
#include <QStyle>
#include <QSystemTrayIcon>
#include <QWidget>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

namespace Ui {
class MainWindow;
}

QT_END_NAMESPACE

class MainWindow : public QMainWindow {
        Q_OBJECT

    public:
        QAction* actionPaste{};
        QWidget* widget{};
        QWidget* gridLayoutWidget{};
        QGridLayout* gridLayout{};
        QPushButton* pushButton{};
        QLineEdit* lineEdit{};
        QLabel* imageLabel{};
        QSpacerItem* horizontalSpacer{};
        QSpacerItem* horizontalSpacer_2{};
        QTextBrowser* info{};
        QLineEdit* fileNameText{};
        QSpacerItem* horizontalSpacer_3{};

        void setupUi() {
            if (objectName().isEmpty()) {
                setObjectName(QString::fromUtf8("MainWindow"));
            }
            setWindowModality(Qt::ApplicationModal);
            setTabletTracking(true);
            setAutoFillBackground(true);
            setAcceptDrops(true);

            setWindowTitle(QCoreApplication::translate("MainWindow", "Qr Decoder", nullptr));
            setWindowFlag(Qt::MSWindowsFixedSizeDialogHint);
            setBackgroundRole(QPalette::ColorRole::Dark);
            resize(500, 700);
            QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
            sizePolicy.setHorizontalStretch(0);
            sizePolicy.setVerticalStretch(0);
            sizePolicy.setHeightForWidth(sizePolicy.hasHeightForWidth());
            setSizePolicy(sizePolicy);
            QFont font;
            font.setPointSize(12);
            setFont(font);

            // widget
            widget = new QWidget(this);
            widget->setObjectName(QString::fromUtf8("widget"));
            widget->setSizePolicy(sizePolicy);
            widget->setSizeIncrement(QSize(1, 1));
            widget->setMouseTracking(true);
            widget->setTabletTracking(false);
            widget->setFocusPolicy(Qt::ClickFocus);
            widget->setAcceptDrops(true);
            widget->setAutoFillBackground(true);
            setCentralWidget(widget);
            gridLayoutWidget = new QWidget(widget);
            gridLayoutWidget->setObjectName(QString::fromUtf8("gridLayoutWidget"));
            gridLayoutWidget->setGeometry(QRect(0, 0, 500, 700));
            gridLayout = new QGridLayout(gridLayoutWidget);
            gridLayout->setSpacing(0);
            gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
            gridLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
            gridLayout->setContentsMargins(0, 0, 0, 0);

            // actionPaste
            actionPaste = new QAction(this);
            actionPaste->setObjectName(QString::fromUtf8("actionPaste"));
            actionPaste->setCheckable(true);
            actionPaste->setAutoRepeat(false);
            actionPaste->setText(QCoreApplication::translate("MainWindow", "Paste", nullptr));
            actionPaste->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+V", nullptr));

            // lineEdit
            lineEdit = new QLineEdit(gridLayoutWidget);
            lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
            QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
            sizePolicy1.setHorizontalStretch(0);
            sizePolicy1.setVerticalStretch(0);
            sizePolicy1.setHeightForWidth(lineEdit->sizePolicy().hasHeightForWidth());
            lineEdit->setSizePolicy(sizePolicy1);
            lineEdit->setMinimumSize(QSize(500, 40));
            QFont font1;
            font1.setPointSize(16);
            lineEdit->setFont(font1);
            lineEdit->setFocusPolicy(Qt::NoFocus);
            lineEdit->setAcceptDrops(false);
            lineEdit->setAlignment(Qt::AlignCenter);
            lineEdit->setReadOnly(true);
            lineEdit->setText(QCoreApplication::translate("MainWindow", "Specify path or drag file", nullptr));
            gridLayout->addWidget(lineEdit, 0, 0);

            // fileNameText
            fileNameText = new QLineEdit(gridLayoutWidget);
            fileNameText->setObjectName(QString::fromUtf8("fileNameText"));
            QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Fixed);
            sizePolicy2.setHorizontalStretch(1);
            sizePolicy2.setVerticalStretch(0);
            sizePolicy2.setHeightForWidth(fileNameText->sizePolicy().hasHeightForWidth());
            fileNameText->setSizePolicy(sizePolicy2);
            fileNameText->setMinimumSize(QSize(701, 31));
            QFont font2;
            font2.setPointSize(14);
            fileNameText->setFont(font2);
            fileNameText->setDragEnabled(true);

            fileNameText->setText(QString());
            fileNameText->setPlaceholderText(QCoreApplication::translate("MainWindow", "Specify Path", nullptr));
            const auto effect = new QGraphicsColorizeEffect(this);
            effect->setColor(Qt::red);
            effect->setEnabled(false);
            fileNameText->setGraphicsEffect(effect);
            gridLayout->addWidget(fileNameText, 1, 0);

            // imageLabel
            imageLabel = new QLabel(gridLayoutWidget);
            imageLabel->setObjectName(QString::fromUtf8("imageLabel"));
            imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
            imageLabel->setScaledContents(true);
            imageLabel->setMinimumSize(QSize(500, 500));
            imageLabel->setMaximumSize(QSize(500, 500));
            gridLayout->addWidget(imageLabel, 2, 0);

            // info
            info = new QTextBrowser(gridLayoutWidget);
            info->setObjectName(QString::fromUtf8("info"));
            info->setMinimumSize(QSize(0, 0));
            info->setMaximumSize(QSize(16777215, 100));
            info->setOpenLinks(true);
            info->setOpenExternalLinks(true);
            gridLayout->addWidget(info, 3, 0);

            // pushButton
            pushButton = new QPushButton(gridLayoutWidget);
            pushButton->setObjectName(QString::fromUtf8("pushButton"));
            pushButton->setText(QCoreApplication::translate("MainWindow", "Parse", nullptr));

            gridLayout->addWidget(pushButton, 4, 0);

            // trayIcon
            trayIcon = new QSystemTrayIcon(this);
            trayIcon->setIcon(this->style()->standardIcon(QStyle::SP_ComputerIcon));
            trayIcon->setToolTip(QString::fromLocal8Bit("QR Decoder"));
            auto* menu = new QMenu(this);
            menu->addAction(QString::fromLocal8Bit("Exit"), this, SLOT(close()))
                ->setIcon(this->style()->standardIcon(QStyle::SP_TitleBarCloseButton));

            trayIcon->setContextMenu(menu);
            trayIcon->showMessage(QString::fromLocal8Bit("QR Decoder"),
                                  QString::fromLocal8Bit(
                                      "The application is minimized to the tray."
                                      "In order to expand the application window, click on the application icon in the tray"),
                                  QSystemTrayIcon::Information,
                                  100);

            connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
                    this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
            connect(pushButton, SIGNAL(clicked()), this, SLOT(buttonClicked()));
            connect(fileNameText, SIGNAL(textEdited(QString)), this, SLOT(clearEffects()));

            QMetaObject::connectSlotsByName(this);
        }

        explicit MainWindow(QWidget* parent = nullptr);

        void showQRCode(const QString& file);

        ~MainWindow() override = default;

    protected:
        void closeEvent(QCloseEvent* event) override;

        void dropEvent(QDropEvent* event) override;

        void dragEnterEvent(QDragEnterEvent* event) override;

    private slots:
        void paste();

        void buttonClicked();

        void clearEffects();

        void iconActivated(QSystemTrayIcon::ActivationReason reason);

    protected:
        void timerEvent(QTimerEvent* event) override;

    private:
        QSystemTrayIcon* trayIcon;

        bool parseQr(const QImage& image);

        void keyPressEvent(QKeyEvent* event) override;
};


#endif //QRDECODER_MAINWINDOW_H
