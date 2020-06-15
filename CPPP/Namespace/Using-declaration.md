# Using 声明
## 概要
> Introduces a name that is defined elsewhere into the **declarative region** where this using-declaration appears.
## 作用
> Using-declarations can be used to introduce namespace members into other namespaces and block scopes, or to introduce base class members into derived class definitions.

using 声明是实实在在的引入了这个namespace member，这个声明是可以加入name lookup的查找集的，这点与using指令是不同的。
## 形式
> A using-declaration with more than one using-declarator is equivalent to a corresponding sequence of using-declarations with one using-declarator.
```cpp
//using declarotor list
using name,name,name;
//equivalent:more than one usinng declarator
using name;using name;using name;
```
## 引入名字的使用
> Names introduced into a namespace scope by a using-declaration can be used just like any other names, including qualified lookup from other scopes:

引入的名字使用和其他名字并无二样，限定性名字也是如此。
```cpp
void f();
namespace A {
    void g();
}
namespace X {
    using ::f; // global f is now visible as ::X::f
    using A::g; // A::g is now visible as ::X::g
    using A::g, A::g; // (C++17) OK: double declaration allowed at namespace scope
}
void h()
{
    X::f(); // calls ::f
    X::g(); // calls A::g
}
```

## 声明点后的namspace扩张
> If, after the using-declaration was used to take a member from a namespace, the namespace is extended and additional declarations for the same name are introduced, those additional declarations do not become visible through the using-declaration (in contrast with using-directive). One exception is when a using-declaration names a class template: partial specializations introduced later are effectively visible, because their lookup proceeds through the primary template.

```cpp
namespace A {
    void f(int);
}
using A::f; // ::f is now a synonym for A::f(int)

namespace A { // namespace extension
   void f(char); // does not change what ::f means
}
void foo() {
    f('a'); // calls f(int), even though f(char) exists.
}
void bar() {
   using A::f; // this f is a synonym for both A::f(int) and A::f(char)
   f('a'); // calls f(char)
}
```

## 隐藏和重载一往如旧
> All restrictions on regular declarations of the same names, hiding, and overloading rules apply to using-declarations:

```cpp
namespace A {
    int x;
}
namespace B {
    int i;
    struct g { };
    struct x { };
    void f(int);
    void f(double);
    void g(char); // OK: function name g hides struct g
}
void func() {
    int i;
    using B::i; // error: i declared twice

    void f(char);
    using B::f; // OK: f(char), f(int), f(double) are overloads
    f(3.5); // calls B::f(double)

    using B::g;
    g('a');      // calls B::g(char)
    struct g g1; // declares g1 to have type struct B::g

    using B::x;
    using A::x;  // OK: hides struct B::x
    x = 99;      // assigns to A::x
    struct x x1; // declares x1 to have type struct B::x
}
```
## 引入完全相同的是病态的（二义性）
> If a function was introduced by a using-declaration, declaring a function with the same name and parameter list is ill-formed (unless the declaration is for the same function). If a function template was introduced by a using-declaration, declaring a function template with the same name, parameter type list, return type, and template parameter list is ill-formed. Two using-declarations can introduce functions with the same name and parameter list, but if a call to that function is attempted, the program is ill-formed.

```cpp
namespace B {
    void f(int);
    void f(double);
}
namespace C {
    void f(int);
    void f(double);
    void f(char);
}
void h() {
    using B::f; // introduces B::f(int), B::f(double)
    using C::f; // introduces C::f(int), C::f(double), and C::f(char)
    f('h');      // calls C::f(char)
    f(1);        // error: B::f(int) or C::f(int)?
    void f(int); // error: f(int) conflicts with C::f(int) and B::f(int)
}
```
## 内外声明冲突
> If an entity is declared, but not defined in some inner namespace, and then declared through using-declaration in the outer namespace, and then a definition appears in the outer namespace with the same unqualified name, that definition is a member of the outer namespace and conflicts with the using-declration:

```cpp
namespace X {
  namespace M {
     void g(); // declares, but doesn't define X::M::g()
  }
  using M::g;
  void g();   // Error: attempt to declare X::g which conflicts with X::M::g()
}
```
