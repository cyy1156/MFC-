# 硬件接口更新维护指南

本代码库中存在过时的硬件访问接口，主要集中在 `PsADCtrl` 模块中。这些接口使用直接端口 I/O 指令 (`_inp`/`_outp`)，这在现代 Windows 操作系统（如 Windows XP/7/10/11）的用户模式应用程序中是被禁止的，会导致程序崩溃或抛出“特权指令”异常。

以下是详细的错误定位及修改建议。

## 1. 问题定位 (Problem Location)

所有相关代码均位于文件 **`realctrl/PsADCtrl/DataAquisit.cpp`** 中。

### 1.1 关键错误：直接端口输入 (`_inp`)
以下行代码直接尝试读取硬件端口，这在现代系统中无法运行：

*   **行 225**: `hbyte = _inp(m_CardInfo.adAddress+2);` —— 读取高字节数据
*   **行 226**: `lbyte = _inp(m_CardInfo.adAddress+3);` —— 读取低字节数据
*   **行 275**: `m_SamplFlag=_inp(m_CardInfo.adAddress+1);` —— 读取状态位

### 1.2 潜在隐患：被注释的端口输出 (`_outp`)
虽然以下代码已被注释，但如果取消注释同样会报错。这些代码展示了原始的控制逻辑：

*   **行 77, 119, 165, 186, 196**: `// _outp(port, databyte );` —— 写入控制字
*   **行 223**: `// _outp(m_CardInfo.adAddress+1,AD_START);` —— 启动 AD 转换

### 1.3 头文件依赖
*   **行 8**: `#include <conio.h>` —— 这是定义 `_inp`/`_outp` 的旧式头文件，在现代 C++ 标准中已非必须，且相关函数已废弃。

### 1.4 硬件地址硬编码
*   **行 17**: `#define CARD_ADDR 0x300` —— 这里硬编码了 ISA 总线地址 `0x300`。现代 PCI/PCIe 采集卡使用动态分配的基地址，不再使用固定的 ISA 地址。

---

## 2. 修改建议 (Proposed Solutions)

根据项目是否需要连接真实硬件，提供两种修改方案。

### 方案 A：仿真模式（推荐用于演示/纯软件环境）
如果项目主要用于算法验证或演示，不需要连接真实物理设备，建议将硬件读取替换为**软件模拟**。

**修改步骤：**
1.  **移除/注释** 所有 `_inp` 和 `_outp` 调用。
2.  在 `DoSampl()` 函数中，编写代码生成模拟波形（如正弦波、随机噪声）。

**示例代码片段 (DataAquisit.cpp):**
```cpp
// float CDataAquisit::DoSampl()
// {
//    // 移除旧代码...
//    
//    // 新增：模拟正弦波输入
//    static float angle = 0.0f;
//    m_SamplVal = (int)(2048.0f * sin(angle) + 2048.0f); // 模拟 12位 AD 值
//    angle += 0.1f;
//    
//    return Coordinate(); // 调用原有的转换函数
// }
```

### 方案 B：对接现代采集卡（用于实际工程）
如果需要连接真实的 AD/DA 板卡（如研华、NI、凌华等），必须使用厂商提供的驱动库（DLL/SDK）。

**修改步骤：**
1.  **引入厂商 SDK**: 将采集卡厂商提供的 `.h` 头文件和 `.lib` 库文件加入项目。
2.  **封装驱动调用**:
    *   在 `DoSampl` 中调用 SDK 的单点采集函数（例如 `DRV_AI_ReadVoltage`）。
    *   在 `DoDAOut` 中调用 SDK 的输出函数。
3.  **移除硬编码地址**: 修改 `CARD_ADDR` 逻辑，改为通过设备号或设备描述符打开设备。

### 方案 C：使用通用驱动 Wrapper（仅限维护旧设备）
如果必须使用旧的 ISA 设备且无法更新驱动，可以使用第三方库如 `InpOut32.dll` 来模拟端口访问（不推荐用于生产环境）。

---

## 3. 重构建议 (Refactoring)

为了提高代码的可维护性，建议对 `CDataAquisit` 类进行抽象：

1.  创建一个抽象基类 `IDataDevice`，定义纯虚函数 `ReadSample()` 和 `WriteOutput()`。
2.  派生出 `CSimulationDevice`（实现方案 A）和 `CRealHardwareDevice`（实现方案 B）。
3.  在 `CDataAquisit` 中持有 `IDataDevice` 指针，根据配置动态切换仿真/实物模式。

## 4. 总结
当前代码中的 `_inp`/`_outp` 是 DOS/Win9x 时代的遗留产物。为了让项目在 Windows 10/11 上运行，**必须**移除这些调用，并根据需求选择“软件模拟”或“接入现代驱动 SDK”的方式进行替换。
