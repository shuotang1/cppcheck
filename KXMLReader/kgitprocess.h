#ifndef __KXMLREADER_KGITPROCESS_H__
#define __KXMLREADER_KGITPROCESS_H__

#include <QString>
#include <QVector>
#include <QThread>

class KGitProcess: public QThread
{
public:
	KGitProcess();
	~KGitProcess();

	void inputOrgNameArg(const QString& orgName);
	void inputOrgNameAndDescribeArg(const QString& orgName, const QString& describe);
	void inputUserArg();
	void inputRepoArg(const QString& orgName, const QString& privateRepo,
		const QString& idAddDefaultFile, const QString& defaultFilePath);
	void inputBindUserAndRepoArg(const QString& orgName);
	void inputPullCodeArg(const QString& orgName, const QString& outputDir);
	void inutDelRepoAndUserArg(const QString& orgName, const QString& stringToPyTool);

	virtual void run() override;

	QString getOutput();
private:
	KGitProcess(const KGitProcess& other) = default;
	void operator=(const KGitProcess& other) = delete;
	KGitProcess(const KGitProcess&& other) = delete;
	KGitProcess& operator=(KGitProcess&& other) = default;

	QString m_output;

	QString m_path;              // m_path : python文件相对路径
	QVector<QString> m_vec;      // m_vec : 参数列表
};

#endif
