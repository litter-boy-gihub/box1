#include "observer.h"
#include "subject.h"

int main(){
    Observer ob;
    Subject sub( 1 );
    sub.attach(&ob);
    sub.doSome( );
	return 0;
}