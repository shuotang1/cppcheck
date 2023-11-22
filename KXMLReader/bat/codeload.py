import json
import urllib
import urllib.request
import os

'''
token = '9a6ef355d1a0cb1b5dc25815622ba5a35bf0a990'
org = 'cug-2023-cpp-tasks'
host = 'https://git.wpsit.cn/api/v1'
'''

def read_txt_file(file_path):
    # 打开文件，读取每一行字符串，并将其保存到一个列表中
    with open(file_path, "r") as f:
        lines = f.readlines()
        string_list = [line.strip() for line in lines]
    return string_list

def get_repos(host,org,token):
  url = host + '/orgs/' + org + '/repos?token=' + token
  res = urllib.request.urlopen(url)   #打开指定url，并将返回的响应对象赋值给变量res（socket）
  data = res.read().decode('utf-8')   #从响应对象res中读取数据
  return json.loads(data)             #将变量中的json字符串解析为python对象

def pullCode(host,org,token,outputDir):              #git仓库拉取远方代码
    repos = get_repos(host,org,token)
    namelist = read_txt_file('userlist')
    print(namelist)
    for repo in repos:
        name = repo['name']
        if name in namelist:
            cwd = os.path.join(outputDir, name)       #将baseDir和name两个路径合并为一个新的路径
            if os.path.exists(cwd) == False:
                os.chdir(outputDir)                   #将当前工作目录更改为baseDir目录
                print('clone ' + repo['clone_url'])
                os.system('git clone ' + repo['clone_url'])  #克隆git仓库的url
            else:
                os.chdir(cwd)
                print('pull ' + repo['clone_url'])
                os.system('git pull')               #从远程仓库拉取最新提交并合并到当前分支
