import os
import sys

# 获取main.py文件的路径
current_path = os.path.dirname(os.path.abspath(__file__))

# 将当前路径添加到Python解释器的搜索路径中
sys.path.append(current_path)
sys.path.append(current_path + r"\venv")

import createOrgProject

org_name = sys.argv[1]

root_url = sys.argv[2]
api_url = root_url + 'api/v1'
access_token = sys.argv[3]

createOrgProject.check_org_exist(org_name, api_url, access_token)   #--TODO bug:当org_name=1时，即使原仓库不存在org_name，也会显示存在