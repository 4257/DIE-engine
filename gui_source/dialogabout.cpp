// Copyright (c) 2020 hors<horsicq@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
#include "dialogabout.h"
#include "ui_dialogabout.h"

struct USERINFO
{
    QString sUser;
    QString sImage;
};

USERINFO _userinfo_records[]=
{
    {QString("Nick Sm"),                QString("NickSM")},
    {QString("hypn0"),                  QString("hypn0")}
};

DialogAbout::DialogAbout(QWidget *pParent) :
    QDialog(pParent),
    ui(new Ui::DialogAbout)
{
    ui->setupUi(this);

    ui->tabWidgetAbout->setCurrentIndex(0);

    QString sInfoText=ui->labelInfo->text();

    sInfoText=sInfoText.replace("#VERSION#",QString("%1 v%2").arg(X_APPLICATIONNAME).arg(X_APPLICATIONVERSION));
    sInfoText=sInfoText.replace("#DATE#",__DATE__);

    ui->labelInfo->setText(sInfoText);

    QString sComponentsText=ui->labelComponents->text();

    sComponentsText=sComponentsText.replace("#QT#",QT_VERSION_STR);
    sComponentsText=sComponentsText.replace("#QWT#",QWT_VERSION_STR);
    sComponentsText=sComponentsText.replace("#CAPSTONE#",QString("%1.%2.%3").arg(CS_VERSION_MAJOR).arg(CS_VERSION_MINOR).arg(CS_VERSION_EXTRA));

    ui->labelComponents->setText(sComponentsText);

    int nCount=sizeof(_userinfo_records)/sizeof(USERINFO);

    {
        QSignalBlocker blocker1(ui->listWidgetThanks);

        for(int i=0;i<nCount;i++)
        {
            QListWidgetItem *pItem=new QListWidgetItem;
            pItem->setText(_userinfo_records[i].sUser);
            pItem->setData(Qt::UserRole,QString("://images/avatars/%1.bmp").arg(_userinfo_records[i].sImage));

            ui->listWidgetThanks->insertItem(i,pItem);
        }
    }

    if(nCount)
    {
        // TODO random
        ui->listWidgetThanks->setCurrentRow(0);
    }
}

DialogAbout::~DialogAbout()
{
    delete ui;
}

void DialogAbout::on_pushButtonOK_clicked()
{
    this->close();
}

void DialogAbout::on_pushButtonCheckUpdates_clicked()
{
    QDesktopServices::openUrl(QUrl(X_UPDATEPAGE));
}

void DialogAbout::on_listWidgetThanks_currentItemChanged(QListWidgetItem *pCurrent, QListWidgetItem *pPrevious)
{
    Q_UNUSED(pPrevious)

    if(pCurrent)
    {
        QString sImagePath=pCurrent->data(Qt::UserRole).toString();

        QPixmap pixmap(sImagePath);
        QIcon buttonIcon(pixmap);
        ui->pushButtonAvatar->setIcon(buttonIcon);
        ui->pushButtonAvatar->setIconSize(pixmap.rect().size());
    }
}
