#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<fstream>
#include<sstream>
using namespace std;

int main(){
    ////getchar putchar
    //char ch = getchar();
    //getchar();
    //ch = getchar();
    //getchar();

    //putchar(ch);
    //putchar('\n');

    ////gets puts
    //char chs[20];
    //gets(chs);
    //ch = getchar();
    //getchar();
    //putchar(ch);
    //if (puts(chs) == EOF) { fprintf(stderr, "puts error"); }


    //char* strin = new  char[20];
    ////输入输出流std 标准对象cin cout cerr clog
    //cin >> strin;
    //
    //cout << strin << endl;

    //cerr << "error infor !" << endl;

    //clog和cerr的区别就是clog有缓冲
    //流输出重定向
    //fstream fs;
    //fs.open("./a.txt",ios_base::out|ios_base::app,_SH_DENYRW);
    //streambuf* coutbuf = clog.rdbuf();
    //clog.rdbuf(fs.rdbuf());//修改缓冲区指针
    //clog << "重定向clog" << endl;
    //fs.close();
    //clog.rdbuf(coutbuf);//恢复clog缓冲区指针
    /*
                                ->ifstream/istringstream
                    ->istream
    ios_base->ios               ->(继承istream+ostream)iostream  ->fstream/stringstream
                    ->ostream
                                ->ofstream/ostringstream
    
    streambuf       ->filebuf/stringbuf
    */
    //char blank='\0';
    //cin >> blank;
    //cout << blank;

    //int i_a = 0;
    //char ch = '\0';
    //cout << i_a;
    //cin >> i_a;
   
    //cout << endl << cin.good() << cin.bad()<<cin.fail()<<cin.eof() << endl;
    //if (!cin.good())
    //{//错误处理参考
    //    cin.clear();//清除错误标志 和 cin.setstate()都可以带参数只是clear带有默认值0，将清空标志
    //    cin.sync();//清空输入缓冲，如果流和输出缓冲也
    //}
    //    
    //cin >> ch;
    //cout << i_a<<"  " <<ch;
    
    char ch1 = '\0';
    char strs[20];
    //ch1 = cin.get();//参数可以是空，返回值为一个字符；参数一个引用，保存到引用中；
    //cout << ch1 << endl;
    //cin.get(ch1);
    //cout << ch1 << endl;
    //cin.get(strs,5,'\n');//字符串,实际只取了4个，最后一个为\0
    //cout << strs << endl;

    //cin.clear();
    //cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');//清除当前行输入缓冲
    ////cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');//清空缓冲区，类似cin.sync()

    //cin.getline(strs, 5, '\n');
    //cout << strs << endl;

    //cout<<".gcount返回最后一次输入操作读取的字符数："<<cin.gcount()<<endl;

    //cin.clear();
    //cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');//清除当前行输入缓冲
    //cin.putback('a');
    //cin.get(ch1);
    //cout << ch1 << endl;


    /*cin >> ch1;
    cout << "cout ch1:"<<ch1<<endl;
    //cin.rdbuf()->pubseekoff(-3L, ios::end, ios::in);  // 
    cin.seekg(1L, ios::cur);
    cout << "peek"<<(char)cin.peek() << '\n';
    cin >> ch1;
    cout << ch1;*/


    char mychar{};
    cin >> mychar;
    cout << (cin.rdstate() & ios::badbit) \
        << (cin.rdstate() & ios::failbit) \
        << (cin.rdstate() & ios::eofbit) << endl;
    cin >> mychar;

    //BUG 记录cout << a & b <<endl; 这种写法是错误的，要把<< 看成一种运算符 需要定义优先级 （a & b）
    ///////////////////////////////////

    cout <<(cin.rdstate() & std::ios_base::badbit)<< endl;; //badbit 4 goodbit 0 eofbit 1 failbit 2 
    //标志位为三位，这些都是常量，通过与运算符可以获得指定bit的值

    char str[20] = "can you need";

    char cc;
    cin >> cc;
    cout << cc << endl;

    //下列函数的负效应清除eofbit：
    //putback  unget seekg 
    cin.seekg(3, ios::beg); //失败设置failbit setstate 实际调用clear(rdstate()| state).
    cin >> cc;
    cout << cc << endl;

    istringstream stm(str);
    string s;
    string s1;
    stm >> s;
    cout << s <<"  "<<stm.tellg()<<(char)stm.peek()<< endl;
    stm.seekg(0, ios::beg);
    stm >> s1;
    cout << s1 << endl;
    cout << stm.good() << endl;

    //cin.read(strs, 5);//少于5则failbit置1
    //strs[5] = '\0';
    //cout <<"read: " <<strs << endl;

    //long long ll = 2;

    //cout << "cin.peek: " << (char)cin.peek() <<'\n';

    //cout << cin.tellg() <<'\n';
    //cout << "peek: " << (char)cin.peek() << (char)cin.peek() << "  tellg" << cin.tellg() <<'\n';
    //cin.seekg(3L, ios_base::beg);
    //cin.clear();
    //
    //cout << cin.tellg()<<(char)cin.peek() <<'\n';
    //cout << (char)cin.get() << endl;
    //cout << cin.tellg() << endl;




    //cin.clear();
    //cout << "cin.good: " << cin.good() << endl;
    //cin.seekg(ll, ios::beg);
    //cout << "cin.peek: " << (char)cin.peek() << endl;
    //cout <<"cin.good: "<< cin.good() << endl;
    //cout << cin.tellg() << endl;//返回指针输入流指针当前的字符，但是并不提取
    //cin.seekg((long long)0, ios::beg);
    //cout << "cin.peek: " << (char)cin.peek() << endl;
    //cout << cin.tellg() << endl;
    
    /*cin.seekg()
    cin.tellg();
    cin.peek();*/
    

    system("pause");
    return 0;
}