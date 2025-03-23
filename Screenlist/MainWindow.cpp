#include <QStandardItemModel>
#include <QItemSelectionModel>
#include <QHeaderView>
#include <QFileDialog>
#include <QFontDialog>
#include <QThread>
#include <QMessageBox>
#include <QDesktopServices>

#include "about.h"
#include "Settings.h"
#include "ProcessingThread.h"
#include "MainWindow.h"
#include "./ui_MainWindow.h"

CStrIntModel::CStrIntModel() : QStringListModel(), DefaultRow{0}
{
}
CStrIntModel::~CStrIntModel()
{
}
int CStrIntModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}
int CStrIntModel::rowCount(const QModelIndex &parent) const
{
    return Data.size();
}
QVariant CStrIntModel::data(const QModelIndex &index, int role) const
{
    if(index.row() >= Data.size())
        return QVariant();
    if(Qt::DisplayRole == role)
        return Data[index.row()].Text;
    return QVariant();
}

void CStrIntModel::setDefaultRow(int def_row)
{
    Q_ASSERT(def_row < Data.size());
    DefaultRow = def_row;
}
int CStrIntModel::getData(int row) const
{
    if(row >= Data.size())
        return Data[DefaultRow].Data;
    return Data[row].Data;
}
int CStrIntModel::getRow(int data) const
{
    int row = 0;
    for(const auto& item : Data)
    {
        if(item.Data == data)
            return row;
        ++row;
    }
    return DefaultRow;
}
void CStrIntModel::addItem(QString text, int data)
{
    Data.push_back(CComboBoxItem(text, data));
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    HeaderView(Qt::Horizontal, this),
    ProcessingItemList(this)
{
    ui->setupUi(this);

    Settings.Load(this);

    //TEST:
    ProcessingItemList.Add("item 1");
    ProcessingItemList.Add("item 2");
    ProcessingItemList.Add("item 3");

    //init header view
    HeaderView.setModel(&ProcessingItemList);
    HeaderView.setSectionResizeMode(QHeaderView::Interactive);
    HeaderView.setSectionsClickable(true);
    HeaderView.setSortIndicatorShown(true);
    HeaderView.setStretchLastSection(true);

    //init table view
    ui->tableView->setHorizontalHeader(&HeaderView);
    ui->tableView->setModel(&ProcessingItemList);
    ui->tableView->verticalHeader()->hide();
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::ExtendedSelection);

    //TODO: init controls
    UpdateOutputDirs();

    //header combo
    HeaderModel.addItem("Disabled", sl::HEADER_DISABLED);
    HeaderModel.addItem("Expanded", sl::HEADER_EXPANDED);
    HeaderModel.addItem("Compact", sl::HEADER_COMPACT);
    HeaderModel.setDefaultRow(1);
    ui->cbHeader->setModel(&HeaderModel);

    //timestamp combo
    TimestampModel.addItem("Disabled", sl::TIMESTAMP_TYPE_DISABLED);
    TimestampModel.addItem("Top-Left", sl::TIMESTAMP_TYPE_TOP_LEFT);
    TimestampModel.addItem("Top-Center", sl::TIMESTAMP_TYPE_TOP_CENTER);
    TimestampModel.addItem("Top-Right", sl::TIMESTAMP_TYPE_TOP_RIGHT);
    TimestampModel.addItem("Bottom-Left", sl::TIMESTAMP_TYPE_BOTTOM_LEFT);
    TimestampModel.addItem("Bottom-Center", sl::TIMESTAMP_TYPE_BOTTOM_CENTER);
    TimestampModel.addItem("Bottom-Right", sl::TIMESTAMP_TYPE_BOTTOM_RIGHT);
    TimestampModel.setDefaultRow(6);
    ui->cbTimestamp->setModel(&TimestampModel);

    //TODO:
    //init current profile
    ui->cbProfiles->setModel(&ProfileModel);
    ui->cbProfiles->setCurrentIndex(0);
    PProfile profile = ProfileModel.getProfile(0);
    ui->cbHeader->setCurrentIndex(HeaderModel.getRow(profile->HeaderType));
    ui->leImageWidth->setText(QString("%1").arg(profile->ImageWidth));
    ui->leGridColumns->setText(QString("%1").arg(profile->GridColumns));
    ui->leGridRows->setText(QString("%1").arg(profile->GridRows));
    ui->cbTimestamp->setCurrentIndex(TimestampModel.getRow(profile->Timestamp));

    //connect actions
    connect(ui->actionAddFiles, &QAction::triggered, this, &MainWindow::onAddFiles);
    connect(ui->actionAddFolder, &QAction::triggered, this, &MainWindow::onAddFolder);
    connect(ui->actionRemoveFailed, &QAction::triggered, this, &MainWindow::onRemoveFailed);
    connect(ui->actionRemoveCompleted, &QAction::triggered, this, &MainWindow::onRemoveCpmpleted);
    connect(ui->actionRemoveAll, &QAction::triggered, this, &MainWindow::onRemoveAll);
    connect(ui->actionStartProcessing, &QAction::triggered, this, &MainWindow::onStartProcessing);
    connect(ui->actionStopProcessing, &QAction::triggered, this, &MainWindow::onStopProcessing);
    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::onExit);
    connect(ui->actionProfileSave, &QAction::triggered, this, &MainWindow::onProfileSave);
    connect(ui->actionProfileDelete, &QAction::triggered, this, &MainWindow::onProfileDelete);
    connect(ui->actionProfilePreview, &QAction::triggered, this, &MainWindow::onProfilePreview);
    connect(ui->actionSettings, &QAction::triggered, this, &MainWindow::onSettings);
    connect(ui->actionGitHub, &QAction::triggered, this, &MainWindow::onGitHub);
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::onAbout);

    //connect buttons
    connect(ui->pbHeaderFont, &QPushButton::clicked, this, &MainWindow::onHeaderFont);
    connect(ui->pbTimestampFont, &QPushButton::clicked, this, &MainWindow::onTimestampFont);
    connect(ui->pbOutputPath, &QPushButton::clicked, this, &MainWindow::onOutputPath);
}
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::onAddFiles()
{
    QFileDialog fd(this);
    fd.setFileMode(QFileDialog::ExistingFiles);
    fd.setAcceptMode(QFileDialog::AcceptOpen);
    fd.setLabelText(QFileDialog::LookIn, "Select video files for processing");
    fd.setNameFilter(FILE_DIALOG_FILTER);

    QStringList fileNames;
    if(fd.exec())
        fileNames = fd.selectedFiles();

    //TODO:
}
void MainWindow::onAddFolder()
{
    //TODO:
}
void MainWindow::onRemoveFailed()
{
    //TODO:
}
void MainWindow::onRemoveCpmpleted()
{
    //TODO:
}
void MainWindow::onRemoveAll()
{
    //TODO:
}
void MainWindow::onStartProcessing()
{
    //TODO:
}
void MainWindow::onStopProcessing()
{
    //TODO:
}
void MainWindow::closeEvent(QCloseEvent *event)
{
    //TODO:
    // QMessageBox mb;
    // mb.setIcon(QMessageBox::Warning);
    // mb.setWindowTitle(APP_NAME);
    // mb.setText("Exit application?");
    // mb.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    // mb.setDefaultButton(QMessageBox::Ok);
    // const bool result = QMessageBox::Ok == mb.exec();

    Settings.Save(this);
    if(event)
        QWidget::closeEvent(event);
    else
        QApplication::exit(0);
}
void MainWindow::onExit()
{
    closeEvent(nullptr);
}
void MainWindow::onProfileSave()
{
    //TODO:
}
void MainWindow::onProfileDelete()
{
    //TODO:
}
void MainWindow::onProfilePreview()
{
    //TODO:
    //sl::generate(nullptr, CurrentProfile,);
    QDesktopServices::openUrl(QUrl("file:///D:/video/trash/data.mp4", QUrl::TolerantMode));
}
void MainWindow::onSettings()
{
    //TODO:
}
void MainWindow::onGitHub()
{
    QDesktopServices::openUrl(QUrl(APP_URL, QUrl::TolerantMode));
}
void MainWindow::onAbout()
{
    QMessageBox mb;
    mb.setIcon(QMessageBox::Information);
    mb.setWindowTitle(APP_NAME);
    mb.setText(APP_FULL_NAME "\n" APP_URL);
    mb.setStandardButtons(QMessageBox::Ok);
    mb.exec();
}
void MainWindow::onHeaderFont()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok, QFont("Times", 10), this);
    if(false == ok)
        return;

    QString font_text = font.family();
    font_text += QString(", %1").arg(font.pointSize());
    ui->pbHeaderFont->setText(font_text);
}
void MainWindow::onTimestampFont()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok, QFont("Times", 10), this);
    if(false == ok)
        return;

    QString font_text = font.family();
    font_text += QString(", %1").arg(font.pointSize());
    ui->pbTimestampFont->setText(font_text);
}
void MainWindow::onOutputPath()
{
    //TODO: Settings.LastOutputDir
    QString dir = QFileDialog::getExistingDirectory(this, "Select directory for output files");
    if(dir.isEmpty())
        return;

    const int item_index = Settings.OutputDirs.Add(dir);
    if(item_index < 0)
        return;

    UpdateOutputDirs(item_index);
}
void MainWindow::UpdateOutputDirs(int item_index /*= 0*/)
{
    ui->cbOutputDir->clear();
    for(const auto& i : Settings.OutputDirs)
        ui->cbOutputDir->addItem(i);
    if(ui->cbOutputDir->count() <= item_index)
        item_index = 0;
    ui->cbOutputDir->setCurrentIndex(item_index);
}
