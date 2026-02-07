import os
import sys

# 设置输出编码
sys.stdout.reconfigure(encoding='utf-8')

# 读取原文件
with open('CInfoFile.cpp', 'r', encoding='gbk') as f:
    lines = f.readlines()

# 找到ReadDocLine函数的开始和结束位置
start_idx = None
end_idx = None
brace_count = 0
in_function = False
in_comment_block = False

for i, line in enumerate(lines):
    # 检查是否在注释块中
    if '/*' in line:
        in_comment_block = True
    if '*/' in line:
        in_comment_block = False
        continue

    # 调试输出
    if i >= 100 and i <= 110:
        print(f'{i+1}: in_comment_block={in_comment_block}, line={repr(line[:50])}')

    if 'void CInfoFile::ReadDocLine()' in line and not in_comment_block:
        # 找到未注释的ReadDocLine函数
        start_idx = i
        in_function = True
        brace_count = 0
        print(f'找到ReadDocLine函数在第 {i+1} 行')
    elif in_function:
        brace_count += line.count('{') - line.count('}')
        if brace_count == -1:
            end_idx = i + 1
            break

print(f'start_idx={start_idx}, end_idx={end_idx}')

if start_idx is not None and end_idx is not None:
    # 新的ReadDocLine函数
    new_func = '''void CInfoFile::ReadDocLine()
{
    ls.clear();
    ifstream file(_F_STOCK, ios::in);
    if (!file.is_open())
    {
        // 文件打开失败
        AfxMessageBox(_T("文件打开失败！路径：") + CString(_F_STOCK));
        return;
    }

    msg data;
    char delimiter;

    // 跳过表头行（如果有）
    string header;
    getline(file, header);

    // 读取数据行
    while (file >> data.id >> delimiter >> data.name >> delimiter
        >> data.price >> delimiter >> data.age >> delimiter >> data.count)
    {
        // 校验分隔符是否为 |
        if (delimiter != '|')
        {
            AfxMessageBox(_T("文件格式错误，分隔符不是 |！"));
            break;
        }
        ls.push_back(data);
        TRACE(_T("读取商品ID: %d, 名称: %s\\n"), data.id, CString(data.name.c_str()));
    }
    num = ls.size();
    file.close();
}
'''

    # 构建新的文件内容
    new_lines = [new_func + '\n']
    new_lines.extend(lines[end_idx:])
    lines = lines[:start_idx] + new_lines

    # 写回文件
    with open('CInfoFile.cpp', 'w', encoding='gbk') as f:
        f.writelines(lines)

    print('修复完成！')
    print(f'已替换第 {start_idx + 1} 行到第 {end_idx} 行的ReadDocLine函数')
else:
    print('未找到ReadDocLine函数')
