import os
import subprocess
import re
import shutil
PROJECT_TYPE = ".sln"
OUTPUT_DIRECTORY = "exec"
log_file = 'build.log'
msbuildPath = 'C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Professional\\MSBuild\\Current\\Bin\\MSBuild.exe'

def delete_files_and_dirs(root_path):
    for root, dirs, files in os.walk(root_path, topdown=False):
        for file in files:
            file_path = os.path.join(root, file)
            if file_path.endswith('.tlog') \
                or file_path.endswith('.lastbuildstate') \
                or file_path.endswith('.idb') \
                or file_path.endswith('.exe') \
                or file_path.endswith('.pdb') \
                or file_path.endswith('.ilk') \
                or file_path.endswith('.obj'):
                os.remove(file_path)    #删除指定路径的文件
                print(f"Deleted {file_path}")
        for dir in dirs:
            dir_path = os.path.join(root, dir)
            if not os.listdir(dir_path):  #检查指定目录是否为空
                os.rmdir(dir_path)

def build_solution(root_dir,working_dir,sln_file_path,f):
    msbuild_cmd = [msbuildPath, '/m', sln_file_path, '/p:Configuration=Debug',
                   '/flp1:errorsonly;logfile={0}'.format(log_file)];
    try:
        subprocess.check_call(msbuild_cmd,cwd=working_dir)  # 在指定工作路径下执行musbuild_cmd命令
        #with open(log_file,'a') as f1:
        #   subprocess.check_call(msbuild_cmd, cwd=working_dir,stdout=f1,stderr=f1)
        return True
    except subprocess.CalledProcessError as error:
        f.write(sln_file_path)
        f.write('\n')
        return False

def copy_output_files(project_path):
    output_path = os.path.join(os.path.dirname(project_path), OUTPUT_DIRECTORY)
    os.makedirs(output_path, exist_ok=True)      #创建指定目录
    for root, dirs, files in os.walk(project_path):
        for file in files:
            if file.endswith('.exe'):
                src_path = os.path.join(root, file)
                dst_path = os.path.join(output_path, file)
                shutil.copy(src_path, dst_path)   #将源文件或目录从src_path复制到目标位置dst_path



def compile_sln_files(root_dir):       #编译指定目录下的sln文件
    errerlist_file = root_dir + '\\errorlist.txt'
    f = open(errerlist_file, 'w')
    for root, dirs, files in os.walk(root_dir):                 #os.walk()函数遍历指定目录下所有文件和目录
        for name in files:
            if re.search(r'\.sln$', name):
                sln_file_path=os.path.join(root,name)
                working_dir=os.path.dirname(sln_file_path)
                if(build_solution(root_dir,working_dir,sln_file_path,f)):
                    copy_output_files(working_dir)
                    delete_files_and_dirs(working_dir)






