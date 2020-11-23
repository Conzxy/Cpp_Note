#include <iostream>
using namespace std;

template<typename B,typename D>
class is_base_of{
  struct conv{
    operator B*();
    operator D*();
  };
  typedef char(&yes)[1];
  typedef char(&no)[2];
  //template<typename T>
  static yes check(D*,int);
  static no check(B*,int);
public:
  static constexpr bool value=sizeof(check(conv(),0))==sizeof(yes);
};

class A{};
class B: public A{};
class C{};
int main(){
    auto v=::is_base_of<A,C>::value;
    cout<<v<<endl;
}
