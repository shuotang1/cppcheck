#ifndef __KXMLREADER_KCOMPILEWIN_H__
#define __KXMLREADER_KCOMPILEWIN_H__

#include"kexewin.h"

#include <QWidget>
#include <QLineEdit>
#include <QListWidget>
#include <QTimer>
#include <QProgressBar>
#include <QPlainTextEdit>

class KCompileWin : public QWidget
{
	Q_OBJECT

public:
	KCompileWin(QWidget* parent = nullptr);
	~KCompileWin();
private slots:
	void handleComDirClick();
	void onHandleCompileClick();
	void onHandleTimeout();
	void onHandleOpenExeDirClick();
private:
	KCompileWin(const KCompileWin& other) = default;
	void operator=(const KCompileWin& other) = delete;
	KCompileWin(const KCompileWin&& other) = delete;
	KCompileWin& operator=(KCompileWin&& other) = default;

	void initWin();

	void initTimerStart();     //��ʱ����������������ʼ����
	void initTimerEnd();       //��ʱ������������������100

	void runExe(const QString& fileName);             //����exe�ļ�

	QLineEdit* m_pComDirLine;

	QListWidget* m_pCompileListWin;       //�������:QListWidget

	QTimer* m_pTimer;
	QProgressBar* m_pProgressBar;

	QPlainTextEdit* m_pPlainTextEdit;      //�����������ʾ�������͵��ı���

	std::unique_ptr<KExeWin> m_pExeWin;
};
#endif
