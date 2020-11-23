---
title: C与C++的作用域
comment: true
tag:
 - C++
 - scope
categories:
 - C++
 -  scope
---
# Scope Region
作用域这个概念不是C++特有的，而是由C继承过来的，但同时添加了C++专属的新作用域，我们先介绍C的作用域再引入C++的作用域。
<!-- more -->
## Translation units
在介绍C的作用域之前，要先了解什么叫做翻译单元（亦称编译单元：compilation units）。  
> **A C program consists of units called source files** (or preprocessing files), which, in addition to `source code`, includes `directives for the C preprocessor`. A translation unit is the output of the C preprocessor – a source file after it has been preprocessed.

正如我们知道的，C（C++）程序是由多个源文件组成的，而源文件包括源代码和预处理指令，如#include指令和#define指令，而翻译单元正是一个源文件经过预处理器合并预处理指令和源代码后产生的临时源文件，由于是预处理器产生的，亦成为编译单元。
> Preprocessing notably consists of expanding a source file to recursively replace all #include directives with the literal file declared in the directive (usually header files, but possibly other source files); **the result of this step is a preprocessing translation unit**. Further steps include macro expansion of #define directives, and conditional compilation of #ifdef directives, among others; **this translates the preprocessing translation unit into a translation unit**. From a translation unit, the compiler generates an object file, which can be further processed and linked (possibly with other object files) to form an executable program.

预处理阶段包括头文件的导入以及宏的处理。  
度过了预处理期，到了编译的后期，每一个编译单元会转换成目标文件（object file），而这些目标文件通过进一步的处理和链接形成可执行程序。
## Scope in C
### 作用域概念
> When the compiler encounters the declaration of a name, it stores that name and its attributes in a symbol table. When the compiler encounters a reference to a name, it looks up the name in the symbol table to find those attributes. **Each declared name is visible — can be found by lookup — only within a portion of the translation unit** called its scope.

作用域的特征很明显：
* 被声明名字是可见的
* 可被查找到（名字查找需要通过作用域）
* 是翻译单元的一部分

作用域各司其职，也正是有了作用域，名字查找才有法可依。
### 作用域分类
> * A name has `file scope` if it's declared in the **outermost scope of a translation unit**, that is, outside of any function, structure, or union declaration. **Its scope begins right after its declaration and runs to the end of the translation unit**.
* A name (other than a statement label) has `block scope` if it's declared within a **function definition (including that function's parameter list) or in a brace-enclosed block within that function**. Its scope begins right after its declaration and runs to the end of the block immediately enclosing that declaration.
* A name has` function prototype scope` if it's declared in the function parameter list of a function declaration that is not also a definition. **Its scope begins right after its declaration and runs to the end of the parameter list.**
* `Statement labels`, and only statement labels, have `function scope`. A label **can be defined only in the body of a function definition** and is in scope **everywhere in that body, even before the label has been defined**.

c的作用域大致可分为：
* 文件作用域（file scope）
* 块作用域（block scope）
* 函数原型作用域（function prototype scope）
* 函数作用域（function scope）

#### file scope
是翻译单元最外层的作用域，在所有function（包括了block）、struct、union之外，从声明点（declaration point）可持续到翻译单元结束。
#### block scope
范围很广，函数定义的参数列表和函数体、for语句、if语句等都属于block scope。
#### function prototype scope。
主要用于函数声明的参数列表。这个不难理解，函数的声明和实现也是依靠这个。如果参数列表不分出来，声明和定义得挨着写。
#### function scope。
这个仅lable专属，function scope使得lable在整个函数中都是可见的，即使是前面的语句也可以使用它。
## Scope in C++
### 作用域分类
C++的作用域在继承C的同时也引入了新特性，大致可分为：
* function scope
* function prototype scope
* namespace scope
* class scope
* local scope
* template parameter scope
* enumeration scope

#### function、function prototype scope
继承自C,这个就不多说了。
#### local scope
对应C的block scope，适应了一些C++的新特性，比如扩展了for语句中初始化语句可以声明：
```cpp
for(int i=0;i<N;i++);
```
#### namespace scope
对应C的file scope，namespace与file是同级别的，但namespace相比file更好的解决了一些命名冲突的问题，同时封装性也更好（你愿意把一大堆声明、定义、类型别名之类的都塞进file scope吗），这也是符合C++特性的。
> C++ provides a facility（设施） called namespaces for grouping names that would otherwise be crowded into file scope. C++ generalizes the rules for names declared at file scope to include names declared in namespaces as well.

原来的C的file scope在C++中被称为`global namespace scope`（或`global scope`）。  
尽管一个namespace是global namespace的成员，但其所在的作用域是global scope，它可以持续到翻译单元结束（因此与file同级）。当然，成员namespace的block属于它自己的namespace scope。因此，global scope是最高级别（top-level）的namespace。
> The scope of an entity declared in an unnamed namespace or in an inline namespace includes the enclosing namespace;

