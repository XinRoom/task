'''
@文件    :files.py
@说明    :文件和路径操作相关函数
@时间    :2021/02/01 13:27:26
@作者    :XinRoom
@版本    :1.0
'''

import os

# 遍历获取目录下的特定后缀文件
def get_all_path(dirname: str, ext=[]):
    result = []
    for maindir, _, file_name_list in os.walk(dirname):
        for filename in file_name_list:
            if ext == [] or os.path.splitext(filename)[-1] in ext:
                apath = os.path.join(maindir, filename)
                result.append(apath)
    result.sort()
    return result

# 搜索文件内容和上下文
def search_file(filename: str, keys: list, 上下文行数=5):
    lines = []
    i = 0
    res = {}
    st = -1
    ed = -1
    with open(filename, 'r', encoding='utf-8', errors='backslashreplace') as f:
        lines = f.readlines()
    for line in lines:
        for key in keys:
            if key not in res:
                res[key] = []
            if key.lower() in line.lower():
                if i >= st and i <= ed:
                    continue    # 避免不同行多次出现
                st = i-上下文行数
                ed = i+上下文行数
                res[key].append("".join(lines[max(0, st):min(len(lines), ed)]))
                break
        i += 1
    return res