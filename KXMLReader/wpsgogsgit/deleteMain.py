import os
import sys

# 获取main.py文件的路径
current_path = os.path.dirname(os.path.abspath(__file__))

# root_url = 'https://git.wpsit.cn/'
# api_url = root_url + 'api/v1'
# access_token = '779a5f76dd3d877c05c0f47662f23c8a1217dd6a'

# 将当前路径添加到Python解释器的搜索路径中
sys.path.append(current_path)
sys.path.append(current_path + r"\venv")

delete_csv_file = current_path + r"\venv\delete.csv"

cpp_org_name = sys.argv[1]

root_url = sys.argv[3]
api_url = root_url + 'api/v1'
access_token = sys.argv[4]

import deleteAccount

repoList = deleteAccount.list_org_repo(cpp_org_name, api_url, access_token)
userNameList, loginList, emailList, adminList = deleteAccount.list_org_Account(cpp_org_name, repoList, api_url, access_token)

if sys.argv[2] == 'listRepoUser':
    deleteAccount.write_deleteList_csv(delete_csv_file, repoList, userNameList, loginList, emailList)
elif sys.argv[2] == 'deleteUser':
    deleteAccount.delete_org_account(userNameList, adminList, api_url, access_token)
elif sys.argv[2] == 'deleteRepo':
    deleteAccount.delete_org_repo(cpp_org_name, repoList, api_url, access_token, root_url)

