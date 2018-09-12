
#include "stdlib.h"
#include<iostream>
using namespace std;

int main()
{
    cout << "输入小写字母：";
    char a;
    cin >> a;
while (a<'a' || a>'z') {
	    cout<<"输入的字母不符合，请重新输入:";
	    cin >> a;
    }
    char b = a - 32;
    cout << "转换后的大写字母为:" << b<<endl;
    return 0;
}
