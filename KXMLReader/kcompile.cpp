// -----------------------------------------------
// kcompile.cpp
// 创建时间:2023/10/26
// 功能描述：运行python脚本，实现编译
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
    process.waitForFinished(-1);      //等待终端输出无限秒
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
    QDir dir(KGlobalDataNameSpace::m_compileDir + "\\exec");   //创建QDir对象，并设置为当前目录
    QStringList fileList = dir.entryList(QDir::Files);         //获取当前目录文件列表
    for (const QString& fileName : fileList)
    {
        fileNames.push_back(fileName);
    }
}