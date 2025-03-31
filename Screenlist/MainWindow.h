#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QStringListModel>
#include <QHeaderView>
#include <QSplitter>
#include <QMainWindow>
#include <QPushButton>

#include "VideoItem.h"
#include "ProfileList.h"
#include "VdeoListModel.h"
#include "GeneratorThread.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    //actions
    void onAddFiles();
    void onAddFolder();
    void onRemoveSelected();
    void onRemoveFailed();
    void onRemoveCompleted();
    void onRemoveAll();
    void onStartProcessing();
    void onStopProcessing();
    void onExit();
    void onProfileSave();
    void onProfileDelete();
    void onProfilePreview();
    void onShowFullPath(bool checked);
    void onOverwriteFiles(bool checked);
    void onSettings();
    void onGitHub();
    void onAbout();
    void onOpenVideo();
    void onOpenPreview();
    void onBrowseToVideo();
    void onBrowseToPreview();
    void onResetSelected();
    void onResetAll();

    //buttons
    void onHeaderFont();
    void onTimestampFont();
    void onAddOutputDir();
    void onOpenOutputDir();

    //combo boxes
    void onProfileChanged(int index);

    //video list
    void sectionClicked(int logicalIndex);
    void doubleClickedVideoList(const QModelIndex &index);
    void contextMenuVideoList(const QPoint &pos);

    //processing thread events
    void threadNotify(int progress);
    void threadFinished(int result, const QString& result_string);

private:
    Ui::MainWindow *ui;

    //settings
    void SaveSettings();
    void LoadSettings();

    //processing thread
    enum : int
    {//items to process
        PROCESS_NONE,
        PROCESS_ALL,
        PROCESS_SELECTED
    };
    int ProcessingState{PROCESS_NONE};
    CGeneratorThread GeneratorThread;
    PVideoItem GetVideoToProcess();

    //video items list
    //PVideoItem CurrentVideo;
    QHeaderView HeaderView;
    CVideoItemModel VideoItemList;
    PVideoItem GetFocusedVideo();

    //combo boxes
    CStrIntModel HeaderModel;
    CStrIntModel TimestampModel;

    //profiles
    QFont HeaderFont;
    QFont TimestampFont;
    CProfileModel ProfileModel;
    PProfile GetCurrentProfile();
    void UpdateProfileView();
    void SetFontButton(QPushButton* button, const QFont &font);

    //dialogs
    void ShowErrorBox(QString error_text);
    bool QueryYesNo(QString prompt);
    void QuerySaveProfile();

    //overrides
    void closeEvent(QCloseEvent *event) override;
    //void dropEvent(QDropEvent *event) override;

    //controls
    void UpdateOutputDirs(int item_index = -1);
    void SwitchState(int state); //arg - PROCESS_ values

    bool OpenURL(QString url);
};
#endif // MAINWINDOW_H
