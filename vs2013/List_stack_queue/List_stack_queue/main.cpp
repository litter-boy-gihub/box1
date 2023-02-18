#include<cstdlib>

template<typename T>
struct Data {
    T data;
    Data* next;
    Data* prev;
    Data( T t ) {
        data = t;
    }
};

template<typename T>
struct List {
    Data<T>* head;
    unsigned int size;
};

template<typename T>
List<T>* initList(List<T>& list) {
    list.head = new Data<T>(0);
    list.head->prev = nullptr;
    list.head->next = nullptr;
    list.size = 0;
    return &list;
}

template<typename T>
void clearList( List<T>& list ) {
    Data<T> *temp,*temp_head = list.head->next;
    while (temp_head != nullptr)
    {
        temp = temp_head->next;
        delete temp_head;
        temp_head = temp;
    }
    list.head = nullptr;
    list.size = 0;
}

template<typename T>
void deleteList( List<T>& list ) {
    clearList( list );//清除只剩下头结点和对象
    delete list.head;
    list.head = nullptr;
    delete &list;
}


template<typename T>
void removeList(List<T>& list, Data<T> *item ) {
    item->prev->next = item->next;
    if(item->next != nullptr ) item->next->prev = item->prev;
    delete item;
    list.size--;
}

/*
*前插 ，需要list.size > 1
*/
template<typename T>
bool insertList( List<T>& list,Data<T> * index,Data<T> * data) {
    if( index == nullptr || data == nullptr ) return false;
    data->next = index;
    data->prev = index->prev;
    index->prev->next = data;
    index->prev = data;
    list.size++;
    return true;
}

template<typename T>
bool pushbackList( List<T>& list , Data<T> * data) {
    if(data == nullptr ) return false;
    Data<T> *temp = list.head;
    while (temp->next != nullptr)
    {
        temp = temp->next;
    }
    data->next = temp->next;
    temp->next = data;
    data->prev = temp;
    list.size++;
    return true;
}

template<typename T>
Data<T>* findList( List<T>& list , int index ) {
    if( index <0 || index > (list.size - 1) ) {
        return nullptr;
    }
    Data<T> * temp , *temp_head = list.head->next;
    int temp_index = 0;
    while( temp_head != nullptr ) {
        temp = temp_head->next;
        if( temp_index == index ) {
            return temp_head;
        }
        temp_index++;
        temp_head = temp;
    }
    return nullptr;
}

template<typename T>
Data<T>* findList( List<T>& list,Data<T> data) {
    Data<T> * temp , * temp_head = list.head->next;
    while( temp_head != nullptr ) {
        temp = temp_head.next;
        if (temp_head->data == data){
            return temp_head;
        }
        temp_head = temp;
    }
    return nullptr;
}
//////////////////////////////////////////////////////////////
//队列的实现Queue ,限定从头删除，从尾部插入
template<typename T>
class Que {
public:
    Que() {
        list = new List<T>;
        initList( *list );
    };
    ~Que( ) {
        deleteList( *list );
    }
    bool pop_front() {
        if( size == 0 )  return false;
        removeList( *list , findList( *list , 0 ) );
        size--;
        return true;
    }
    bool push_back(T t) {
        if( size == max_size ) return false;
        Data<T> *ptr = new Data<T>(T());
        ptr->data = t;
        pushbackList( *list , ptr );
        size++;
        return true;
    }
    T front( ) {
        if( size == 0 ) throw size;
        Data<T> * ret = findList( *list , 0 );
        return ret->data;
    }

    bool setMaxSize( unsigned int size) {
        this->max_size = size;
    }
private:
    List<T> *list;
    unsigned int size = 0;
    unsigned int max_size = 128;
};

//////////////////////////////////////////////////////////////
//堆栈的实现，只能从堆栈顶插入删除
template<typename T>
class Stack {
public:
    Stack( ) {
        list = new List<T>;
        initList( *list );
    };
    ~Stack( ) {
        deleteList( *list );
    };
    bool push(T t) {
        Data<T> *ptr = new Data<T>(T());
        ptr->data = t;
        pushbackList( *list , ptr );
        size++;
        return true;
    }
    bool pop( ) {
        if( size == 0 ) return false;
        removeList( *list , findList( *list , size - 1 ) );
        size--;
        return true;
    }
    T top( ) {
        if( size == 0 ) throw size;
        Data<T> *ptr = findList( *list , size - 1 );
        return ptr->data;
    }
private:
    List<T>* list;
    unsigned int size = 0;
    unsigned int max_size = 128;
};

//////////////////////////////////////////////////////////////



int main(){
    List<int>* list = initList( *new List<int> );
    pushbackList( *list , new Data<int>(0) );
    pushbackList( *list , new Data<int>( 1 ) );
    pushbackList( *list , new Data<int>( 2 ) );
    pushbackList( *list , new Data<int>( 3 ) );
    removeList( *list , findList( *list , 3 ) );
    insertList( *list , findList( *list , 2 ) , new Data<int>( 5 ) );
    deleteList( *list );
    
    Que<int> queue;
    queue.push_back( 1 );
    queue.push_back( 2 );
    int num = queue.front( );
    queue.pop_front( );

    //
    Stack<int> stack;
    stack.push( 1 );
    stack.push( 2 );
    stack.push( 3 );
    stack.push( 4 );
    num = stack.top( );
    stack.pop( );
    system( "pause" );
	return 0;
}