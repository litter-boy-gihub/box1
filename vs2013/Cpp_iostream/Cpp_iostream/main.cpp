#include<iostream>
#include<iomanip>
#include<sstream>

using namespace std;
int main(){
    //put
   /* char ch1 = '#';
    cout.put(ch1);
    cout << endl;

    //write(char*,int)
    char* str1 = "cout块输出";
    cout.write(str1, strlen(str1)) << endl;

    //多字符输入,不能输入string
    while ((ch1 = cin.get()) != '\n'){
        cout << ch1;
    }
    cout << endl;
    char str2[10];
    cin.getline(str2, sizeof(str2));
    cout << str2 << endl;  */

    ////////////////
    /////格式化/////<iomanip>
    //fmtflags
    ios_base::fmtflags stdfmtflag = 0;
    stdfmtflag = cout.flags();
    cout << stdfmtflag << endl;
    

    //ws
    std::stringstream is("   game is beginning!");
    is >> ws;//消耗所有前置空白符
    string line;
    getline(is, line);
    cout << line << endl;

    //ends endl flush unitbuf
    //'\0' 换行刷新 刷新 刷新控制符 单独换行可以用'\n'
    cout << 1 << ends;
    cout <<flush;
    cout <<"endl"<< endl;
    cout << unitbuf<<"unitbuf";
    cout.flags(stdfmtflag);

    //进制
    int n = 32; 
    cout << dec << n << endl;
    cout << oct << n << endl;
    cout << hex << n << endl;
    cout.flags(stdfmtflag);

    //域宽和对齐
    //setw每次只作用一个输出字段,setw(n)每一个数都要写,默认右对齐
    int a = 1, b = 234;
    cout << setw(3) << a <<setw(3)<< b <<setw(3)<< n << endl;
    cout << setiosflags(ios::left);
    cout << setw(3) << a << setw(3) << b << setw(3) << n << endl;
    cout << setw(5) << a <<setw(5) <<b <<setw(5)<< n << endl;
    cout << setiosflags(ios::internal);
    cout << setfill('*');//internam作用是什么，为什么和right一样
    cout << setw(5) << a << setw(5) << b << setw(5) << n << endl;
    cout.flags(stdfmtflag);
    cout << setw(2) << a <<setw(2)<< b <<setw(2)<< n << endl;
    cout << setfill(' ');
    //setprecision(n)  n是总位数，小数前+小数后,不够补零，超过四舍五入
    //setprecision 和setw不同的是，只用设置一次，直到下次更改前都有效
    float f = 123.123456789;
    cout << f << endl;//默认为6
    cout << setprecision(7) << f << endl;
    //fixed 和scientific
    cout << "fixed: " << fixed << 0.01 << endl;
    cout << "scientific: " << scientific << 0.01 << endl;
    cout << "hexfloat: " << hexfloat << 0.01 << endl;
    cout << "defaultfloat: " << defaultfloat << 0.01 << endl;
    //boolapha showbase showpoint showpos skipws uppercase
    cout.flags(stdfmtflag);
    cout << "boolapha: " << boolalpha << bool(1) << endl;
    cout << showbase << hex << 32 << endl;
    cout << "showpoint: " << showpoint << 1.0 << endl;
    cout <<"显示+："<< showpos << 1.2 << endl;
    cout<<"跳过前面空白符(仅用于输入时是否输入空白符)" << endl;
    istringstream istr(" jjj");
    char ch;
    istr>>noskipws>> ch;
    cout << "noskipws:" << ch << "***输入字符为空白符" << endl;
    cout <<"输出里字母大写（非字符串）："<< uppercase << 0x32 << endl;
    cout.flags(stdfmtflag);

    


    system("pause");
    return 0;
}