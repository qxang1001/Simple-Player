#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Qt GStreamer Player");
    resize(800, 600);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QString appStyle = R"(
        /* ========== 主窗口和通用控件 ========== */
        QMainWindow {
            background-color: #1e1e1e;
        }
        QWidget {
            background-color: transparent; /* 让子控件默认透明，继承父窗口背景 */
            color: #f0f0f0; /* 全局文字颜色 */
        }
        QLabel {
            color: #f0f0f0;
            font-size: 15px;
            qproperty-alignment: 'AlignCenter';
        }

        /* ========== 按钮样式 ========== */
        QPushButton {
            background-color: #3a3a3a;
            border: 1px solid #555;
            color: #f0f0f0;
            padding: 5px;
            border-radius: 4px;
            font-weight: bold; /* 加粗字体 */
        }
        QPushButton:hover {
            background-color: #4a4a4a;
            border-color: #777;
        }
        QPushButton:pressed {
            background-color: #2a2a2a;
        }
        QPushButton:disabled {
            background-color: #2a2a2a;
            color: #666;
            border-color: #444;
        }

        /* ========== 滑块样式 ========== */
        /* --- 水平滑块 (带进度填充效果) --- */
        QSlider::groove:horizontal {
            border: none;
            height: 8px;
            background: #444;
            border-radius: 4px;
        }
        QSlider::sub-page:horizontal {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #4a9eff, stop:1 #7bb3f0);
            border-radius: 4px;
        }
        QSlider::handle:horizontal {
            background: #ffffff;
            border: 1px solid #4a9eff;
            width: 16px;
            margin: -5px 0; /* 让滑块柄更大，更容易点击 */
            border-radius: 8px;
        }

        /* --- 垂直滑块 --- */
        QSlider::groove:vertical {
            border: none;
            width: 8px;
            background: #444;
            border-radius: 4px;
        }
        QSlider::sub-page:vertical {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #4a9eff, stop:1 #7bb3f0);
            border-radius: 4px;
        }
        QSlider::handle:vertical {
            background: #ffffff;
            border: 1px solid #4a9eff;
            height: 16px;
            margin: 0 -5px;
            border-radius: 8px;
        }

        /* ========== 下拉框样式 ========== */
        QComboBox {
            background-color: #3a3a3a;
            border: 1px solid #555;
            color: #f0f0f0;
            padding: 3px 5px;
            border-radius: 4px;
            min-width: 80px; /* 设置最小宽度 */
        }
        QComboBox:hover {
            border-color: #777;
        }
        QComboBox::drop-down {
            subcontrol-origin: padding;
            subcontrol-position: top right;
            width: 15px;
            border-left-width: 1px;
            border-left-color: #555;
            border-left-style: solid;
            border-top-right-radius: 4px;
            border-bottom-right-radius: 4px;
            background-color: #444;
        }
        QComboBox::down-arrow {
            image: none; /* 不使用图片 */
            width: 8px;
            height: 8px;
            border-style: solid;
            border-width: 0px 4px 6px 4px;
            border-color: transparent transparent #f0f0f0 transparent; /* 用CSS画一个三角形 */
        }
        QComboBox QAbstractItemView {
            background-color: #3a3a3a;
            border: 1px solid #555;
            color: #f0f0f0;
            selection-background-color: #4a9eff;
            outline: none;
        }
    )";

    this->setStyleSheet(appStyle);

    mainLayout->addWidget(ui->videoWidget, 1); // 拉伸因子为1，占据大部分空间

    QWidget *controlPanel = new QWidget();
    controlPanel->setMaximumHeight(100); // 限制控制面板高度
    controlPanel->setStyleSheet("background-color: #2b2b2b; border-radius: 5px;");

    QVBoxLayout *controlLayout = new QVBoxLayout(controlPanel);

    QHBoxLayout *progressLayout = new QHBoxLayout();
    progressLayout->addWidget(ui->currenttime);
    progressLayout->addWidget(ui->progress, 1); // 进度条占据剩余空间
    progressLayout->addWidget(ui->totaltime);

    QHBoxLayout *buttonLayout = new QHBoxLayout();

    QHBoxLayout *leftControls = new QHBoxLayout();
    leftControls->addWidget(ui->Filebutton);
    leftControls->addWidget(ui->Playbutton);
    leftControls->addWidget(ui->Pausebutton);

    QHBoxLayout *centerControls = new QHBoxLayout();
    centerControls->addStretch(); // 添加弹性空间
    centerControls->addWidget(new QLabel("速度:"));
    centerControls->addWidget(ui->speedComboBox);
    centerControls->addStretch();

    QHBoxLayout *rightControls = new QHBoxLayout();
    rightControls->addWidget(new QLabel("音量:"));
    ui->volume->setOrientation(Qt::Horizontal);
    rightControls->addWidget(ui->volume); // 改为水平音量滑块
    rightControls->addWidget(ui->volumeLabel);

    buttonLayout->addLayout(leftControls);
    buttonLayout->addStretch();
    buttonLayout->addLayout(centerControls);
    buttonLayout->addStretch();
    buttonLayout->addLayout(rightControls);

    controlLayout->addLayout(progressLayout);
    controlLayout->addLayout(buttonLayout);

    mainLayout->addWidget(controlPanel);

    QWidget *centralWidget = new QWidget();
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    ui->Playbutton->setEnabled(false);
    ui->Pausebutton->setEnabled(false);
    ui->currenttime->setText("00:00:00");
    ui->totaltime->setText("00:00:00");
    ui->progress->setRange(0, 100);
    ui->volume->setRange(0, 100);
    ui->volume->setValue(50);
    ui->volumeLabel->setText(QString("Volume: %1%").arg(50));

    ui->speedComboBox->addItem("0.5x", 0.5);
    ui->speedComboBox->addItem("1.0x", 1.0);
    ui->speedComboBox->addItem("1.5x", 1.5);
    ui->speedComboBox->addItem("2.0x", 2.0);
    ui->speedComboBox->setCurrentText("1.0x");

    player = new Player(ui->videoWidget,this);

    connect(this, &MainWindow::fileSelected, player, &Player::setUri);
    connect(this, &MainWindow::playRequested, player, &Player::play);
    connect(this, &MainWindow::pauseRequested, player, &Player::pause);

    connect(player, &Player::durationChanged, this, &MainWindow::updateDurationLabel);
    connect(player, &Player::positionChanged, this, &MainWindow::updatePositionLabel);
    connect(player, &Player::positionChanged, this, &MainWindow::updateProgress);

    connect(ui->speedComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int index) {
        double rate = ui->speedComboBox->itemData(index).toDouble();
        player->setPlaybackRate(rate);
    });

    connect(ui->progress, &QSlider::sliderPressed, player, &Player::onSliderPressed);
    connect(ui->progress, &QSlider::sliderReleased, this, [this]() {
        player->onSliderReleased(ui->progress->value());
    });

    connect(ui->volume, &QSlider::valueChanged, player, &Player::setVolume);
    connect(ui->volume, &QSlider::valueChanged, this, &MainWindow::updateVolumeLabel);

    connect(player, &Player::stateChanged, this, &MainWindow::updateUiState);
    connect(player, &Player::error, this, &MainWindow::showError);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Filebutton_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Open Video File", "", "Video Files (*.mp4 *.avi *.mkv *.mov);;All Files (*)");
    if (!filePath.isEmpty()) {
        // Enable play/pause buttons after a file is selected
        ui->Playbutton->setEnabled(true);
        ui->Pausebutton->setEnabled(false);
        emit fileSelected(filePath);
    }
}

