#ifdef OOO_C
typedef void( *FUN )( );
typedef struct This {
    FUN con;
    FUN des;
    FUN set;
}This;

#endif

void donothing( );