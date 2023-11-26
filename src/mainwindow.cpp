//
// Created by nelos on 26.11.2023.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include "mainwindow.h"
#include "ui_MainWindow.h"
#include <QZXing>
#include <QUrl>
#include <QTextStream>
#include <QMimeData>
#include <QMenu>
#include <QStyle>
#include <QGraphicsColorizeEffect>
#include <QMessageBox>
#include <QClipboard>

MainWindow::MainWindow(QWidget *parent):
        QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    setAcceptDrops(true);
    setWindowFlag(Qt::MSWindowsFixedSizeDialogHint);
    setBackgroundRole(QPalette::ColorRole::Dark);

    auto effect = new QGraphicsColorizeEffect(this);
    effect->setColor(Qt::red);
    effect->setEnabled(false);
    ui->fileNameText->setGraphicsEffect(effect);

    ui->info->setOpenLinks(true);
    ui->info->setOpenExternalLinks(true);

    // trayIcon
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(this->style()->standardIcon(QStyle::SP_ComputerIcon));
    trayIcon->setToolTip(QString::fromLocal8Bit("QR Decoder"));


    auto *menu = new QMenu(this);
    menu->addAction(QString::fromLocal8Bit("Exit"), this, SLOT(close()))
            ->setIcon(this->style()->standardIcon(QStyle::SP_TitleBarCloseButton));

    trayIcon->setContextMenu(menu);
    trayIcon->showMessage(QString::fromLocal8Bit("QR Decoder"),
                          QString::fromLocal8Bit("The application is minimized to the tray."
                                                 "In order to expand the application window, click on the application icon in the tray"),
                          QSystemTrayIcon::MessageIcon(QSystemTrayIcon::Information),
                          100);
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

}

void MainWindow::closeEvent(QCloseEvent *event) {
    if (this->isVisible()) {
        event->ignore();
        this->hide();
        trayIcon->show();
    } else {
        event->accept();
    }
}


void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason) {
    switch (reason) {
        case QSystemTrayIcon::Trigger:
            if (!this->isVisible()) {
                this->show();
                trayIcon->hide();
            } else {
                trayIcon->show();
            }
            break;
        default:
            break;
    }
}

void MainWindow::showQRCode(const QString &file) {
    if (!file.isEmpty()) {
        ui->imageLabel->clear();
        if (parseQr(QImage(file))) {
            ui->fileNameText->setText(file);
            return;
        }
    }
    ui->fileNameText->setPlaceholderText(QString::fromLocal8Bit("Please specify path"));
    ui->fileNameText->setFocus();
    ui->fileNameText->graphicsEffect()->setEnabled(true);
}

void MainWindow::buttonClicked() {
    showQRCode(ui->fileNameText->displayText());
}

void MainWindow::dropEvent(QDropEvent *e) {
    if (e->mimeData()->hasUrls()) {
        QString fileName = e->mimeData()->urls().first().toLocalFile();
        ui->fileNameText->setText(fileName);
        showQRCode(fileName);
        e->acceptProposedAction();
    }
    e->ignore();
}


void MainWindow::dragEnterEvent(QDragEnterEvent *event) {
    event->acceptProposedAction();
}


MainWindow::~MainWindow() {
    delete ui;
}

bool MainWindow::parseQr(const QImage &image) {
    if (image.isNull()) {
        return false;
    }
    clearEffects();
    ui->imageLabel->setPixmap(QPixmap::fromImage(image));


    // QR code decoding

    QZXing decoder;
    decoder.setDecoder(
            QZXing::DecoderFormat_QR_CODE |
            QZXing::DecoderFormat_Aztec |
            QZXing::DecoderFormat_CODABAR |
            QZXing::DecoderFormat_CODE_39 |
            QZXing::DecoderFormat_CODE_93 |
            QZXing::DecoderFormat_CODE_128 |
            QZXing::DecoderFormat_DATA_MATRIX |
            QZXing::DecoderFormat_EAN_8 |
            QZXing::DecoderFormat_EAN_13 |
            QZXing::DecoderFormat_ITF |
            QZXing::DecoderFormat_MAXICODE |
            QZXing::DecoderFormat_PDF_417 |
            QZXing::DecoderFormat_RSS_14 |
            QZXing::DecoderFormat_RSS_EXPANDED |
            QZXing::DecoderFormat_UPC_A |
            QZXing::DecoderFormat_UPC_E |
            QZXing::DecoderFormat_UPC_EAN_EXTENSION |
            QZXing::DecoderFormat_CODE_128_GS1
    );
    decoder.setTryHarder(true);
    decoder.setTryHarderBehaviour(QZXing::TryHarderBehaviour_ThoroughScanning | QZXing::TryHarderBehaviour_Rotate);
    auto strQRCode = decoder.decodeImage(image);


    if (!strQRCode.isEmpty()) {
        ui->info->setMarkdown(strQRCode);
    } else {
        ui->info->setTextColor(Qt::red);
        ui->info->setText(QString::fromLocal8Bit("Failed to identify the QR code information!"));
    }
    return true;
}

void MainWindow::clearEffects() {
    ui->fileNameText->graphicsEffect()->setEnabled(false);
}


void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (event->matches(QKeySequence::Paste)) {
        paste();
    }

    if (event->matches(QKeySequence::Close) ||
        event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_Q) {
        close();
        close();
    }
}


void MainWindow::paste() {
    QClipboard *board = QApplication::clipboard();
    auto image = board->image();
    parseQr(image);
}

void MainWindow::timerEvent(QTimerEvent *event) {
    QObject::timerEvent(event);
}


