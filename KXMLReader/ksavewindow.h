#ifndef __KFILELISTENER_KSAVEWINDOW_H__
#define __KFILELISTENER_KSAVEWINDOW_H__

#include <QWidget>
#include<QLineEdit>

class KSaveWindow : public QWidget
{
	Q_OBJECT

public:
	KSaveWindow(QWidget* parent = nullptr);
	~KSaveWindow();

	void initWindow();
private slots:
	void onHandleDirectoryOpen();       //��ȡ�洢�ļ�Ŀ¼
	void onHandleSaveClicked();         //����SVG�ļ�����
	void onHandleCancleClicked();       //Cancleȡ����ť���
private:
	KSaveWindow(const KSaveWindow& other) = default;
	void operator=(const KSaveWindow& other) = delete;
	KSaveWindow(const KSaveWindow&& other) = delete;
	KSaveWindow& operator=(KSaveWindow&& other) = default;

	QLineEdit* m_pDirectoryLine;
};
#endif
