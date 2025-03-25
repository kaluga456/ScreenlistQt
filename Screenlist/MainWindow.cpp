#include <QDebug>
#include <QStandardPaths>
#include <QStandardItemModel>
#include <QItemSelectionModel>
#include <QHeaderView>
#include <QInputDialog>
#include <QFileDialog>
#include <QFontDialog>
#include <QThread>
#include <QMessageBox>
#include <QDesktopServices>

#include "about.h"
#include "sl_options.h"
#include "Settings.h"
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
    VideoItemList(this)
{
    ui->setupUi(this);

    Settings.Load(this);

    //TEST:
    VideoItemList.Add("D:/video/trash/data.mp4");
    VideoItemList.Add("item 1");
    VideoItemList.Add("item 2");
    VideoItemList.Add("item 3");

    //init header view
    HeaderView.setModel(&VideoItemList);
    HeaderView.setSectionResizeMode(QHeaderView::Interactive);
    HeaderView.setSectionsClickable(true);
    HeaderView.setSortIndicatorShown(true);
    HeaderView.setStretchLastSection(true);

    //init table view
    ui->tableView->setHorizontalHeader(&HeaderView);
    ui->tableView->setModel(&VideoItemList);
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

    //init profiles
    ProfileModel.Load();
    ui->cbProfiles->setModel(&ProfileModel);

    //TODO: read from settings
    const int row = ProfileModel.getProfileRow(Settings.ProfileName);
    ui->cbProfiles->setCurrentIndex(row);
    UpdateProfileView();

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

    //connect processing thread events
    connect(&GeneratorThread, &CGeneratorThread::threadNotify, this, &MainWindow::threadNotify);
    connect(&GeneratorThread, &CGeneratorThread::threadFinished, this, &MainWindow::threadFinished);

    connect(ui->cbProfiles, &QComboBox::currentIndexChanged, this, &MainWindow::onProfileChanged);

    //
    connect(&HeaderView, &QHeaderView::sectionClicked, this, &MainWindow::sectionClicked);


    setWindowTitle(APP_FULL_NAME);
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
void MainWindow::sectionClicked(int logicalIndex)
{
    //TODO:
    static bool video_accending = true;
    static bool state_accending = true;
    if(CVideoItemModel::COLUMN_VIDEO == logicalIndex)
    {
        VideoItemList.sort(CVideoItemModel::COLUMN_VIDEO, video_accending ? Qt::AscendingOrder : Qt::DescendingOrder);
        video_accending = !video_accending;
    }
    else if(CVideoItemModel::COLUMN_STATE == logicalIndex)
    {
        VideoItemList.sort(CVideoItemModel::COLUMN_STATE, state_accending ? Qt::AscendingOrder : Qt::DescendingOrder);
        state_accending = !state_accending;
    }
}
void MainWindow::onStartProcessing()
{
    if(GeneratorThread.isRunning())
        return;

    PVideoItem video_item = GetVideoToProcess();
    if(nullptr == video_item)
        return;

    GeneratorThread.Stop();
    PProfile profile = GetCurrentProfile();
    sl::COptions options;
    CurrentVideo = video_item;
    GeneratorThread.Start(video_item, profile, options);
}
void MainWindow::onStopProcessing()
{
    GeneratorThread.Stop();
}
void MainWindow::threadNotify(int progress)
{
    //TODO:
    qDebug() << "threadNotify: " << progress;
    if(CurrentVideo)
        CurrentVideo->State = progress;

    VideoItemList.Update(CurrentVideo.get(), false);
}
void MainWindow::threadFinished(int result, const QString &result_string)
{
    //TODO:
    qDebug() << "threadFinished: " << result_string;
    if(CurrentVideo)
    {
        CurrentVideo->State = result;
        CurrentVideo->ResultString = result_string;
        VideoItemList.Update(CurrentVideo.get());
    }
}
void MainWindow::closeEvent(QCloseEvent *event)
{
    if(true == GeneratorThread.isRunning())
    {
        if(false == QueryYesNo("Stop current processing?"))
            return;
    }
    QuerySaveProfile();

    Settings.ProfileName = ui->cbProfiles->currentText();
    Settings.Save(this);
    ProfileModel.Save();

    if(event)
        QWidget::closeEvent(event);
    else
        QApplication::exit(0);
}
void MainWindow::onExit()
{
    closeEvent(nullptr);
}
PProfile MainWindow::GetCurrentProfile()
{
    PProfile result(new CProfile);
    result->HeaderType = HeaderModel.getData(ui->cbHeader->currentIndex());
    result->ImageWidth = ui->leImageWidth->text().toInt();
    result->GridColumns = ui->leGridColumns->text().toInt();
    result->GridRows= ui->leGridRows->text().toInt();
    result->Timestamp = TimestampModel.getData(ui->cbTimestamp->currentIndex());
    result->HeaderFont = HeaderFont;
    result->TimestampFont = TimestampFont;
    return result;
}
void MainWindow::UpdateProfileView()
{
    PProfile profile = ProfileModel.getProfile(ui->cbProfiles->currentIndex());
    if(nullptr == profile)
        return;

    ui->cbHeader->setCurrentIndex(HeaderModel.getRow(profile->HeaderType));
    ui->leImageWidth->setText(QString("%1").arg(profile->ImageWidth));
    ui->leGridColumns->setText(QString("%1").arg(profile->GridColumns));
    ui->leGridRows->setText(QString("%1").arg(profile->GridRows));
    ui->cbTimestamp->setCurrentIndex(TimestampModel.getRow(profile->Timestamp));
    SetFontButton(ui->pbHeaderFont, profile->HeaderFont);
    SetFontButton(ui->pbTimestampFont, profile->TimestampFont);
}
void MainWindow::QuerySaveProfile()
{
    PProfile current = ProfileModel.getCurrentProfile();
    if(nullptr == current)
        return;

    //no changes to current profile
    PProfile changed = MainWindow::GetCurrentProfile();
    if(true == current->Compare(*changed))
        return;

    if(false == QueryYesNo("Save profile \"" + current->Name + "\" ?"))
        return;

    changed->Name = current->Name;
    ProfileModel.setProfile(ProfileModel.GetCurrentRow(), changed);
}
void MainWindow::onProfileChanged(int index)
{
    //QuerySaveProfile();
    ProfileModel.SetCurrentRow(index);
    UpdateProfileView();
}
void MainWindow::onProfileSave()
{
    QString profile_name = ui->cbProfiles->currentText();

    QInputDialog dlg;
    dlg.setWindowTitle(APP_NAME);
    dlg.setInputMode(QInputDialog::TextInput);
    dlg.setTextValue(profile_name);
    dlg.setLabelText("Save current profile?");
    dlg.exec();
    if(QMessageBox::Accepted != dlg.result())
        return;

    //don`t save profile with empty name
    profile_name = dlg.textValue().trimmed();
    if(profile_name.isEmpty())
        return;

    const int row = ProfileModel.getProfileRow(profile_name);
    PProfile current = GetCurrentProfile();

    if(row < 0) //new profile
    {
        const int new_row = ProfileModel.addProfile(profile_name, current);

        //TODO: don`t save existing profile
        ui->cbProfiles->setCurrentIndex(new_row);
        return;
    }

    //existing profile
    current->Name = profile_name;
    ProfileModel.setProfile(row, current);
}
void MainWindow::onProfileDelete()
{
    QString profile_name = ui->cbProfiles->currentText();
    if(profile_name.isEmpty())
        return;

    const int row = ProfileModel.getProfileRow(profile_name);
    if(row < 0)
        return;
    if(false == QueryYesNo("Delete profile \"" + profile_name + "\" ?"))
        return;
    ProfileModel.deleteProfile(row);
}
void MainWindow::onProfilePreview()
{
    const CProfile* profile = GetCurrentProfile().get();
    sl::COptions options;
    options.OverwriteFiles = true;
    options.OutputPath = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
    QString result_string;
    if(sl::RESULT_SUCCESS != sl::generate(nullptr, *profile, options, result_string))
    {
        ShowErrorBox("Can`t make preview image\n" + result_string);
        return;
    }
    QString file_path = options.OutputPath + "//" + sl::PREVIEW_FILE_NAME;
    QDesktopServices::openUrl(QUrl(file_path, QUrl::TolerantMode));
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
    QMessageBox dlg;
    dlg.setIcon(QMessageBox::Information);
    dlg.setWindowTitle(APP_NAME);
    dlg.setText(APP_FULL_NAME "\n" APP_URL);
    dlg.setStandardButtons(QMessageBox::Ok);
    dlg.exec();
}
void MainWindow::onHeaderFont()
{
    bool ok = false;
    QFont font = QFontDialog::getFont(&ok, HeaderFont, this);
    if(false == ok)
        return;
    SetFontButton(ui->pbHeaderFont, font);
}
void MainWindow::onTimestampFont()
{
    bool ok = false;
    QFont font = QFontDialog::getFont(&ok, TimestampFont, this);
    if(false == ok)
        return;
    SetFontButton(ui->pbTimestampFont, font);
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
void MainWindow::SetFontButton(QPushButton* button, const QFont &font)
{
    QFont pb_font = font;
    pb_font.setPointSize(8);
    QString pb_text = QString("%1, ").arg(font.family());
    pb_text += QString("%1").arg(font.pointSize());
    button->setFont(pb_font);
    button->setText(pb_text);

    if(button == ui->pbHeaderFont)
        HeaderFont = font;
    else if(button == ui->pbTimestampFont)
        TimestampFont = font;
}
PVideoItem MainWindow::GetVideoToProcess()
{
    const int count = VideoItemList.rowCount();
    for(int row = 0; row < count; ++row)
    {
        PVideoItem video = VideoItemList.Get(row);
        Q_ASSERT(video);
        if(video->State != PIS_WAIT)
            continue;
        return video;
    }

    return nullptr;
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
void MainWindow::ShowErrorBox(QString error_text)
{
    QMessageBox dlg;
    dlg.setIcon(QMessageBox::Warning);
    dlg.setWindowTitle(APP_NAME);
    dlg.setText(error_text);
    dlg.setStandardButtons(QMessageBox::Ok);
    dlg.setDefaultButton(QMessageBox::Ok);
    dlg.exec();
}
bool MainWindow::QueryYesNo(QString prompt)
{
    QMessageBox dlg;
    dlg.setIcon(QMessageBox::Warning);
    dlg.setWindowTitle(APP_NAME);
    dlg.setText(prompt);
    dlg.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    dlg.setDefaultButton(QMessageBox::Ok);
    return QMessageBox::Ok == dlg.exec();
}
