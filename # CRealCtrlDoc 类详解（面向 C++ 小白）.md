# CRealCtrlDoc 类详解（面向 C++ 小白）

本文档目的：逐步讲解 `RealCtrlDoc.cpp` 中的 `CRealCtrlDoc` 类，帮助你理解类的职责、成员、主要函数的工作流程和常见注意点。假设你对 C++/MFC 基本概念（类、指针、new/delete、列表）有初步了解。

---

## 一、总体职责概览
`CRealCtrlDoc` 是 MFC 文档类（继承自 `CDocument`），在此项目中承担以下核心职责：
- 初始化并管理控制算法（如预测控制、PID、自适应等）。
- 周期性采样（或仿真输出）、执行控制算法并维护显示缓冲（历史输出/控制）。
- 管理网络通信：监听客户端连接、接收并解析客户端消息、向客户端广播系统状态。
- 提供启动/暂停/手动/停止等控制接口以响应 UI 或远端命令。

---

## 二、重要成员变量（简要）
- `m_pSysForm`：指向 `CSysForm` 的指针，表示用于序列化/网络传输的系统数据结构（参数、状态等）。
- `m_msg`：封装发送/接收消息的数据结构，内部包含 `m_pSysForm`。
- 控制算法对象：`m_Predict`, `m_Pid`, `m_FNPid`, `m_NeuralPid`, `m_Adeptive`（分别对应不同控制器实现）。
- 模式配置句柄：`m_pPredictMode`, `m_pPidMode`, `m_pExpertMode`, `m_pFuzyMode`, `m_pAdeptiveMode`（用于向客户端展示/调整参数）。
- 网络相关：`m_pSocket`（监听套接字 `CListeningSocket*`），`m_connectionList`（已连接客户端列表，元素为 `CClientSocket*`）。
- 数据缓存与计数：`m_OutArray`、`m_CtrlArray`（显示/历史数组）、`m_OutPredictArray`（预测数组）、`m_ControlCounter`、`m_ControlInterval` 等。
- 采样相关：`m_bRealTime`（真实采样或仿真）、`m_DataAqisit`（数据采集封装）。

---

## 三、构造与析构（`CRealCtrlDoc()` / `~CRealCtrlDoc()`）
- 构造函数做了大量初始化工作：
  - 初始化互斥量、控制间隔、系统信息 `m_SysInfo`、控制算法实例。
  - 分配并初始化 `CSysForm`、多个 `CCtrlMode`（控制模式）、`CParaList`（参数链表）并把它们与算法内部参数指针关联，便于网络传输/远程调参。
  - 设置 `m_msg.m_pSysForm = m_pSysForm`，初始化通信结构。
- 析构函数负责释放资源：关闭并删除监听套接字、删除 `m_pSysForm` 和按条件删除各个模式对象（注意这里的删除逻辑比较繁琐，要小心避免重复 delete 或漏删）。

注意（对初学者）：每次 `new` 都应对应一个 `delete`。当前代码对删除的条件判断复杂，存在内存管理风险；建议后续使用智能指针（如 `std::unique_ptr`）重构。

---

## 四、初始化方法（`InitPredict`、`InitPID` 等）
这些函数把控制算法的参数（例如 PID 的 P/I/D）包装到 `CParaList` 链表中：
- 每个 `CParaList` 节点包含名称、数据类型、以及指向具体内存（算法参数成员）的指针。
- 这样做的目的是：当需要把参数通过网络发给客户端或从客户端读取参数时，只需操作这些 `CParaList`，就能把参数与界面/网络保持同步。

---

## 五、网络与消息流（关键函数）
- `OnNewDocument()`：创建 `m_pSocket = new CListeningSocket(this)` 并调用 `Create(BASE_ADDRESS)` + `Listen()` 来启动监听（端口由 `BASE_ADDRESS` 定义）。
- `ProcessPendingAccept()`：当有新连接到来时，创建 `CClientSocket`，调用 `Accept` 并加入 `m_connectionList`。
- `ProcessPendingRead(CClientSocket* pSocket)`：循环读取客户端缓冲区：
  - 调用 `ReadMsg(pSocket)` 填充 `m_msg`。
  - 根据 `m_pSysForm->m_ReceaveMode` 的值分支处理不同类型的消息（例如 `GIVENVALUE` 表示客户端发送新的设定值，`CONTROLSTATUS` 表示启动/停止请求）。
  - 对某些类型（如 `REQUISITION`）会把 `m_pSysForm->m_Requisition` 的请求放到 `m_pSysForm->m_SendMode` 并通过 `SendMsg` 回复。
- `ReadMsg` / `SendMsg`：分别调用 `CClientSocket::ReceiveMsg` 和 `CClientSocket::SendMsg`，并在异常时 `Abort()` 套接字并将 `m_msg.m_bConnect = FALSE`。
- `CloseSocket`：关闭并从连接列表中移除指定客户端，然后 `delete` 它。
- `UpdateClients()`：广播当前 `m_msg` 到所有已连接的客户端。

关键概念：所有网络操作都通过 `CMsg`/`CSysForm`/`CParaList` 结构进行序列化和反序列化，使得客户端可以读取并修改参数、请求数据或接收实时状态。

---

