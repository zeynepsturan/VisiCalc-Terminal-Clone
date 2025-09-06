#include <string>
using namespace std;

string col_to_str(int num){
    string str;
    int carry=0;
    if(num>26){ //like AA,AB
        carry=num/26;
        num=num%26;
        str+=('A'+carry-1);
    }

    if(num>0)
        str+=('A'+num-1);
    return str;
}

int col_to_int(string str){
    int num = 0;
    for (char c:str){
        num=num *26+(c-'A'+1);
    }
    return num;
}