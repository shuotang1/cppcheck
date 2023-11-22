#include"kjplagsimilarity.h"
#include"kglobaldata.h"

#include <fstream>
#include <algorithm>
#include <codecvt> 
#include <QProcess>  
#include <QFileDialog>
#include <QDirIterator>
#include <QMessageBox>
#include <QTextCodec>

#include <direct.h>  
#include <windows.h> 

KJplagSimilarity::KJplagSimilarity(const QString& path)
    :m_path(path)
{
}

KJplagSimilarity::~KJplagSimilarity()
{
}

void KJplagSimilarity::removeBomCharacter(std::string& filePath)
{
    std::ifstream file(filePath, std::ios::binary);
    if (!file)
        return;

    // 读取文件前3个字节  
    char bom[3];
    file.read(bom, 3);

    // 检查是否是UTF-8 BOM  
    if (!(file && bom[0] == (char)0xEF && bom[1] == (char)0xBB && bom[2] == (char)0xBF))
        return;              // 文件不是UTF-8 BOM编码 

    // 文件是UTF-8 BOM编码,读取文件内容  
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    std::wstring utf16_content = converter.from_bytes(content);
    file.close();

    // 保存转换后的内容到文件  
    std::ofstream outfile(filePath, std::ios::binary);
    if (!outfile)
        return;
    outfile << converter.to_bytes(utf16_content);
    outfile.close();
}

void KJplagSimilarity::processDirectory(const std::string& directoryPath)
{
    WIN32_FIND_DATAA fileData;
    HANDLE directoryHandle = FindFirstFileA((directoryPath + "/*").c_str(), &fileData);
    if (directoryHandle != INVALID_HANDLE_VALUE)
    {
        do {
            if (fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                if (std::strcmp(fileData.cFileName, ".") != 0 && std::strcmp(fileData.cFileName, "..") != 0)
                {
                    std::string subdirectoryPath = directoryPath + "\\" + fileData.cFileName;
                    processDirectory(subdirectoryPath);  // 递归处理子目录  
                }
            }
            else
            {
                std::string fileName = fileData.cFileName;
                std::string filePath = directoryPath + "\\" + fileName;
                removeBomCharacter(filePath);
            }
        } while (FindNextFileA(directoryHandle, &fileData));

        FindClose(directoryHandle);
    }
    else
    {
        return;
    }
}

void KJplagSimilarity::outputDirectorySimilarity(const QString& dirName, QVector<SimilarityResults>& SimilarityVecResults)
{
    QString qParentPath = KGlobalDataNameSpace::getFatherDirName() + "\\jdk-17.0.8\\bin";

    QString command = qParentPath + "\\java -jar " + qParentPath + "\\jplag-4.3.0-jar-with-dependencies.jar -l cpp " + dirName;

    QProcess process;
    process.setProcessChannelMode(QProcess::MergedChannels);  // 将标准输出与标准错误合并
    process.start(command);
    process.waitForFinished(-1);

    QByteArray output = process.readAll();  //获取命令输出  

    QStringList lines = QString(output).split("\n");
    for (const QString& line : lines)
    {
        if (line.contains("[INFO] ParallelComparisonStrategy - Comparing"))
        {
            // 提取文件名和相似度  
            int colonPos = line.lastIndexOf(":");
            QString fileName = line.mid(line.indexOf("Comparing") + 10, colonPos - line.indexOf("Comparing") - 10);
            double similarity = line.mid(colonPos + 1).toDouble();

            SimilarityResults comparison;
            addComparisonContents(fileName, similarity, comparison);
            SimilarityVecResults.push_back(comparison);
        }
    }
 
    std::sort(SimilarityVecResults.begin(), SimilarityVecResults.end(), [](const SimilarityResults& c1, const SimilarityResults& c2) {
        return c1.similarity > c2.similarity;
    });
}

