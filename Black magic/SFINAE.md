# Substitution Failure is not an Error
## 概念
SFINAE技术是指在编译期将函数模板的形参换成实参，如果替换失败不会算作编译错误，而是继续寻找合适的特化版本，直到所有的模板版本都替换失败编译器才报错。
## Substitution解释
### 发生场合
> Function template parameters are substituted (replaced by template arguments) twice:
> * **explicitly specified** template arguments are substituted **before** template argument deduction
> * **deduced arguments** and the arguments obtained from the **defaults** are substituted **after** template argument deduction
### 发生时候
> Substitution occurs in
* all types used in the `function type` (which includes return type and the types of all parameters)
* all types used in the `template parameter` declarations

> * all `expressions` used in the function type
* all `expressions` used in a template parameter declaration
(since C++11)

> * all expressions used in the explicit specifier
(since C++20)

自C++11后，表达式也纳入范围
### 什么样的Failure被当作error
> Only the failures in the types and expressions in the **immediate context** of the function type or its template parameter types or its explicit specifier (since C++20) are SFINAE errors. If the evaluation of a substituted type/expression causes a side-effect such as instantiation of some template specialization, generation of an implicitly-defined member function, etc, errors in those side-effects are treated as hard errors.
## Example1
```cpp
template<typename C>
class IsClass{
  typedef char one;
  typedef struct { char a[2]; } two;
  template<typename T>
  static one check(int T::*);
  template<typename T>
  static two check(...);
public:
  static constexpr bool value=sizeof(IsClass<C>::check<C>(nullptr))==1;
};
```
type_traits中就运用了SFINAE技术写了诸如is_class,is_base_of等函数  
这个例子就是如此，当类类型作为模板实参替换时，那么就会与第一个check匹配，而非类类型作为模板实参替换时第一个匹配失败，编译器不报错，继续匹配，然后第二个成功，这就是SFINAE的作用。

## Example2（和SFINAE没太大关系，私活）
```cpp
template<typename B,typename D>
class is_base_of{
  struct conv{
    operator B*()const;
    operator D*();
  };
  typedef char(&yes)[1];
  typedef char(&no)[2];
  template<typename T>
  static yes check(D*,T);
  static no check(B*,int);
public:
  static constexpr bool value=sizeof(check(conv(),0))==sizeof(yes);
};
```
这里有几个问题：
* 为什么`static yes check(D*,T)`用模板
* 为什么`operator B()const`要加const

### If they are related
candidate function of check:  
first:  

           D*(conv()&)
second:

           B*(conv()const&)
           D*(conv()&)

对于一个non-const对象，显然第二个转换函数比第一个好。（这么考虑是因为type_traits会把cv属性去除）
但如果把const去掉呢？  
对于一般的重载决策情况，调用已经产生二义性，但是转换函数会根据返回类型决定优先级：
> For conversion functions, however, there is a backdoor. If two conversion functions are equally good, then the return type of them decides who is best according to 13.3.3/1.

> The rule is that the better conversion from the return type of the conversion function to the destination type wins

         conv()->operator D*->D*->B*
         conv()->operator B*->B*

`B*->B* better than to D*->B*`,所以去掉const会导致总是调用第二个check。**const的作用就是强迫编译器去选择第二个转换函数**，把类型转换弄回正轨。

把const再加上来，调用第二个转换函数，`D*->D* better than to D*->B*`,如此就能识别B是D的基类。
### If they are not related
candidata function of check:  
first:

       D*(conv()&)
second:  

       B*(conv()const&)

当D与B不存在继承关系时，非模板和模板都可以匹配，但非模板优于模板(因此没产生二义性)，第二个转换函数因为无法转换成B*直接舍弃，因此直接调用第二个check。

<如果第二个函数也是模板会怎样呢？>  
B不是D的基类会产生调用二义性。首先解释一下为什么B是D的基类反而没事，因为此时D*->B*存在，那转换就存在优先级，因此可以直接调用第一个check，而B不是D的基类，优先级一样(没有竞争因素）

       conv->conv const->B*
       conv->D*

如果第一个函数不是模板也是如此。

**PS：有两个转换函数的类一定要考虑优先级，否则二义性**
