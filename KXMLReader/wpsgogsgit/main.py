import os
import sys

# 获取main.py文件的路径
current_path = os.path.dirname(os.path.abspath(__file__))

# 将当前路径添加到Python解释器的搜索路径中
sys.path.append(current_path)
sys.path.append(current_path + r"\venv")

import createOrgProject

org_name = sys.argv[1]
org_description = sys.argv[2]


root_url = sys.argv[3]
api_url = root_url + 'api/v1'
access_token = sys.argv[4]

# Gogs API 地址和访问令牌
# root_url = 'https://git.wpsit.cn/'
# api_url = root_url + 'api/v1'
# access_token = '779a5f76dd3d877c05c0f47662f23c8a1217dd6a'

createOrgProject.create_org(org_name, org_description, root_url, api_url, access_token)