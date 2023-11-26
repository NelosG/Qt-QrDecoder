//
// Created by nelos on 26.11.2023.
//

#ifndef QRDECODER_MAINWINDOW_H
#define QRDECODER_MAINWINDOW_H

#include <QMainWindow>
#include <QDragEnterEvent>
#include <QSystemTrayIcon>
#include <QWidget>
#include <QString>
#include <QCloseEvent>
#include <QDropEvent>
#include <QImage>
#include <QKeyEvent>
#include <QFile>
#include <QAction>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    void showQRCode(const QString &file);


    ~MainWindow() override;

protected:
    void closeEvent(QCloseEvent *event) override;

    void dropEvent(QDropEvent *event) override;

    void dragEnterEvent(QDragEnterEvent *event) override;

private slots:

    void paste();

    void buttonClicked();

    void clearEffects();

    void iconActivated(QSystemTrayIcon::ActivationReason reason);

protected:
    void timerEvent(QTimerEvent *event) override;

private:

    Ui::MainWindow *ui;
    QSystemTrayIcon *trayIcon;

    bool parseQr(const QImage& image);

    void keyPressEvent(QKeyEvent *event) override;

};


#endif //QRDECODER_MAINWINDOW_H
