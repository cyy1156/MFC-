# 算法扩展可行性与硬件接口定位分析

## 1. 是否需要更换 MFC 框架？

**结论：不需要更换，但取决于您选择的技术路线。**

MFC (Microsoft Foundation Classes) 主要是用于构建 Windows 图形用户界面 (GUI) 的框架，它本身并不限制您在后台运行什么样的算法逻辑。C++ 作为一种高性能语言，完全有能力承载主流的智能算法。

### 1.1 方案一：保留 MFC 框架（C++ 原生集成）
如果您计划使用 C++ 编写或调用智能算法，MFC 完全适用。
*   **可行性**：
    *   **传统算法**：如模糊控制、遗传算法等，直接用 C++ 实现即可（本项目已包含此类示例）。
    *   **深度学习/机器学习**：可以集成 **LibTorch** (PyTorch C++ API)、**TensorFlow C++**、**ONNX Runtime** 或 **OpenCV** 等库。这些库都能在 MFC 程序中完美运行。
*   **优势**：无需重写现有的界面和底层驱动，维护成本低，运行效率极高。
*   **劣势**：MFC 的界面开发较为繁琐，数据可视化（如复杂的动态图表）不如 Web 或 Python 生态方便。

### 1.2 方案二：保留 MFC 框架（跨语言调用）
如果您希望使用 **Python** 生态中的算法（如 PyTorch, TensorFlow, Scikit-learn），但不想重写界面。
*   **可行性**：
    *   通过 **Python C API** 在 C++ 中嵌入 Python 解释器。
    *   通过 **Socket/管道** 通信，让 MFC 程序作为前端，Python 脚本作为后台算法服务（推荐）。
*   **优势**：可以利用最先进的 Python 算法库。

### 1.3 方案三：更换框架（完全重构）
*   **场景**：如果您觉得 MFC 界面太古老，或者希望系统能跨平台（Linux/Web）。
*   **建议**：改用 Qt (C++) 或 Electron/Web 技术栈。这相当于从头开发，成本最高。

---

## 2. 硬件接入函数位置定位

本项目的硬件交互逻辑被封装在 **`PsADCtrl`** (Process Analog/Digital Control) 动态链接库中。

### 2.1 文件位置
*   **文件路径**: `realctrl/PsADCtrl/DataAquisit.cpp`
*   **类名**: `CDataAquisit`

### 2.2 核心函数详解

#### (1) 输入接口：数据采样 (AD Convert)
*   **函数名**: `DoSampl()`
*   **代码位置**: 约第 219 行
*   **功能**: 负责从硬件读取原始数据并转换为工程量。
*   **修改点**: 如果更换新硬件，需要替换掉函数内部的 `_inp` 读取逻辑，调用新硬件驱动提供的 `Read` 函数。

```cpp
// 原始代码片段 (DataAquisit.cpp)
float CDataAquisit::DoSampl()
{   
    // ...
    // 旧硬件读取逻辑
    // while (GetSamplState()){};
    // hbyte = _inp(m_CardInfo.adAddress+2);
    // lbyte = _inp(m_CardInfo.adAddress+3);
    // ...
    return Coordinate(); 
}
```

#### (2) 输出接口：控制量输出 (DA Convert)
*   **函数名**: `DoDAOut(int Value, int daNum)`
*   **代码位置**: 约第 232 行
*   **功能**: 负责将计算好的控制量写入硬件。
*   **修改点**: 当前该函数为空（或部分被注释），需要在此处添加调用新硬件驱动的 `Write` 函数。

```cpp
// 原始代码片段 (DataAquisit.cpp)
void CDataAquisit::DoDAOut(int Value,int daNum)
{
    // 当前为空，需要在此处添加新硬件的输出代码
    // 例如: Drv_WriteAnalog(hDevice, daNum, Value);
}
```

### 2.3 辅助配置接口
*   **`SetCardInfo`** / **`GetCardInfo`**: 用于设置板卡的基地址、通道号等。如果新硬件使用 USB 或 PCI 自动识别，可能不需要手动设置地址，但可能需要传递设备 ID。

## 3. 总结建议

1.  **算法升级**: 建议**保留 MFC 框架**。如果算法是 C++ 的，直接集成；如果是 Python 的，采用 Socket 通信对接。重构 UI 的成本通常远大于收益。
2.  **硬件替换**: 集中修改 `PsADCtrl/DataAquisit.cpp` 文件中的 `DoSampl` 和 `DoDAOut` 两个函数即可，保持上层应用逻辑不变。
