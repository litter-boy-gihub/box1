#include<iostream>
using namespace std;
void prt( char** arr ) {
    for (int i = 0; i < 8 ; i++)
    {
    	for (int j = 0; j < 8 ; j++)
    	{
            cout << arr[i][j]<<" ";
    	}
        cout << endl;
    }
}



void prt( char** arr,int *ret) {
    for (int i = 0; i < 8 ; i++)
    {
    	for (int j = 0; j < 8 ; j++)
    	{
            if( j == ret [i] ) {
                cout << "+";
                continue;
            }
            cout << "*";

    	}
        cout << endl;
    }
}

int main( ) {
    char** arr;
    arr = new char*[8];
    for (int i = 0; i < 8 ; i++)
    {
        arr [i] = new char [8];
    }
   


    bool exist_c [8]{0};
    bool exist_l [15]{0};
    bool exist_r [15]{0};
    int count;
    int ret [8]{-1,-1,-1,-1,-1,-1,-1,-1};//�����������
    bool col_f [8]{};
    for (int i = 0; i < 8 ; i++)
    {//��
        count = 0;
        for( int j = 0; j < 8; j++ ) //��
        {
            for( int k = 0; k < 8; k++ )//������ѡһ��
            {
                //�ж��Ƿ�Ϸ�����
                if( !exist_c [j + k] && !exist_l [j + k] && !exist_r [j + k] ) {
                    ret [j] =  k;//j�����ѡ��k
                    exist_c [k]= true;
                    exist_l [j + k] = true;
                    exist_r [j + k] = true;
                    count++;
                    if( count == 8 ) prt( arr , ret );
                }
                else continue;//���Ϸ�ѭ������
                 
            }
            //��ѭ������  �����ϲ�
            
            
            if( j > 0 ) {
                if( ret [j] != -1 ) {
                    count -= 2;
                    //������־
                    exist_c [j] = false;
                    exist_l [j + ret [j]] = false;
                    exist_r [j + ret [j]] = false;
                    exist_c [j-1] = false;
                    exist_l [j-1 + ret [j-1]] = false;
                    exist_r [j-1 + ret [j-1]] = false;
                    ret [j] = -1;
                    ret [j - 1] = -1;
                }
                else {
                    count--;
                    //������־
                    exist_c [j - 1] = false;
                    exist_l [j - 1 + ret [j - 1]] = false;
                    exist_r [j - 1 + ret [j - 1]] = false;
                    ret [j - 1] = -1;
                }
                    
            }
            
        }

    }
    

    system( "pause" );
    return 0;
}