void KJplagSimilarity::outputTwoFileCompareSim(const QString& firFilePath, const QString& secFilePath, QVector<SimilarityResults>& SimilarityVecResults)
{
    QString qParentPath = KGlobalDataNameSpace::getFatherDirName() + "\\jdk-17.0.8\\bin";

    QDir jplagTestCreateDir(KGlobalDataNameSpace::getFatherDirName());
    if (!jplagTestCreateDir.mkdir("jplag"))
    {
        QMessageBox::warning(Q_NULLPTR, QStringLiteral("subDirCreateFailed"), QObject::tr("subDirCreateFailedTr"), QMessageBox::Ok);
        return;
    }
    QString jPlagTestPath = KGlobalDataNameSpace::getFatherDirName() + "\\jplag";

    QString targetFirFilePath = jPlagTestPath + QDir::separator() + "1" + QFileInfo(firFilePath).fileName();
    QFile::rename(firFilePath, targetFirFilePath);
    QString targetSecFilePath = jPlagTestPath + QDir::separator() + "2" + QFileInfo(secFilePath).fileName();
    QFile::rename(secFilePath, targetSecFilePath);

    QString command = qParentPath + "\\java -jar " + qParentPath + "\\jplag-4.3.0-jar-with-dependencies.jar -l cpp " + jPlagTestPath;

    QProcess process;
    process.setProcessChannelMode(QProcess::MergedChannels);  // 将标准输出与标准错误合并
    process.start(command);
    process.waitForFinished(-1);

    QByteArray output = process.readAll();  //获取命令输出  

    QStringList lines = QString(output).split("\n");
    for (const QString& line : lines)
    {
        if (line.contains("[INFO] ParallelComparisonStrategy - Comparing"))
        {
            // 提取文件名和相似度  
            int colonPos = line.lastIndexOf(":");
            QString fileName = line.mid(line.indexOf("Comparing") + 10, colonPos - line.indexOf("Comparing") - 10);
            double similarity = line.mid(colonPos + 1).toDouble();

            SimilarityResults comparison;
            addComparisonContents(fileName, similarity, comparison);
            SimilarityVecResults.push_back(comparison);
        }
    }

    QFile::rename(targetFirFilePath, firFilePath);
    QFile::rename(targetSecFilePath, secFilePath);

    QDir jplagTestClearDir(jPlagTestPath);
    if (jplagTestClearDir.exists())
        jplagTestClearDir.removeRecursively();
}

void KJplagSimilarity::outputTwoDirCompareSim(const QString& firDirPath, const QString& secDirPath, QVector<SimilarityResults>& SimilarityVecResults)
{
    QString qParentPath = KGlobalDataNameSpace::getFatherDirName() + "\\jdk-17.0.8\\bin";

    QDir jplagTestCreateDir(KGlobalDataNameSpace::getFatherDirName());
    if (!jplagTestCreateDir.mkdir("jplag"))
    {
        QMessageBox::warning(Q_NULLPTR, QStringLiteral("subDirCreateFailed"), QObject::tr("subDirCreateFailedTr"), QMessageBox::Ok);
        return;
    }
    QString jPlagTestPath = KGlobalDataNameSpace::getFatherDirName() + "\\jplag";

    moveDirectoryFromSourceToDes(firDirPath, jPlagTestPath);
    moveDirectoryFromSourceToDes(secDirPath, jPlagTestPath);

    QString command = qParentPath + "\\java -jar " + qParentPath + "\\jplag-4.3.0-jar-with-dependencies.jar -l cpp " + jPlagTestPath;

    QProcess process;
    process.setProcessChannelMode(QProcess::MergedChannels);  // 将标准输出与标准错误合并
    process.start(command);
    process.waitForFinished();

    QByteArray outputData = process.readAll();
    QTextCodec* codec = QTextCodec::codecForLocale();         //获取当前系统本地编码对象
    QString output = codec->toUnicode(outputData);            //将字节流解码为Unicode字符

    QStringList lines = QString(output).split("\n");
    for (const QString& line : lines)
    {
        if (line.contains("[INFO] ParallelComparisonStrategy - Comparing"))
        {
            // 提取文件名和相似度  
            int colonPos = line.lastIndexOf(":");
            QString fileName = line.mid(line.indexOf("Comparing") + 10, colonPos - line.indexOf("Comparing") - 10);
            double similarity = line.mid(colonPos + 1).toDouble();

            SimilarityResults comparison;
            addComparisonContents(fileName, similarity, comparison);
            SimilarityVecResults.push_back(comparison);
        }
    }

    QDir jplagTestdir(jPlagTestPath);
    QStringList fileList = jplagTestdir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (const QString& fileName : fileList)
    {
        moveDirectoryFromDesToSource(jPlagTestPath + QDir::separator() + fileName, m_sourceToDesMap[fileName]);
    }

    QDir jplagTestClearDir(jplagTestdir);
    if (jplagTestClearDir.exists())
        jplagTestClearDir.removeRecursively();

    m_sourceToDesMap.clear();
}

