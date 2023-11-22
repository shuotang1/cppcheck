#include "kerrorshowmainwin.h"
#include "krallfile.h"

#include <QHBoxLayout>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QTextBlock>
#include <QStringBuilder>

KErrorShowMainWin::KErrorShowMainWin(const QString& fileName, int lineNumber, int fileToTableLine, QWidget* parent)
	: QWidget(parent)
	, m_pPlainTextEdit(nullptr)
	, m_fileName(fileName)
	, m_line(lineNumber)
	, m_fileToTableLine(fileToTableLine)
{
	initWindow();
}

KErrorShowMainWin::~KErrorShowMainWin()
{
}

void KErrorShowMainWin::initWindow()
{
	setMinimumWidth(800);
	setMaximumWidth(800);

	setWindowTitle(tr("numberTr") + ": " + QString::number(m_fileToTableLine) + "   " + tr("fileNameTr") + ": " + m_fileName);

	m_pPlainTextEdit = new QPlainTextEdit(this);
	m_pPlainTextEdit->setReadOnly(true);

	m_pPlainTextEdit->setFont(QFont("Consolas", 12));

	QVBoxLayout* m_pVMainLay = new QVBoxLayout(this);
	m_pVMainLay->addWidget(m_pPlainTextEdit);
	setLayout(m_pVMainLay);

	KRallFile file(m_fileName, "read");
	QTextStream in(&file.getFile());
	m_pPlainTextEdit->setPlainText(in.readAll());

	// ��m_pPlainTextEdit�Ƶ�Ŀ����λ��
	QTextCursor cursor = m_pPlainTextEdit->textCursor();
	cursor.movePosition(QTextCursor::Start);       //������ƶ����ı���ͷ
	cursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, m_line - 1);  //arg1:�����ƶ� arg2���ƶ����ȡ��ѡ���ı� 
	m_pPlainTextEdit->setTextCursor(cursor);

	// �����ı�����ı���ɫΪ��ɫ
	QTextBlock targetBlock = m_pPlainTextEdit->document()->findBlockByLineNumber(m_line - 1);
	QTextCursor colorCursor(targetBlock);
	colorCursor.select(QTextCursor::BlockUnderCursor);
	QTextCharFormat format;
	format.setForeground(Qt::red);
	colorCursor.mergeCharFormat(format);
}
