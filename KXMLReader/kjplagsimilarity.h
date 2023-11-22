// ------------------------------------------------------------
// 2023/10/10
// kjplagsimilarity.h
// ����������������Ŀ¼��C++�ļ����ļ����ƶ�
// -------------------------------------------------------------

#ifndef __KJPLAGSIMILARITY_KJPLAGSIMILARITY_H__
#define __KJPLAGSIMILARITY_KJPLAGSIMILARITY_H__

#include <QString>
#include <QMap>

struct SimilarityResults
{
	QString firstFileName;
	QString secondFileName;
	QString similarity;
};

class KJplagSimilarity
{
public:
	KJplagSimilarity(const QString& path);
	~KJplagSimilarity();

	void outputDirectorySimilarity(const QString& dirName, QVector<SimilarityResults>& SimilarityVecResults);
	void outputSpecificDirSim(const QString& dirName, QVector<SimilarityResults>& SimilarityVecResults);
	void outputTwoFileCompareSim(const QString& firFilePath, const QString& secFilePath, QVector<SimilarityResults>& SimilarityVecResults);
	void outputTwoDirCompareSim(const QString& firDirPath, const QString& secDirPath, QVector<SimilarityResults>& SimilarityVecResults);

	void processDirectory(const std::string& directoryPath);      //�ݹ鴦��Ŀ¼��������ļ���UTF-8 BOM���룬����ת��ΪUTF-8����
private:
	KJplagSimilarity(const KJplagSimilarity& other) = default;
	void operator=(const KJplagSimilarity& other) = delete;
	KJplagSimilarity(const KJplagSimilarity&& other) = delete;
	KJplagSimilarity& operator=(KJplagSimilarity&& other) = default;

	void removeBomCharacter(std::string& filePath);               //������ļ���UTF-8 BOM���룬����ת��ΪUTF-8����
	
	void addComparisonContents(QString& fileName, double similarity, SimilarityResults& comparison);//���ն����ݽ��д�����ΪQTableView��Ҫ��ʽ

	void moveDirectoryFromSourceToDes(const QString& source, const QString& des);  //��ԴĿ¼�������ļ����е�Ŀ��Ŀ¼��
	void moveDirectoryFromDesToSource(const QString& source, const QString& des);

	QString m_path;

	QMap<QString, QString> m_sourceToDesMap;     //key:ԴĿ¼��Ŀ��Ŀ¼����   value��ԴĿ¼����
};

#endif 


