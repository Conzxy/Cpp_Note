# C++中的名字查找
## ADL
ADL全名为**Argument dependent lookup**<br>
规则是对于**无限定域**的函数调用时，除了解析当前所在namespace，还会继续在函数参数的namespace中寻找名字。
```cpp
function(args);           //无限定域
namespace function(args); //有限定域
```
## name lookup in template function
C++17 Standard said：
> Such names are unbound and are looked up at the point of the template instantiation (17.6.4.1) in both the context of the template definition and the context of the point of instantiation.

模板的名字查找分两个阶段：定义阶段和实例化阶段
> 1.from the template **definition**: for non-dependent names
2.during **instantiation**: from the definition but also, via ADL only, from the instantiation context

名字查找取决于该名字是否依赖于模板，即名字可分为dependent name 和non-dependent name<br>
### 第一阶段：
> Non-dependent names are `just looked up` in the context where the template is defined. **Anything relating to the actual instantiation is not taken into consideration**: Since the name is determined as not depending on the template argument, it wouldn't make sense to take the instantiation into account. This is phase I look-up.

所以在第一阶段实例化函数并不会考虑在内，而且注意just looked up这句话，也就是说non-dependent在第一阶段就已经查找完成，如果没找到，会直接报错。（实例一也表现了这点）
### 第二阶段
>  Dependent function names are looked up taking the instantiation into account. This uses **all arguments and determines associated namespaces to look up functions in these associated namespace only**.For `built-in types`, the associated namespace added is the `global namespace`. For `other types`, the associated namespaces added are `the namespace they live in` plus `all enclosing namespace`. In addition, the associated namespace of things visible from the `class definition` are added: the associated namespaces of `base classes`, for `templates the namespaces of the template arguments`, etc. This is phase II look-up and also called argument dependent look-up (I think the terms are not entirely identical and the details are not as easy as described above, of course).

第二阶段是通过ADL进行的，但也保留了第一阶段的名字。
根据ADL，会继续在函数参数的关联namespace中寻找。
* 对于`内置类型`，namespace可以扩大到全局域
* 对于`其他类型`，包括它们在的namespace和其他封闭的namespace（参数与其要有关联）
* 对于`类定义`，还可以增加基类namespace和模板参数的namespace的可访问部分（若有的话）

## 为什么要分两次查找
> What is the reason for this splitting name lookup into phases? For one thing, we want names to be looked up early, **to spot errors when a template is parsed, rather than used.** However, dependent names simply cannot be looked up before the template arguments are known. Therefore, only nondependent names are bound at the point of definition of a template, and dependent names are bound at the point of instantiation, when a template is actually used.

## 模板基类
```cpp
#include<iostream>
using namespace std;

struct C{};

void f(C){
 cout<<"C"<<endl;
}

template<typename T>
void f(T)
{
  cout<<"T"<<endl;
}
template<typename T>
class A
{
    public:
    void f() {
    cout << "A::f()" << endl;
 }
};

template<typename T>
class B:public A<T>
{
    public:
      void g()
      {
        f(C());//gloabl f():f non-template declaration
        this->f();//A::f()
        A<T>::f();//A::f()
      }
    };
int main(){
    B<int> b;
    b.g();
}
```
f是个non-dependent name，在模板定义阶段进行查找，此时一个不受限的模板基类是不会被考虑的（C++标准），我们可以将它变成一个dependent name以达到拖延到第二阶段进行查找的目的。<br>

> C++ FAQ:
Here’s the rule: the compiler does not look in dependent base classes (like A<T>) when looking up nondependent names (like f).

合理的解释：B的模板定义阶段，B的基类A<T>依赖于模板参数T，此时，A<T>对于B来说是不可见的（还未实例化），此时编译器认为这是个non-member函数；
而到了B的模板实例化阶段，编译器已经认为这个函数是个non-member函数，便不会到基类中寻找名字了；<br>

不过，f能找到全局中的f且不是template define的版本，由此可见，它可能通过ADL在全局域找到了f,此时非模板f又比模板f更加匹配，所以输出C，所以很奇妙的是f作为non-dependent的同时可以通过ADL去关联namespace中寻找名字。
## 实例
```cpp
#include <iostream>
using namespace std;

template<typename T>
void adl(T)
{
  cout << "T";
}

struct S
{
};

template<typename T>
void call_adl(T t)
{
  adl(S());
  adl(t);
}

void adl(S)
{
  cout << "S";
}

int main ()
{
  call_adl(S());
}
```

> Why the result is "TS"?

adl(S())是non-dependent name，所以此时在作用域内的只有template define的adl(S()),所以输出"T"。<br>
adl(t)是dependent name，此时在作用域内有两个函数可以匹配，但非模板的更好，所以输出"S"。<br>
如果注释掉template declaration的`adl`的话，`adl(S())`会报错，因为它找不到template define，同时也不会进入第二阶段。
