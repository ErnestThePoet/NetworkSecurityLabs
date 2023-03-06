# Lab1
## FileBridge
基于TCP套接字和自定义应用层协议的Linux C/S结构双向文件传输应用，自动对传输数据进行SHA256验证，服务端支持多线程  

使用了来自[crypto-algorithms](https://github.com/B-Con/crypto-algorithms)的SHA256库  
### 程序基本工作方式
程序使用自定义的应用层协议，协议报文格式的说明在`common/packets.h`中。  
客户端程序在识别用户输入参数后，对于上传操作，首先向服务器发送一个上传请求报文，其中包含用户输入的服务器端文件路径和文件大小。服务端检查磁盘空间并以写入方式打开该文件，若检查通过则发送一个上传许可响应，否则发送拒绝上传响应，其中包含拒绝原因字符串。客户端收到上传许可后，将发送文件，服务端按照先前收到的文件大小读取套接字进行接收。为了支持大文件传输，客户端对文件的读取、发送和服务端对数据的接收、写入都是分块进行的。客户端在发送时，服务端在接收时，都会同时计算文件的SHA256哈希值。之所以不单独读取文件计算SHA256，是为了避免对较大的文件进行多次读写，同时利用发送时的分块避免开辟过大的缓冲区。客户端完成上传后，立即发送计算完成的文件SHA256，服务端接收并进行验证。若验证成功，服务端返回一个上传成功响应，否则返回一个包含错误原因的上传失败响应。  
对于下载操作，客户端将首先向服务端发送下载含有服务端文件路径的下载请求报文，服务端收到后打开文件，若成功则返回一个包含文件大小的下载许可响应，否则发回包含拒绝原因的拒绝响应。服务端发送完下载许可后，就将分块读取并发送文件，客户端在此时进行接收。服务端、客户端也会同时计算文件的SHA256。服务端在发送完成后，立即发送计算得到的SHA256，客户端接收后进行校验，若校验失败则给出警告提示。  
值得我们注意的是，TCP套接字的特性是：能保证发送数据的按顺序到达，但取决于传输情况，不能保证一次`recv`能从缓冲区读出多少数据。为此，当我们期望接收某固定大小的数据时，就需要记录当前总共已读出的字节数，在循环中不断读取套接字，直到总共读取出的字节数达到我们的目标为止。**只要想从套接字中读取固定大小的数据，就一定要采取此方式读取。**这一读取方式封装在`common/read_socket.c`内的`ReadSocket()`函数中。

<image src="https://github.com/ErnestThePoet/NetworkSecurityLabs/blob/master/Lab1/file-bridge-usage.png"/>

<image src="https://github.com/ErnestThePoet/NetworkSecurityLabs/blob/master/Lab1/file-bridge.png"/>

## ConnectRadar
使用WPF编写的基于Connect扫描的Windows端口扫描程序  

<image src="https://github.com/ErnestThePoet/NetworkSecurityLabs/blob/master/Lab1/connect-radar.png"/>
