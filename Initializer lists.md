# Initializer lists
C++03 inherited the initializer-list feature from C. A struct or array is given a list of arguments in braces, **in the order of the members' definitions** in the struct. These initializer-lists are **recursive**, so an array of structs or struct containing other structs can use them.
```cpp
struct Object
{
    float first;
    int second;
};

Object scalar = {0.43f, 10}; //One Object, with first=0.43f and second=10
Object anArray[] = {{13.4f, 3}, {43.28f, 29}, {5.934f, 17}}; //An array of three Objects
```
This is very useful for `static lists`, or initializing a `struct to some value`. C++ also provides constructors to initialize an object, but they are often not as convenient as the initializer list. **However, C++03 allows initializer-lists only on structs and classes that conform to the `Plain Old Data` (POD) definition;** C++11 extends initializer-lists, so they can be used for `all classes` including standard containers like `std::vector`.

C++11 binds the concept to a template, called `std::initializer_list`. This allows constructors and other functions to **take initializer-lists as parameters**. For example:
```cpp
class SequenceClass
{
public:
    SequenceClass(std::initializer_list<int> list);
};
```
This allows SequenceClass to be constructed from a sequence of integers, such as:
```cpp
SequenceClass some_var = {1, 4, 5, 6};
```
This constructor is a special kind of constructor, called an `initializer-list-constructor`. Classes with such a constructor are treated specially during uniform initialization.

The list can be copied once constructed, which is **cheap and will act as a copy-by-reference**(the class is typically implemented as a pair of begin/end pointers). An std::initializer_list is `constant`: its members cannot be changed once it is created, and nor can the data in those members be changed (which rules out moving from them, requiring copies into class members, etc.).

Although its construction is specially treated by the compiler, an std::initializer_list is a real type, and so it can be used in other places besides class constructors.
# Uniform initializer
C++03 has a number of problems with initializing types. Several ways to do this exist, and some produce different results when interchanged. **The traditional constructor syntax, for example, can look like a function declaration, and steps must be taken to ensure that the compiler's most vexing parse rule will not mistake it for such**. Only `aggregates` and `POD types` can be initialized with `aggregate initializers` (using SomeType var = {/*stuff*/};).

C++11 provides a syntax that allows for **fully uniform type initialization** that works on `any object`. It expands on the initializer list syntax:
```cpp
struct BasicStruct
{
    int x;
    double y;
};

struct AltStruct
{
    AltStruct(int x, double y)
        : x_{x}
        , y_{y}
    {}

private:
    int x_;
    double y_;
};

BasicStruct var1{5, 3.2};
AltStruct var2{2, 4.3};   
```
The initialization of `var1` behaves exactly as though it were aggregate-initialization. That is, each data member of an object, in turn, will be copy-initialized with the corresponding value from the initializer-list.

Implicit type conversion will be used where needed. If no conversion exists, or only a narrowing conversion exists, the program is `ill-formed`. The initialization of `var2` invokes the constructor.

One can also do this:
```cpp
struct IdString
{
    std::string name;
    int identifier;
};

IdString get_string()
{
    return {"foo", 42}; //Note the lack of explicit type.
}
```
**Uniform initialization does not replace constructor syntax, which is still needed at times.** If a class has an initializer list constructor (TypeName(initializer_list<SomeType>);), then it takes `priority` over other forms of construction, provided that the initializer list conforms to the sequence constructor's type.
