#include "subject.h"
Subject::Subject( int n ) { m_n = n; };
Subject::~Subject( ) { };
void Subject::attach( IObserver* ob ) {
    this->observers_.push_back( ob );
};
void Subject::detach( IObserver* ob ) {
    auto it = std::find( this->observers_.begin( ) , this->observers_.end( ) , ob );
    this->observers_.erase( it );
};
void Subject::notify( ) {
    for( auto i : this->observers_ ) {
        i->update( );
    }
};