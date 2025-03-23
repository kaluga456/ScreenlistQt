#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QHeaderView>
#include <QSplitter>
#include <QMainWindow>

#include "sl_interface.h"
#include "VideoItem.h"
#include "VdeoListModel.h"


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
    void onRemoveFailed();
    void onRemoveCpmpleted();
    void onRemoveAll();
    void onStartProcessing();
    void onStopProcessing();
    void onExit();
    void onProfileSave();
    void onProfileDelete();
    void onProfilePreview();
    void onSettings();
    void onGitHub();
    void onAbout();

    //buttons
    void onHeaderFont();
    void onTimestampFont();
    void onOutputPath();

private:
    Ui::MainWindow *ui;

    //TODO:
    //QItemSelectionModel SelectionModel;
    QHeaderView HeaderView;
    CProcessingItemList ProcessingItemList;

    //profiles
    sl::CProfile CurrentProfile;

    //event overrides
    void closeEvent(QCloseEvent *event) override;

    //controls
    void UpdateOutputDirs(int item_index = 0);
};
#endif // MAINWINDOW_H