void KJplagSimilarity::outputSpecificDirSim(const QString& dirName, QVector<SimilarityResults>& SimilarityVecResults)
{
    QString qParentPath = KGlobalDataNameSpace::getFatherDirName() + "\\jdk-17.0.8\\bin";

    QDir jplagTestCreateDir(KGlobalDataNameSpace::getFatherDirName());
    if (!jplagTestCreateDir.mkdir("jplag"))
    {
        QMessageBox::warning(Q_NULLPTR, QStringLiteral("subDirCreateFailed"), QObject::tr("subDirCreateFailedTr"), QMessageBox::Ok);
        return;
    }
    QString jPlagTestPath = KGlobalDataNameSpace::getFatherDirName() + "\\jplag";

    QDirIterator it(m_path, QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks | QDir::Hidden, QDirIterator::Subdirectories);
    while (it.hasNext()) 
    {
        QString absoluteFilePath = it.next();
        QString lastDirName = absoluteFilePath.mid(m_path.length());

        int slashIndex1 = lastDirName.indexOf('/');
        int slashIndex2 = lastDirName.indexOf('/', slashIndex1 + 1);
        if (slashIndex1 != -1 && slashIndex2 != -1) 
        {
            QString subPath1 = lastDirName.mid(slashIndex2 + 1);
            QString subPath2 = dirName.mid(dirName.indexOf('/', dirName.indexOf('/') + 1) + 1);

            if (subPath1 == subPath2)
            {
                QFileInfo fileInfo(absoluteFilePath);
                if (fileInfo.isDir()) 
                    moveDirectoryFromSourceToDes(absoluteFilePath, jPlagTestPath);
            }
        }
    }

    QString command = qParentPath + "\\java -jar " + qParentPath + "\\jplag-4.3.0-jar-with-dependencies.jar -l cpp " + jPlagTestPath;

    QProcess process;
    process.setProcessChannelMode(QProcess::MergedChannels);  // 将标准输出与标准错误合并
    process.start(command);
    process.waitForFinished();

    QByteArray outputData = process.readAll();
    QTextCodec* codec = QTextCodec::codecForLocale();         //获取当前系统本地编码对象
    QString output = codec->toUnicode(outputData);            //将字节流解码为Unicode字符

    QStringList lines = QString(output).split("\n");
    for (const QString& line : lines)
    {
        if (line.contains("[INFO] ParallelComparisonStrategy - Comparing"))
        {
            // 提取文件名和相似度  
            int colonPos = line.lastIndexOf(":");
            QString fileName = line.mid(line.indexOf("Comparing") + 10, colonPos - line.indexOf("Comparing") - 10);
            double similarity = line.mid(colonPos + 1).toDouble();

            SimilarityResults comparison;
            addComparisonContents(fileName, similarity, comparison);
            SimilarityVecResults.push_back(comparison);
        }
    }
  
    std::sort(SimilarityVecResults.begin(), SimilarityVecResults.end(), [](const SimilarityResults& c1, const SimilarityResults& c2) {
        return c1.similarity > c2.similarity;
    });

    QDir jplagTestdir(jPlagTestPath);
    QStringList fileList = jplagTestdir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    for (const QString& fileName : fileList)
    {
        moveDirectoryFromDesToSource(jPlagTestPath + QDir::separator() + fileName, m_sourceToDesMap[fileName]);
    }

    QDir jplagTestClearDir(jplagTestdir);
    if (jplagTestClearDir.exists())
        jplagTestClearDir.removeRecursively();

    m_sourceToDesMap.clear();
}

void KJplagSimilarity::addComparisonContents(QString& fileName, double similarity, SimilarityResults& comparison)
{
    QStringList parts = fileName.split("-");
    int counts = parts.size();          
    QString firstFileName, secondFileName;
    int midIndex = counts / 2;
    
    comparison.firstFileName = parts.mid(0, midIndex).join("-");
    comparison.secondFileName = parts.mid(midIndex).join("-");

    comparison.similarity = QString::number(similarity * 100, 'f', 2) + "%";    //保留两位小数
}

void KJplagSimilarity::moveDirectoryFromSourceToDes(const QString& source, const QString& des)
{
    QDir sourceDir(source);
    QDir desDir(des);
 
    QRegExp regExp("[<>:\"/\\|?*]");
    QString cleanedDirPath = sourceDir.absolutePath().replace(regExp, " ");
    if (!desDir.mkdir(cleanedDirPath))
    {
        QMessageBox::warning(Q_NULLPTR, QStringLiteral("subDirCreateFailed"), QObject::tr("subDirCreateFailedTr"), QMessageBox::Ok);
        return;
    }

    QString resPath = des + QDir::separator() + cleanedDirPath;
    m_sourceToDesMap[cleanedDirPath] = source;

    QDirIterator iterator(sourceDir, QDirIterator::Subdirectories);
    while (iterator.hasNext()) 
    {
        iterator.next();

        QString filePath = iterator.filePath();
        QString relativeFilePath = QDir(sourceDir).relativeFilePath(filePath);
        QString targetFilePath = resPath + QDir::separator() + relativeFilePath;

        if (iterator.fileInfo().isDir())
            QDir().mkpath(targetFilePath);
        else 
            QFile::rename(filePath, targetFilePath);
    }
}

void KJplagSimilarity::moveDirectoryFromDesToSource(const QString& source, const QString& des)
{
    QDirIterator iterator(source, QDirIterator::Subdirectories);
    while (iterator.hasNext())
    {
        iterator.next();

        QString filePath = iterator.filePath();
        QString relativeFilePath = QDir(source).relativeFilePath(filePath);
        QString targetFilePath = des + QDir::separator() + relativeFilePath;

        if (iterator.fileInfo().isDir())
            QDir().mkpath(targetFilePath);
        else
            QFile::rename(filePath, targetFilePath);
    }
}
