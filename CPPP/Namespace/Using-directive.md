# using指令
## 概要
A using-directive is a block-declaration（{/*block*/}）
## 作用
> Using-directives are allowed only in `namespace scope` and in `block scope`. From the point of view of unqualified name lookup of any name after a using-directive and until the end of the scope in which it appears, every name from namespace-name is visible as if it were declared in the **nearest enclosing namespace which contains both the using-directive and namespace-name**.
## using指令与声明区域
> Using-directive does not add any names to the declarative region in which it appears (unlike the using-declaration), and thus does not prevent identical names from being declared.

using指令并不是在当前的声明区域中添加它的名字，而仅仅是可以使用它的名字，向使用using指令的namespace注入它的名字，同时，它也就不能阻止有与该namespace中相同名字的声明，这点与using声明是不同的。
## 传递性
> Using-directives are transitive for the purposes of unqualified lookup: if a scope contains a using-directive that nominates a namespace-name, which itself contains using-directive for some namespace-name-2, the effect is as if the using directives from the second namespace appear within the first. The order in which these transitive namespaces occur does not influence name lookup.

```cpp
namespace A {
    int i;
}
namespace B {
    int i;
    int j;
    namespace C {
        namespace D {
            using namespace A; // all names from A injected into global namespace
            int j;
            int k;
            int a = i; // i is B::i, because A::i is hidden by B::i
        }
        using namespace D; // names from D are injected into C
                           // names from A are injected into global namespace
        int k = 89; // OK to declare name identical to one introduced by a using
        int l = k;  // ambiguous: C::k or D::k
        int m = i;  // ok: B::i hides A::i
        int n = j;  // ok: D::j hides B::j
    }
}
```
## directive point后的namespace扩张
If, after a using-directive was used to nominate some namespace, the namespace is extended and additional members and/or using-directives are added to it, those additional members and the additional namespaces are visible through the using-directive (in contrast with using-declaration)
```cpp
namespace D {
   int d1;
   void f(char);
}
using namespace D; // introduces D::d1, D::f, D::d2, D::f,
                   //  E::e, and E::f into global namespace!
int d1; // OK: no conflict with D::d1 when declaring
namespace E {
    int e;
    void f(int);
}
namespace D { // namespace extension
    int d2;
    using namespace E; // transitive using-directive
    void f(int);
}
void f() {
    d1++; // error: ambiguous ::d1 or D::d1?
    ::d1++; // OK
    D::d1++; // OK
    d2++; // OK, d2 is D::d2
    e++; // OK: e is E::e due to transitive using
    f(1); // error: ambiguous: D::f(int) or E::f(int)?
    f('a'); // OK: the only f(char) is D::f(char)
}
```
