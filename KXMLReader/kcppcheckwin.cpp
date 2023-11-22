#include "kcppcheckwin.h"
#include "kcppcheckxml.h"
#include "kglobaldata.h"
#include "kdatabase.h"

#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QDesktopServices>
#include <QMessageBox>
#include <QUrl>
#include <QFileDialog>
#include <QHeaderView>
#include <QCoreApplication>

KCppCheckWin::KCppCheckWin(QWidget *parent)
	: QWidget(parent)
	, m_pCheckDirectoryLine(nullptr)
	, m_pTableView(nullptr)
	, m_pModel(nullptr)
	, m_pDelegate(nullptr)
	, m_pTableViewMenu(nullptr)
	, m_pErrorShowMainWin(nullptr)
{
	initCheckModel();
	initWin();
}

KCppCheckWin::~KCppCheckWin()
{
	if (m_pModel != nullptr)
	{
		delete m_pModel;
		m_pModel = nullptr;
	}
	if (m_pDelegate != nullptr)
	{
		delete m_pDelegate;
		m_pDelegate = nullptr;
	}
}

void KCppCheckWin::initWin()
{
	QLabel* checkLabel = new QLabel(tr("checkLabelTr") + ":", this);
	checkLabel->setToolTip(tr("checkLabelToolTipTr"));
	m_pCheckDirectoryLine = new QLineEdit(this);
	QPushButton* checkDirBtn = new QPushButton(QIcon(":/icons/folder.png"), QStringLiteral(""), this);
	QPushButton* checkBtn = new QPushButton(tr("checkBtnTr"), this);
	checkBtn->setEnabled(false);

	m_pTableView = new QTableView(this);
	m_pTableView->horizontalHeader()->setStretchLastSection(true);                  //设置最后一列自动填充整个表格
	m_pTableView->setEditTriggers(QTableView::NoEditTriggers);
	m_pTableView->setModel(m_pModel);
	m_pDelegate = new KCustomItemDelegate();
	m_pTableView->setItemDelegate(m_pDelegate);

	m_pTableViewMenu = new QMenu(m_pTableView);
	QAction* openFileAction = new QAction("Open File", m_pTableViewMenu);
	m_pTableViewMenu->addAction(openFileAction);
	QAction* showErrorAction = new QAction(tr("showErrorActionTr"), m_pTableViewMenu);
	m_pTableViewMenu->addAction(showErrorAction);
	m_pTableView->setContextMenuPolicy(Qt::CustomContextMenu); // 在QTableView的line列上注册customContextMenuRequested信号  

	QHBoxLayout* m_pHCheckLineLay = new QHBoxLayout;
	m_pHCheckLineLay->addWidget(checkLabel);
	m_pHCheckLineLay->addWidget(m_pCheckDirectoryLine);
	m_pHCheckLineLay->addWidget(checkDirBtn);
	m_pHCheckLineLay->addWidget(checkBtn);

	QVBoxLayout* m_pVCheckMainLay = new QVBoxLayout(this);
	m_pVCheckMainLay->addLayout(m_pHCheckLineLay);
	m_pVCheckMainLay->addWidget(m_pTableView);
	setLayout(m_pVCheckMainLay);

	(void)connect(checkDirBtn, &QPushButton::clicked, this, &KCppCheckWin::handleCheckDirClick);

	(void)connect(m_pCheckDirectoryLine, &QLineEdit::textChanged, this, [=]() {
		checkBtn->setEnabled(!m_pCheckDirectoryLine->text().isEmpty());
	});

	(void)connect(checkBtn, &QPushButton::clicked, this, &KCppCheckWin::handleCheckClick);

	(void)connect(m_pTableView, &QTableView::customContextMenuRequested, this, [&](const QPoint& pos) {
		QModelIndex index = m_pTableView->indexAt(pos);
		int columnIndex = index.column();

		if (columnIndex == 0)
			m_pTableViewMenu->exec(m_pTableView->viewport()->mapToGlobal(pos));
	});

	(void)connect(openFileAction, &QAction::triggered, this, [&]() {
		QString fileName = m_pTableView->model()->index(m_pTableView->currentIndex().row(), 0).data().toString();
		QFile file(fileName);
		if (file.exists())
			QDesktopServices::openUrl(QUrl::fromLocalFile(file.fileName()));
		else
			QMessageBox::warning(Q_NULLPTR, QStringLiteral("openFailed"), tr("openUrlFailedTr"), QMessageBox::Ok);
	});

	(void)connect(showErrorAction, &QAction::triggered, this, [&]() {
		QString fileName = m_pTableView->model()->index(m_pTableView->currentIndex().row(), 0).data().toString();
		int lineNumber = m_pTableView->model()->index(m_pTableView->currentIndex().row(), 5).data().toInt();
		int fileToTableLine = m_pTableView->currentIndex().row() + 1;
		m_pErrorShowMainWin = std::make_unique<KErrorShowMainWin>(fileName, lineNumber, fileToTableLine);
		m_pErrorShowMainWin->show();
	});
}

