// Foundamentals_c.cpp : �������̨Ӧ�ó������ڵ㡣
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



    //�ο�https://m.jb51.net/article/247795.htm
    char stra[16] = "find a error ";
    char strb[] = "in my life round";
    char strc[] = "in my life round";

    std::cout << "len of stra: " << strlen(stra) << "sizeof : " << sizeof(stra) << std::endl;
    std::cout << &stra << std::endl;

    //�����������ַ����ӵ�����һ���һ���ַ���ĵ�ַ�ϣ����Ҳ�������һ�ĳ��ȣ��ᵼ�������������
    strcat(stra, strb);
    std::cout << stra << std::endl;
    std::cout << &stra << std::endl;
    std::cout << sizeof(stra) << std::endl;

    //������������������һ��ַ������鳤�ȣ��ᵼ�����������
    //strcpy�ĵײ�ʵ��Ϊ char* ret = pa;//��¼��ʼ��ַ while(*pa++ = *pb++ != '\0') return pa;�������ġ�\0���ǿ������ģ������17����stra
    std::cout << sizeof(strb) << std::endl;
    strcpy(strb, stra);

    std::cout << stra << std::endl;
    std::cout << strb << std::endl;
    std::cout << strc << std::endl;

    //��ȷ���0�����ڷ��ء�0��С�ڷ��ء�0
    std::cout << std::boolalpha << strcmp(stra, strb) << std::endl;
    std::cout << std::boolalpha << strcmp(strc, strb) << std::endl;

    char str1[10] = "canyou";
    char str2[] = "help";
    char str3[] = "mee";
    //strncat strncpy strncmp 
    //strncat��det��source��num�� ����strlen��str����num��С�ĸ�������һ�����ַ���
    strncat(str1, str2, 3);
    std::cout <<"strncat str1:"<< str1<<"  str2�� "<<str2<< std::endl;

    //strncpy strlen��num��С�ĸ��Ʋ����0��num����
    strncpy(str1, str2, 10);
    std::cout << "strncpy str1: " << str1 << std::endl;
    strncpy(str1, str3, 2);
    std::cout << "strncpy str1: " << str1<<"  str3: "<<str3 << std::endl;
    str1[2] = '\0';

    //strncmp
    std::cout<<"strncmp: "<<strncmp(str1, str3, 2)<<std::endl;

    //strstrѰ�ҵ�str1�е�һ�γ���str2��ָ��λ��
    char str_f[] = "lp";
    str1[2] = 'l';
    char * pret = strstr(str1, str_f);
    std::cout << "strstr: " << pret << std::endl;

    //strtok�����ַ��ָ�
    char str_k[] = "help.me*to_find";
    char sep[] = ".*_";

    for (char *str = strtok(str_k, sep); str != NULL; str = strtok(NULL, sep)){
        std::cout << str << std::endl;
    }

////////////////////////////////////�ڴ����
    cout << "������ʼ��ַ��__alignof����" << endl;
    cout << "align:  char: " << __alignof(ch) <<" short: "<<__alignof(st)<<" int :"<<__alignof(it)<<" double: "<<__alignof(d)<< endl;
    cout << "��ַ��char " <<(void*)&ch <<" short: "<<(void*)&st<<" int "<<(void*)&it<<" double "<<(void*)&d<<endl;
    
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
    cout << "�ṹ���Ա����" << endl;
    cout <<"align of struct Data: "<<__alignof(data) <<" �ṹ���ַ: "<<(void*)&data.a<<" "<<(void*)&data<<" sizeof(data): "<<sizeof(data)<< endl;
    cout << "align of struct DATA: " << __alignof(Dat) << " �ṹ���ַ: " << (void*)&Dat.a <<" "<<(void*)&Dat<< " sizeof(Dat): " << sizeof(Dat) << endl;

    //////////////λ������struct λ����{����˵���� λ������λ�򳤶�} ��������ֻ�������ͣ���signed/unsigned��char short int long.
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
    //�������Ͳ�ͬ���ֱ�ӵ�������һ���������Ϳ�ʼ��š�
    cout << "����������Ͳ�ͬ��" << sizeof(bif) << endl;

    struct bif_double{
        char a;
        char c;
        double b;
    }bfd;
    bfd.a = -256;
    bfd.b = 0;
    bfd.c = 15;
    cout << "double�Ķ���ֵ������8����4��" << sizeof(bfd) << endl;

    struct bit_f{
        char a;
        char b;
        int c;
    }bitf;
    bitf.a = -256;
    bitf.b = 15;
    bitf.c = 0;
    cout << "λ��Ķ��룺" << sizeof(bitf) << endl;

    //��С��
    unsigned int intager = 0x11223344;//С��ģʽ���ڴ���Ϊ44332211 ���ģʽ���ڴ���Ϊ11223344
    unsigned char *buf = (unsigned char*)&intager;

    //�����ʾΪ44332211˵��c++Ĭ�ϲ��õ���С��ģʽ�洢����
    cout << "0x";
    for (int i = 0; i < 4; i++){
        printf("%x", *buf);
        buf += 1;
    }

    //��С������ת����Ҫ����WinSock2.h ws2_32.lib
    //��Ҫ�����ĸ� htonl htons ntohl ntohs
    unsigned short ust = 0x1122;    //��ַ��Ϊ2211
    unsigned int uint = 0x11223344; //��ַ��Ϊ44332211

    printf("ת��ǰushort & uint: ");
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

    printf("ת����net_ushort & net_uint: ");
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
    
    //ntohl��ntohs �ֱ��htonl��ͬ htons��ͬ




    getchar();
	return 0;
}

