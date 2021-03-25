'''
@文件    :process.py
@说明    :进程io管理相关，主要使用环境为python38+
@时间    :2021/02/01 11:58:55
@作者    :XinRoom
@版本    :1.1
'''

import os, sys
from subprocess import Popen
import asyncio, threading
from io import BytesIO
from time import sleep, time
import platform
THISOS = platform.system().lower()
import locale
DECODING = locale.getdefaultlocale()[1]

if THISOS == 'windows':
    def run_show(cmd):
        """新建一个窗体并运行(windwos)"""
        from subprocess import CREATE_NEW_CONSOLE
        Popen(cmd, creationflags=CREATE_NEW_CONSOLE)

class BytesLoop():
    """异步内存缓冲区"""
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

    def readline(self, timeout=10):
        """按行读取，默认超时10s，返回可能不是完整的一行，但不影响下一行的读取"""
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
    
    def readlines(self, lines: int, timeout=5):
        """读取多个行，每行等待默认5s"""
        _buf = b''
        _lines = 0
        while (_lines:=_lines+1)<lines:
            _buf += self.readline(timeout)
        return _buf

    def readuntll(self, untll: bytes, timeout=None):
        """读取二进制，直到匹配到untull末尾"""
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

class Cmd:
    """可以控制输入输出的异步子进程管理类"""

    def __init__(self, cmd, cwd = None, encoding=DECODING):
        """cmd=执行命令，cwd=工作路径，encoding=编码格式"""
        self.encoding = encoding
        self.env = os.environ
        self.env.update({"PYTHONIOENCODING": "utf-8", "LANG": "zh_CN.UTF-8"})
        self.env.update({"PYTHONUNBUFFERED": "1"})  # 防止python的print缓冲导致无输出
        # self.subp = pexpect.spawn()  creationflags=subprocess.CREATE_NEW_CONSOLE
        # self.subp = subprocess.Popen(cmd, shell=True, env=env, cwd=cwd, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE, start_new_session=True) #encoding=locale.getdefaultlocale()[1], errors="replace"
        self.outbuf_io = BytesLoop()
        self.errbuf_io = BytesLoop()
        self.retruncode = None

        interpreter = [] # 解释器
        if THISOS == 'windows':
            interpreter = ["cmd.exe", "/c", cmd]
        else:
            interpreter = ["bash", "-c", '"{}"'.format(cmd.replace('"', '\\"'))]
             
        self._execute(
            interpreter,
            cwd,
            lambda x: self.outbuf_io.write(x),
            lambda x: self.errbuf_io.write(x),
        )

    def decode(self, b: bytes) -> str:
        """输出时二进制转字符串"""
        if self.encoding == None:
            return b
        return b.decode(self.encoding, "backslashreplace")

    def _execute(self, cmd, cwd, stdout_cb, stderr_cb):
        """开一个线程执行异步事件"""
        self.t = threading.Thread(target=asyncio.run, args=(self._stream_subprocess(
                cmd,
                cwd,
                stdout_cb,
                stderr_cb,
        ),))   #通过当前线程开启新的线程去启动事件循环
        self.t.setDaemon(True)  # 当主线程退出，子线程也退出
        self.t.start()
        sleep(0.5)  # 等待子线程和子进程启动
    
    def stop(self):
        """杀死子进程"""
        if not self.isOver():
            self.subp.kill()

    def readline(self) -> str:
        """读取一行"""
        if self.isOver():
            return self.decode(self.outbuf_io.read())
        return self.decode(self.outbuf_io.readline(1))

    def print(self, timeout=None):
        """实时输出执行结果"""
        buf = ""
        _st = time()
        while not self.isOver():
            if timeout and time()-_st > timeout:
                break
            _buf = self.decode(self.outbuf_io.readline(1))
            buf += _buf
            print(_buf, end='')
            sys.stdout.flush()
        _buf = self.decode(self.errbuf_io.read())
        buf += _buf
        print(_buf, end='')
        _buf = self.decode(self.outbuf_io.read())
        buf += _buf
        print(_buf, end='')
        return buf
    
    def readall(self) -> str:
        """一次性读取执行结果"""
        _buf = ''
        while not self.isOver():
            _buf += self.decode(self.outbuf_io.readline(1))
        _buf +=  self.decode(self.errbuf_io.read())
        _buf += '\n'
        _buf +=  self.decode(self.outbuf_io.read())
        return _buf
    
    def readallbyte(self) -> bytes:
        """一次性读取执行结果，返回二进制形式"""
        _buf = b''
        while not self.isOver():
            _buf += self.outbuf_io.readline(1)
        _buf += self.outbuf_io.read()
        return _buf
    
    def send(self, data: str):
        """向子进程输入内容"""
        self.subp.stdin.write(data.encode(self.encoding))
    
    def sendline(self, data: str):
        """向子进程输入内容并回车"""
        if THISOS == 'windows':
            self.send(data + "\r\n")
        else:
            self.send(data + "\n")

    async def _read_stream(self, stream, cb):
        """异步流传递"""
        while (_b := await stream.read(1)):
            cb(_b)

    async def _stream_subprocess(self, cmd, cwd, stdout_cb, stderr_cb):
        """异步子进程"""
        self.subp = await asyncio.create_subprocess_exec(*cmd,
                env=self.env, cwd=cwd,
                stdin=asyncio.subprocess.PIPE, stdout=asyncio.subprocess.PIPE, stderr=asyncio.subprocess.PIPE)

        await asyncio.wait([
            self._read_stream(self.subp.stdout, stdout_cb),
            self._read_stream(self.subp.stderr, stderr_cb)
        ])
        return await self.subp.wait()

    def isOver(self):
        """判断子进程是否执行完毕"""
        self.retruncode = self.subp.returncode
        return self.retruncode != None


if __name__ == '__main__':
    # 测试使用
    if THISOS == 'windows':
        _p = Cmd('cmd.exe')
    else:
        _p = Cmd('bash')
    _p.sendline('whoami')
    _p.sendline('exit')
    _p.print()