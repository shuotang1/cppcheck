// -------------------------------------------------
// kexewin.h
// ����ʱ��:2023/10/27
// �����������������������exe�ļ�ʱ������ָý��棬ͨ��ȷ������������ó����յ�������
// --------------------------------------------------

#include "kexewin.h"
#include "kglobaldata.h"

#include <QLabel>
#include <QPushButton>
#include <QProcess>

KExeWin::KExeWin(const QString& fileName, QWidget* parent)
	: QWidget(parent)
	, m_fileName(fileName)
	, m_PVBoxLay(nullptr)
	, m_pLineEdit(nullptr)
{
	initWindow();
}

KExeWin::~KExeWin()
{
}

void KExeWin::initWindow()
{
	setMinimumSize(200, 400);
	QLabel* addInputLabel = new QLabel(tr("addInputLabelTr") + ":", this);
	QPushButton* addInputBtn = new QPushButton(QIcon(":/icons/add.png"), tr("addInputBtnTr"), this);
	QPushButton* runBtn = new QPushButton(QIcon(":/icons/runoutput.png"), tr("runBtnTr"), this);

	QHBoxLayout* m_pHBoxLay = new QHBoxLayout;
	m_pHBoxLay->addWidget(addInputLabel);
	m_pHBoxLay->addWidget(addInputBtn);
	m_pHBoxLay->addWidget(runBtn);

	m_PVBoxLay = new QVBoxLayout(this);
	m_PVBoxLay->addLayout(m_pHBoxLay);

	(void)connect(addInputBtn, &QPushButton::clicked, this, &KExeWin::onHandleAddInputLineClick);
	(void)connect(runBtn, &QPushButton::clicked, this, &KExeWin::onHandleRunClick);
}

void KExeWin::onHandleAddInputLineClick()
{
	QLabel* inputLabel = new QLabel(QStringLiteral("arg:"), this);
	QLineEdit* lineEdit = new QLineEdit(this);
	m_lineList.append(lineEdit);
	
	QHBoxLayout* m_pHInputLay = new QHBoxLayout;
	m_pHInputLay->addWidget(inputLabel);
	m_pHInputLay->addWidget(lineEdit);

	m_PVBoxLay->addLayout(m_pHInputLay);
}

void KExeWin::onHandleRunClick()
{
	QString command = KGlobalDataNameSpace::m_compileDir + "\\exec\\" + m_fileName;

	for (const QLineEdit* lineEdit : m_lineList)
	{
		command = command + " " + lineEdit->text();
	}

	QProcess process;
	process.setProcessChannelMode(QProcess::MergedChannels);  // ����׼������׼����ϲ�
	process.start(command);
	process.waitForFinished();

	QByteArray output = process.readAll();  //��ȡ�������

	if (m_pLineEdit==nullptr)
	{
		QLabel* inputLabel = new QLabel(tr("inputLabelTr"), this);
		m_pLineEdit = new QLineEdit(this);

		QHBoxLayout* m_pHOutputLay = new QHBoxLayout;
		m_pHOutputLay->addWidget(inputLabel);
		m_pHOutputLay->addWidget(m_pLineEdit);

		m_PVBoxLay->addLayout(m_pHOutputLay);
	}

	m_pLineEdit->setText(QString(output));
}
