# 流的随机访问

!!!：通常istream和ostream不支持随机访问，因此该内容仅适用于fstream，stringstream

| 接口               | 功能                                                         |
| ------------------ | ------------------------------------------------------------ |
| tellg()            | 返回输入流中当前标记的位置                                   |
| tellp()            | 返回输出流······························                     |
| seekg(offset,from) | 将标记定位在from之前/后的offset个字符。                      |
| seekp(offset,from) | from可以取beg/cur/end                                        |
| seekg(pos)         | 在输入流中将标记重定位到指定的绝对地址                       |
| seekp(pos)         | 在输出流························································ |

## 只有一个标记

流中只有一个标记，即并不存在独立的读标记和写标记。

`ifstream/istringstream`只能调用seekg/tellg

`ofstream/ostringstream`只能调用seekp/tellp

`fstream/stringstream`都能调用

实际上，-g/-p的读写位置都映射到这个单一标记