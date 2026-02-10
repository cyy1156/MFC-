#pragma once
#include <afxwin.h>
#include <string>
#include <list>
#include <fstream>
#include <iostream>

// 使用标准命名空间，确保 string 和 list 可以直接使用
// 这是一个遗留项目的常见做法，为了兼容性保留
using namespace std;

// 定义默认文件路径
#define _F_LOGIN "./login.ini"
#define _F_STOCK "./stack.txt"

struct msg
{
    int id;
    string name;
    int price;
    int num;   // 库存
    int count; // 销量
};

class CInfoFile
{
public:
    CInfoFile();
    CInfoFile(LPCTSTR szFilePath);
    virtual ~CInfoFile();

    // INI 文件读写
    BOOL WriteString(LPCTSTR szSection, LPCTSTR szKey, LPCTSTR szValue);
    BOOL WriteInt(LPCTSTR szSection, LPCTSTR szKey, int nValue);
    BOOL WriteFloat(LPCTSTR szSection, LPCTSTR szKey, double fValue);
    CString ReadString(LPCTSTR szSection, LPCTSTR szKey, LPCTSTR szDefault = _T(""));
    int ReadInt(LPCTSTR szSection, LPCTSTR szKey, int nDefault = 0);
    double ReadFloat(LPCTSTR szSection, LPCTSTR szKey, double fDefault = 0.0);

    // 业务逻辑函数
    void ReadLoginInfo(CString& name, CString& pwd);
    void WriteLoginInfo(char* name, char* pwd);
    
    // 商品数据读写
    void ReadDocLine();
    void WriteDocLine();
    void AddDocLine(CString name, int num, int price);
    void DeleteDocLine(CString name);

public:
    // 必须为 public 且为 list，因为其他对话框直接访问了这个成员
    list<msg> ls; 
    int num;

private:
    CString m_strFilePath;
};
