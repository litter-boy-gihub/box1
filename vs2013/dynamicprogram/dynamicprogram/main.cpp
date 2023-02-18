//��̬�滮�˻ʺ�����

#include <cstdlib>
#include <iostream>
#include <iomanip>

int col [8]{0};
int left_slide [15]{0};
int right_slide [15]{0};
int row_result [8]{0};
int sum = 0;

void print_line( int n ) {
    char line [8];
    memset( line , '*' , 8 );
    line [n] = '0';
    for (int i = 0; i < 8 ; i++)
    {
       
        std::cout<<std::setw(3)<<line [i];
    }
}

void print_rect( ) {
    for (int i = 0; i < 8 ; i++)
    {
        print_line( row_result [i] );
        std::cout << std::endl;
    }
}

void recursive( int n ) {
    for( int i = 0; i < 8;i++ )
    {   
        //�ɷ�
        if( col [i] == 0 && left_slide[n+i] == 0 && right_slide[n-i+7] == 0) {
            //�޸�����
            col [i] = 1;
            left_slide [n + i] = 1;
            right_slide [n - i + 7] = 1;
            row_result [n] = i;
            if( n == 7 ) {
                //�������㣬��¼
                sum++;
                std::cout << "[ ";
                for (int it = 0; it < 7 ; it++)
                {
                    std::cout << row_result [it] << " , ";
                }
                std::cout << row_result [7] << " ]\n";
                print_rect( );
            }
            //�ݹ���һ��
            recursive( n + 1 );
            //��������
            col [i] = 0;
            left_slide [n + i] = 0;
            right_slide [n - i + 7] = 0;
        }
        else continue;
    }
    return;
}




int main(){
    recursive( 0 );
    sum;
    std::cout << "�˻ʺ�����ⷨ��" << sum << "��" << std::endl;
    system( "pause" );
	return 0;
}