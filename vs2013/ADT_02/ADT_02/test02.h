#ifdef TEST02_C
typedef void( *FUN )( );
typedef struct This {
    FUN con;
    FUN des;
}This;

#endif