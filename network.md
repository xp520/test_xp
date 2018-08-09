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
PeerNet为基础类；定义了基本peernet的基本行为（peer配置、移除无效或错误的peer），具体构建由CNetwrok类完成。

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
virtual void HandlePeerWriten(CPeer *pPeer);//获取peer写入
void EnterLoop();
void LeaveLoop();
void HeartBeat();
void Timeout(uint64 nNonce,uint32 nTimerId);
std::size_t GetMaxOutBoundCount();
bool ClientAccepted(const boost::asio::ip::tcp::endpoint& epService,CIOClient *pClient);
bool ClientConnected(CIOClient *pClient);
void ClientFailToConnect(const boost::asio::ip::tcp::endpoint& epRemote);
void HostResolved(const CNetHost& host,const boost::asio::ip::tcp::endpoint& ep);
CPeer* AddNewPeer(CIOClient *pClient,bool fInBound);
void RewardPeer(CPeer *pPeer,const CEndpointManager::Bonus& bonus);
void RemovePeer(CPeer *pPeer,const CEndpointManager::CloseReason& reason);
CPeer* GetPeer(uint64 nNonce);
void AddNewNode(const CNetHost& host);
void AddNewNode(const boost::asio::ip::tcp::endpoint& epNode,
                const std::string& strName = "",const boost::any& data = boost::any());
void RemoveNode(const CNetHost& host);
void RemoveNode(const boost::asio::ip::tcp::endpoint& epNode);
std::string GetNodeName(const boost::asio::ip::tcp::endpoint& epNode);
bool GetNodeData(const boost::asio::ip::tcp::endpoint& epNode,boost::any& data); 
bool SetNodeData(const boost::asio::ip::tcp::endpoint& epNode,const boost::any& data);
void RetrieveGoodNode(std::vector<CNodeAvail>& vGoodNode,int64 nActiveTime,std::size_t nMaxCount);
virtual std::string GetLocalIP();
virtual CPeer* CreatePeer(CIOClient *pClient,uint64 nNonce,bool fInBound);
virtual void DestroyPeer(CPeer* pPeer);
virtual CPeerInfo* GetPeerInfo(CPeer* pPeer,CPeerInfo* pInfo = NULL);
bool HandleEvent(CWalleveEventPeerNetGetIP& eventGetIP); 
bool HandleEvent(CWalleveEventPeerNetGetCount& eventGetCount); 
bool HandleEvent(CWalleveEventPeerNetGetPeers& eventGetPeers); 
bool HandleEvent(CWalleveEventPeerNetAddNode& eventAddNode); 
bool HandleEvent(CWalleveEventPeerNetRemoveNode& eventRemoveNode); 
bool HandleEvent(CWalleveEventPeerNetGetBanned& eventGetBanned); 
bool HandleEvent(CWalleveEventPeerNetSetBan& eventSetBan); 
bool HandleEvent(CWalleveEventPeerNetClrBanned& eventClrBanned);
bool HandleEvent(CWalleveEventPeerNetReward& eventReward);
bool HandleEvent(CWalleveEventPeerNetClose& eventClose);
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

上层组网主要是靠 CNetwork，包括


|整理者|日期|
|---|--|
|xp|201808|