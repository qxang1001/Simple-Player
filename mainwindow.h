#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QComboBox>
#include <QStyle>
#include <QFileDialog>
#include <QMessageBox>
#include <QTime>

#include "player.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    // Signals to be emitted by UI elements and caught by the Controller
    void playRequested();
    void pauseRequested();
    void fileSelected(const QString &filePath);

private slots:
    void on_Filebutton_clicked();
    void on_Playbutton_clicked();
    void on_Pausebutton_clicked();

    void updateProgress(qint64 position);
    void updateDurationLabel(qint64 duration);
    void updatePositionLabel(qint64 position);
    void updateVolumeLabel(int level);

    void updateUiState(const QString &state);
    void showError(const QString &message);

private:
    Ui::MainWindow *ui;
    Player* player;
};
#endif // MAINWINDOW_H
