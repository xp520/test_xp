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
PeerNet为基础类；定义了基本peernet的基本行为（peer配置、移除无效或错误的peer），具体构建由CNetwrok类完成。通过CEndpointManager对象对网络端点进行管理。

|源文件  	            |      类            |       描述|
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
void EnterLoop();//获取配置中的peer列表，并建立连接
void LeaveLoop();//断开所有连接中的peer,停止p2pnet
void HeartBeat();//心跳
void Timeout(uint64 nNonce,uint32 nTimerId);//节点超时移除
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

## CEndpointManager简述
---
////

|源文件|类 | 描述|
| ------------|------------|----------|
|epmngr.h,epmngr.cpp   |CEndpointManager   |  |
||CConnAttempt|工具类,用于稳定尝试间隔|
||CAddressStatus|地址状态|
||CAddressBanned||
||CNodeAvail||

### 主要变量及函数
#### 主要变量

|类	|变量名称	|类型		|描述|
|-------|---------------|---------------|----------------------------------|
|CEndpointManager|mngrNode|CNodeManager|节点管理对象|
||mapAddressStatus|std::map\<boost::asio::ip::address,CAddressStatus>||
##### 枚举
```cpp
//关闭原因枚举
enum CloseReason
    {
        HOST_CLOSE = 0,//主机关闭
        CONNECT_FAILURE,//连接失败
        NETWORK_ERROR ,//网络错误
        RESPONSE_FAILURE,//返回失败
        PROTOCOL_INVALID,//协议无效
        DDOS_ATTACK,//DDOS攻击
        NUM_CLOSEREASONS,//原因计数
    };
//奖励类型枚举
enum Bonus
    {
        OPERATION_DONE = 0,
        MINOR_DATA,
        MAJOR_DATA,//交易奖励
        VITAL_DATA,//出块奖励
        NUM_BONUS,
    };   
enum {
      MAX_ADDRESS_COUNT = 2048,//最大地址数量
      MAX_INACTIVE_TIME = 864000//最长闲置时间
  }; 
```
#### 主要函数
##### CConnAttempt
```cpp
bool Attempt(int64 ts);//限制流量,稳定访问间隔,预防ddos
```
##### CAddressStatus

## XX简述
---
////

|源文件|类|描述|
|-----|--|---|
|xx.h,xx.cpp   |xx             |  |

### 主要变量及函数
#### 主要变量

|类	|变量名称	|类型		|描述                          |
|-------|---------------|----------|----------------|
|||||

#### 主要函数
```cpp

```

上层组网主要是靠 CNetwork，包括


|整理者|日期|
|---|--|
|xp|201808|