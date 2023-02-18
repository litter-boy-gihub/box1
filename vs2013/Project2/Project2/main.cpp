#include<iostream>
#include<fstream>
#include<string>
#include<sstream>

using namespace std;


int main(){
    //д�ļ�
    ofstream ofs("a.txt", ios::out | ios::trunc);
    cout << "file is open: "<<boolalpha << ofs.is_open() << endl;
    ofs << "string 123 3.45" << endl;
    ofs.close();
    //���ļ�
    char str[20];
    int i = 0;
    float f = 0;
    ifstream ifs("a.txt");
    ifs >> str >> i >> f;
    cout <<"������� "<< str <<"  "<< i <<"  "<< f << endl;
    ifs.close();
    //�����ƶ�д
    ofs.open("b.txt", ios::out | ios::trunc | ios::binary);
    cout <<"strlen of str:"<<strlen(str) << endl;
    ofs.write((const char*)&str, strlen(str)+1);
    ofs.write((const char*)&i, sizeof(int));
    ofs.write((const char*)&f, sizeof(float));
    ofs.close();
    //�����ƶ�
    char  str2[20];
    int a = 0;
    float b = 0;
    ifs.open("b.txt", ios::in | ios::binary);
    char* pch = str2;
    do{
        ifs.read(pch++, 1);
    } while (*(pch-1) != '\0');
    ifs.read((char*)&a, sizeof(a));
    ifs.read((char*)&b, sizeof(b));
    cout << "eof:" << ifs.eof() << endl;
    ifs.close();
    cout << "�����ƶ������"<<str2<<"  "<< a << "  " << b << endl;

    //����ʽ��д д������stringstream ��ֻ����c��sscanf(char* ,formt,...);c++û�и�ʽ��
    stringstream str_stream;
    str_stream << "������" << "tom" << "  " << "���䣺" << 23 << endl;
    ofs.open("c.txt", ios::out | ios::trunc);
    ofs << str_stream.str();
    ofs.close();

    string line;
    ifs.open("c.txt", ios::in);
    str_stream.str("");//��ջ�����
    cout << "str_stream��պ�" <<str_stream.str()<< endl;
    while (1){
        getline(ifs, line);
        if (ifs.eof()) break;
    }
    cout << "eof:" << ifs.eof() << endl;
    sscanf_s(line.c_str(), "������%s  ���䣺%d", str2,sizeof(str2), &a);
    printf("������%s  ���䣺%d", str2, a);
    ifs.close();
    

    //������������ļ���ȡ�������½ṹ
    //  while(1){
    //     ifstream.read(data);//��
    //     if(ifstream.eof()) break;
    //     dataȷ����Ч���ű����ȡ����

    //��������д
    /*ifstream.read(data);
    while (!ifstream.eof()){
        data; ȷ����Ч�������ȡ����
        ifstream.read(data);
    }*/

    system("pause");
    return 0;
}