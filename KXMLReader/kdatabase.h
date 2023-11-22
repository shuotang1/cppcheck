// ---------------------------------------------------------------------
// kdatabase.h
// �����������洢�����ļ������ݿ�
// ----------------------------------------------------------------------

#ifndef __KFILELISTENER_KDATABASE__
#define __KFILELISTENER_KDATABASE__

#include <QSqlDatabase>
#include <QVector>
#include <QString>

class KDataBase
{
public:
	KDataBase();
	~KDataBase();

	bool createTable(const QString& tabname);
	bool inputRecord(const QVector<QString>& cppcheckTable);
	bool clearRecord();

	bool createConfigTable(const QString& configTabName);
	bool insertConfig(const QVector<QString>& configVec);
	bool selectConfig(QVector<QString>& configVec);
	bool clearConfig();

	bool saveCSV(const QString& filePath);   //�����������Ϊcsv�ļ�
private:
	KDataBase(const KDataBase& other) = delete;
	void operator=(const KDataBase& other) = delete;
	KDataBase(const KDataBase&& other) = delete;
	KDataBase& operator=(KDataBase&& other) = delete;

	bool initDataBase(const QString& dbname);
	static QSqlDatabase m_db;
};
#endif
