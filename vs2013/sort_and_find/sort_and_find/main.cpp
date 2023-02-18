/*
二分查找算法，冒泡排序，选择排序，插入排序，二分插入排序，希尔排序
*/

#include <cstdlib>
#include<math.h>
#include<algorithm>

//二分查找的前提是有序序列
template<typename T>
unsigned int binaray_search(T t,T* arr,unsigned int size) {
    unsigned int beg = 0,end = size-1,mid;
    bool find = false;
    while (beg<=end)
    {
        mid = floor((beg + end) / 2.0);
        if( arr [mid] == t ) return mid;
        else if( arr [mid] > t ) end = mid - 1;
        else if( arr [mid] < t ) beg = mid + 1;
    }
    return -1;
}

//冒泡排序
template<typename T>
bool bubble_sort( T* arr , unsigned int size ) {
    if( arr == nullptr || size < 2) return false;
    unsigned int s = size;
    for (int i = s-1; s > 0 ; s-- )
    {
        for (int j = 0; j < i ; j++)
        {
            if( arr [j] > arr [j + 1] ) std::swap( arr [j] , arr [j + 1] );
        }	
    }
    return true;
}

//选择排序
template<typename T>
bool select_sort( T* arr , unsigned int size ) {
    if( arr == nullptr || size < 2) return false;
    int min;
    for( int i = 0; i < ( size - 1 ); i++ )
    {   
        min = i;
        for( int j = i; j < size; j++ )
    	{
            if( arr [j] < arr [min] ) min = j;
    	}
        std::swap( arr [i] , arr [min] );
    }
    return true;
}

//插入排序
template<typename T>
bool insert_sort( T* arr , unsigned int size ) {
    int already_sorted_num = 1;
    T temp;
    for( int i = 1; i < size; i++ )
    {
        if( arr[i] >= arr[i-1] ) continue;
        for( int j = 0; j < already_sorted_num; j++ )
        {
            if( arr [i] <= arr [j] ) {
                temp = arr [i];
                for( int k = i; k > j ; k-- )
                {
                    std::swap( arr [k] , arr [k - 1] );
                }
                std::swap( temp , arr [j] );
                break;
            }
        }
           	
    }
    return true;
}


//二分插入排序
template<typename T>
bool binaray_insert_sort( T* arr , unsigned int size ) {
    if( arr == nullptr || size < 2 ) return false;
    int beg,end,mid; //mid是第一个小于的位置
    T temp;
    for( int i = 1; i < size; i++ ) {
        if( arr [i] >= arr [i - 1] ) continue;
        
        beg = 0 , end = i - 1;
        while (beg<=end)
        {
            if( beg == end && beg == 0 ) {
                mid = 0;
            }
            else {
                mid = floor( ( beg + end ) / 2.0 );
            }
            bool than_before = true;
            if( mid > beg ) {
                if( arr [i] <= arr [mid - 1] ) than_before = false;
            }
            if( arr [i] <= arr [mid] && than_before) {
                temp = arr [i];
                for( int k = i; k > mid; k-- ) {
                    std::swap( arr [k] , arr [k - 1] );
                }
                std::swap( temp , arr [mid] );
                break;
            }
            else if( !than_before ) {
                end = mid - 1; 
            }
            else { 
                beg = mid + 1; 
            }
        }
    }
    return true;
}


//分组二分插入排序
//n 分组长度
template<typename T>
bool group_binaray_insert_sort( T* arr,int n,int size) {
    unsigned int group_size = 0;
    for (int i = 0;; i++)
    {
        if( ( i * n ) < size ) {
            group_size++;
        }
        else {
            break;
        }
    }
    if( arr == nullptr || group_size < 2 ) return false;
    int beg , end , mid; //mid是第一个小于的位置
    T temp;
    for( int i = n; i < size; i +=n ) {
        if( arr [i] >= arr [i - n] ) continue;

        beg = 0 , end = i - n;
        while( beg <= end ) {
            if( beg == end && beg == 0 ) {
                mid = 0;
            }
            else {
                mid = n * floor(floor( ( beg + end ) / 2.0 ) / float(n) ) ; //注意这里
            }
            bool than_before = true;
            if( mid > beg ) {
                if( arr [i] <= arr [mid - n] ) than_before = false;
            }
            if( arr [i] <= arr [mid] && than_before ) {
                temp = arr [i];
                for( int k = i; k > mid; k -= n ) {
                    std::swap( arr [k] , arr [k - n] );
                }
                std::swap( temp , arr [mid] );
                break;
            }
            else if( !than_before ) {
                end = mid - n;
            }
            else {
                beg = mid + n;
            }
        }
    }
    return true;
}

//希尔排序
template<typename T>
bool shell_sort( T* arr , unsigned int size ) {
    if( arr == nullptr || size < 2 ) return false;
    int group_size = floor( size / 2.0 );
    while( group_size >= 1 ) {
        group_binaray_insert_sort( arr , group_size , size );
        if( group_size == 1 ) {
            group_size = 0;
        }
        else {
            group_size = floor( group_size / 2.0 );
        }
    }
    return true;
}


int main(){
    int arr[] {9,8,5,2,1};
    unsigned int index = binaray_search<int>( 8 , arr , 5 );
    //bool b = bubble_sort<int>( arr , 5 );

    //bool b = select_sort( arr , 5 );
    
    //bool b = insert_sort( arr , 5 );

    //bool b = binaray_insert_sort( arr , 5 );
      
    //bool b = group_binaray_insert_sort( arr , 2 , 5 );

    bool b = shell_sort( arr , 5 );

    return 0;
}