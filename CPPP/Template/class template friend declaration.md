# 类模板的友元声明
非模板友元，则可以访问所有模板实例<br>
模板友元，可授权给所有友元模板实例，也可授权给特定友元模板实例<br>
## 特定实例（一对一友元关系）
这种情况，需要前置声明模板自身，比如：<br>
```cpp
template<typename> class Queue;
template<typename T>
void printQueue(const Queue<T>&);
template<typename T>
void swap(Queue<T>&,Queue<T>&);

template<typename Datatype>
class Queue{
  ....
public:
  ....
  friend void printQueue<Datatype>(const Queue<Datatype>&);
  friend void swap<Datatype>(Queue<Datatype> &,Queue<Datatype> &);
}
```

## 所有实例
这种情况不用前置声明模板自身，但是友元声明必须使用与类模板本身不同的模板参数
```cpp
template<typename T>
class A{
  //func的所有实例都成为A的友元
  template<typename X>
  friend void func(/*prar...*/){}

  //与A相同实例化的fun成为A的友元
  friend void fun(/*operator...*/){}
}
```
