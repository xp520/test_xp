# P2P network

## CPeer简述
  peer为p2p的基本类，通过封装CIOClient类对象实现netio   (boost::asio::ip::tcp::socket);

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
PeerNet为基础类；定义了基本peernet的基本行为:包括peer的创建,配置,奖励和惩罚,以及移除无效或错误的peer,包括对peer的事件响应及针对peer的查询等功能;具体构建由CNetwrok类完成初始化。通过CEndpointManager对象对网络端点进行管理。
##### 实现peer相关的RPC命令(getpeercount,listpeer,addnode,removenode)

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
void HeartBeat();//获取一个节点,连接
void Timeout(uint64 nNonce,uint32 nTimerId);//节点超时移除
bool ClientAccepted(const boost::asio::ip::tcp::endpoint& epService,CIOClient *pClient);//接受客户端连接
bool ClientConnected(CIOClient *pClient);//客户端连接
void ClientFailToConnect(const boost::asio::ip::tcp::endpoint& epRemote);//连接失败
void HostResolved(const CNetHost& host,const boost::asio::ip::tcp::endpoint& ep);//host状态为toremove时移除,否则添加新节点
CPeer* AddNewPeer(CIOClient *pClient,bool fInBound);//添加新peer
void RewardPeer(CPeer *pPeer,const CEndpointManager::Bonus& bonus);//奖励节点,减少连接延时
void RemovePeer(CPeer *pPeer,const CEndpointManager::CloseReason& reason);//移除peer
CPeer* GetPeer(uint64 nNonce);//获取peer
void AddNewNode(const CNetHost& host);//添加新节点
void AddNewNode(const boost::asio::ip::tcp::endpoint& epNode,
                const std::string& strName = "",const boost::any& data = boost::any());//添加新节点
void RemoveNode(const CNetHost& host);//移除节点
void RemoveNode(const boost::asio::ip::tcp::endpoint& epNode);//移除节点
std::string GetNodeName(const boost::asio::ip::tcp::endpoint& epNode);//获取节点名称
bool GetNodeData(const boost::asio::ip::tcp::endpoint& epNode,boost::any& data); //获取节点数据
bool SetNodeData(const boost::asio::ip::tcp::endpoint& epNode,const boost::any& data);//设置节点数据
void RetrieveGoodNode(std::vector<CNodeAvail>& vGoodNode,int64 nActiveTime,std::size_t nMaxCount);//重新获取好的节点列表
virtual std::string GetLocalIP();//获取本地地址
virtual CPeer* CreatePeer(CIOClient *pClient,uint64 nNonce,bool fInBound);//创建peer
virtual void DestroyPeer(CPeer* pPeer);//销毁peer
virtual CPeerInfo* GetPeerInfo(CPeer* pPeer,CPeerInfo* pInfo = NULL);//获取节点信息
bool HandleEvent(CWalleveEventPeerNetGetIP& eventGetIP); //获取本地地址事件
bool HandleEvent(CWalleveEventPeerNetGetCount& eventGetCount);//获取peer数量事件(RPC:getpeercount) 
bool HandleEvent(CWalleveEventPeerNetGetPeers& eventGetPeers); //获取peer列表事件(RPC:listpeer)
bool HandleEvent(CWalleveEventPeerNetAddNode& eventAddNode); //添加节点事件(RPC:addnode)
bool HandleEvent(CWalleveEventPeerNetRemoveNode& eventRemoveNode); //移除节点事件(RPC:removenode))
bool HandleEvent(CWalleveEventPeerNetGetBanned& eventGetBanned); //获取禁止列表事件(无RPC)
bool HandleEvent(CWalleveEventPeerNetSetBan& eventSetBan); //禁止节点事件(无RPC)
bool HandleEvent(CWalleveEventPeerNetClrBanned& eventClrBanned);//清空禁止节点列表事件(无RPC)
bool HandleEvent(CWalleveEventPeerNetReward& eventReward);//奖励节点事件(添加新block和tx时触发)
bool HandleEvent(CWalleveEventPeerNetClose& eventClose);//断开指定节点连接事件(CNetChannel中触发)
```
---
## CMvPeerNet简述
---
对peernet做了更上层的封装，

|源文件|类|描述|
|-----|--|---|
|xx.h,xx.cpp|xx|  |

### 主要变量及函数
#### 主要变量

|类	|变量名称	|类型		|描述                          |
|-------|---------------|----------|----------------|


#### 主要函数
```cpp