还要注意未命名空间和内联命名空间的作用域是可以包含在封闭命名空间中持续的。如：
```cpp
namespace N { // scope of N begins (as a member of global namespace)
    int i; // scope of i begins
    int g(int a) { return a; } // scope of g begins
    int j(); // scope of j begins
    void q(); // scope of q begins
    namespace {
        int x; // scope of x begins
    } // scope of x does not end
    inline namespace inl { // scope of inl begins
      int y; // scope of y begins
    } // scope of y does not end
} // scope of i,g,j,q,inl,x,y interrupted

namespace {
    int l=1; // scope of l begins
} // scope of l does not end (it's a member of unnamed namespace)

namespace N { // scope of i,g,j,q,inl,x,y continues
    int g(char a) {  // overloads N::g(int)
        return l+a;  // l from unnamed namespace is in scope
    }
    int i; // error: duplicate definition (i is already in scope)
    int j(); // OK: repeat function declaration is allowed
    int j() { // OK: definition of the earlier-declared N::j()
        return g(i); // calls N::g(int)
    }
    int q(); // error: q is already in scope with different return type
} // scope of i,g,j,q,inl,x,y interrupted

int main() {
    using namespace N; // scope of i,g,j,q,inl,x,y resumes
    i = 1; // N::i is in scope
    x = 1; // N::(anonymous)::x is in scope
    y = 1; // N::inl::y is in scope
    inl::y = 2; // N::inl is also in scope
} // scope of i,g,j,q,inl,x,y interrupted
```
注意到用词，namspace不是用end，而是interrupted，因为作用域可以再次被打开，namespace的作用域是割裂的，因此用中断更为恰当，在翻译单元结束时它才会随之结束。
#### class scope
> The potential scope of a name declared in a class begins at the point of declaration and includes the rest of the class body and **all function bodies** (even if defined outside the class definition or before the declaration of the name), `default arguments`, `exception specifications`, `in-class brace-or-equal initializers`, and all these things in `nested classes`, recursively.

在类中定义的名字从声明点开始到剩余的类部分都算作该名字的作用域（注意声明点位置），所有函数（member）是被包含在了这些作用域中的，也就是说这些函数可以访问到所有定义在类的名字，除此之外还有嵌套类（相当于member class）、类内初始化（by() or =）、异常处理、默认实参等也被包含在这些作用域中。友元类虽然也可以访问所有成员，但不是作用域嵌套关系。  
同时注意以下情况：
```cpp
typedef int c; // ::c
enum { i = 1 }; // ::i
class X {
    char v[i]; // Error: at this point, i refers to ::i
               // but there is also X::i
    int f() {
         return sizeof(c); // OK: X::c, not ::c is in scope inside a member function
    }
    char c; // X::c
    enum { i = 2 }; // X::i
};

typedef char* T;
struct Y {
    T a; // Error: at this point, T refers to ::T
         // but there is also Y::T
    typedef long T;
    T b;
};
```
X::i，Y::T的声明点在后面，那么前面同作用域的将无法访问它它，因此到全局中寻找，而f()是成员函数，是可以访问到类中的任何成员的，因此是X::c。  
如果我想用的是类中的i（X::i）而不是全局的（::i），那么这个设计是不合理的（ill-formed）。
#### template parameter scope
> The potential scope of a template parameter name **begins immediately at the point of declaration and continues to the end of the smallest template declaration in which it was introduced.** In particular, a template parameter can be used in the declarations of **subsequent template parameters and in the specifications of base classes**, but can't be used in the declarations of the **preceding template parameters**.

模板参数作用域类似于前面所提到的函数原型作用域，是与参数列表有关的。模板参数作用域包括了当前的声明点可以持续到模板参数被引入的最小作用域结束。    
看例子可能理解的更好：
```cpp
template< typename T, // scope of T begins
          T* p,       // T can be used for a non-type parameter
          class U = T // T can be used for a default type
        >
class X : public Array<T> // T can be used in base class name
{
   // T can be used inside the body as well
}; // scopes of T and U end, scope of X continues
```
随后的模板参数和基类可以使用之前声明的模板参数。
```cpp
template< template< // template template parameter
                    typename Y,     // scope of Y begins
                    typename G = Y // Y is in scope
                  > // scopes of Y and G end
          class T,
//          typename U = Y // Error: Y is not in scope
          typename U
        >
class X
{
}; // scopes of T and U end
```
如果模板参数列表内嵌了一个模板参数列表，那么内嵌的模板参数列表将会是以<>为作用域，而不是延续到被嵌套的模板参数列表结束。（你想，被嵌套的模板参数列表可能是与类、函数绑定的，那么内嵌的模板参数不应该进入其中）
#### 枚举类型
枚举类型分为两种：限定作用域的和不限定作用域的，限定作用域的枚举（scoped enumeration）是C++11引入的。  
* 不限定作用域的枚举类型，其枚举成员与枚举类型本身所在的作用域相同。
* 如果限定作用域，则遵循一般的作用域原则，它是封闭的，在作用域外是不可访问的。

```cpp
enum e1_t { // unscoped enumeration
  A,
  B = A*2
}; // scope of A and B does not end

enum class e2_t { // scoped enumeration
    SA,
    SB = SA*2 // SA is in scope
}; // scope of SA and SB ends

e1_t e1 = B; // OK, B is in scope
// e2_t e2 = SB; // Error: SB is not in scope
e2_t e2 = e2_t::SB; // OK
```
## 补充
file scope在C++中file可表示在单一源文件（single source file），所以又区别出了两种对象file scoped variable和global variable（object），主要是它们的链接属性（linkage）不同，global variable具有external linkage而file scoped variable只具有lnternal linkage。所以C++中file scope和global scope可以算是两种作用域以区别两种对象是不同的链接属性的。
```cpp
static int nvalue;//file scoped variable
int x;//global variable
int main()
{
  float f;//local variable
}
```
像这样在global scope中声明加上static修饰符的话，就被限制在了当前源文件（只能在其中被访问）。关于storage class specifiers和linkage我会在另一篇文详细的讲。
## 总结
C++的作用域可分为：
* local scope(block scope)
* global (namesapce) scope(file scope,top-level namesapce scope)
* function scope
* function prototype scope
* namespace scope
* class scope
* template parameter scope
* enumeration scope

我们区分作用域大小一个良好的习惯就是要留意声明点的位置（declaration point）
