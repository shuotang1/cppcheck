#ifndef __KXMLREADER_KCPPCHECKXML_H__
#define __KXMLREADER_KCPPCHECKXML_H__ 

#include<Qvector>

struct ErrorInfo
{
    QString id;                          //����warnning��error��style��performance....����id
    QString severity;                    //warning��error��style....
    QString msg;                         //����ļ�Ҫ����
    QString verbose;                     //��ϸ�Ĵ�������
    QString cwe = "NULL";                //����������Common Weakness Enumeration��CWE������
    QString file;                        //���������ļ�·��
    int line;                            //���������к�
    int column;                          //���������к�
    QString info = "NULL";               //�йش���Ķ�����Ϣ
    QString symbol = "none";             //�������صķ��Ż������
};

class KCppCheckXML
{
public:
	KCppCheckXML();
	~KCppCheckXML();

	QVector<ErrorInfo>& outputCppcheckRunResult(const QString& dirPath);
private:
    KCppCheckXML(const KCppCheckXML& other) = default;
    void operator=(const KCppCheckXML& other) = delete;
    KCppCheckXML(const KCppCheckXML&& other) = delete;
    KCppCheckXML& operator=(KCppCheckXML&& other) = default;

    void outputRunCppcheckToCsv(const QString& dirPath);
    void readCppcheckCsv(const QString& csvFilePath);

    QVector<ErrorInfo> m_errorList;
};
#endif

