#include<iostream>
#include<direct.h>
#include"print.h"
#include"prtdll.h"

#pragma comment(lib,"jttDll.lib");

int __stdcall fun1();
int _stdcall fun2();

constexpr 

int main(){
    

    

    /*char* buf = nullptr;
    if ((buf = _getcwd(NULL, 0)) == NULL){
        std::cout << "error" << std::endl;
    }
    else{
        std::cout << "cwd: " << buf << std::endl;
        free(buf);
    }*/

    std::cout << "using dll for mul:" << mul(2, 3) << std::endl;
    print();
    getchar();
    return 0;
}