#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QSpinBox>
#include<QFontComboBox>
#include<QLabel>
#include<QActionGroup>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    //自定义槽实现字体和字号的调整
    void fontChanged(int fontSize);
    void fontFamilyChanged(const QFont &font);

    void on_actFile_New_triggered();

    void on_actFile_Open_triggered();

    void on_actFile_Save_triggered();

    void on_textEdit_copyAvailable(bool b);

    void on_actEdit_Clear_triggered();

    void on_actFont_Bold_triggered(bool checked);

    void on_actFont_ltalic_triggered(bool checked);

    void on_actFont_UnderLine_triggered(bool checked);

    void on_textEdit_selectionChanged();

    void on_actSys_ToggleText_triggered(bool checked);

private:
    Ui::MainWindow *ui;
private:
    QSpinBox *sFontSize;//调整字号大小对象
    QFontComboBox *cFontName;//调整字体的对象
    //状态栏上添加两个内容显示；显示文件操作和字体选择
    QLabel *labFile;
    QLabel *labFontName;
    void initUI();//初始化各类控件
    QActionGroup *mGroup;
};

#endif // MAINWINDOW_H
