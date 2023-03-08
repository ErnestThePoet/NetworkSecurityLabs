**本仓库代码仅用于给大家提供实现思路，请不要照抄**
# Lab1
## FileBridge
基于TCP套接字和自定义应用层协议的Linux C/S结构双向文件传输应用，自动对传输数据进行SHA256验证，服务端支持多线程  

使用了来自[crypto-algorithms](https://github.com/B-Con/crypto-algorithms)的SHA256库  
### 程序基本工作方式
程序使用自定义的应用层协议，协议报文格式的说明在`common/packets.h`中。
客户端程序首先检查并识别用户输入的参数，确定操作类型。
#### 上传操作
对于上传操作，首先向服务器发送一个上传请求报文，其中包含用户输入的服务器端文件路径和文件大小。服务端检查磁盘空间并以写入方式打开该文件，若检查通过则发送一个上传许可响应，否则发送拒绝上传响应，其中包含拒绝原因字符串。客户端收到上传许可后，将发送文件，服务端按照先前收到的文件大小读取套接字进行接收。为了支持大文件传输，客户端对文件的读取、发送和服务端对数据的接收、写入都是分块进行的。客户端在发送时，服务端在接收时，都会同时计算文件的SHA256哈希值。之所以不单独读取文件计算SHA256，是为了避免对较大的文件进行多次读写，同时利用发送时的分块避免开辟过大的缓冲区。客户端完成上传后，立即发送计算完成的文件SHA256，服务端接收并进行验证。若验证成功，服务端返回一个上传成功响应，否则返回一个包含错误原因的上传失败响应。
#### 下载操作
对于下载操作，客户端将首先向服务端发送含有服务端文件路径的下载请求报文，服务端收到后打开文件，若成功则返回一个包含文件大小的下载许可响应，否则发回包含拒绝原因的拒绝响应。服务端发送完下载许可后，就将分块读取并发送文件，客户端在此时进行接收。服务端、客户端也会同时计算文件的SHA256。服务端在发送完成后，立即发送计算得到的SHA256，客户端接收后进行校验，若校验失败则给出警告提示。
#### 获取服务端文件列表操作
对于获取服务端文件列表操作，客户端向服务端发送获取文件列表请求报文，其中含有用户指定的目录路径。服务端利用`cd`和`ls`命令获取目录下的文件和目录列表，并将结果字符串或报错信息封装成响应报文发回客户端。
#### 套接字接收数据的注意事项
值得我们注意的是，TCP套接字的特性是：能保证发送数据的按顺序到达，但取决于传输情况，不能保证一次`recv()`能从缓冲区读出多少数据。为此，当我们期望接收某固定大小的数据时，就需要记录当前总共已读出的字节数，在循环中不断读取套接字，直到总共读取出的字节数达到我们的目标为止。**只要想从套接字中读取固定大小的数据，就一定要采取此方式读取。** 这一读取方式封装在`common/read_socket.c`内的`ReadSocket()`函数中。

<image src="https://github.com/ErnestThePoet/NetworkSecurityLabs/blob/master/Lab1/file-bridge.png"/>

## ConnectRadar
使用WPF编写的基于Connect扫描的Windows端口扫描程序  
### 程序基本工作方式
根据输入的IP地址范围和端口范围，枚举出IP地址-端口组合，分配给各线程进行扫描。Connect扫描原理很简单，当TCP套接字的Connect方法调用失败/超时，就认为端口关闭。实现中，有三点值得注意：
#### 扫描任务的分配
假设有1000个扫描任务（IP地址-端口组合）要分配给300个线程，若直接根据`1000/300=3`来让每个线程执行3个扫描任务，则当为最后一个线程分配任务时，仍有`1000—299*3=103`个任务没有分配。为了较好地解决此问题，在每次分配完一个线程的任务后，根据当前剩余的任务数量和线程数量，重新计算每个线程要执行的任务数量，用于下一个线程的任务分配。此分配方法的伪代码如下：
```
if(总线程数量>总任务数量){
    总线程数量=总任务数量; // 确保每个线程至少执行一个扫描任务
}
剩余任务数量=总任务数量;
剩余线程数量=总线程数量;
当前每个线程要分配的任务数量=剩余任务数量/剩余线程数量;
当前线程的任务集合=空;
for(每个IP地址ip){
    for(每个端口号port){
        当前线程的任务集合.Add({ip,port});
        剩余任务数量--;
        
        if(当前线程的任务集合.Size==当前每个线程要分配的任务数量){
            启动线程;
            当前线程的任务集合=空;
            
            剩余线程数量--;
            
            // 剩余线程数量为0时，说明已经完成了最后一个线程的任务分配。由于给最后一个线程
            // 分配的任务数量一定等于所有剩余的任务数量，因此此时所有任务都已经分配完成，
            // 循环即将退出。
            if(剩余线程数量>0){
                当前每个线程要分配的任务数量=剩余任务数量/剩余线程数量;
            }
        }
    }
}
```
#### Connect超时的设置
若不手动设置连接超时时间，系统就会使用默认的比较长的超时时间，在被扫描的端口关闭时，等待时间较长。为此可以设置连接的最长超时时间，各语言中的实现方法各有不同，可以谷歌"Set socket connect timeout"寻找相关资料。

#### 扫描结果展示时的线程安全
设添加一条扫描结果是通过写入变量S实现的，则如果多个线程同时直接进行写入，就有可能发生访问竞争，导致某些线程写入的数据丢失。因此，在各线程修改S时，需要对S加锁（如互斥锁），确保对S的写入是线程安全的，即当前线程写入S时其他线程无法写入S。高级语言一般都提供了很简单的方式实现线程安全，可自行搜索例子。

<image src="https://github.com/ErnestThePoet/NetworkSecurityLabs/blob/master/Lab1/connect-radar.png"/>
