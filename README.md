# SCUT 未来技术学院 2023 OS课设
linux环境下分别执行client和server中的compile.sh脚本即可编译，需要g++
## 程序结构
### common
common文件夹下定义与实现了一些通用的类、结构体和枚举，主要分为FileSystem和ComState
#### class FileSystem
一个模拟的文件系统，采用Inode类型
#### enum ComState
定义了一些Socket通信所用的信号
### client
client文件夹下定义与实现允许的用户行为，用户可以在终端中选择并执行这些行为
#### class Client
实现了Socket通信，对收发信息进行了包装，与class Server相对应
#### class FileSystemUI
类中通过调用FileSystem的接口操作FileSystem，使得用户能够使用指令进行文件操作
#### class SIM
学生信息管理系统，用户通过它访问服务端，执行权限内的操作
### server
server文件夹下定义与实现了服务端需要的功能
#### class Server
实现了Socket通信，对收发信息进行了包装，与class Client相对应
当一个客户端链接后，创建一个子线程处理客户端的请求
#### class Handler
定义与实现了处理客户端的各个请求的对应方法，由class Server启用，往下对接class Database
#### class Database
包装了一些文件操作以方便上层class Handler处理请求，在这一级实现对文件系统的并行化保护