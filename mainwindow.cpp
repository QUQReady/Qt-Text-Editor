#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QDir>
#include<QFileDialog>
#include<QFile>
#include<QMessageBox>
#include<QTextStream>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initUI();
    //CN和EN两个Action的互斥操作
    mGroup=new QActionGroup(this);
    mGroup->addAction(ui->actLang_CN);
    mGroup->addAction(ui->actLang_EN);
    mGroup->setExclusive(true);
    //窗口加载时，只显示图标
    ui->toolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);

    //连接自定义槽实现字号大小调整
    connect(sFontSize,&QSpinBox::valueChanged,this,&MainWindow::fontChanged);

    //连接字体的自定义槽和信号
    connect(cFontName,&QFontComboBox::currentFontChanged,this,&MainWindow::fontFamilyChanged);

    // connect(mGroup, &QActionGroup::triggered, this, [=](QAction *act){
    //     if (act == ui->actLang_CN) {
    //         // 这里写加载中文翻译的逻辑
    //         qDebug() << "切换到了中文";
    //     } else if (act == ui->actLang_EN) {
    //         // 这里写加载英文翻译的逻辑
    //         qDebug() << "Switched to English";
    //     }
    // });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::fontChanged(int fontSize)
{
    QTextCharFormat ft=ui->textEdit->currentCharFormat();
    ft.setFontPointSize(fontSize);
    ui->textEdit->mergeCurrentCharFormat(ft);
}
//字体调整
void MainWindow::fontFamilyChanged(const QFont &font)
{
    QTextCharFormat ft=ui->textEdit->currentCharFormat();
    ft.setFont(font);
    ui->textEdit->mergeCurrentCharFormat(ft);
}

void MainWindow::initUI()
{
    //字号调整控件
    sFontSize=new QSpinBox(this);
    sFontSize->setMinimumWidth(80);
    ui->toolBar->addWidget(sFontSize);
    //字体选择控件
    cFontName=new QFontComboBox(this);
    cFontName->setMinimumWidth(100);
    ui->toolBar->addWidget(cFontName);
    //添加一个分隔符
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(ui->actCloss);
    //状态栏中的两个内容显示QLabel
    labFile=new QLabel(this);
    labFile->setMinimumWidth(150);
    labFile->setText("文件操作：");
    ui->statusbar->addWidget(labFile);

    labFontName=new QLabel(this);
    labFontName->setMinimumWidth(150);
    labFontName->setText("选择字体：");
    ui->statusbar->addPermanentWidget(labFontName);


}

void MainWindow::on_actFile_New_triggered()
{
    ui->textEdit->clear();
    ui->textEdit->document()->setModified(false);
    labFile->setText("文件操作：新建文件");
}

//打开文件
void MainWindow::on_actFile_Open_triggered()
{
    QString curPath=QDir::currentPath();//获取当前路径
    QString filter="C/C++程序(*.h *.cpp);;""文本文件(*.txt);;""所有文件(*.*)";
    QString dlgTitle="打开文件";
    QString aFileName=QFileDialog::getOpenFileName(this,dlgTitle,curPath,filter);
    if(aFileName.isEmpty())
    {
        return ;
    }
    QFile aFile(aFileName);
    if(!aFile.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        QMessageBox::warning(this,"错误","打开文件失败："+aFile.errorString());
        return;
    }
    QTextStream aStream(&aFile);//文件流
    QString txt=aStream.readAll();//读的过程
    aFile.close();

    ui->textEdit->setPlainText(txt);//读的过程
    ui->textEdit->document()->setModified(false);

    labFile->setText("文件操作：打开文件"+aFileName);
    setWindowTitle(aFileName);
}

//保存文件
void MainWindow::on_actFile_Save_triggered()
{
    QString curPath=QDir::currentPath();//获取当前路径
    QString filter="C/C++程序(*.h *.cpp);;""文本文件(*.txt);;""所有文件(*.*)";
    QString dlgTitle="保存文件";
    QString aFileName=QFileDialog::getSaveFileName(this,dlgTitle,curPath,filter);
    if(aFileName.isEmpty())
    {
        return ;
    }
    QFile aFile(aFileName);
    if(!aFile.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        QMessageBox::warning(this,"错误","保存文件失败："+aFile.errorString());
        return;
    }
    QTextStream aStream(&aFile);
    aStream<<ui->textEdit->toPlainText();//把文本内容写入文本流
    //QString txt=aStream.readAll();
    aFile.close();

    //ui->textEdit->setPlainText(txt);
    ui->textEdit->document()->setModified(false);

    labFile->setText("文件操作：打开文件"+aFileName);
    setWindowTitle(aFileName);
}


void MainWindow::on_textEdit_copyAvailable(bool b)
{
    ui->actEdit_Paste->setEnabled(ui->textEdit->canPaste());
}

//使用全选+删除的方式实现清空效果，这样撤销操作才有用
void MainWindow::on_actEdit_Clear_triggered()
{
    QTextCursor cursor=ui->textEdit->textCursor();
    cursor.select(QTextCursor::Document);//全选
    cursor.removeSelectedText();//删除
}

//粗体
void MainWindow::on_actFont_Bold_triggered(bool checked)
{
    QTextCharFormat ft=ui->textEdit->currentCharFormat();
    if(checked)
        ft.setFontWeight(QFont::Bold);
    else
        ft.setFontWeight(QFont::Normal);
    ui->textEdit->mergeCurrentCharFormat(ft);
}


void MainWindow::on_actFont_ltalic_triggered(bool checked)
{
    QTextCharFormat ft=ui->textEdit->currentCharFormat();
    ft.setFontItalic(checked);
    ui->textEdit->mergeCurrentCharFormat(ft);
}


void MainWindow::on_actFont_UnderLine_triggered(bool checked)
{
    QTextCharFormat ft=ui->textEdit->currentCharFormat();
    ft.setFontUnderline(checked);
    ui->textEdit->mergeCurrentCharFormat(ft);
}

//光标在文本编辑框位置发生变化时，更新粗体，斜体，下划线三个按钮的checked状态
void MainWindow::on_textEdit_selectionChanged()
{
    QTextCharFormat ft=ui->textEdit->currentCharFormat();
    ui->actFont_Bold->setChecked(ft.font().bold());
    ui->actFont_ltalic->setChecked(ft.font().italic());
    ui->actFont_UnderLine->setChecked(ft.font().underline());
}


void MainWindow::on_actSys_ToggleText_triggered(bool checked)
{
    if(checked)
        ui->toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    else
        ui->toolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);
}

