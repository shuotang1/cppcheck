#include "kdatabase.h"
#include "krallfile.h"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QFile>
#include <QTextStream>

QSqlDatabase KDataBase::m_db = QSqlDatabase::addDatabase("QSQLITE");

KDataBase::KDataBase()
{
	initDataBase("cppcheck.db");
}

KDataBase::~KDataBase()
{
	if (m_db.isOpen())
		m_db.close();
}

bool KDataBase::initDataBase(const QString& dbname)
{
	if (m_db.tables().empty())
	{
		m_db = QSqlDatabase::addDatabase("QSQLITE");
		m_db.setDatabaseName(dbname); 
		bool ok = m_db.open();
		if (!ok)
			return false;
		ok = createTable("CppCheckType");
		if (!ok)
			return false;
		ok = createConfigTable("config");
		if (!ok)
			return false;
	}
	return true;
}

bool KDataBase::createTable(const QString& tabname)
{
	bool isExistTable = m_db.tables().contains(tabname);
	if (!isExistTable)
	{
		QSqlQuery query;
		//" %1 %2 %3" .arg("xxx").arg("yyyy")
		QString sql = QString("CREATE TABLE IF NOT EXISTS %1(id text not null"
			",severity text"
			",msg text"
			",verbose text"
			",cwe text"
			",file text,line text,column text,info text,symbol text)").arg(tabname);
		bool ok = query.exec(sql);
		if (!ok)
			return false;
	}
	return true;
}

bool KDataBase::createConfigTable(const QString& configTabName)
{
	bool isExistTable = m_db.tables().contains(configTabName);
	if (!isExistTable)
	{
		QSqlQuery query;
		QString sql = QString("CREATE TABLE IF NOT EXISTS %1(rootUrl text, token text)").arg(configTabName);
		bool ok = query.exec(sql);
		if (!ok)
			return false;
	}
	return true;
}

bool KDataBase::inputRecord(const QVector<QString>& cppcheckTable)
{
	QSqlQuery query;
	QString sql = "INSERT INTO CppCheckType(id,severity,msg,verbose,cwe,file,line,column,info,symbol) VALUES(?,?,?,?,?,?,?,?,?,?);";
	query.prepare(sql);

	int index = 0;
	for (const auto& str : cppcheckTable)
	{
		query.bindValue(index, str);
		index++;
	}
	bool ok = query.exec();
	if (!ok)
		return false;
	return true;
}

bool KDataBase::insertConfig(const QVector<QString>& configVec)
{
	QSqlQuery query;
	QString sql = "INSERT INTO config(rootUrl,token) VALUES(?,?);";
	query.prepare(sql);
	query.bindValue(0, configVec[0]);
	query.bindValue(1, configVec[1]);
	bool ok = query.exec();
	if (!ok)
		return false;
	return true;
}

bool KDataBase::clearRecord()
{
	QSqlQuery query;
	query.prepare("DELETE FROM CppCheckType");
	if (!query.exec())
		return false;
	return true;
}

bool KDataBase::clearConfig()
{
	QSqlQuery query;
	query.prepare("DELETE FROM config");
	if (!query.exec())
		return false;
	return true;
}

bool KDataBase::saveCSV(const QString& filePath)
{
	QSqlQuery query;
	QString sql = "SELECT * FROM CppCheckType;";
	query.prepare(sql);
	if (!query.exec())
		return false;

	KRallFile file(filePath, "write");
	QTextStream stream(&(file.getFile()));

	// 写入表头
	QStringList headerRow;
	for (int i = 0; i < query.record().count(); ++i)
	{
		headerRow << query.record().fieldName(i);
	}
	stream << headerRow.join(",") << "\n";

	// 写入数据
	while (query.next())
	{
		QStringList rowData;
		for (int i = 0; i < query.record().count(); ++i)
		{
			QString value = query.value(i).toString();
			if (value.contains(",") || value.contains("\""))  
			{
				value.replace("\"", "\"\"");       //将双引号替换为两个双引号
				value = "\"" + value + "\"";       //csv将','作为分隔符，因此需要在含有','的字符中，将其变为一个整体字符串
			}
			rowData << value;
		}
		stream << rowData.join(",") << "\n";
	}
	return true;
}

bool KDataBase::selectConfig(QVector<QString>& configVec)
{
	QSqlQuery query;
	QString sql = "SELECT * FROM config";
	query.prepare(sql);
	if (!query.exec())
		return false;
	int columnLen = query.record().count();
	while (query.next())
	{
		for (int i = 0; i < columnLen; i++)
		{
			configVec.push_back(query.value(i).toString());
		}
	}
	return true;
}