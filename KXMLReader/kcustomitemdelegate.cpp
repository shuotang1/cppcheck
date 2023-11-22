// ----------------------------------------------------------------
// kspinboxdelegate.cpp
// �����������Զ��������,ʵ�ֵ�Ԫ����ɫ��Ⱦ
// -----------------------------------------------------------------

#include "kcustomitemdelegate.h"

#include<QTableView>

KCustomItemDelegate::KCustomItemDelegate(QObject *parent)
	: QStyledItemDelegate(parent)
{
	m_optToBackVec.reserve(200);
}

KCustomItemDelegate::~KCustomItemDelegate()
{
}

void KCustomItemDelegate::executeOptToBackMemory(const QVector<ErrorInfo>& errorList)
{
	QString oldFileName;
	bool flag = false;
	for (const ErrorInfo& errorInfo : errorList)
	{
		QString fileName = errorInfo.file;
		if (oldFileName.isEmpty() || oldFileName != fileName)
		{
			oldFileName = fileName;
			flag = !flag;
		}
		m_optToBackVec.push_back(QPair<const QString, bool>(fileName, flag));
	}
}

void KCustomItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	QModelIndex fileNameIndex = index.model()->index(index.row(), 0);  //��ȡ�ļ����е�����
	QString fileName = index.model()->data(fileNameIndex).toString();  //��ȡ�ļ���

	for (const auto& pair : m_optToBackVec)
	{
		if (pair.first == fileName)
		{
			if (!pair.second)
				painter->fillRect(option.rect, QColor("#f3f3f3"));
			else
				painter->fillRect(option.rect, QColor("#d1d1d1"));
			break;
		}
	}

	if (option.state & QStyle::State_Selected)
		painter->fillRect(option.rect, QColor(255, 0, 0, 100));

	QString valueString = index.model()->data(index, Qt::DisplayRole).toString();//��ģ���л�ȡ����
	painter->drawText(option.rect, Qt::AlignCenter, valueString);

	return;
}

void KCustomItemDelegate::clearOptToBackMemory()
{
	m_optToBackVec.clear();
}