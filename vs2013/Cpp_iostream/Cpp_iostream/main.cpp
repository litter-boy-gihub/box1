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
    char* str1 = "cout�����";
    cout.write(str1, strlen(str1)) << endl;

    //���ַ�����,��������string
    while ((ch1 = cin.get()) != '\n'){
        cout << ch1;
    }
    cout << endl;
    char str2[10];
    cin.getline(str2, sizeof(str2));
    cout << str2 << endl;  */

    ////////////////
    /////��ʽ��/////<iomanip>
    //fmtflags
    ios_base::fmtflags stdfmtflag = 0;
    stdfmtflag = cout.flags();
    cout << stdfmtflag << endl;
    

    //ws
    std::stringstream is("   game is beginning!");
    is >> ws;//��������ǰ�ÿհ׷�
    string line;
    getline(is, line);
    cout << line << endl;

    //ends endl flush unitbuf
    //'\0' ����ˢ�� ˢ�� ˢ�¿��Ʒ� �������п�����'\n'
    cout << 1 << ends;
    cout <<flush;
    cout <<"endl"<< endl;
    cout << unitbuf<<"unitbuf";
    cout.flags(stdfmtflag);

    //����
    int n = 32; 
    cout << dec << n << endl;
    cout << oct << n << endl;
    cout << hex << n << endl;
    cout.flags(stdfmtflag);

    //���Ͷ���
    //setwÿ��ֻ����һ������ֶ�,setw(n)ÿһ������Ҫд,Ĭ���Ҷ���
    int a = 1, b = 234;
    cout << setw(3) << a <<setw(3)<< b <<setw(3)<< n << endl;
    cout << setiosflags(ios::left);
    cout << setw(3) << a << setw(3) << b << setw(3) << n << endl;
    cout << setw(5) << a <<setw(5) <<b <<setw(5)<< n << endl;
    cout << setiosflags(ios::internal);
    cout << setfill('*');//internam������ʲô��Ϊʲô��rightһ��
    cout << setw(5) << a << setw(5) << b << setw(5) << n << endl;
    cout.flags(stdfmtflag);
    cout << setw(2) << a <<setw(2)<< b <<setw(2)<< n << endl;
    cout << setfill(' ');
    //setprecision(n)  n����λ����С��ǰ+С����,�������㣬������������
    //setprecision ��setw��ͬ���ǣ�ֻ������һ�Σ�ֱ���´θ���ǰ����Ч
    float f = 123.123456789;
    cout << f << endl;//Ĭ��Ϊ6
    cout << setprecision(7) << f << endl;
    //fixed ��scientific
    cout << "fixed: " << fixed << 0.01 << endl;
    cout << "scientific: " << scientific << 0.01 << endl;
    cout << "hexfloat: " << hexfloat << 0.01 << endl;
    cout << "defaultfloat: " << defaultfloat << 0.01 << endl;
    //boolapha showbase showpoint showpos skipws uppercase
    cout.flags(stdfmtflag);
    cout << "boolapha: " << boolalpha << bool(1) << endl;
    cout << showbase << hex << 32 << endl;
    cout << "showpoint: " << showpoint << 1.0 << endl;
    cout <<"��ʾ+��"<< showpos << 1.2 << endl;
    cout<<"����ǰ��հ׷�(����������ʱ�Ƿ�����հ׷�)" << endl;
    istringstream istr(" jjj");
    char ch;
    istr>>noskipws>> ch;
    cout << "noskipws:" << ch << "***�����ַ�Ϊ�հ׷�" << endl;
    cout <<"�������ĸ��д�����ַ�������"<< uppercase << 0x32 << endl;
    cout.flags(stdfmtflag);

    


    system("pause");
    return 0;
}