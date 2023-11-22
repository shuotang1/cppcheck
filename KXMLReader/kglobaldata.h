#ifndef __KFILELISTENER_KGLOBALDATA_H__
#define __KFILELISTENER_KGLOBALDATA_H__

#include <QStandardPaths>
#include <QString>

namespace KGlobalDataNameSpace
{
	inline QString m_fatherDirName;                      //当前目录名称
	inline QString m_compileDir;                         //编译目录    
	inline QString m_isSelected = "1111111111";          //存储视图中被选中的菜单项    
	inline QString m_openDirDefaultPath = QStandardPaths::DocumentsLocation;                 //按钮打开目录的默认路径    
	inline QString m_rootUrl;                            //root_url    
	inline QString m_token;                              //token    
	inline QString m_personFilePath;                     //手动配置默认文件路径 

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

			// 获取父目录路径  
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
