# des: 用来将百度脑图导出的md进一步转换成紧凑清晰的md文件
# time: 2021年3月19日
# author: XinRoom


class MkToMd:
    def __init__(self, filename, output=""):
        self.filename = filename
        if not output:
            _t = filename.rsplit(".")[0]
            self.output = filename.replace(_t, _t+"_out", 1)
        else:
            self.output = output

        self.start_num = 4

        # 存储类型状态计数
        self.status = {}
        self.main()
        
    def main(self):
        # outfile
        outf = open(self.output, "w", encoding="utf-8")
        # 读取文件
        with open(self.filename, encoding="utf-8") as f:
            last_status = 0
            last_line = ""
            code_status = False
            while 1:
                line = f.readline()
                if line == "":
                    break
                # line = line.rstrip("\r\n")
                # 匹配
                if line.startswith("#"):
                    _len = len(line.split(" ", 1)[0])
                    last_status = _len
                    if _len == 1:
                        pass
                    elif _len >= self.start_num:  # 转换为列表
                        line = line.replace("#"*_len, "  "*(_len-self.start_num)+"-", 1)
                    else:   # 添加序号
                        if _len not in self.status:
                            self.status[_len] = 0
                        self.status[_len]+=1

                        # 正向序号清空
                        _i = 1
                        while _len+_i in self.status:
                            del self.status[_len+_i]
                            _i += 1

                        _to_str = str(self.status[_len])
                        _i = 1
                        # 方向序号添加
                        while _len-_i in self.status:
                            _to_str = str(self.status[_len-_i]) + "." + _to_str
                            _i += 1
                        if "." not in _to_str:
                            _to_str += "."

                        # 
                        _t = line.split(" ", 1)[0]
                        line = line.replace(_t, _t+" "+_to_str, 1)
                else:
                    _add_str = "  "*(last_status-self.start_num)

                    # 避免code区域的空行被清除
                    if line.rstrip("\r\n").startswith("```"):
                        code_status = not code_status
                    # 去掉多余的空行
                    if line.rstrip("\r\n") == "" and not code_status:
                        continue
                    _add_str += "  "
                    line = _add_str + line
                
                if line.lstrip(' ').startswith("- "):
                    last_line = line
                outf.write(line)

if __name__ == "__main__":
    MkToMd("1外网信息收集.md")