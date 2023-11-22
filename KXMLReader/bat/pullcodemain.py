import os
import sys

# 获取main.py文件的路径
current_path = os.path.dirname(os.path.abspath(__file__))

# 将当前路径添加到Python解释器的搜索路径中
sys.path.append(current_path)

import codeload

host = sys.argv[1]
org = sys.argv[2]
token = sys.argv[3]
outputDir = sys.argv[4]

path = codeload.pullCode(host,org,token,outputDir)