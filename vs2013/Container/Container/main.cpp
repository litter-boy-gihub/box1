#include<iostream>
#include<string>
#define nel '\n'
using namespace std;

int main(){
    //构造
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
    cout << s9.data() << endl;//返回const char *底层指针,不要求空终止
    cout << s9.c_str() << endl;//返回空终止的const char*

    //容量
    cout << "empty:" << boolalpha << s9.empty()<< endl;
    cout << "capacity size and size_max:" <<s9.capacity()<<"  "<<s9.size()<<"  "<< s9.max_size() << endl;
    s9.reserve(s9.capacity() + 1);//大于当前capacity()自动扩容
    cout << "reseved capacity:" << s9.capacity() << endl;
    s9.shrink_to_fit();//减少capacity到size的请求，但取决实现，vs实现是减少到
    cout << "shrint_to_fit size and capacity" << s9.size() << "  " << s9.capacity() << endl;
    //resize 代码最后

    char s[] = "abcdefghi";
    string mystr(s);
    //操作
    mystr.clear();
    cout << "cleared size: " << mystr.size()<<endl;
    mystr = s;
    //插入,在某个位置插入，即这个位置的数据[）开始都向后移动
    //可以插入字符，字符串，string，可以通过整数指定位置，也可以通过itarater指定范围
    //字符
    mystr.insert(0, 3, 'q');//(pos ,count ,ch)
    mystr.insert(mystr.begin() + 2, 'x');
    mystr.insert(mystr.begin() + 3, 3, 'y');
    cout << "插入字符：" << mystr << endl;
    //char*字符串
    mystr.insert(0, s); //空终止
    mystr.insert(0, s, 3); //[s,s+count]其中能够含有空终止字符
    cout << "插入c字符串：" << mystr << endl;
    //string
    mystr.insert(0, s2);//pos all of string
    mystr.insert(0, s2, 3, 5);//pos string range[b,e)
    mystr.insert(mystr.begin(), s3.begin() + 2, s3.end() - 3); //range [first,end)
    cout << mystr << endl;

    //erase
    mystr.erase(0, 4);//pos count 移除count 和从pos开始最大字符数中较小的min(count,size-pos);
    mystr.erase(mystr.begin());//移除itr位置字符
    mystr.erase(mystr.begin(), mystr.begin() + 6);//移除[first,end)
    cout << "erased: " << mystr << endl;

    //campare
    cout << "compare str:"<<boolalpha << mystr.compare(s3) << endl;
    cout<<"compare pos count str: "<<mystr.compare(0, 3, s3)<<endl;
    cout << "compare pos1 count1 string pos2 count2： " << mystr.compare(19, 3, s3, 0, 3) << endl;
    //c字符串和string比较一样有三种类似方式 1（c_str）2(pos,count,c_str) 3(pos1,count1,c_str,count2) 但是第三种不同，只能从c字符串开始比较
    
    //replace主要有六种(替换的长度可以不同）
    cout <<"before replace,string is: "<<mystr << nel;
    cout<<"replace 1 :"<<mystr.replace(0, 3, string("***"))<<nel;
    cout<<"replace 2 :"<< mystr.replace(0, 3, string("*$$$*"),1,3)<<nel;
    cout << "replace 3: " << mystr.replace(0, 3, "@@@")<<nel;
    cout << "replace 4: " << mystr.replace(0, 3, "%%%%", 4)<<nel;
    cout << "replace 5: " << mystr.replace(0, 4, 4, '!')<<nel;
    cout << "replace 6: " << mystr.replace(mystr.begin(), mystr.begin() + 4, { '+', '+', '+', '+' })<<nel;

    char destination[20];
    //其他操作substr copy swap resise
    cout << "substr: " << mystr.substr(0, 4) << nel;
    //mystr.copy((char*)destination, 4, 0);//拷贝后不会添加空终止 copy被弃用 _Copy
    mystr._Copy_s(destination, 20, 4, 0);
    destination[4] = '\0';
    cout << "copy: " << destination << nel;
    cout << "swap before s3 and mystr:" << s3 << "  " << mystr << endl;
    mystr.swap(s3);
    cout << "after swap s3 and mystr:" << s3 << "  " << mystr << endl;
    //resize(count,[ch]) 如果count>size,将自动添加charT（），小于，则缩减到首count个字符。
    cout << "before resize size:" << mystr.size() << " " << mystr << nel;
    mystr.resize(mystr.size() + 3);
    cout << "resize size:" << mystr.size() << " " << mystr << nel;
    mystr.resize(mystr.size() + 3,'*');
    cout << "resize size with char '*':" << mystr.size() << " " << mystr << nel;
    mystr.resize(mystr.size() - 9);
    cout << "resize size:" << mystr.size() << " " << mystr << nel;

    //查找find寻找第一次出现(str/char*/charT,pos,[count])从pos开始搜索
    cout << "string is:" << mystr << nel;
    cout<<"find def:"<<mystr.find(string("def"))<<nel;
    char fc[] = "cdef";
    cout<<"find cde:"<<mystr.find(fc, 0, 3)<<nel;//搜索始于pos,[fc,fc+count)
    cout<<"find cdef:"<<mystr.find(fc, 0)<<nel;//搜索始于pos
    cout<<"find f:"<<mystr.find('f', 0)<<nel;
    cout << "find failed: return == npos?:" << mystr.find("fe") <<boolalpha<<(mystr.find("fe") == mystr.npos)<< nel;
    //rfind 寻找寻找最后一次出现，同find
    //find_first_of find_first_not_of
    //find_last_of find_last_not_of
    //参数可以是string，char*[count],  ch, 开始结束位置位置pos默认=npos
    string str("abcabcabc");
    char fl[] = "abc";
    cout << "find last of "<<str << nel;
    cout<<"find abc:"<<str.find_last_of(string("abc"),string::npos)<<nel;
    cout<<"find ab:"<<str.find_last_of(fl, string::npos, 2)<<nel;//(char*,pos,count) 寻找[pos,pos+count)
    cout<<"find abc:"<<str.find_last_of(fl, string::npos)<<nel;//(char*,pos)
    cout<<"find b:"<<str.find_last_of('b', string::npos)<<nel;
    

    system("pause");
    return 0;
}