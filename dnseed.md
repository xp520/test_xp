## DNSeed服务流程及要点
  #### 核心功能: 为全网节点提供初始接入节点地址列表
#### 流程及要点:
  * DNSeed节点:只提供DNSeed服务的节点*
  * DNSeed作为新的运行模式,通过输入配置参数加载对应模块提供DNSeed服务 类似./multiverse dnseed &*
  * DNSeed服务监听6811端口,连接到DNSeed服务的节点都将被记录,但不会立即做连接测试,未测试节点存储在内存中*
  * DNSeed服务每天筛选一次列表.筛选测试流程为:建立测试连接,获取目标节点的块高度与本地对比,差异不大则视为可用地址入库,否则过滤.测试完成即断开连接.*
  * 节点连接到DNSeed服务,DNSeed服务抽取一部分可用地址列表发送给新节点.(随机抽取)*
  * DNSeed发送出去的列表中的地址必然经过测试*
  * 普通节点只有在初次入网或者本地没有可用节点时才会向DNSeed服务请求列表 *
  * 节点按照DNSeed服务提供的地址列表分别连接各节点. *
  * 普通节点只会把握手成功的节点添加到列表和库,连接失败的节点将从列表和库移除. *
  * DNSeed节点通过统计连接的节点预估全网区块高度,用作筛选规则
  
### 其他:
  * DNSeed服务地址写入代码.*
  * 普通节点维护的地址列表<=DNSeed服务地址列表.*
  * 普通节点不会去主动测试地址列表，是通过和节点握手成功数以及交换消息的有效性给节点评分

#### TODO 
  * 高度的评分机制
  * 测试(V6)

#### DONE
  * 服务流程及要点整理
  * 添加连接节点进入列表,连接DNSeed server获取其他节点地址列表
  * 列表入库
  * 重新整理服务流程及要点
  * 重新实现DNSeed结构.(DNSeed服务替换原有peernet,单独事件处理,要从mvpeer派生出来做自己那部分功能?) 需做优化
  * 连接获取的地址列表中节点
  * 定时任务(筛选)
  * 连接失败地址达到最大尝试次数移除,最大尝试次数可配置


#### 要点新
* 数据库服务器可以共用，但数据库要改名，不能在同一个库里操作，另外datadir也要换，这个你建个其它用户 (软连接?)
* 其实dnseed可以部署同一个服务器，只需要换一下端口和数据库


#### steup
##### db
* >create database dnseed;
* >grant all privileges on dnseed.* to multiverse@localhost identified by 'multiverse';
##### datadir_ add another user
* >sudo su
* >useradd dnseed     || useradd -g sudo -d /home/dnseed -s /bin/bash -m dnseed
* >passwd dnseed

------------------------------------------------------------------------------------------------------
#### DNSeed测试方案
##### 测试目标
[dnseed服务连接数较多时可能引发的问题]
* dnseed服务接入的节点为新入网节点或失去所有伙伴的孤独节点,稳定在网节点不会访问dnseed服务,且访问均为短时访问不容易出现短时内集中大量爆发访问的情况.
* dnseed服务的最大接入连接数取值目前是默认125,节点接入获取地址列表时,是从内存中取得,并没有产生数据库IO,所以dnseed服务能承受的最大接入数量应该是socket能承受的最大数量,针对可能出现的压力可以适当调高最大接入数值.

[dnseed地址列表过长时进行连接测试时可能引发的问题]
*  测试连接是利用原有p2p网络对列表中的所有地址进行连接握手,成功则保留地址,失败则扣减可用次数.可用次数为可配置项.
*  原有p2p网络进行连接时,无论多少节点插入到队列,均以每秒一个的频率弹出并进行连接测试,同时存在的连接数量不会过大,理论上不会超过10个,连接压力也不会过大.但有可能因为地址列表过长,导致的一个测试周期无法完成整个列表测试(比如测试周期如果是一天,列表理论最大测试数量为86400),为避免该情况的出现,可以采用动态测试周期的方式.
*  动态测试周期: 当列表地址数量少于某一阀值时,可用某一可配置固定周期进行测试.当地址列表过长,每次开始测试时可动态根据当前待测列表长度,动态下一次触发测试的时机.确保列表一定能测试完成.
 


#### PR issues
* don't insert any dnseed server code into CMvPeerNet/CMvPeer/CNetwork
* the dynamic dnseed list is unsafe for user' wallet，since any dnseed IP from other node is unverifiable
* there is a memory leak issue when create DNSeedService object
####
* what difference between MVPROTO_CMD_GETDNSEED/MVPROTO_CMD_DNSEED and MVPROTO_CMD_GETADDRESS/MVPROTO_CMD_ADDRESS?
* "virtual" is unnecessary in walleve/walleve/peernet/peernet.h 
* please keep copyright the same as other files 
* put comma at the end of line instead of the begin of the next one


****************************普通节点不维护列表!!!!!!!!!!
DNSeed地址也可从配置项中来
数据库不用分离



#### 疑问
* 可信节点怎么建立:通过配置
* 如果没有配置可信节点,则在列表连接测试时推测出可信高度.
**** 高度作为评分标准,高度作为列表优先抽取的标准

连接数量的压力测试,压力测试客户端

暂不考虑X 端口
暂不考虑X 分支节点相关的DNSeed路由
暂不考虑X DNSeed服务之间交换列表(是否需要)
##### 侵入原因
* network 获取数据库配置,连接失败消息捕获,初始化dnseedservice
* mvpeernet 保存节点地址列表(可以在peer中拦截处理)
* CMvPeer 没有侵入



