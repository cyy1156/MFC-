# RealCtrl 类与关系详解

本文档详细介绍了 RealCtrl 项目中各个类的作用、职责以及它们之间的相互关系。

## 1. 核心类概览 (Core Classes)

项目遵循 MFC 的文档/视图 (Document/View) 架构，主要类如下：

### 1.1 应用程序与框架
- **`CRealCtrlApp`**
  - **作用**: 应用程序入口类。
  - **职责**: 初始化 OLE 库，注册文档模板，处理命令行参数，启动主消息循环。
  - **联系**: 它是整个程序的容器，负责创建 `MainFrame`。

- **`CMainFrame`**
  - **作用**: 主窗口框架类。
  - **职责**: 管理菜单栏、工具栏和状态栏；处理全局性的命令消息（如视图切换、帮助）。
  - **联系**: 它是 `CRealCtrlView` 的父窗口，并响应来自后台线程的 `WM_USER_UPDATEVIEW` 消息来刷新界面。

### 1.2 文档与数据 (Model & Controller Logic)
- **`CRealCtrlDoc`**
  - **作用**: 核心业务逻辑与数据中心。
  - **职责**:
    - **状态管理**: 维护系统运行状态（开始/暂停/停止）、控制模式（手动/自动）和当前算法选择。
    - **算法调度**: 拥有所有控制算法对象（`CPID`, `CNeuralPIDControl` 等），在 `Control()` 函数中根据配置调用相应的算法。
    - **线程管理**: 创建并管理 `CDcsThread` 实时线程。
    - **通讯中心**: 拥有 `CListeningSocket` 和 `CMsg`，处理网络数据收发。
  - **联系**: 是所有算法类和辅助类的拥有者（Owner）；被 `CRealCtrlView` 引用以显示数据；被 `CDcsThread` 调用以执行周期性任务。

### 1.3 视图与界面 (View)
- **`CRealCtrlView`**
  - **作用**: 用户交互与数据显示层。
  - **职责**:
    - **控件管理**: 初始化并绘制各种自定义控件（`CDispScreen` 曲线图, `CBulb` 指示灯, `CPointer` 仪表等）。
    - **交互处理**: 响应鼠标点击、菜单选择，将用户操作转化为对 `CRealCtrlDoc` 的指令。
    - **定时刷新**: 通过定时器或消息响应更新界面上的数值和曲线。
  - **联系**: 依赖 `CRealCtrlDoc` 获取实时数据 (`SYSINFO`)；包含多个 `Element` DLL 中的 UI 控件对象。

### 1.4 实时线程
- **`CDcsThread`**
  - **作用**: 实时任务执行线程。
  - **职责**:
    - 维护一个 `while` 循环，模拟实时时钟。
    - 周期性调用 `m_pDoc->Sample()` (采样) 和 `m_pDoc->Control()` (控制)。
    - 计算完成后发送消息通知界面刷新。
  - **联系**: 由 `CRealCtrlDoc` 启动，直接操作 `CRealCtrlDoc` 的数据接口。

---

## 2. 控制算法类 (Control Algorithms)

这些类封装了具体的控制数学模型，均由 `CRealCtrlDoc` 实例化和调用。

- **`CPID`**
  - **作用**: 标准 PID 控制器。
  - **职责**: 实现经典的比例-积分-微分算法。
- **`CNeuralPIDControl`**
  - **作用**: 单神经元 PID 控制器。
  - **职责**: 利用神经网络的学习能力在线调整 PID 参数。
- **`CFNPIDControl`**
  - **作用**: 模糊神经网络 PID 控制器。
  - **职责**: 结合模糊控制规则与神经网络优化。
- **`CAdeptive`**
  - **作用**: 自适应控制器。
  - **职责**: 根据误差变化自动调整控制策略，适应对象模型的变化。
- **`CPredict`**
  - **作用**: 预测控制器。
  - **职责**: 基于模型预测未来输出，通过优化算法计算当前最优控制量。
- **`CModel`**
  - **作用**: 仿真对象模型。
  - **职责**: 根据传递函数（差分方程）模拟被控对象的响应。用于系统仿真（作为虚拟对象）或预测控制中的内部模型。

---

## 3. 自定义控件类 (Element DLL)

这些类位于 `Element/` 目录下，用于构建工业风格的监控界面。

- **`CDispScreen`**: 实时曲线显示屏，类似于示波器，用于绘制给定值、输出值等曲线。
- **`CBulb`**: 多态指示灯，可显示开/关状态，或作为带灯按钮使用。
- **`CPointer`**: 模拟仪表盘指针，用于直观显示数值（如压力表、速度表）。
- **`CColorStripe`**: 彩色条带，用于液位计或进度显示。
- **`CScoreBar`**: 刻度尺控件。
- **`CEllipseWnd`**: 圆形窗口或指示器。

---

## 4. 类之间的协作关系 (Class Interactions)

### 4.1 实时控制循环流程
这是一个典型的闭环控制数据流：

1.  **触发**: `CDcsThread` 线程唤醒。
2.  **采样**: 线程调用 `CRealCtrlDoc::Sample()`。
    - `CRealCtrlDoc` 从 `CDataAquisit`（硬件或模拟）获取当前值 (`CurrentValue`)。
3.  **计算**: 线程调用 `CRealCtrlDoc::Control()`。
    - `CRealCtrlDoc` 根据 `m_Algrithem` 选择算法（如 `m_Pid`）。
    - 调用 `m_Pid.OutValueCalculate()`。
    - 算法类读取 `SYSINFO` 中的误差，计算出新的控制量 (`ControlValue`)。
4.  **输出**: `CRealCtrlDoc` 将控制量发送给 `CDataAquisit` 或更新对象模型 `m_ObjectModel`。
5.  **反馈**: 线程发送 `WM_USER_UPDATEVIEW` 消息。
6.  **显示**: `CMainFrame` 收到消息，通知 `CRealCtrlView` 重绘 `CDispScreen` 和更新仪表指针。

### 4.2 参数配置流程
1.  用户在菜单中选择 "Model Setting"。
2.  `CRealCtrlView` 创建 `CModelDlg` 对话框实例。
3.  用户输入模型参数（阶数、系数）。
4.  点击确认后，数据回写到 `CRealCtrlDoc` 的 `m_ObjectModel` 中。

### 4.3 网络通讯流程
1.  `CRealCtrlDoc` 初始化时创建 `CListeningSocket` 并在指定端口监听。
2.  当有客户端连接时，`CListeningSocket` 通知 `CRealCtrlDoc`。
3.  `CRealCtrlDoc` 创建 `CClientSocket` 与对方建立连接。
4.  定时将 `SYSINFO` 数据打包，通过 `CClientSocket` 发送给远程监控端。

