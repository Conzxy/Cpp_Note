# Block
Compound statements or blocks are brace-enclosed sequences of statements.

    attr{ statement...(optional) }

When one statement is expected, **but multiple statements need to be executed in sequence** (for example, in an if statement or a loop), a compound statement may be used:

```cpp
if (x > 5)          // start of if statement
{                   // start of block
    int n = 1;      // declaration statement
    std::cout << n; // expression statement
}                   // end of block, end of if statement
```
Each compound statement introduces its own block scope; variables declared inside a block are destroyed at the closing brace in **reverse order**:
```cpp
int main()
{
    {                                // start of block
        std::ofstream f("test.txt"); // declaration statement
        f << "abc\n";                // expression statement
    }                                // end of block: f is flushed and closed
    std::ifstream f("test.txt");
    std::string str;
    f >> str;
}
```
和构造函数与析构函数的定义和销毁顺序相似
