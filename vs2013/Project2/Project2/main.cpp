#include<iostream>
#include<fstream>
#include<string>
#include<sstream>

using namespace std;


int main(){
    //写文件
    ofstream ofs("a.txt", ios::out | ios::trunc);
    cout << "file is open: "<<boolalpha << ofs.is_open() << endl;
    ofs << "string 123 3.45" << endl;
    ofs.close();
    //读文件
    char str[20];
    int i = 0;
    float f = 0;
    ifstream ifs("a.txt");
    ifs >> str >> i >> f;
    cout <<"读结果： "<< str <<"  "<< i <<"  "<< f << endl;
    ifs.close();
    //二进制读写
    ofs.open("b.txt", ios::out | ios::trunc | ios::binary);
    cout <<"strlen of str:"<<strlen(str) << endl;
    ofs.write((const char*)&str, strlen(str)+1);
    ofs.write((const char*)&i, sizeof(int));
    ofs.write((const char*)&f, sizeof(float));
    ofs.close();
    //二进制读
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
    cout << "二进制读结果："<<str2<<"  "<< a << "  " << b << endl;

    //按格式读写 写可以用stringstream 读只能用c的sscanf(char* ,formt,...);c++没有格式读
    stringstream str_stream;
    str_stream << "姓名：" << "tom" << "  " << "年龄：" << 23 << endl;
    ofs.open("c.txt", ios::out | ios::trunc);
    ofs << str_stream.str();
    ofs.close();

    string line;
    ifs.open("c.txt", ios::in);
    str_stream.str("");//清空缓冲区
    cout << "str_stream清空后：" <<str_stream.str()<< endl;
    while (1){
        getline(ifs, line);
        if (ifs.eof()) break;
    }
    cout << "eof:" << ifs.eof() << endl;
    sscanf_s(line.c_str(), "姓名：%s  年龄：%d", str2,sizeof(str2), &a);
    printf("姓名：%s  年龄：%d", str2, a);
    ifs.close();
    

    //如果是完整的文件读取采用如下结构
    //  while(1){
    //     ifstream.read(data);//读
    //     if(ifstream.eof()) break;
    //     data确认有效，才保存读取数据

    //或者如下写
    /*ifstream.read(data);
    while (!ifstream.eof()){
        data; 确认有效，保存读取数据
        ifstream.read(data);
    }*/

    system("pause");
    return 0;
}