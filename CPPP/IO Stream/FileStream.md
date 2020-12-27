# File Stream

`ifstream`继承自`istream`

`ofstream`继承自`ostream`

`fstream`继承自`iostream`

## 构造

| 格式                  | 功能                                                       |
| --------------------- | ---------------------------------------------------------- |
| fstream fstrm         | 创建未绑定文件流                                           |
| fstream fstrm(s)      | 创建文件流并绑定名为s的文件，s为string类型或C-style 字符串 |
| fstream fstrm(s,mode) | 在第二个方式的基础上指定文件模式                           |

## 文件模式

| 模式   | 功能                          |
| ------ | ----------------------------- |
| in     | 以read方式打开                |
| out    | 以write方式打开               |
| ate    | 打开文件后立即定位文件末尾    |
| app    | 每次write操作前定位在文件末尾 |
| trunc  | 截断文件                      |
| binary | 以二进制方式进行IO            |

### 默认文件模式

`ifstream`默认`in`

`ofstream`默认`out`

`fstream`默认`in`和`out`

### 注意事项

* 只可以对`ifstream`和`fstream`设定`out`
* 只可以对`ofstream`和`fstream`设定`in`
* 只有当设定`out`时才能设定`trunc`
* 只有当`trunc`没被指定时，就可以设定`app`。`app`隐式指定
* 默认情况，即使不指定`trunc`，以`out`指定也会截断，如果不想截断
  * `ofstream::out|ofstream::app`
  * `fstream::in|fstream::out`

* `ate`和`binary`适用于任何文件流对象

## 以out模式打开时会丢弃已有数据

正如上面第5点所说，默认out模式打开文件会截断文件，如果不想被截断文件：

```cpp
ofstream app("filename",ofstream::out|ofstream::app);	//explicit
ofstream app("filename",ofstream::app);					//implicit
```

## 文件接口

| 接口                    | 功能                          |
| ----------------------- | ----------------------------- |
| fstrm.open(s):void      | 打开名为s的文件，并与其绑定   |
| fstrm.open(s,mode):void | 在上述基础上指定文件模式      |
| fstrm.close():void      | 关闭与fstrm绑定的文件         |
| fstrm.is_open():bool    | 指出是否成功打开fstrm关联文件 |

* 当一个fstream对象被销毁时，close会被自动调用（估计析构函数调用了close）

* 在open时都会确定文件模式，未指定就默认指定，不过推荐显式指定，避免莫名其妙的截断

