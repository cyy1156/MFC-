# DcsPrtcl 模块类与功能详解

`DcsPrtcl` (DCS Protocol) 模块是一个 **MFC 扩展 DLL**，主要负责定义和处理与 DCS（分布式控制系统）或远程监控端进行通信的数据协议和格式。它提供了一套灵活的数据结构来序列化和反序列化系统配置及运行时数据。

## 1. 核心类详解

该模块主要在 `SysForm.h` 和 `SysForm.cpp` 中定义了三个紧密相关的类，构成了一个层级化的数据模型。

### 1.1 `CParaList` (参数节点类)
这是数据结构的最底层单元，类似于一个链表节点，用于存储单个参数及其元数据。

*   **功能**: 存储单一的数据项（如一个浮点数值、一个整数设置项）。
*   **核心成员**:
    *   `m_Name`: 参数名称 (CString)。
    *   `m_Para`: 参数值，使用 `union PARATYPE` 存储多种类型（`char`, `int`, `float`, `double` 等）。
    *   `m_DataType`: 数据类型标识（`isFloat`, `isInt` 等）。
    *   `m_NextNode`: 指向下一个参数节点的指针，形成链表。
*   **关键方法**:
    *   `GetData` / `SetData`: 通用的数据存取接口，根据 `m_DataType` 自动处理类型转换。
    *   `Serialize`: 实现了 MFC 的序列化机制，支持将参数名和值写入归档或网络流。

### 1.2 `CCtrlMode` (控制模式类)
位于中间层，代表一种具体的控制策略或模式，它包含了一组相关的参数。

*   **功能**: 组织和管理一组 `CParaList` 参数。例如，一个 "PID控制模式" 对象可能包含 P、I、D 三个参数节点。
*   **核心成员**:
    *   `m_CtrlName`: 模式名称。
    *   `m_pCtrlPara`: 指向参数链表头部的指针 (`CParaList*`)。
    *   `m_NextMode`: 指向下一个模式对象的指针，支持多模式切换。
*   **关键方法**:
    *   `AddTail` / `RemoveTail`: 动态管理参数链表。
    *   `Serialize`: 递归调用参数列表的序列化方法，保存整个模式的数据。

### 1.3 `CSysForm` (系统表单/协议包类)
这是顶层容器类，定义了完整的通讯协议包格式。

*   **功能**: 封装了系统的一次完整交互数据，包括所有的控制模式、设定值、采样值和状态信息。
*   **核心成员**:
    *   **控制模式**: `m_pCtrlMode` (CCtrlMode 链表)。
    *   **各类数据列表**:
        *   `m_pSetValueList` (设定值)
        *   `m_pSamplValueList` (采样值)
        *   `m_pCtrlValueList` (控制输出值)
        *   `m_pCtrlStatuList` (控制状态)
        *   `m_pRunStatuList` (运行状态)
        *   `m_pGraphList` (图表数据)
    *   **通讯指令**: `m_SendMode` / `m_ReceaveMode` (发送/接收模式)，决定了本次数据包传输的是哪一部分数据（如仅传输设定值 `GIVENVALUE`，或传输全部配置 `SYSFORM`）。
*   **序列化机制 (`Serialize`)**:
    *   这是该类的核心。根据 `m_SendMode` 的不同，选择性地打包或解包不同部分的数据。
    *   这种设计极大地优化了网络带宽，允许“按需传输”（例如实时监控时只传采样值，初始化时才传完整的系统表单）。

## 2. 通讯协议标识

模块定义了一系列宏来标识数据包类型：
*   `SYSFORM` (1): 完整的系统配置表单。
*   `CONTROLMODE` (2): 仅控制模式数据。
*   `GIVENVALUE` (4): 设定值更新。
*   `SAMPLEVALUE` (5): 采样值更新（最常用）。
*   `REQUISITION` (11): 请求指令。

## 3. 总结

`DcsPrtcl` 模块通过 **Chain of Responsibility（责任链）** 或 **Composite（组合）** 模式的变体（SysForm -> CtrlMode -> ParaList），构建了一个高度可扩展的数据通讯协议。它不仅支持简单数值的传输，还能动态描述复杂的控制结构，非常适合用于工业组态软件或远程监控系统的底层数据交换。
