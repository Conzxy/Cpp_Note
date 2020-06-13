# Base class and derivad class
## Inheritance of member functions
基类有两种menber functions：
* 直接继承不改变
* 继承但覆盖基类版本

后者称为**虚函数(Virtual function)**，任何constructor外的non-static functions都可以成为Virtual，并且注意Virtual只能出现在declaration时，而不能出现在类外的define中。<br?
Virtual的解析过程发生在运行阶段，也就是说如果不是虚函数，那么解析过程发生在编译期，这时就无关derived class的细节，不管是base object还是derived object都是调用唯一的那个函数。

## Class derivation list
: 访问说明符 base class name
```cpp
class base{/*contents*/};
//public
class derived :public base;
//private
class derived :private base;
//protectd
class derived :protected base;
```
一般都是public继承，这样base class的public成为derived class接口的一部分，同时我们可以将public derived object绑定到base class的引用和指针上，从而在任何需要base引用或指针的地方我们都可以使用derived object。

## derived-to-base
一个derived object有多个组成成分：
* derived 自己定义的（non-static）成员的子对象
* base 对应的子对象
* 如果有更多base，则对应子对象也多

正是因为派生类对象含有其基类对应的组成部分，所以我们能把派生类对象当成基类对象来使用，并且可以将基类的指针或引用绑定到派生类对象中的基类部分。<br>
这个隐式转换使得我们可以在需要基类引用或指针的情况下，用派生类对象代替。
## derived constructor
各类管好自己成员的初始化，派生类对于基类继承来的成员并不能直接初始化，因此派生类必须用基类的构造函数来初始化它的基类部分。<br>

**我们必须明确：每个类负责定义各自的接口，要与类的对象进行交互必须使用接口**。<br>
另外注意按照声明顺序进行初始化，否则可能产生未定义的行为。
## derived declaration
派生类的声明中不需要类派生列表，
```cpp
class base;
class derived :public base; //error
class derived;              //true
```
并且注意派生类的定义必须在基类之后

## direct base and indirect base
```cpp
class base{/*........*/};
class D1 :public base{/*........*/};
class D2 :public D1{/*..........*/};
```
base是D1的直接基类（direct base），而D1是D2的直接基类，同时base也是D2的间接基类（indirect base）。<br>
每个类都会继承基类所有成员，那么每个派生类都会含有基类部分，如此迭代，最终的派生类将会包含它的直接基类以及每个间接基类的基类部分，其对象包含所有基类的子对象。
## final
```cpp
class base final{/*......*/}
```
final关键字可以防止继承发生
