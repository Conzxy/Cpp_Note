# 流的状态

## 条件状态(condition case)

strm为IO类型，s为strm类型对象

| 属性          | 作用                                               |
| ------------- | -------------------------------------------------- |
| strm::iostate | 一种与机器相关的类型，提供了表达条件状态的完整功能 |
| strm::badbit  | 指出流已崩溃                                       |
| strm::failbit | 指出IO操作失败                                     |
| strm::eofbit  | 指出到达了文件末尾                                 |
| strm::goodbit | 指出流未处于错误状态。该值保证为0                  |

| 接口                      | 功能                                                         |
| ------------------------- | ------------------------------------------------------------ |
| s.eof():bool              | 若strm::eofbit置位，返回true                                 |
| s.fail():bool             | 若failbit或badbit置位，返回true                              |
| s.bad():bool              | 若badbit置位，则返回true                                     |
| s.good():bool             | 若流处于有效状态，则返回true                                 |
| s.clear():void            | 将所有条件状态复位，使流设置为有效。                         |
| s.clear(flags):void       | 根据给定的flags标志位，将对应条件状态复位。flags:strm::iostate |
| s.setstate(flags):void    | 根据给定的flags标志位，将对应条件状态置位。                  |
| s.rdstate():strm::iostate | 返回当前条件状态                                             |

## 查询流的状态

以上提到的四种条件状态是四个为iostate的constexpr值，表示特定的IO条件:

```cpp
static constexpr _Iostate goodbit = static_cast<_Iostate>(0x0);
static constexpr _Iostate eofbit  = static_cast<_Iostate>(0x1);
static constexpr _Iostate failbit = static_cast<_Iostate>(0x2);
static constexpr _Iostate badbit  = static_cast<_Iostate>(0x4);
```

`badbit`表示系统级错误，如不可恢复的读写错误。一般badbit被置位后，流就无法使用了。

`failbit`表示可以修正的错误，此时流还可以继续使用。

如果到达文件结束位置，failbit和eofbit都会被置位

goodbit为0表示流处于有效状态

badbit、failbit、eofbit任一个被置位，则检测流的条件失败

（其实访问流对象的条件状态也行，比如cin.failbit）

---

good函数在所有错误均未置位时返回true，而对应的错误位被置位，对应函数返回true

badbit被置位，fail也返回true。

因此，一般用来判断流的总体特征用`good`或`fail`，而特定错误用`eof`和`bad`

## 管理条件状态

rdstate返回当前流的条件状态。

clear则是进行复位，无参版本全部复位，有参版本可以接受iostate值表示新状态：

```cpp
cin.clear(cin.rdstate()&~cin.failbit&&~cin.badbit);
//复位failbit和badbit，其他位不变
```

setstate并不需要rdstate，因为：

```cpp
void __CLR_OR_THIS_CALL setstate(iostate _State, bool _Reraise = false) { // merge _State into state, possibly reraise exception
    clear(rdstate() | _State, _Reraise);
}
```