# std::swap构造拷贝（和移动）赋值运算符的坑
我们来看一下循环队列头文件中：[SQ.h](https://github.com/Conzxy/Data-Structures/blob/master/Queue/SQ.h)<br>
有以下代码（已注释）：
```cpp
template<typename T>
Queue<T>::Queue<T>& operator=(const Queue<T> &rhs)
{
    Queue copy=rhs;
    std::swap(*this,copy);
    return *this;
}
```
上面这个拷贝赋值运算符重载是错误的，我们首先要清楚swap是以怎样的形式工作的：<br>
`swap(T &a,T &b) {T temp(move(a));a=move(b);b=move(temp);}`<br>
所以`std::swap`需要移动构造函数和移动赋值运算符，而拷贝构造函数和拷贝赋值运算符阻止了编译器自动合成它们（假设我们并没有设置自定义的），swap第一个构造尚且可以用拷贝构造代替移动构造，但是下面的赋值运算符找不到移动赋值运算符，就会不断调用拷贝赋值运算符，因此它会一直卡死在这里，我们可以加个调试信息来看看：<br>
```cpp
....
std::cout<<"run"<<std::endl;
Queue copy....
....
```
这样会一直输出run，这是因为缺少了移动赋值运算符，从而导致的无限递归。<br>
根据0原则，Big Five要么一个都不定义，要么就应该定义全部。<br>
```cpp
Queue& operator=(Queue&& rhs)
{
      swap(*this,rhs);
      return *this;
}
template<typename T>
void swap(Queue<T> &lhs,Queue<T> &rhs)
{
     using std::swap;
     swap(lhs.front,rhs.front);
     swap(lhs.rear,rhs.rear);
     swap(lhs.maxSize,rhs.maxSize);
     swap(lhs.theSize,rhs.theSize);
     swap(lhs.data,rhs.data);
}
```

## 一个意外的错误
如果在有自定义移动构造和移动赋值的情况下，这样写的话：
```cpp
Queue& operator=(Queue rhs)
{
    std::swap(*this,rhs);
    return *this;
}
```
这个会提示如下错误信息：<br>

    error: ambiguous overload for’operator=’(operand types are ’ Queue<int>’and ’std: :remove_ ref erence<Queue<int>&>::type {aka Queue<int>}' )

这是在std::swap第二步调用赋值运算符时出错的，因为此时，右侧运算对象是右值，所以既可以调用拷贝赋值也可以调用移动赋值，我们使用自定义的swap可以避免。<br>
但是这个应该避免，这样在右值赋值时会和移动赋值冲突。
## 自定义swap
由上面这个例子可以看出调用std::swap进行自赋值是错误且不安全的，因此我们通常调用自己的swap成员函数。<br>
至于还有一些东西等我看完effective C++条款25再说吧。
