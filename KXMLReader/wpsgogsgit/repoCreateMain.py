import os
import sys

# 获取main.py文件的路径
current_path = os.path.dirname(os.path.abspath(__file__))

# 将当前路径添加到Python解释器的搜索路径中
sys.path.append(current_path)
sys.path.append(current_path + r"\venv")

repoList_csv_file = current_path + r"\venv\repoList.csv"
local_path = current_path + r'\venv\temp'

import createGogsProject

org_name = sys.argv[1]
if sys.argv[2] == '1':
    repo_private = True
else:
    repo_private = False

if sys.argv[4] == '':
    default_filepath = current_path + r"\default"
else:
    default_filepath = sys.argv[4]

root_url = sys.argv[5]
api_url = root_url + 'api/v1'
access_token = sys.argv[6]

cpplist = createGogsProject.create_gogs_group(org_name, repo_private, repoList_csv_file, root_url, api_url, access_token)
if sys.argv[3] == '1':
    createGogsProject.group_set_default_groupfile(local_path, default_filepath, cpplist, org_name, root_url)
