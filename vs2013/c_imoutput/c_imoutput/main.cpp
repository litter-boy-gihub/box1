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
    ////���������std ��׼����cin cout cerr clog
    //cin >> strin;
    //
    //cout << strin << endl;

    //cerr << "error infor !" << endl;

    //clog��cerr���������clog�л���
    //������ض���
    //fstream fs;
    //fs.open("./a.txt",ios_base::out|ios_base::app,_SH_DENYRW);
    //streambuf* coutbuf = clog.rdbuf();
    //clog.rdbuf(fs.rdbuf());//�޸Ļ�����ָ��
    //clog << "�ض���clog" << endl;
    //fs.close();
    //clog.rdbuf(coutbuf);//�ָ�clog������ָ��
    /*
                                ->ifstream/istringstream
                    ->istream
    ios_base->ios               ->(�̳�istream+ostream)iostream  ->fstream/stringstream
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
    //{//������ο�
    //    cin.clear();//��������־ �� cin.setstate()�����Դ�����ֻ��clear����Ĭ��ֵ0������ձ�־
    //    cin.sync();//������뻺�壬��������������Ҳ
    //}
    //    
    //cin >> ch;
    //cout << i_a<<"  " <<ch;
    
    char ch1 = '\0';
    char strs[20];
    //ch1 = cin.get();//���������ǿգ�����ֵΪһ���ַ�������һ�����ã����浽�����У�
    //cout << ch1 << endl;
    //cin.get(ch1);
    //cout << ch1 << endl;
    //cin.get(strs,5,'\n');//�ַ���,ʵ��ֻȡ��4�������һ��Ϊ\0
    //cout << strs << endl;

    //cin.clear();
    //cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');//�����ǰ�����뻺��
    ////cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');//��ջ�����������cin.sync()

    //cin.getline(strs, 5, '\n');
    //cout << strs << endl;

    //cout<<".gcount�������һ�����������ȡ���ַ�����"<<cin.gcount()<<endl;

    //cin.clear();
    //cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');//�����ǰ�����뻺��
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

    //BUG ��¼cout << a & b <<endl; ����д���Ǵ���ģ�Ҫ��<< ����һ������� ��Ҫ�������ȼ� ��a & b��
    ///////////////////////////////////

    cout <<(cin.rdstate() & std::ios_base::badbit)<< endl;; //badbit 4 goodbit 0 eofbit 1 failbit 2 
    //��־λΪ��λ����Щ���ǳ�����ͨ������������Ի��ָ��bit��ֵ

    char str[20] = "can you need";

    char cc;
    cin >> cc;
    cout << cc << endl;

    //���к����ĸ�ЧӦ���eofbit��
    //putback  unget seekg 
    cin.seekg(3, ios::beg); //ʧ������failbit setstate ʵ�ʵ���clear(rdstate()| state).
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

    //cin.read(strs, 5);//����5��failbit��1
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
    //cout << cin.tellg() << endl;//����ָ��������ָ�뵱ǰ���ַ������ǲ�����ȡ
    //cin.seekg((long long)0, ios::beg);
    //cout << "cin.peek: " << (char)cin.peek() << endl;
    //cout << cin.tellg() << endl;
    
    /*cin.seekg()
    cin.tellg();
    cin.peek();*/
    

    system("pause");
    return 0;
}