void MainWindow::on_Playbutton_clicked()
{
    emit playRequested();
}

void MainWindow::on_Pausebutton_clicked()
{
    emit pauseRequested();
}

void MainWindow::updateDurationLabel(qint64 duration)
{
    QTime durTime(0, 0, 0, 0);
    durTime = durTime.addMSecs(duration);
    ui->totaltime->setText(durTime.toString("hh:mm:ss"));
    //qDebug() << "totaltime changed to:" << duration;
}

void MainWindow::updateProgress(qint64 position)
{
    qint64 duration = player->get_duration();
    if(duration != 0)
    {
        int progress = static_cast<int>((position * 100) / duration);
        if (progress < 0) progress = 0;
        if (progress > 100) progress = 100;
        ui->progress->setValue(progress);
    }
}

void MainWindow::updatePositionLabel(qint64 position)
{
    QTime posTime(0, 0, 0, 0);
    posTime = posTime.addMSecs(position);
    ui->currenttime->setText(posTime.toString("hh:mm:ss"));
    //qDebug() << "currenttime changed to:" << position;
}

void MainWindow::updateVolumeLabel(int level)
{
    ui->volumeLabel->setText(QString("Volume: %1%").arg(level));
}


void MainWindow::updateUiState(const QString &state)
{
    if (state == "PLAYING") {
        //player->play();
        ui->Playbutton->setEnabled(false);
        ui->Pausebutton->setEnabled(true);
    }
    else if (state == "PAUSED") {
        //player->pause();
        ui->Playbutton->setEnabled(true);
        ui->Pausebutton->setEnabled(false);
    }
    else if (state == "NULL") {
        ui->Playbutton->setEnabled(false);
        ui->Pausebutton->setEnabled(false);
    }
}

void MainWindow::showError(const QString &message)
{
    statusBar()->showMessage(message, 10000);
    QMessageBox::critical(this, // 父窗口
                          "Playback Error", // 对话框标题
                          message);         // 对话框内容
    updateUiState("NULL");
}
