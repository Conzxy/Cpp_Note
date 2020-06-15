# Argument dependent-name lookup
## 引入情景
```cpp
namespace N
{
  class X {};
  void f(X);
  X& operator++(X&);
}

int main()
{
  // define an object of type X
  N::X x;

  // apply f to it
  N::f(x);

  // apply operator++ to it
  ???
}
```
> Of course you could have written N::operator++(x), but that would have defeated the whole point of operator overloading. Therefore a solution had to be found which allowed the compiler to find operator++(X&) despite the fact that it was not in scope. On the other hand, it still should not find another operator++ defined in another, unrelated namespace which might make the call ambiguous (in this simple example, you wouldn't get ambiguity, but in more complex examples, you might). The solution was Argument Dependent Lookup (ADL), called that way since the lookup depends on the argument (more exactly, on the argument's type). Since the scheme was invented by Andrew R. Koenig, it is also often called Koenig lookup.

> The trick is that for function calls, in addition to normal name lookup (which finds names in scope at the point of use), there is done a second lookup in the scopes of the types of any arguments given to the function. So in the above example, if you write x++ in main, it looks for operator++ not only in global scope, but additionally in the scope where the type of x, N::X, was defined, i.e. in namespace N. And there it finds a matching operator++, and therefore x++ just works. Another operator++ defined in another namespace, say N2, will not be found, however. Since ADL is not restricted to namespaces, you also can use f(x) instead of N::f(x) in main().

所以没有ADL，今日的Hello world应该这样写：
```cpp
//ADL
std::cout<<"Hello world!"<<"\n";
//non-ADL
std::operator<<(std::operator<<(std::cout,"Hello world!"),"\n");
```
这就违背了运算符重载的初衷了。有了ADL，我们处理<<就没有这么麻烦，可以去参数cout的所在namespace中寻找operator<<并选择最佳匹配（模板实例化阶段用ADL匹配），然后你会发现每个operator<<的左参数是std的，所以现在我们可以写出
```cpp
std::cout<<"......"<<"....."<<"...."<<std::endl;
```
但要注意endl必须要加std，因为endl是std的名字，
那么我们必须这样写：
```cpp
std::cout<<endl;  //error.endl is not declaration in this namespace;
endl(std::cout)   //true.The argument of endl is in std namespace so it finds the std::endl;
```
## ADL的触发条件
> First, the argument-dependent lookup is not considered if the lookup set produced by usual unqualified lookup contains any of the following:<br>
1) a declaration of a class member<br>
2) a declaration of a function at block scope (that's not a using-declaration)<br>
3) any declaration that is not a function or a function template (e.g. a function object or another variable whose name conflicts with the name of the function that's being looked up)<br>

> What this means is that ADL is ignored only when unqualified lookup produces one of the above three results. Since we are not dealing with a class member, the function is declared at namespace scope, not block scope, and we **only find functions** we continue on and use ADL.

所以，如果查找集中有类成员声明、在块作用域中的函数声明（没有using声明）或者所有声明都不是函数的声明（包括模板），那么ADL不会被考虑（区别block scope和namespace scope）。
## ADL的好处
> As the simple code example above demonstrates, Koenig lookup provides convenience and ease of usage to the programmer. Without Koenig lookup there would be an overhead on the programmer, to repeatedly specify the fully qualified names, or instead, use numerous using-declarations.

## ADL的弊端
Consider the example of std::swap, which is a standard library algorithm to swap two values. <br>
With the Koenig lookup one would have to be cautious while using this algorithm because:

`std::swap(obj1,obj2);`<br>
may not show the same behavior as:
```cpp
using std::swap;
swap(obj1, obj2);
```
With ADL, which version of swap function gets called would depend on the namespace of the arguments passed to it.<br>
If there exists an namespace A and if A::obj1, A::obj2 & A::swap() exist then the second example will result in a call to A::swap(), which might not be what the user wanted.<br>

Further, if for some reason both `A::swap(A::MyClass&, A::MyClass&)` and `std::swap(A::MyClass&, A::MyClass&)`are defined, then the first example will call `std::swap(A::MyClass&, A::MyClass&)` but the second will not compile because `swap(obj1, obj2)` would be ambiguous.
## example
```cpp
namespace A {
      struct X;
      struct Y;
      void f(int);
      void g(X);
}

namespace B {
    void f(int i) {
        f(i);   // calls B::f (endless recursion)
    }
    void g(A::X x) {
        g(x);   // Error: ambiguous between B::g (ordinary lookup)
                //        and A::g (argument-dependent lookup)
    }
    void h(A::Y y) {
        h(y);   // calls B::h (endless recursion): ADL examines the A namespace
                // but finds no A::h, so only B::h from ordinary lookup is used
    }
}
```