void KCppCheckWin::initCheckModel()
{
	if (m_pModel == nullptr)
	{
		m_pModel = new QStandardItemModel(0, 10);
		m_pModel->setHeaderData(0, Qt::Horizontal, tr("checkFileModelTr"));
		m_pModel->setHeaderData(1, Qt::Horizontal, tr("checkSymbolModelTr"));
		m_pModel->setHeaderData(2, Qt::Horizontal, tr("checkMsgModelTr"));
		m_pModel->setHeaderData(3, Qt::Horizontal, tr("checkVerboseModelTr"));
		m_pModel->setHeaderData(4, Qt::Horizontal, tr("checkIdModelTr"));
		m_pModel->setHeaderData(5, Qt::Horizontal, tr("checkLineModelTr"));
		m_pModel->setHeaderData(6, Qt::Horizontal, tr("checkColumnModelTr"));
		m_pModel->setHeaderData(7, Qt::Horizontal, tr("checkInfoModelTr"));
		m_pModel->setHeaderData(8, Qt::Horizontal, tr("checkSeverityModelTr"));
		m_pModel->setHeaderData(9, Qt::Horizontal, tr("checkCweModelTr"));
	}
	else
	{
		QString isSelected = KGlobalDataNameSpace::m_isSelected;
		int count = isSelected.count("1");
		delete m_pModel;
		m_pModel = new QStandardItemModel(0, count);
		m_pTableView->setModel(m_pModel);
		m_pModel->setHeaderData(0, Qt::Horizontal, "file");
		int cnt = 1;
		if (isSelected[1] == "1")
		{
			m_pModel->setHeaderData(cnt, Qt::Horizontal, "symbol");
			cnt++;
		}
		if (isSelected[2] == "1")
		{
			m_pModel->setHeaderData(cnt, Qt::Horizontal, "msg");
			cnt++;
		}
		if (isSelected[3] == "1")
		{
			m_pModel->setHeaderData(cnt, Qt::Horizontal, "verbose");
			cnt++;
		}
		if (isSelected[4] == "1")
		{
			m_pModel->setHeaderData(cnt, Qt::Horizontal, "id");
			cnt++;
		}
		if (isSelected[5] == "1")
		{
			m_pModel->setHeaderData(cnt, Qt::Horizontal, "line");
			cnt++;
		}
		if (isSelected[6] == "1")
		{
			m_pModel->setHeaderData(cnt, Qt::Horizontal, "column");
			cnt++;
		}
		if (isSelected[7] == "1")
		{
			m_pModel->setHeaderData(cnt, Qt::Horizontal, "info");
			cnt++;
		}
		if (isSelected[8] == "1")
		{
			m_pModel->setHeaderData(cnt, Qt::Horizontal, "severity");
			cnt++;
		}
		if (isSelected[9] == "1")
		{
			m_pModel->setHeaderData(cnt, Qt::Horizontal, "cwe");
			cnt++;
		}
	}
}

