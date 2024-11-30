#include "util.h"
using namespace std;

string my_to_string(unsigned int a)
{
    string str="";
    while(a>0)
    {
        str += (char)('0'+a%10);
        a=a/10;
    }
    string astr;
    astr.resize(str.length());
    for(int i=str.length()-1;i>=0;i--)
        astr[str.length()-1-i] = str[i];
    return astr ;
}