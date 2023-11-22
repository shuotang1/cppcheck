import os
import sys

# 获取main.py文件的路径
current_path = os.path.dirname(os.path.abspath(__file__))

# 将当前路径添加到Python解释器的搜索路径中
sys.path.append(current_path)
sys.path.append(current_path + r"\venv")

repoList_csv_file = current_path + r"\venv\repoList.csv"

import createGogsProject

org_name = sys.argv[1]
# org_name = "test"

root_url = sys.argv[2]
api_url = root_url + 'api/v1'
access_token = sys.argv[3]

createGogsProject.associate_user_and_repository(org_name,repoList_csv_file, api_url, access_token, root_url)