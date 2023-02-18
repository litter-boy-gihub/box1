// Foundamentals_c.cpp : 定义控制台应用程序的入口点。
//

#include<iostream>
#include<stdio.h>
#include<string.h>
#include<WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
//#pragma pack(4)
#pragma pack(show)

using std::cout;
using std::endl;

int main(void)
{
    char ch = 'a';
    short st = 5;
    int it = 10;
    long l = 20;
    float f = 2.123456789;
    double d = 3.123456789;

    printf("char: %c size:%d\n", ch, sizeof(ch));
    printf("short: %d size:%d\n", st, sizeof(st));
    printf("int: %d size:%d\n", it, sizeof(it));
    printf("long: %d size:%d\n", l, sizeof(l));
    printf("float: %f size:%d\n", f, sizeof(f));
    printf("double: %.8lf size:%d\n", d, sizeof(d));



    //参考https://m.jb51.net/article/247795.htm
    char stra[16] = "find a error ";
    char strb[] = "in my life round";
    char strc[] = "in my life round";

    std::cout << "len of stra: " << strlen(stra) << "sizeof : " << sizeof(stra) << std::endl;
    std::cout << &stra << std::endl;

    //将参数二的字符串接到参数一最后一个字符后的地址上，并且不检查参数一的长度，会导致溢出但不报错
    strcat(stra, strb);
    std::cout << stra << std::endl;
    std::cout << &stra << std::endl;
    std::cout << sizeof(stra) << std::endl;

    //将参数二拷贝到参数一地址，不检查长度，会导致溢出不报错
    //strcpy的底层实现为 char* ret = pa;//记录起始地址 while(*pa++ = *pb++ != '\0') return pa;所以最后的‘\0’是拷贝来的，下面把17长度stra
    std::cout << sizeof(strb) << std::endl;
    strcpy(strb, stra);

    std::cout << stra << std::endl;
    std::cout << strb << std::endl;
    std::cout << strc << std::endl;

    //相等返回0，大于返回》0，小于返回《0
    std::cout << std::boolalpha << strcmp(stra, strb) << std::endl;
    std::cout << std::boolalpha << strcmp(strc, strb) << std::endl;

    char str1[10] = "canyou";
    char str2[] = "help";
    char str3[] = "mee";
    //strncat strncpy strncmp 
    //strncat（det，source，num） 复制strlen（str）和num中小的个数加上一个空字符。
    strncat(str1, str2, 3);
    std::cout <<"strncat str1:"<< str1<<"  str2： "<<str2<< std::endl;

    //strncpy strlen和num中小的复制并填充0到num个。
    strncpy(str1, str2, 10);
    std::cout << "strncpy str1: " << str1 << std::endl;
    strncpy(str1, str3, 2);
    std::cout << "strncpy str1: " << str1<<"  str3: "<<str3 << std::endl;
    str1[2] = '\0';

    //strncmp
    std::cout<<"strncmp: "<<strncmp(str1, str3, 2)<<std::endl;

    //strstr寻找到str1中第一次出现str2的指针位置
    char str_f[] = "lp";
    str1[2] = 'l';
    char * pret = strstr(str1, str_f);
    std::cout << "strstr: " << pret << std::endl;

    //strtok用于字符分割
    char str_k[] = "help.me*to_find";
    char sep[] = ".*_";

    for (char *str = strtok(str_k, sep); str != NULL; str = strtok(NULL, sep)){
        std::cout << str << std::endl;
    }

////////////////////////////////////内存对齐
    cout << "变量起始地址被__alignof整除" << endl;
    cout << "align:  char: " << __alignof(ch) <<" short: "<<__alignof(st)<<" int :"<<__alignof(it)<<" double: "<<__alignof(d)<< endl;
    cout << "地址：char " <<(void*)&ch <<" short: "<<(void*)&st<<" int "<<(void*)&it<<" double "<<(void*)&d<<endl;
    
    struct Data{
        int a;
        double b;
        char c;
        float f;
        char ch;
    };
    
    struct DATA{
        int a;
        double b;
        char c;
        float f;
        char ch;
    } Dat;
    Data data={ 1, 2.0, 3, 4.0 };
    char buffer[320];
    Data * d2 = (Data *)&buffer[1];
    cout << "结构体成员对齐" << endl;
    cout <<"align of struct Data: "<<__alignof(data) <<" 结构体地址: "<<(void*)&data.a<<" "<<(void*)&data<<" sizeof(data): "<<sizeof(data)<< endl;
    cout << "align of struct DATA: " << __alignof(Dat) << " 结构体地址: " << (void*)&Dat.a <<" "<<(void*)&Dat<< " sizeof(Dat): " << sizeof(Dat) << endl;

    //////////////位域声明struct 位域名{类型说明符 位域名：位域长度} 其中类型只能是整型，（signed/unsigned）char short int long.
    struct Bit_field{
        int a : 32;
        int : 0;
        int e : 5;
    }bit_field;
    bit_field.a = 1;
    bit_field.e = 2;
    cout << bit_field.a << " " << bit_field.e << endl;

    cout << sizeof(bit_field) << endl;

    struct bif{
        char a : 1;
        int b : 2;
    };
    //数据类型不同则会直接调整到下一个数据类型开始存放。
    cout << "如果数据类型不同：" << sizeof(bif) << endl;

    struct bif_double{
        char a;
        char c;
        double b;
    }bfd;
    bfd.a = -256;
    bfd.b = 0;
    bfd.c = 15;
    cout << "double的对齐值到底是8还是4：" << sizeof(bfd) << endl;

    struct bit_f{
        char a;
        char b;
        int c;
    }bitf;
    bitf.a = -256;
    bitf.b = 15;
    bitf.c = 0;
    cout << "位域的对齐：" << sizeof(bitf) << endl;

    //大小端
    unsigned int intager = 0x11223344;//小端模式下内存中为44332211 大端模式下内存中为11223344
    unsigned char *buf = (unsigned char*)&intager;

    //结果显示为44332211说明c++默认采用的是小端模式存储数据
    cout << "0x";
    for (int i = 0; i < 4; i++){
        printf("%x", *buf);
        buf += 1;
    }

    //大小端数据转换需要包含WinSock2.h ws2_32.lib
    //主要的有四个 htonl htons ntohl ntohs
    unsigned short ust = 0x1122;    //地址中为2211
    unsigned int uint = 0x11223344; //地址中为44332211

    printf("转换前ushort & uint: ");
    buf = (unsigned char *)&ust;
    for (int i = 0; i < 2; i++){
        printf("%x", *buf);
        buf += 1;
    }
    printf("   ");
    buf = (unsigned char *)&uint;
    for (int i = 0; i < 4; i++){
        printf("%x", *buf);
        buf += 1;
    }
    printf("\n");

    unsigned short n_ust = htons(ust);
    unsigned int n_uint = htonl(uint);

    printf("转换后net_ushort & net_uint: ");
    buf = (unsigned char *)&n_ust;
    for (int i = 0; i < 2; i++){
        printf("%x", *buf);
        buf += 1;
    }
    printf("   ");
    buf = (unsigned char *)&n_uint;
    for (int i = 0; i < 4; i++){
        printf("%x", *buf);
        buf += 1;
    }
    printf("\n");
    
    //ntohl和ntohs 分别和htonl等同 htons等同




    getchar();
	return 0;
}

