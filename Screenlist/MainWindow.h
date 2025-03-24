#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QStringListModel>
#include <QHeaderView>
#include <QSplitter>
#include <QMainWindow>
#include <QPushButton>

#include "sl_interface.h"
#include "VideoItem.h"
#include "ProfileList.h"
#include "VdeoListModel.h"
#include "ProcessingThread.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

//data model for combo boxes
class CStrIntModel : public QStringListModel
{
public:
    CStrIntModel();
    ~CStrIntModel();

    //overrides
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void setDefaultRow(int def_row);
    void addItem(QString text, int data);

    int getRow(int data) const;
    int getData(int row) const;

private:
    int DefaultRow;
    class CComboBoxItem
    {
    public:
        CComboBoxItem(QString text, int data) : Text{text}, Data{data} {}
        QString Text;
        int Data;
    };
    std::vector<CComboBoxItem> Data;
};

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

    QHeaderView HeaderView;
    CVideoItemModel ProcessingItemList;
    CProcessingThread ProcessingThread;

    void ShowErrorBox(QString error_text);

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

    //video items
    PVideoItem GetVideoToProcess();

    //event overrides
    void closeEvent(QCloseEvent *event) override;

    //controls
    void UpdateOutputDirs(int item_index = 0);
};
#endif // MAINWINDOW_H
