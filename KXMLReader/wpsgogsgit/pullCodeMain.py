import os
import sys

# 获取main.py文件的路径
current_path = os.path.dirname(os.path.abspath(__file__))

# 将当前路径添加到Python解释器的搜索路径中
sys.path.append(current_path)
sys.path.append(current_path + r"\venv")

import loadcode

org_name = sys.argv[1]
outputDir = sys.argv[2]

root_url = sys.argv[3]
api_url = root_url + 'api/v1'
access_token = sys.argv[4]

repository_list = loadcode.get_org_repository_list(org_name, api_url, access_token)
loadcode.pullCode(repository_list, outputDir)