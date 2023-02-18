#include<iostream>
#include<string>
#define nel '\n'
using namespace std;

int main(){
    //����
    string s1();
    string s2(5, '*');
    char chs[10] = "abcdefghi";
    string s3(chs);
    string s4(chs, 5);
    string s5(s4, 2, 3);
    string s6(s4.begin(), s4.end() - 2);
    string s7(s6);
    string s8(std::move(string("lost")));
    string s9{'i','n','i','t'};
    cout << s2 << endl << s3 << endl<<s4<<endl<<s5<<endl<<s6<<endl<<s7<<endl<<s8<<endl<<s9<<endl;

    cout << "at and []: " << s9.at(0) <<"   "<<s9[s9.size()-1]<< endl;
    cout << "front and back: " << s9.front() << "   " << s9.back() << endl;
    cout << s9.data() << endl;//����const char *�ײ�ָ��,��Ҫ�����ֹ
    cout << s9.c_str() << endl;//���ؿ���ֹ��const char*

    //����
    cout << "empty:" << boolalpha << s9.empty()<< endl;
    cout << "capacity size and size_max:" <<s9.capacity()<<"  "<<s9.size()<<"  "<< s9.max_size() << endl;
    s9.reserve(s9.capacity() + 1);//���ڵ�ǰcapacity()�Զ�����
    cout << "reseved capacity:" << s9.capacity() << endl;
    s9.shrink_to_fit();//����capacity��size�����󣬵�ȡ��ʵ�֣�vsʵ���Ǽ��ٵ�
    cout << "shrint_to_fit size and capacity" << s9.size() << "  " << s9.capacity() << endl;
    //resize �������

    char s[] = "abcdefghi";
    string mystr(s);
    //����
    mystr.clear();
    cout << "cleared size: " << mystr.size()<<endl;
    mystr = s;
    //����,��ĳ��λ�ò��룬�����λ�õ�����[����ʼ������ƶ�
    //���Բ����ַ����ַ�����string������ͨ������ָ��λ�ã�Ҳ����ͨ��itaraterָ����Χ
    //�ַ�
    mystr.insert(0, 3, 'q');//(pos ,count ,ch)
    mystr.insert(mystr.begin() + 2, 'x');
    mystr.insert(mystr.begin() + 3, 3, 'y');
    cout << "�����ַ���" << mystr << endl;
    //char*�ַ���
    mystr.insert(0, s); //����ֹ
    mystr.insert(0, s, 3); //[s,s+count]�����ܹ����п���ֹ�ַ�
    cout << "����c�ַ�����" << mystr << endl;
    //string
    mystr.insert(0, s2);//pos all of string
    mystr.insert(0, s2, 3, 5);//pos string range[b,e)
    mystr.insert(mystr.begin(), s3.begin() + 2, s3.end() - 3); //range [first,end)
    cout << mystr << endl;

    //erase
    mystr.erase(0, 4);//pos count �Ƴ�count �ʹ�pos��ʼ����ַ����н�С��min(count,size-pos);
    mystr.erase(mystr.begin());//�Ƴ�itrλ���ַ�
    mystr.erase(mystr.begin(), mystr.begin() + 6);//�Ƴ�[first,end)
    cout << "erased: " << mystr << endl;

    //campare
    cout << "compare str:"<<boolalpha << mystr.compare(s3) << endl;
    cout<<"compare pos count str: "<<mystr.compare(0, 3, s3)<<endl;
    cout << "compare pos1 count1 string pos2 count2�� " << mystr.compare(19, 3, s3, 0, 3) << endl;
    //c�ַ�����string�Ƚ�һ�����������Ʒ�ʽ 1��c_str��2(pos,count,c_str) 3(pos1,count1,c_str,count2) ���ǵ����ֲ�ͬ��ֻ�ܴ�c�ַ�����ʼ�Ƚ�
    
    //replace��Ҫ������(�滻�ĳ��ȿ��Բ�ͬ��
    cout <<"before replace,string is: "<<mystr << nel;
    cout<<"replace 1 :"<<mystr.replace(0, 3, string("***"))<<nel;
    cout<<"replace 2 :"<< mystr.replace(0, 3, string("*$$$*"),1,3)<<nel;
    cout << "replace 3: " << mystr.replace(0, 3, "@@@")<<nel;
    cout << "replace 4: " << mystr.replace(0, 3, "%%%%", 4)<<nel;
    cout << "replace 5: " << mystr.replace(0, 4, 4, '!')<<nel;
    cout << "replace 6: " << mystr.replace(mystr.begin(), mystr.begin() + 4, { '+', '+', '+', '+' })<<nel;

    char destination[20];
    //��������substr copy swap resise
    cout << "substr: " << mystr.substr(0, 4) << nel;
    //mystr.copy((char*)destination, 4, 0);//�����󲻻���ӿ���ֹ copy������ _Copy
    mystr._Copy_s(destination, 20, 4, 0);
    destination[4] = '\0';
    cout << "copy: " << destination << nel;
    cout << "swap before s3 and mystr:" << s3 << "  " << mystr << endl;
    mystr.swap(s3);
    cout << "after swap s3 and mystr:" << s3 << "  " << mystr << endl;
    //resize(count,[ch]) ���count>size,���Զ����charT������С�ڣ�����������count���ַ���
    cout << "before resize size:" << mystr.size() << " " << mystr << nel;
    mystr.resize(mystr.size() + 3);
    cout << "resize size:" << mystr.size() << " " << mystr << nel;
    mystr.resize(mystr.size() + 3,'*');
    cout << "resize size with char '*':" << mystr.size() << " " << mystr << nel;
    mystr.resize(mystr.size() - 9);
    cout << "resize size:" << mystr.size() << " " << mystr << nel;

    //����findѰ�ҵ�һ�γ���(str/char*/charT,pos,[count])��pos��ʼ����
    cout << "string is:" << mystr << nel;
    cout<<"find def:"<<mystr.find(string("def"))<<nel;
    char fc[] = "cdef";
    cout<<"find cde:"<<mystr.find(fc, 0, 3)<<nel;//����ʼ��pos,[fc,fc+count)
    cout<<"find cdef:"<<mystr.find(fc, 0)<<nel;//����ʼ��pos
    cout<<"find f:"<<mystr.find('f', 0)<<nel;
    cout << "find failed: return == npos?:" << mystr.find("fe") <<boolalpha<<(mystr.find("fe") == mystr.npos)<< nel;
    //rfind Ѱ��Ѱ�����һ�γ��֣�ͬfind
    //find_first_of find_first_not_of
    //find_last_of find_last_not_of
    //����������string��char*[count],  ch, ��ʼ����λ��λ��posĬ��=npos
    string str("abcabcabc");
    char fl[] = "abc";
    cout << "find last of "<<str << nel;
    cout<<"find abc:"<<str.find_last_of(string("abc"),string::npos)<<nel;
    cout<<"find ab:"<<str.find_last_of(fl, string::npos, 2)<<nel;//(char*,pos,count) Ѱ��[pos,pos+count)
    cout<<"find abc:"<<str.find_last_of(fl, string::npos)<<nel;//(char*,pos)
    cout<<"find b:"<<str.find_last_of('b', string::npos)<<nel;
    

    system("pause");
    return 0;
}