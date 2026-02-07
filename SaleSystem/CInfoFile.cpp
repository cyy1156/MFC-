#include "pch.h"
#include "CInfoFile.h"

#include <tchar.h>

// 默认构造函数
CInfoFile::CInfoFile()
{
    m_strFilePath = _F_LOGIN; 
    num = 0;
    ReadDocLine();  // 读取商品信息
}

// 带参数构造函数
CInfoFile::CInfoFile(LPCTSTR szFilePath)
{
    m_strFilePath = szFilePath;
    num = 0;
    ReadDocLine(); // 读取商品信息
}

// 析构函数
CInfoFile::~CInfoFile()
{
    WriteDocLine();  // 自动保存商品信息
}

// -------------------------- 配置文件读写实现 --------------------------
BOOL CInfoFile::WriteString(LPCTSTR szSection, LPCTSTR szKey, LPCTSTR szValue)
{
    return ::WritePrivateProfileString(szSection, szKey, szValue, m_strFilePath);
}

BOOL CInfoFile::WriteInt(LPCTSTR szSection, LPCTSTR szKey, int nValue)
{
    CString strValue;
    strValue.Format(_T("%d"), nValue);
    return WriteString(szSection, szKey, strValue);
}

BOOL CInfoFile::WriteFloat(LPCTSTR szSection, LPCTSTR szKey, double fValue)
{
    CString strValue;
    strValue.Format(_T("%.2f"), fValue);
    return WriteString(szSection, szKey, strValue);
}

CString CInfoFile::ReadString(LPCTSTR szSection, LPCTSTR szKey, LPCTSTR szDefault)
{
    CString strValue;
    // 使用 GetPrivateProfileString 读取配置
    ::GetPrivateProfileString(szSection, szKey, szDefault, strValue.GetBuffer(1024), 1024, m_strFilePath);
    strValue.ReleaseBuffer();
    return strValue;
}

int CInfoFile::ReadInt(LPCTSTR szSection, LPCTSTR szKey, int nDefault)
{
    CString strValue = ReadString(szSection, szKey);
    if (strValue.IsEmpty())
        return nDefault;
    return _ttoi(strValue);
}

double CInfoFile::ReadFloat(LPCTSTR szSection, LPCTSTR szKey, double fDefault)
{
    CString strValue = ReadString(szSection, szKey);
    if (strValue.IsEmpty())
        return fDefault;
    return _ttof(strValue);
}

// -------------------------- 登录信息读写 --------------------------
void CInfoFile::ReadLoginInfo(CString& name, CString& pwd)
{
    name = ReadString(_T("Login"), _T("UserName"));
    pwd = ReadString(_T("Login"), _T("Password"));
}

void CInfoFile::WriteLoginInfo(char* name, char* pwd)
{
    CString strName(name);
    CString strPwd(pwd);
    WriteString(_T("Login"), _T("UserName"), strName);
    WriteString(_T("Login"), _T("Password"), strPwd);
}

// -------------------------- 商品库存读写 --------------------------

void CInfoFile::ReadDocLine()
{
    ls.clear();
    ifstream file(_F_STOCK);
    if (!file.is_open())
    {
        // 如果文件不存在，不报错，可能是首次运行
        return;
    }

    string line;
    // 读取第一行表头
    getline(file, line);

    while (getline(file, line))
    {
        if (line.empty()) continue;

        msg data;
        // 解析逻辑：假设格式为 id|name|price|stock|sales
        
        size_t pos = 0;
        size_t next_pos = 0;

        // ID
        next_pos = line.find('|', pos);
        if (next_pos == string::npos) continue;
        data.id = atoi(line.substr(pos, next_pos - pos).c_str());
        pos = next_pos + 1;

        // Name
        next_pos = line.find('|', pos);
        if (next_pos == string::npos) continue;
        string sName = line.substr(pos, next_pos - pos);
        // 去除首尾空格
        size_t first = sName.find_first_not_of(" \t");
        size_t last = sName.find_last_not_of(" \t");
        if (first != string::npos && last != string::npos)
             data.name = sName.substr(first, last - first + 1);
        else
             data.name = sName;
        pos = next_pos + 1;

        // Price
        next_pos = line.find('|', pos);
        if (next_pos == string::npos) continue;
        data.price = atoi(line.substr(pos, next_pos - pos).c_str());
        pos = next_pos + 1;

        // Age (库存)
        next_pos = line.find('|', pos);
        if (next_pos == string::npos) continue;
        data.age = atoi(line.substr(pos, next_pos - pos).c_str());
        pos = next_pos + 1;

        // Count (销量)
        data.count = atoi(line.substr(pos).c_str());

        ls.push_back(data);
    }
    num = (int)ls.size();
    file.close();
}

void CInfoFile::WriteDocLine()
{
    // 如果 ls 为空，且原本文件不存在，则不写入。
    // 但为了数据安全，只要 ls 有数据，或者我们确定要保存状态，就写入。
    // 这里采用只在有数据时写入，避免清空文件的风险。
    
    if (ls.empty()) {
        // 检查文件是否存在，如果存在且非空，而 ls 为空，说明可能读取失败，
        // 此时不要覆盖文件！
        ifstream checkFile(_F_STOCK);
        if (checkFile.is_open()) {
            checkFile.seekg(0, ios::end);
            if (checkFile.tellg() > 0) {
                checkFile.close();
                return; // 文件有内容但内存为空，拒绝覆盖
            }
            checkFile.close();
        }
    }

    ofstream file(_F_STOCK);
    if (!file.is_open()) return;

    if (ls.size() > 0) {
        file << "ID|Name|Price|Stock|Sales" << endl;
        // 使用传统迭代器
        for (list<msg>::iterator it = ls.begin(); it != ls.end(); it++)
        {
            file << it->id << "|"
                << it->name << "|"
                << it->price << "|"
                << it->age << "|"
                << it->count << endl;
        }
    }
    file.close();
}

void CInfoFile::AddDocLine(CString name, int num, int price)
{
    msg newData;
    newData.id = ls.empty() ? 1 : ls.back().id + 1;
    newData.name = CT2A(name); // CString 转 std::string
    newData.price = price;
    newData.age = 0; // 库存默认为0
    newData.count = num; 
    
    ls.push_back(newData);
    this->num = (int)ls.size();
}
