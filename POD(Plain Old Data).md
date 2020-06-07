# Plain Old Data
## Wikipedia
In computer science and object-oriented programming, a passive data structure (PDS, also termed a plain old data structure, or plain old data, POD), is a term for a record, to contrast with objects.
**It is a data structure that is represented only as passive collections of field values (instance variables), 
without using object-oriented features**.

A PDS type in C++, or Plain Old C++ Object, is defined as either a scalar type or a PDS class. A PDS class has no `user-defined 
copy assignment operator`, no `user-defined destructor`, and no `non-static data members` that are not themselves PDS. Moreover,
a PDS class must be an aggregate, meaning it has `no user-declared constructors`, no `private` nor `protected non-static data`, 
no `virtual base classes` and no `virtual functions`. The standard includes statements about how PDS must behave in C++. 

In some contexts, C++ allows only PDS types to be used. For example, 
a union in C++98 cannot contain a class that has virtual functions or nontrivial constructors or destructors. 
This restriction is imposed because the compiler cannot determine which constructor or destructor should be called for a union. 
PDS types can also be used for interfacing with C, which supports only PDS.

## C++03 Standard
**3.9(10)**: "Arithmetic types (3.9.1), enumeration types, pointer types, and pointer to member types (3.9.2) and cv-qualified versions of these types (3.9.3) are collectively caller scalar types. Scalar types, POD-struct types, POD-union types (clause 9), arrays of such types and cv-qualified versions of these types (3.9.3) are collectively called POD types"

**9(4)**: "A POD-struct is an aggregate class that has no non-static data members of type non-POD-struct, non-POD-union (or array of such types) or reference, and has no user-define copy operator and no user-defined destructor. Similarly a POD-union is an aggregate union that has no non-static data members of type non-POD-struct, non-POD-union (or array of such types) or reference, and has no user-define copy operator and no user-defined destructor.

**8.5.1(1)**: "An aggregate is an array or class (clause 9) with no user-declared constructors (12.1), no private or protected non-static data members (clause 11), no base classes (clause 10) and no virtual functions (10.3)."

PS:cv-qualified:"c" is const and "v" is vilatile. 

## 归纳
* `int`, `char`, `wchar_t`, `bool`, `float`, `double` are PODs, as are `long/short` and `signed/unsigned` versions of them.
* `pointers` (including pointer-to-function and pointer-to-member) are PODs,
* `enums` are PODs
* a `const` or `volatile` POD is a POD.
* a `class`, `struct` or `union` of PODs is a POD provided that all non-static data members are `public`, 
and it has no base class and no constructors, destructors, or virtual methods. 
Static members don't stop something being a POD under this rule. 
This rule has changed in C++11 and certain private members are allowed.
