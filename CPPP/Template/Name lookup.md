# C++中的名字查找
## name lookup in template function
C++17 Standard said：
> Such names are unbound and are looked up at the point of the template instantiation (17.6.4.1) in both the context of the template definition and the context of the point of instantiation.

模板的名字查找分两个阶段：定义阶段和实例化阶段
> 1.from the template **definition**: for non-dependent names
2.during **instantiation**: from the definition but also, via ADL only, from the instantiation context

名字查找取决于该名字是否依赖于模板，即名字可分为dependent name 和non-dependent name<br>
### 第一阶段：
> For a non-dependent name used in a template definition, unqualified name lookup takes place when the template definition is examined. The binding to the declarations made at that point is not affected by declarations visible at the point of instantiation.

第一阶段执行的是非限定查找，在实例化点之前，在当前作用域和更高级作用域中可见的一切声明，都可以被考虑，包括已定义的模板和非模板函数，但是若非模板版本的声明在要调用该函数之后则不会考虑，该声明会在实例化阶段可见，若之前则立刻与它绑定，这大概与实例化阶段才能生成代码有关。
### 第二阶段
>  Dependent function names are looked up taking the instantiation into account. This uses **all arguments and determines associated namespaces to look up functions in these associated namespace only**.For `built-in types`, the associated namespace added is the `global namespace`. For `other types`, the associated namespaces added are `the namespace they live in` plus `all enclosing namespace`. In addition, the associated namespace of things visible from the `class definition` are added: the associated namespaces of `base classes`, for `templates the namespaces of the template arguments`, etc. This is phase II look-up and also called argument dependent look-up (I think the terms are not entirely identical and the details are not as easy as described above, of course).

第二阶段是通过ADL进行的，但也保留了第一阶段的名字。
根据ADL，会继续在函数参数类型的关联namespace中寻找。
* 对于`内置类型`，会到全局查找
* 对于`其他类型`，包括它们在的namespace和其他封闭的namespace（参数与其要有关联）
* 除此之外，还可以增加基类namespace和模板参数的namespace的可访问部分（若有的话）



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
f是个non-dependent name，在模板定义阶段进行查找，此时一个不受限的模板基类是不会被考虑的（C++标准），因为模板基类在实例化阶段才会生成代码，我们可以将它变成一个dependent name以达到拖延到第二阶段进行查找，从而达到想要的效果。
this->或者::都可以起到这个作用。

> C++ FAQ:
Here’s the rule: the compiler does not look in dependent base classes (like A<T>) when looking up non-dependent names (like f).

合理的解释：B的模板定义阶段，B的基类A<T>依赖于模板参数T，此时，A<T>对于B来说是不可见的（还未实例化），此时编译器认为这是个non-member函数；
而到了B的模板实例化阶段，编译器已经认为这个函数是个non-member函数，便不会到基类中寻找名字了；<br>
## 实例
1.
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

adl(S())是non-dependent name，又非模板版本在被调用点以下，那么在实例化阶段才会可见，此时只有template版本可以绑定，所以输出"T"。<br>
adl(t)是dependent name，此时在作用域内有两个函数可以匹配，但非模板的更好，所以输出"S"。<br>
如果将非模板版本调至调用点之前，则输出"SS"

2.
```cpp
void f(char); // first declaration of f

template<class T>
void g(T t) {
    f(1);    // non-dependent name: lookup finds ::f(char) and binds it now
    f(T(1)); // dependent name: lookup postponed
    f(t);    // dependent name: lookup postponed
//  dd++;    // non-dependent name: lookup finds no declaration
}

enum E { e };
void f(E);   // second declaration of f
void f(int); // third declaration of f
double dd;

void h() {
    g(e);  // instantiates g<E>, at which point
           // the second and the third uses of the name 'f'
           // are looked up and find ::f(char) (by lookup) and ::f(E) (by ADL)
           // then overload resolution chooses ::f(E).
           // This calls f(char), then f(E) twice
    g(32); // instantiates g<int>, at which point
           // the second and the third uses of the name 'f'
           // are looked up and find ::f(char) only
           // then overload resolution chooses ::f(char)
           // This calls f(char) three times
}
```

## 为什么要分两次查找
> What is the reason for this splitting name lookup into phases? For one thing, we want names to be looked up early, **to spot errors when a template is parsed, rather than used.** However, dependent names simply cannot be looked up before the template arguments are known. Therefore, only nondependent names are bound at the point of definition of a template, and dependent names are bound at the point of instantiation, when a template is actually used.
