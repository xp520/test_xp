# P2P network

## CPeer简述
  peer为p2p的基本类，通过封装CIOClient类对象实现netio;

|源文件  	     |      类            |
| -------------------|------------------- |
|peer.h,peer.cpp     |	    CPeer         |

### 主要变量及函数
#### 主要变量

|类	|变量名称	|类型		|描述                               |
|-------|---------------|---------------|----------------------------------|
|CPeer 	| nNonce	|uint64 	|创建的时候生成的随机数peernet中唯一标识|
|	|pClient	|CIOClient*	|基础通讯对象,此类多数方法均借由调用此对象方法实现|
|	|pPeerNet	|CPeerNet *	|p2p网络对象                        |
|       |ssRecv|CWalleveBufStream|接收流|
|       |ssSend[2]|CWalleveBufStream|发送流|
#### 主要函数
```cpp
void Activate()； 		               //初始化                          
void Read(size_t nLength,CompltFunc fnComplt); //接收消息，并回调（由子类mvpeer调用）
void Write();                                  //发送ssSend流
void HandleRead(std::size_t nTransferred,CompltFunc fnComplt);//读消息
void HandleWriten(std::size_t nTransferred);   //写消息
const boost::asio::ip::tcp::endpoint GetRemote();//参考CIOClient
const boost::asio::ip::tcp::endpoint GetLocal();//参考CIOClient
```
---
## PeerNet 简述
PeerNet为CNetwork的基类CMvPeerNet的基类，基础类；

|源文件  	            |      类            |       其他|
| ---------------------|-------------------|----------|
|peernet.h,peernet.cpp |CPeerNet             |  基类CIOProc,CWallevePeerEventListener|
||CPeerService|实体类，存储地址端口和最大连接数量,在CNetwork中构造|
||CPeerNetConfig|实体类，存储一些列peerService和NetHost,在CNetwork中构造|

### 主要变量及函数
#### 主要变量
|类	|变量名称	|类型		|描述                          |
|-------|---------------|----------|----------------|
|CPeerNet|confNetwork|CPeerNetConfig|配置信息|
||mapPeer|std::map<uint64,CPeer*>||
||localIP|boost::asio::ip::address||
||epMngr|CEndpointManager||
|CPeerNetConfig|vecService|std::vector<CPeerService>||
||vecNode|std::vector<CNetHost>||
||nMaxOutBounds|std::size_t||
||nPortDefault|unsigned short||
|CPeerService|epListen|boost::asio::ip::tcp::endpoint|本地地址和端口|
||nMaxInBounds|std::size_t|最大绑定数量|

#### 主要函数

```cpp

```

## CMvPeer简述
---
////

|源文件  	            |      类            |       基类|
| ---------------------|-------------------|----------|
|mvpeer.h,mvpeer.cpp   |CMvPeer             |CPeer  |

### 主要变量及函数
#### 主要变量

|类	|变量名称	|类型		|描述|
|-------|---------------|---------------|----------------------------------|

#### 主要函数
```cpp

```


## XX简述
---
////

|源文件  	            |      类            |       基类|
| ---------------------|-------------------|----------|
|xx.h,xx.cpp   |xx             |  |

### 主要变量及函数
#### 主要变量

|类	|变量名称	|类型		|描述                          |
|-------|---------------|----------|----------------|

#### 主要函数
```cpp

```

CNetwork


|整理者|日期|
|---|--|
|xp|201808|