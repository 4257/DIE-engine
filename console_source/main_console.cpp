/* Copyright (c) 2020-2021 hors<horsicq@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "../global.h"
#include <QCoreApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include "die_script.h"
#include "entropyprocess.h"
#include "xoptions.h"

void ScanFiles(QList<QString> *pListArgs,DiE_Script::SCAN_OPTIONS *pScanOptions, DiE_Script *pDieScript)
{
    QList<QString> listFileNames;

    for(qint32 i=0;i<pListArgs->count();i++)
    {
        QString sFileName=pListArgs->at(i);

        if(QFileInfo::exists(sFileName))
        {
            XBinary::findFiles(sFileName,&listFileNames);
        }
        else
        {
            printf("Cannot find: %s\n",sFileName.toLatin1().data());
        }
    }

    bool bShowFileName=listFileNames.count()>1;

    qint32 nNumberOfFiles=listFileNames.count();

    for(qint32 i=0;i<nNumberOfFiles;i++)
    {
        QString sFileName=listFileNames.at(i);

        if(bShowFileName)
        {
            printf("%s:\n",sFileName.toLatin1().data());
        }

        if(pScanOptions->bShowEntropy)
        {
            QString sResult;

            EntropyProcess::DATA epData=EntropyProcess::processRegionsFile(sFileName);

            if(pScanOptions->bResultAsJSON)
            {
                sResult=EntropyProcess::dataToJsonString(&epData);
            }
            else if(pScanOptions->bResultAsXML)
            {
                sResult=EntropyProcess::dataToXmlString(&epData);
            }
            else if(pScanOptions->bResultAsCSV)
            {
                sResult=EntropyProcess::dataToCsvString(&epData);
            }
            else if(pScanOptions->bResultAsTSV)
            {
                sResult=EntropyProcess::dataToTsvString(&epData);
            }
            else
            {
                sResult=EntropyProcess::dataToPlainString(&epData);
            }

            printf("%s",sResult.toLatin1().data());
        }
        else
        {
            QString sResult;

            DiE_Script::SCAN_RESULT scanResult=pDieScript->scanFile(sFileName,pScanOptions);

            if(pScanOptions->bResultAsJSON)
            {
                sResult=DiE_Script::scanResultToJsonString(&scanResult);
            }
            else if(pScanOptions->bResultAsXML)
            {
                sResult=DiE_Script::scanResultToXmlString(&scanResult);
            }
            else if(pScanOptions->bResultAsCSV)
            {
                sResult=DiE_Script::scanResultToCsvString(&scanResult);
            }
            else if(pScanOptions->bResultAsTSV)
            {
                sResult=DiE_Script::scanResultToTsvString(&scanResult);
            }
            else
            {
                sResult=DiE_Script::scanResultToPlainString(&scanResult);
            }

            printf("%s",sResult.toLatin1().data());

            if(scanResult.listErrors.count())
            {
                printf("%s",DiE_Script::getErrorsString(&scanResult).toLatin1().data());
            }
        }
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName(X_ORGANIZATIONNAME);
    QCoreApplication::setOrganizationDomain(X_ORGANIZATIONDOMAIN);
    QCoreApplication::setApplicationName(X_APPLICATIONNAME);
    QCoreApplication::setApplicationVersion(X_APPLICATIONVERSION);

    QCoreApplication app(argc, argv);

    QCommandLineParser parser;
    QString sDescription;
    sDescription.append(QString("%1 v%2\n").arg(X_APPLICATIONDISPLAYNAME,X_APPLICATIONVERSION));
    sDescription.append(QString("%1\n").arg("Copyright(C) 2006-2008 Hellsp@wn 2012-2021 hors<horsicq@gmail.com> Web: http://ntinfo.biz"));
    parser.setApplicationDescription(sDescription);
    parser.addHelpOption();
    parser.addVersionOption();

    parser.addPositionalArgument("target","The file or directory to open.");

    QCommandLineOption clDeepScan       (QStringList()<<    "d"<<   "deepscan",     "Deep scan.");
    QCommandLineOption clEntropy        (QStringList()<<    "e"<<   "entropy",      "Show entropy.");
    QCommandLineOption clResultAsXml    (QStringList()<<    "x"<<   "xml",          "Result as XML.");
    QCommandLineOption clResultAsJson   (QStringList()<<    "j"<<   "json",         "Result as JSON.");
    QCommandLineOption clResultAsCSV    (QStringList()<<    "c"<<   "csv",          "Result as CSV.");
    QCommandLineOption clResultAsTSV    (QStringList()<<    "t"<<   "tsv",          "Result as TSV.");
    QCommandLineOption clShowDatabase   (QStringList()<<    "s"<<   "showdatabase", "Show database.");

    parser.addOption(clDeepScan);
    parser.addOption(clEntropy);
    parser.addOption(clResultAsXml);
    parser.addOption(clResultAsJson);
    parser.addOption(clResultAsCSV);
    parser.addOption(clResultAsTSV);
    parser.addOption(clShowDatabase);

    parser.process(app);

    QList<QString> listArgs=parser.positionalArguments();

    DiE_Script::SCAN_OPTIONS scanOptions={0};

    scanOptions.bShowType=true;
    scanOptions.bShowOptions=true;
    scanOptions.bShowVersion=true;
    scanOptions.bDeepScan=parser.isSet(clDeepScan);
    scanOptions.bShowEntropy=parser.isSet(clEntropy);
    scanOptions.bResultAsXML=parser.isSet(clResultAsXml);
    scanOptions.bResultAsJSON=parser.isSet(clResultAsJson);
    scanOptions.bResultAsCSV=parser.isSet(clResultAsCSV);
    scanOptions.bResultAsTSV=parser.isSet(clResultAsTSV);

    DiE_Script die_script;

    QString sDatabase=XOptions().getApplicationDataPath()+QDir::separator()+"db";

    // TODO set database

    die_script.loadDatabase(sDatabase);

    if(parser.isSet(clShowDatabase))
    {
        // TODO Show database
    }

    if(listArgs.count())
    {
        ScanFiles(&listArgs,&scanOptions,&die_script);
    }
    else
    {
        parser.showHelp();
        Q_UNREACHABLE();
    }

    return 0;
}
