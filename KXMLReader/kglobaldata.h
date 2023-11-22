#ifndef __KFILELISTENER_KGLOBALDATA_H__
#define __KFILELISTENER_KGLOBALDATA_H__

#include <QStandardPaths>
#include <QString>

namespace KGlobalDataNameSpace
{
	inline QString m_fatherDirName;                      //��ǰĿ¼����
	inline QString m_compileDir;                         //����Ŀ¼    
	inline QString m_isSelected = "1111111111";          //�洢��ͼ�б�ѡ�еĲ˵���    
	inline QString m_openDirDefaultPath = QStandardPaths::DocumentsLocation;                 //��ť��Ŀ¼��Ĭ��·��    
	inline QString m_rootUrl;                            //root_url    
	inline QString m_token;                              //token    
	inline QString m_personFilePath;                     //�ֶ�����Ĭ���ļ�·�� 

	inline const QString m_createOrgPyDir = "\\wpsgogsgit\\main.py";
	inline const QString m_checkOrgExistPyDir = "\\wpsgogsgit\\orgExistMain.py";
	inline const QString m_createAccountPyDir = "\\wpsgogsgit\\userCreateMain.py";
	inline const QString m_createRepoPyDir = "\\wpsgogsgit\\repoCreateMain.py";
	inline const QString m_bindRepoAndAccountPyDir = "\\wpsgogsgit\\bindUserAndRepoMain.py";
	inline const QString m_pullCodePyDir = "\\wpsgogsgit\\pullCodeMain.py";
	inline const QString m_delRepoUserPyDir = "\\wpsgogsgit\\deleteMain.py";
	inline const QString m_compilePyDir = "\\bat\\main.py";
	inline const QString m_configFileDir = "\\config\\config.xml";

	inline QString getFatherDirName()
	{
		if (m_fatherDirName.isEmpty())
		{
			char fileBuffer[_MAX_PATH];
			if (_fullpath(fileBuffer, __FILE__, _MAX_PATH) == nullptr)
				return "";

			std::string filePathTemp = fileBuffer;

			// ��ȡ��Ŀ¼·��  
			std::size_t lastSlash = filePathTemp.find_last_of("/\\");
			std::string m_fatherDirName1 = filePathTemp.substr(0, lastSlash);

			m_fatherDirName = QString::fromStdString(m_fatherDirName1);

			/*QString filePath = QCoreApplication::applicationFilePath();
			QFileInfo fileInfo(filePath);
			m_fatherDirName = fileInfo.dir().path();*/
		}
		return m_fatherDirName;
	}
}
#endif
