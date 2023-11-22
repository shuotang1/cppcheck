#ifndef __KXMLREADER_KCOMPILE_H__
#define __KXMLREADER_KCOMPILE_H__

#include <QThread>

class KCompile : public QThread
{
public:
	KCompile(const QString& path);
	~KCompile();

	void run()override;      //�������б���������һ�����ŵ��߳���ִ�� 
	void getExeFileName(QVector<QString>& fileName);

	QStringList getDirLogs(const QString& path);    //������Ŀ¼��־������QStringList��
private:
	KCompile(const KCompile& other) = default;
	void operator=(const KCompile& other) = delete;
	KCompile(const KCompile&& other) = delete;
	KCompile& operator=(KCompile&& other) = default;

	QString m_path; 
};
#endif
