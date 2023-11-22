#include "kcppcheckxml.h"
#include "kglobaldata.h"
#include "krallfile.h"

#include <QProcess>  
#include <QFileDialog>
#include <QTextStream>
#include <QTextCodec>
#include <QMessageBox>
#include <QFile>
#include <QXmlStreamReader>

KCppCheckXML::KCppCheckXML()
{
    m_errorList.reserve(200);
}

KCppCheckXML::~KCppCheckXML()
{

}

void KCppCheckXML::outputRunCppcheckToCsv(const QString& dirPath)
{
    QString qParentPath = KGlobalDataNameSpace::getFatherDirName() + "\\CPPcheck";

    QString command = qParentPath + "\\cppcheck --platform=win64 --enable=warning,style,performance,portability  --verbose --xml " + dirPath;

    QProcess process;
    process.setProcessChannelMode(QProcess::MergedChannels);  // ����׼������׼����ϲ�
    process.start(command);
    process.waitForFinished();

    QByteArray outputData = process.readAll();
    QTextCodec* codec = QTextCodec::codecForLocale();         //��ȡ��ǰϵͳ���ر������
    if (codec == nullptr)
        return;
    QString output = codec->toUnicode(outputData);            //���ֽ�������ΪUnicode�ַ�

    //������XML�ļ�
    KRallFile outputFile("output.xml", "clearBeforeWrite");
    QTextStream stream(&outputFile.getFile());
    stream.setGenerateByteOrderMark(true);
    stream.setCodec("UTF-8");
    stream << output; 
}

void KCppCheckXML::readCppcheckCsv(const QString& csvFilePath)
{
    KRallFile file(csvFilePath, "read");
    QXmlStreamReader xmlReader(&file.getFile());

    while (!xmlReader.atEnd() && !xmlReader.hasError())
    {
        QXmlStreamReader::TokenType token = xmlReader.readNext();

        // ����StartDocument��ǣ�������ȡ��һ�����
        if (token == QXmlStreamReader::StartDocument)
            continue;

        if (token == QXmlStreamReader::Invalid)
            break;

        if (token == QXmlStreamReader::StartElement)
        {
            QString elementName = xmlReader.name().toString();
            if (elementName == "error")
            {
                ErrorInfo error;
 
                QXmlStreamAttributes attributes = xmlReader.attributes();
                error.id = attributes.value("id").toString();
                error.severity = attributes.value("severity").toString();
                error.msg = attributes.value("msg").toString();
                error.verbose = attributes.value("verbose").toString();
                error.cwe = attributes.value("cwe").toString();

                // ����errorԪ�ص���Ԫ��  
                while (!(xmlReader.tokenType() == QXmlStreamReader::EndElement && xmlReader.name() == "error"))
                {
                    xmlReader.readNext();

                    if (xmlReader.tokenType() == QXmlStreamReader::StartElement)
                    {
                        if (xmlReader.name() == "location")
                        {
                            error.file = xmlReader.attributes().value("file").toString();
                            error.line = xmlReader.attributes().value("line").toInt();
                            error.column = xmlReader.attributes().value("column").toInt();
                            error.info = xmlReader.attributes().value("info").toString();
                        }
                        else if (xmlReader.name() == "symbol")
                        {
                            xmlReader.readNext();
                            error.symbol = xmlReader.text().toString();
                        }
                    }
                }
                m_errorList.push_back(error);
            }
        }
    }
}

QVector<ErrorInfo>& KCppCheckXML::outputCppcheckRunResult(const QString& dirPath)
{
    outputRunCppcheckToCsv(dirPath);
    readCppcheckCsv("output.xml");
    return m_errorList;
}
