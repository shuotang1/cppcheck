import os
import sys

columnName = []
columnPassword = []
columnEmail = []
columnRepo = []
columnCheck = []

# 获取main.py文件的路径
current_path = os.path.dirname(os.path.abspath(__file__))

csv_file = current_path + r"\venv\userList.csv"
res_csv_file = current_path + r"\venv\resCsvFile.csv"
repoList_csv_file = current_path + r"\venv\repoList.csv"

# 将当前路径添加到Python解释器的搜索路径中
sys.path.append(current_path)
sys.path.append(current_path + r"\venv")

root_url = sys.argv[1]
api_url = root_url + 'api/v1'
access_token = sys.argv[2]

import createGogsAccount

createGogsAccount.get_account_data(csv_file, columnName, columnPassword, columnEmail, columnRepo, columnCheck)
createGogsAccount.create_gogs_account(columnRepo, columnPassword, columnEmail, columnCheck, api_url, access_token)
createGogsAccount.write_res_csv(res_csv_file, columnName, columnPassword, columnEmail, columnRepo, columnCheck)
createGogsAccount.write_repoList_csv(repoList_csv_file, columnRepo)