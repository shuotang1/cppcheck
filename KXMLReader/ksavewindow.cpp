// --------------------------------------------------------------
// ksavewindow.cpp
// 创建时间:2023/10/26
// 功能描述：保存为csv文件窗口
// ----------------------------------------------------------------
#include "ksavewindow.h"
#include "kglobaldata.h"
#include "kdatabase.h"

#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QStandardPaths>
#include <QMessageBox>

KSaveWindow::KSaveWindow(QWidget* parent)
	: QWidget(parent)
	, m_pDirectoryLine(nullptr)
{
	initWindow();
}

KSaveWindow::~KSaveWindow()
{
}

void KSaveWindow::initWindow()
{
	setMinimumSize(200, 100);

	QLabel* m_pFileNameLabel = new QLabel(QStringLiteral("File Name: "), this);
	QLabel* m_pReportFormat = new QLabel(tr("reportFormatLabelTr") + ": ", this);

	QPushButton* m_pDirectoryBtn = new QPushButton(QIcon(":/icons/folder.png"), "", this);
	QPushButton* m_pSavBtn = new QPushButton(QStringLiteral("Save"), this);
	QPushButton* m_pCancelBtn = new QPushButton(QStringLiteral("Cancel"), this);

	m_pDirectoryLine = new QLineEdit(this);

	QComboBox* m_pReportFormatBox = new QComboBox(this);
	m_pReportFormatBox->addItem(tr("reportFormatBoxAddCheckTr"));

	QHBoxLayout* m_pHFirFileNameLay = new QHBoxLayout;
	m_pHFirFileNameLay->addWidget(m_pFileNameLabel);
	m_pHFirFileNameLay->addWidget(m_pDirectoryLine);
	m_pHFirFileNameLay->addWidget(m_pDirectoryBtn);

	QHBoxLayout* m_pHSecReportLay = new QHBoxLayout;
	m_pHSecReportLay->addWidget(m_pReportFormat);
	m_pHSecReportLay->addWidget(m_pReportFormatBox);

	QHBoxLayout* m_pHThirBtnLay = new QHBoxLayout;
	m_pHThirBtnLay->addWidget(m_pSavBtn);
	m_pHThirBtnLay->addWidget(m_pCancelBtn);

	QVBoxLayout* m_pVMainLay = new QVBoxLayout(this);
	m_pVMainLay->addLayout(m_pHFirFileNameLay);
	m_pVMainLay->addLayout(m_pHSecReportLay);
	m_pVMainLay->addLayout(m_pHThirBtnLay);

	(void)connect(m_pDirectoryBtn, &QPushButton::clicked, this, &KSaveWindow::onHandleDirectoryOpen);
	(void)connect(m_pSavBtn, &QPushButton::clicked, this, &KSaveWindow::onHandleSaveClicked);
	(void)connect(m_pCancelBtn, &QPushButton::clicked, this, &KSaveWindow::onHandleCancleClicked);
}

void KSaveWindow::onHandleDirectoryOpen()
{
	QString filePath = QFileDialog::getSaveFileName(this, tr("filePathTr"),KGlobalDataNameSpace::m_openDirDefaultPath, QStringLiteral("CSV(*.csv)"));
	if (filePath.isEmpty())
		return;
	m_pDirectoryLine->setText(filePath);
}

void KSaveWindow::onHandleSaveClicked()
{
	KDataBase db;
	bool ok = db.saveCSV(m_pDirectoryLine->text());
	if (!ok)
		return;
	QMessageBox::warning(Q_NULLPTR, QStringLiteral("InputError"), QStringLiteral("success"), QMessageBox::Ok);
	m_pDirectoryLine->clear();
	onHandleCancleClicked();
}

void KSaveWindow::onHandleCancleClicked()
{
	this->hide();
}