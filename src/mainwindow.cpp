//
// Created by nelos on 26.11.2023.
//

#include "mainwindow.h"

#include <iostream>
#include <ostream>
#include <QClipboard>
#include <QGraphicsColorizeEffect>
#include <QMenu>
#include <QMimeData>
#include <QStyle>
#include <QUrl>
#include <QString>
#include <QZXing>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setupUi();
}

void MainWindow::closeEvent(QCloseEvent* event) {
    if (this->isVisible()) {
        event->ignore();
        this->hide();
        trayIcon->show();
    } else {
        event->accept();
    }
}


void MainWindow::iconActivated(const QSystemTrayIcon::ActivationReason reason) {
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

QString adjustFilePath(QString file) {
    return file.remove(QString("file:///"));
}

void MainWindow::showQRCode(const QString& file) {
    if (!file.isEmpty()) {
        imageLabel->clear();

        QString filteredFilePath = adjustFilePath(file);
        if (parseQr(QImage(filteredFilePath))) {
            fileNameText->setText(filteredFilePath);
            return;
        }
    }
    fileNameText->setPlaceholderText(QString::fromLocal8Bit("Please specify path"));
    fileNameText->setFocus();
    fileNameText->graphicsEffect()->setEnabled(true);
}

void MainWindow::buttonClicked() {
    showQRCode(fileNameText->displayText());
}

void MainWindow::dropEvent(QDropEvent* e) {
    if (e->mimeData()->hasUrls()) {
        QString fileName = e->mimeData()->urls().first().toLocalFile();
        fileNameText->setText(fileName);
        showQRCode(fileName);
        e->acceptProposedAction();
    }
    e->ignore();
}


void MainWindow::dragEnterEvent(QDragEnterEvent* event) {
    event->acceptProposedAction();
}

bool MainWindow::parseQr(const QImage& image) {
    if (image.isNull()) {
        return false;
    }
    clearEffects();
    imageLabel->setPixmap(QPixmap::fromImage(image));


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
        info->setMarkdown(strQRCode);
    } else {
        info->setTextColor(Qt::red);
        info->setText(QString::fromLocal8Bit("Failed to identify the QR code information!"));
    }
    return true;
}

void MainWindow::clearEffects() {
    fileNameText->graphicsEffect()->setEnabled(false);
}


void MainWindow::keyPressEvent(QKeyEvent* event) {
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
    const QClipboard* board = QApplication::clipboard();
    const auto image = board->image();

    if (!image.isNull()) {
        parseQr(image);
    } else {
        const auto path = board->text();
        fileNameText->setText(path);
        showQRCode(path);
    }
}

void MainWindow::timerEvent(QTimerEvent* event) {
    QObject::timerEvent(event);
}
