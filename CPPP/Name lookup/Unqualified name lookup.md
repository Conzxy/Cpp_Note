# 非限定型查找
> Name lookup stops in the scope in which a name is found and it finds all declarations in that scope matching that name. It does not continue to look in enclosing scopes even when there could be better matching functions located there. Then there's also ADL, which is a different matter.

非限定型查找分两个部分：OL（ordinary lookup）和ADL（Argument dependent lookup），OL会停在寻找到名字的那个作用域，同时将那个作用域的其他匹配名字加入查找集（注意，尽管进一步查找会有更好的也不会去查了），同时根据ADL的三个触发条件：<br>
* 无类成员声明
* 无块作用域中的函数声明（using不算）
* 所有声明都是函数声明

全部条件都满足，就会继续进行ADL，将查找到的名字加入查找集，并在ADL结束后进行重载决议，如果OL结束，则提前。
