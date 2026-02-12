# PsADCtrl 模块类与功能详解

`PsADCtrl` (Process Analog/Digital Control) 是 RealCtrl 系统中负责**数据采集与底层硬件控制**的动态链接库 (DLL)。该模块主要封装了与 AD/DA（模数/数模）转换卡交互的逻辑，将物理信号转换为计算机可处理的数字信号。

## 1. 核心类：`CDataAquisit`

该类是本模块的核心，位于 `DataAquisit.h` 和 `DataAquisit.cpp` 中。

### 1.1 主要作用
*   **硬件抽象**：封装了对底层硬件端口（I/O Port）的读写操作（`_inp`, `_outp`），屏蔽了硬件细节。
*   **信号处理**：负责将采集到的原始二进制数据转换为具有实际物理意义的工程量（如电压、电流、温度）。
*   **输入输出控制**：提供统一接口进行数据采样（AD转换）和控制量输出（DA转换）。

### 1.2 关键数据结构

该类定义了两个重要的结构体来描述硬件和信号特性：

1.  **`CARDUSAGEINFO` (板卡配置信息)**
    *   `adAddress`: 板卡的基地址（Base Address，如 `0x300`）。
    *   `adChannel`: 使用的通道号。
    *   `adBitWidth` / `adOutWidth`: AD/DA 转换的位宽（通常为 12位）。
    *   `adMaxOut` / `adMinOut`: 输出范围限制。

2.  **`SIGNALINFO` (信号配置信息)**
    *   `sgSensorType`: 传感器类型。支持多种类型：
        *   `VOLTAGE` (电压)
        *   `CURRENT` (电流)
        *   `THERMOCOUPLE_*` (各种热电偶 S/B/K/T/E)
        *   `THERMISTOR_*` (热电阻 PT100/CU100/CU50)
    *   `sgMaxData` / `sgMinData`: 量程范围（用于归一化计算）。

### 1.3 核心成员函数

*   **构造函数 `CDataAquisit()`**
    *   自动检测操作系统版本（Win95/NT/Win32s）。
    *   初始化默认的板卡参数和信号参数。

*   **数据采样 `DoSampl()`**
    *   **流程**: 启动 AD 转换 -> 等待转换完成 (`GetSamplState`) -> 读取高低字节 -> 拼接数据 (`DataLink`) -> 转换为工程量 (`Coordinate`)。
    *   *注意*: 当前代码中该函数直接返回 `0.f`，具体的硬件指令部分被注释或禁用，可能处于仿真模式。

*   **数据转换 `Coordinate()`**
    *   根据 `sgSensorType`（传感器类型），应用不同的公式将原始采样值 (`m_SamplVal`) 转换为实际物理量 (`m_TreatedVal`)。
    *   例如电压模式下：`结果 = 比例系数 * (采样值 - 零点偏移) + 最小量程`。

*   **底层数据拼接 `DataLink()`**
    *   处理高低字节的位移和拼接，将两个 8位数据合并为 12位数据，并处理符号位（双极性/单极性）。

*   **配置接口**
    *   `SetCardInfo` / `GetCardInfo`: 设置/获取板卡硬件参数。
    *   `SetSignalInfo` / `GetSignalInfo`: 设置/获取信号处理参数。
    *   `SetInputRange` / `SetOutputRange`: 动态调整量程。

## 2. 模块初始化 (PsADCtrl.cpp)

*   **`DllMain`**: 标准的 MFC 扩展 DLL 入口点。
*   **`CDynLinkLibrary`**: 在初始化时创建该对象，确保 DLL 资源能被主程序正确加载。

## 3. 总结

`PsADCtrl` 文件夹实现了一个通用的数据采集驱动层。在 `CDataAquisit` 类中，通过配置不同的传感器类型和量程，可以适配多种工业现场信号。虽然部分底层 I/O 代码被注释，但其架构完整地展示了从“硬件端口读取”到“物理量转换”的全过程。