```


## CEndpointManager简述
---
对节点进行管理,防止DDOS攻击等

|源文件|类 | 描述|
| ------------|------------|----------|
|epmngr.h,epmngr.cpp   |CEndpointManager   |  |
||CConnAttempt|工具类,用于控制尝试间隔|
||CAddressStatus|地址状态,控制尝试链接的间隔,包含频繁访问惩罚|
||CAddressBanned|实体类,被禁止访问的地址|
||CNodeAvail|实体类,|

### 主要变量及函数
#### 主要变量

|类	|变量名称	|类型		|描述|
|-------|---------------|---------------|----------------------------------|
|CEndpointManager|mngrNode|CNodeManager|节点管理对象|
||mapAddressStatus|std::map\<address,CAddressStatus>|地址状态|
|CAddressStatus|nScore|int |评分,评估惩罚-500到+100|
||nLastSeen|longlong|上次尝试时间|
||nBanTo|longlong|禁止到_时间|
||connAttempt|CConnAttempt|稳定访问间隔|
||nConnections|int|尝试计数|
|CAddressBanned|addrBanned|boost::asio::ip::address|禁止地址|
||nScore|int|评分|
||nBanTime|longlong|禁止到_时间|
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
##### CConnAttempt类
```cpp
bool Attempt(int64 ts);//限制流量,控制访问间隔,预防ddos
```
##### CAddressStatus类
```cpp
bool InBoundAttempt(int64 ts);//尝试访问
void Reward(int nPoints,int64 ts);//奖励,增加评分,减少访问间隔
void Penalize(int nPoints,int64 ts);//惩罚,降低,增加访问间隔+50
```
##### CEndpointManager类
```cpp
int  GetEndpointScore(const boost::asio::ip::tcp::endpoint& ep);//获取指定地址评分
void GetBanned(std::vector<CAddressBanned>& vBanned);//获取禁止访问的地址列表
void SetBan(std::vector<boost::asio::ip::address>& vAddrToBan,int64 nBanTime);//禁止地址
void ClearBanned(std::vector<boost::asio::ip::address>& vAddrToClear);//为列表中的地址解除禁止访问
void ClearAllBanned();//解除所有禁止地址

void AddNewOutBound(const boost::asio::ip::tcp::endpoint& ep,const std::string& strName,
                    const boost::any& data);//添加新节点
void RemoveOutBound(const boost::asio::ip::tcp::endpoint& ep);//移除节点
std::string GetOutBoundName(const boost::asio::ip::tcp::endpoint& ep);//获取节点名称
bool GetOutBoundData(const boost::asio::ip::tcp::endpoint& ep,boost::any& dataRet);//获取节点数据
bool SetOutBoundData(const boost::asio::ip::tcp::endpoint& ep,const boost::any& dataIn);//设置节点数据
bool FetchOutBound(boost::asio::ip::tcp::endpoint& ep);//获取一个节点
bool AcceptInBound(const boost::asio::ip::tcp::endpoint& ep);//接受一个节点
void RewardEndpoint(const boost::asio::ip::tcp::endpoint& ep,Bonus bonus);//奖励一个节点,降低访问延时
void CloseEndpoint(const boost::asio::ip::tcp::endpoint& ep,CloseReason reason);//惩罚一个节点
void RetrieveGoodNode(std::vector<CNodeAvail>& vGoodNode,
                        int64 nActiveTime,std::size_t nMaxCount);//重新获取好节点
void CleanInactiveAddress();//清除不活跃的地址
```
---
## CNodeManager简述
---
节点管理工具

|源文件|类|描述|
|-----|--|---|
|nodemngr.h,nodemngr.cpp   |CNodeManager|节点管理器|
||CNode|节点,实体类|

### 主要变量及函数
#### 主要变量

|类	|变量名称	|类型		|描述                          |
|-------|---------------|----------|----------------|
|CNode|ep|boost::asio::ip::tcp::endpoint|地址|
||strName|std::string|节点名称|
||data|boost::any|节点数据|
||nRetries|int|重试次数|
|CNodeManager|mapNode|std::map\<boost:..:endpoint,CNode>||节点集合
||mapIdle|std::multimap\<int64,boost:..:endpoint>|闲置节点集合,被禁止的节点|

#### 主要函数
```cpp
void AddNew(const boost::asio::ip::tcp::endpoint& ep,const std::string& strName,
            const boost::any& data);//添加节点
void Remove(const boost::asio::ip::tcp::endpoint& ep);
std::string GetName(const boost::asio::ip::tcp::endpoint& ep);//移除节点
bool GetData(const boost::asio::ip::tcp::endpoint& ep,boost::any& dataRet);//获取节点数据
bool SetData(const boost::asio::ip::tcp::endpoint& ep,const boost::any& dataIn);//设置节点数据
void Clear();//清除所有节点和数据
void Ban(const boost::asio::ip::address& address,int64 nBanTo);//禁止指定节点
bool Employ(boost::asio::ip::tcp::endpoint& ep);//雇佣下一个地址
void Dismiss(const boost::asio::ip::tcp::endpoint& ep,bool fForceRetry);//闲置地址
void Retrieve(std::vector<CNode>& vNode);//重新获取节点
void RemoveInactiveNodes();//移除不活跃的节点(禁止时间超过28800)
```
---




|整理者|日期|
|---|--|
|xp|TODO|



---
## XX简述
---
////

|源文件|类|描述|
|-----|--|---|
|xx.h,xx.cpp|xx|  |

### 主要变量及函数
#### 主要变量

|类	|变量名称	|类型		|描述                          |
|-------|---------------|----------|----------------|


#### 主要函数
```cpp

```

上层组网主要是靠 CNetwork，包括


