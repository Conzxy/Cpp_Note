# Argument dependent-name lookup
## 引入
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

## ADL的好处
> As the simple code example above demonstrates, Koenig lookup provides convenience and ease of usage to the programmer. Without Koenig lookup there would be an overhead on the programmer, to repeatedly specify the fully qualified names, or instead, use numerous using-declarations.