void KCppCheckWin::handleCheckDirClick()
{
	QString folderPath = QFileDialog::getExistingDirectory(this, tr("folderPathTr"), KGlobalDataNameSpace::m_openDirDefaultPath);
	if (folderPath.isEmpty())
		return;
	m_pCheckDirectoryLine->setText(folderPath);
}

void KCppCheckWin::handleCheckClick()
{
	KCppCheckXML cppcheckXml;
	QVector<ErrorInfo> errorList = cppcheckXml.outputCppcheckRunResult(m_pCheckDirectoryLine->text());

	qSort(errorList.begin(), errorList.end(), [](const ErrorInfo& c1, const ErrorInfo& c2) {
		return c1.file > c2.file;
	});

	int rowCount = m_pModel->rowCount();
	m_pModel->removeRows(0, rowCount);   //清空原表
	m_pDelegate->clearOptToBackMemory();

	initCheckModel();

	m_pDelegate->executeOptToBackMemory(errorList);
	addNewRowToModel(errorList);

	KDataBase db;
	db.clearRecord();
	for (const ErrorInfo& errorInfo : errorList)
	{
		const QString lineString = QString::number(errorInfo.line);
		const QString columnString = QString::number(errorInfo.column);
		QVector<QString> cppcheckVec;
		cppcheckVec.reserve(10);
		cppcheckVec.push_back(errorInfo.id);
		cppcheckVec.push_back(errorInfo.severity);
		cppcheckVec.push_back(errorInfo.msg);
		cppcheckVec.push_back(errorInfo.verbose);
		cppcheckVec.push_back(errorInfo.cwe);
		cppcheckVec.push_back(errorInfo.file);
		cppcheckVec.push_back(lineString);
		cppcheckVec.push_back(columnString);
		cppcheckVec.push_back(errorInfo.info);
		cppcheckVec.push_back(errorInfo.symbol);
		db.inputRecord(cppcheckVec);
	}
}

void KCppCheckWin::addNewRowToModel(const QVector<ErrorInfo>& errorList)
{
	int len = errorList.length();
	int index = 0;
	while (index < len)
	{
		QStandardItem* idItem = new QStandardItem(errorList.at(index).id);
		QStandardItem* severityItem = new QStandardItem(errorList.at(index).severity);
		QStandardItem* msgItem = new QStandardItem(errorList.at(index).msg);
		QStandardItem* verboseItem = new QStandardItem(errorList.at(index).verbose);
		QStandardItem* cweItem = new QStandardItem(errorList.at(index).cwe);
		QStandardItem* fileItem = new QStandardItem(errorList.at(index).file);
		QStandardItem* lineItem = new QStandardItem(QString::number(errorList.at(index).line));
		QStandardItem* columnItem = new QStandardItem(QString::number(errorList.at(index).column));
		QStandardItem* infoItem = new QStandardItem(errorList.at(index).info);
		QStandardItem* symbolItem = new QStandardItem(errorList.at(index).symbol);

		QList<QStandardItem*> rowItems;
		QString isSelected = KGlobalDataNameSpace::m_isSelected;
		rowItems.append(fileItem);
		if (isSelected[1] == "1")
			rowItems.append(symbolItem);
		if (isSelected[2] == "1")
			rowItems.append(msgItem);
		if (isSelected[3] == "1")
			rowItems.append(verboseItem);
		if (isSelected[4] == "1")
			rowItems.append(idItem);
		if (isSelected[5] == "1")
			rowItems.append(lineItem);
		if (isSelected[6] == "1")
			rowItems.append(columnItem);
		if (isSelected[7] == "1")
			rowItems.append(infoItem);
		if (isSelected[8] == "1")
			rowItems.append(severityItem);
		if (isSelected[9] == "1")
			rowItems.append(cweItem);
		m_pModel->appendRow(rowItems);
		index++;
		QCoreApplication::processEvents();
	}
}