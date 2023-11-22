// -----------------------------------------------
// kcompile.cpp
// ����ʱ��:2023/10/26
// ��������������python�ű���ʵ�ֱ���
// -----------------------------------------------

#include "kcompile.h"
#include "kglobaldata.h"
#include "krallfile.h"

#include <QProcess>  
#include <QDirIterator>
#include <QCoreApplication>
#include <QTextStream>
#include <QMessageBox>

KCompile::KCompile(const QString& path)
    : m_path(path)
{
}

KCompile::~KCompile()
{
}

void KCompile::run()
{
    QString parentPath = KGlobalDataNameSpace::getFatherDirName() + "\\python312";
    QString pyPath = KGlobalDataNameSpace::getFatherDirName() + KGlobalDataNameSpace::m_compilePyDir;

    QStringList arguments;
    arguments << pyPath << m_path;

    QDir::setCurrent(parentPath);

    QProcess process;
    process.start("python", arguments);
    process.waitForFinished(-1);      //�ȴ��ն����������
}

QStringList KCompile::getDirLogs(const QString& path)
{
    QStringList logContents;

    QDirIterator it(path, QStringList() << "build.log", QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext())
    {
        KRallFile file(it.next(), "read");
        QTextStream stream(&file.getFile());
        logContents.append(stream.readAll());
    }

    return logContents;
}

void KCompile::getExeFileName(QVector<QString>& fileNames)
{
    QDir dir(KGlobalDataNameSpace::m_compileDir + "\\exec");   //����QDir���󣬲�����Ϊ��ǰĿ¼
    QStringList fileList = dir.entryList(QDir::Files);         //��ȡ��ǰĿ¼�ļ��б�
    for (const QString& fileName : fileList)
    {
        fileNames.push_back(fileName);
    }
}