## 六、采样、预测与输出缓冲
- `Sample()`：
  - 若 `m_bRealTime` 为真，从硬件采集模块 `m_DataAqisit.DoSampl()` 读取实时样本到 `m_SysInfo.CurrentValue`。
  - 否则在仿真模式下使用预测数组 `m_OutPredictArray[0]` 作为当前值。
  - 更新最大输出值 `m_MaxOutputValue` 与超调率 `m_MaxOver`（用于性能指标）。
  - 将采样值写入 `m_pSamplValueList`（用于通信），并设置 `m_SampleFlag = TRUE`。
- `UpdateOutArray()`：
  - 当 `m_SampleFlag` 设置时，把 `m_SysInfo.CurrentValue` 和 `m_SysInfo.CurrentControl` 添加到 `m_OutArray` / `m_CtrlArray` 作为历史数据（用于画图）。
  - 在仿真模式下，还会基于上次控制的增量 `DeltaControl` 与被控对象模型 `m_ObjectModel.m_ModelArray` 更新 `m_OutPredictArray`，用于预测未来输出。
  - 保持历史数组长度不超过 `m_DispLenth`。

---

## 七、控制执行逻辑（`Control()`）
- 代码使用 `m_ControlCounter` 与 `m_ControlInterval` 来控制控制器的执行频率：
  - 如果 `m_ControlCounter != 0`，则每次调用将其递增，直到达到 `m_ControlInterval`，此时重置为 0 并返回（下一次 0 时执行控制）。
  - 当 `m_ControlCounter == 0` 时，进入执行分支：根据 `m_Algrithem`（枚举）选择调用相应控制器的 `OutValueCalculate()`，并有时先调用 `PrepareWork()`（例如神经 PID/模糊神经）。
  - 执行后把当前控制值写入 `m_pCtrlValueList` 供通信使用。
- 这段逻辑看起来有点拐弯。直观做法通常是按时间或定时器判断“是否到达控制周期”，可以考虑简化。

---

## 八、启停与状态控制（`Start` / `Pause` / `Manual` / `Stop`）
- `Start()`：
  - 设置 `m_bStart = TRUE`，通知客户端（更改 `m_pCtrlStatuList` 并调用 `UpdateClients()`）。
  - 重置计数器、最大值和历史数组；若非暂停状态则清零当前值并初始化预测数组。
  - 调用当前算法对象的 `Start()` 初始化。
- `Pause()`：切换 `m_bPause`，通知客户端，并调用对应算法的 `Pause(m_bPause)`。
- `Manual()`：切换手动模式 `m_bManual`，并通知算法对象以进入/退出手动控制。
- `Stop()`：设置 `m_bStart = FALSE`，通知客户端，并调用算法的 `Stop()`。

---

## 九、线程与关闭（`OnCloseDocument`）
- `OnCloseDocument()` 会等待并清理后台线程对象 `m_pDcsThread`：
  - 如果线程仍在运行（`STILL_ACTIVE`），设置 `m_bDone = TRUE` 让线程结束。
  - 否则删除线程对象。注意这里使用了一个循环等待 `m_pDcsThread->m_SampleFlag`，确保线程不在采样阶段再删除。
- 终结时需确保所有网络连接已正确关闭并释放，以免出现资源泄漏。

---

## 十、常见问题与改进建议（给初学者的实践建议）
1. 内存管理：当前代码广泛使用 `new` / `delete`，初学者要注意每个 `new` 是否有对应 `delete`。更好的方式是使用 RAII（智能指针如 `std::unique_ptr`）。
2. 并发与线程安全：`m_connectionList`、`m_msg` 等在不同线程/回调中被访问，建议使用互斥量（`m_Mutex`）在读写时加锁，避免数据竞争。
3. 错误处理：`ReadMsg` / `SendMsg` 捕获 `CFileException` 并调用 `Abort()`，但应确保最终调用 `CloseSocket` 来从列表中移除断开的客户端。
4. 可读性：`Control()` 的计数逻辑不直观，建议改为基于时间的周期判断或更明确的状态机。
5. 使用日志与调试：在 Visual Studio 中使用 __Output__ 窗口查看运行日志或在关键点打断点（__Debug > Start Debugging__ / __Debug > Windows > Output__）。

---

## 十一、如何编译与调试（在 Visual Studio）
- 打开解决方案后：使用 __Build > Build Solution__ 编译。
- 运行并调试：使用 __Debug > Start Debugging__（F5）或 __Debug > Start Without Debugging__（Ctrl+F5）。
- 查看输出或异常信息：使用 __Debug > Windows > Output__。

---

## 十二、小白学习路线（快速）
1. 先理解类成员和构造函数，知道对象如何被创建与关联。
2. 跟踪程序运行：从 `OnNewDocument()` 开始，观察监听套接字如何创建、accept 如何触发 `ProcessPendingAccept()`。
3. 模拟一次客户端连接，观察 `ProcessPendingRead()` 如何解析消息并调用 `Start()` / `UpdateSetValue()` 等。
4. 学习 `Sample()`、`UpdateOutArray()`、`Control()` 的周期性调用（通常由后台线程负责），理解采样→控制→发送的流程。
5. 阅读 `CClientSocket` 与 `CSysForm` 的实现，了解序列化与消息格式。

---

如果你愿意，我可以：
- 给出 `Control()` 的更直观实现建议并生成补丁；
- 或把 `ProcessPendingRead()` 逐行注释成更易懂的版本；
- 或生成一张 mermaid 类关系图帮助可视化类间关系（告诉我要哪种）。  