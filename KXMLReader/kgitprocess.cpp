#include "kgitprocess.h"
#include "kglobaldata.h"

#include <QCoreApplication>
#include <QDir>
#include <QProcess>
#include <QTextCodec>

KGitProcess::KGitProcess()
{
}

KGitProcess::~KGitProcess()
{
}

void KGitProcess::run()
{
	QString parentPath = KGlobalDataNameSpace::getFatherDirName() + "\\Python312";
	QString pyPath = KGlobalDataNameSpace::getFatherDirName() + m_path;

	QStringList arguments;	
	arguments.append(pyPath);

	for (const QString& arg : m_vec)
	{
		arguments.append(arg);
	}

	QDir::setCurrent(parentPath);

	QProcess process;
	process.start("python", arguments);
	process.waitForFinished(-1);      //等待终端输出无限秒

	QByteArray outputData = process.readAll();
	QTextCodec* codec = QTextCodec::codecForLocale();         //获取当前系统本地编码对象
	if (codec == nullptr)
		return;
	m_output = codec->toUnicode(outputData);            //将字节流解码为Unicode字符
}

void KGitProcess::inputOrgNameArg(const QString& orgName)
{
	m_vec.clear();
	m_vec.push_back(orgName);
	m_vec.push_back(KGlobalDataNameSpace::m_rootUrl);
	m_vec.push_back(KGlobalDataNameSpace::m_token);

	m_path = KGlobalDataNameSpace::m_checkOrgExistPyDir;
}

void KGitProcess::inputOrgNameAndDescribeArg(const QString& orgName, const QString& describe)
{
	m_vec.clear();
	m_vec.push_back(orgName);
	m_vec.push_back(describe);
	m_vec.push_back(KGlobalDataNameSpace::m_rootUrl);
	m_vec.push_back(KGlobalDataNameSpace::m_token);

	m_path = KGlobalDataNameSpace::m_createOrgPyDir;
}

void KGitProcess::inputUserArg()
{
	m_vec.clear();
	m_vec.push_back(KGlobalDataNameSpace::m_rootUrl);
	m_vec.push_back(KGlobalDataNameSpace::m_token);

	m_path = KGlobalDataNameSpace::m_createAccountPyDir;
}

void KGitProcess::inputRepoArg(const QString& orgName, const QString& privateRepo,
	const QString& isAddDefaultFile, const QString& defaultFilePath)
{
	m_vec.clear();
	m_vec.push_back(orgName);
	m_vec.push_back(privateRepo);
	m_vec.push_back(isAddDefaultFile);
	m_vec.push_back(defaultFilePath);
	m_vec.push_back(KGlobalDataNameSpace::m_rootUrl);
	m_vec.push_back(KGlobalDataNameSpace::m_token);

	m_path = KGlobalDataNameSpace::m_createRepoPyDir;
}

void KGitProcess::inputBindUserAndRepoArg(const QString& orgName)
{
	m_vec.clear();
	m_vec.push_back(orgName);
	m_vec.push_back(KGlobalDataNameSpace::m_rootUrl);
	m_vec.push_back(KGlobalDataNameSpace::m_token);

	m_path = KGlobalDataNameSpace::m_bindRepoAndAccountPyDir;
}

void KGitProcess::inputPullCodeArg(const QString& orgName, const QString& outputDir)
{
	m_vec.clear();
	m_vec.push_back(orgName);
	m_vec.push_back(outputDir);
	m_vec.push_back(KGlobalDataNameSpace::m_rootUrl);
	m_vec.push_back(KGlobalDataNameSpace::m_token);

	m_path = KGlobalDataNameSpace::m_pullCodePyDir;
}

void KGitProcess::inutDelRepoAndUserArg(const QString& orgName, const QString& stringToPyTool)
{
	m_vec.clear();
	m_vec.push_back(orgName);
	m_vec.push_back(stringToPyTool);
	m_vec.push_back(KGlobalDataNameSpace::m_rootUrl);
	m_vec.push_back(KGlobalDataNameSpace::m_token);

	m_path = KGlobalDataNameSpace::m_delRepoUserPyDir;
}

QString KGitProcess::getOutput()
{
	return m_output;
}


// --TODO set/get方法可以用property解决
