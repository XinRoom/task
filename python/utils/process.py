'''
@文件    :process.py
@说明    :进程io管理相关，主要使用环境为Windows，python38
@时间    :2021/02/01 11:58:55
@作者    :XinRoom
@版本    :1.0
'''

import os, sys
from subprocess import Popen, CREATE_NEW_CONSOLE
import asyncio, threading
import locale
from io import BytesIO
from time import sleep, time

DECODING = locale.getdefaultlocale()[1]

# 新建一个窗体并运行
def run_show(cmd):
    Popen(cmd, creationflags=CREATE_NEW_CONSOLE)

# 异步内存缓冲区
class BytesLoop():
    def __init__(self, s=b'', encoding=None):
        self._buffer = BytesIO(s)
        self._readseek = 0
        self._writeseek = 0
        self._lock = threading.Lock()
        self.encoding = encoding
    
    def __del__(self):
        self._buffer.close()

    def read(self, n=-1):
        self._lock.acquire()
        self._buffer.seek(self._readseek)
        chunk = self._buffer.read(n)
        self._readseek += len(chunk)
        self._lock.release()
        return chunk

    def write(self, s: bytes):
        self._lock.acquire()
        self._buffer.seek(self._writeseek)
        writelen = self._buffer.write(s)
        self._writeseek += writelen
        self._lock.release()
        return writelen

    # 按行读取，默认超时10s，返回可能不是完整的一行，但不影响下一行的读取
    def readline(self, timeout=10):
        _buf = b''
        _b = b''
        _st = time()
        while not _buf.endswith(b'\n') and time()-_st < timeout:
            _b = self.read(1)
            if _b == b'':
                sleep(0.02)
            else:
                _buf += _b
        return _buf
    
    # 读取多个行，每行等待默认5s
    def readlines(self, lines: int, timeout=5):
        _buf = b''
        _lines = 0
        while (_lines:=_lines+1)<lines:
            _buf += self.readline(timeout)
        return _buf

    # 读取二进制，直到匹配到untull末尾
    def readuntll(self, untll: bytes, timeout=None):
        _buf = b''
        _b = b''
        _st = time()
        while not _buf.endswith(untll):
            if timeout and (time()-_st > timeout):
                break
            _b = self.read(1)
            # 如果读取到了空内容就停顿一段时间
            if _b == b'':
                sleep(0.02)
            else:
                _buf += _b
        return _buf

# 可以控制输入输出的异步子进程管理类
class Cmd:
    # cmd=执行命令，cwd=工作路径，encoding=编码格式
    def __init__(self, cmd, cwd = None, encoding=DECODING):
        self.encoding = encoding
        self.env = os.environ
        self.env.update({"PYTHONIOENCODING": "utf-8", "LANG": "zh_CN.UTF-8"})
        self.env.update({"PYTHONUNBUFFERED": "1"})  # 防止python的print缓冲导致无输出
        # self.subp = pexpect.spawn()  creationflags=subprocess.CREATE_NEW_CONSOLE
        # self.subp = subprocess.Popen(cmd, shell=True, env=env, cwd=cwd, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE, start_new_session=True) #encoding=locale.getdefaultlocale()[1], errors="replace"
        self.outbuf_io = BytesLoop()
        self.errbuf_io = BytesLoop()
        self.retruncode = None
        self._execute(
            ["cmd.exe", "/c", cmd],
            cwd,
            lambda x: self.outbuf_io.write(x),
            lambda x: self.errbuf_io.write(x),
        )

    # 输出时二进制转字符串
    def decode(self, b):
        if self.encoding == None:
            return b
        return b.decode(self.encoding, "backslashreplace")

    # 开一个线程执行异步事件
    def _execute(self, cmd, cwd, stdout_cb, stderr_cb):  
        self.t = threading.Thread(target=asyncio.run, args=(self._stream_subprocess(
                cmd,
                cwd,
                stdout_cb,
                stderr_cb,
        ),))   #通过当前线程开启新的线程去启动事件循环
        self.t.setDaemon(True)  # 当主线程退出，子线程也退出
        self.t.start()
        sleep(0.5)  # 等待子线程和子进程启动
    
    # 杀死子进程
    def stop(self):
        if not self.isOver():
            self.subp.kill()

    # 读取一行
    def readline(self):
        if self.isOver():
            return self.decode(self.outbuf_io.read())
        return self.decode(self.outbuf_io.readline(1))

    # 实时输出执行结果
    def print(self):
        while not self.isOver():
            print(self.decode(self.outbuf_io.readline(1)), end='')
        print(self.decode(self.errbuf_io.read()), end='')
        print(self.decode(self.outbuf_io.read()), end='')
    
    # 一次性读取执行结果
    def readall(self):
        _buf = ''
        while not self.isOver():
            _buf += self.decode(self.outbuf_io.readline(1))
        _buf += self.decode(self.errbuf_io.read())
        _buf += '\n'
        _buf += self.decode(self.outbuf_io.read())
        return _buf
    
    # 一次性读取执行结果，二进制形式
    def readallbyte(self):
        _buf = b''
        while not self.isOver():
            _buf += self.outbuf_io.readline(1)
        _buf += self.outbuf_io.read()
        return _buf
    
    # 向子进程输入内容
    def write(self, data):
        self.subp.stdin.write(data.encode(self.encoding))
    
    # 向子进程输入内容并回车
    def writeline(self, data):
        self.write(data + "\r\n")

    # 异步流传递
    async def _read_stream(self, stream, cb):
        while (_b := await stream.read(1)):
            cb(_b)

    # 异步子进程
    async def _stream_subprocess(self, cmd, cwd, stdout_cb, stderr_cb):
        self.subp = await asyncio.create_subprocess_exec(*cmd,
                env=self.env, cwd=cwd,
                stdin=asyncio.subprocess.PIPE, stdout=asyncio.subprocess.PIPE, stderr=asyncio.subprocess.PIPE)

        await asyncio.wait([
            self._read_stream(self.subp.stdout, stdout_cb),
            self._read_stream(self.subp.stderr, stderr_cb)
        ])
        return await self.subp.wait()

    # 判断子进程是否执行完毕
    def isOver(self):
        self.retruncode = self.subp.returncode
        return self.retruncode != None


if __name__ == '__main__':
    # 测试使用
    _p = Cmd('cmd.exe')
    _p.writeline('whoami')
    _p.writeline('exit')
    _p.print()