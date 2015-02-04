
#ifndef __monad_maybe_h
#define __monad_maybe_h

template<class T>
struct maybe
{
    maybe( const T& t ) : argument( t ), valid( true ) {}
    maybe() : argument(), valid( false ) {}

    T argument;
    bool valid;
};

template<class T>
maybe<T> just( const T& t ) { return maybe<T>(t); }

template<class T>
maybe<T> nothing() { return maybe<T>(); }

auto terminal_maybe = [] ( auto term ) {
    return [=] ( auto func ) {
        return func( term );
    };
};

auto fmap_maybe = [] ( auto f ) {
    return [=] ( auto t ) {
        if( t.valid ) {
            try {
                t.argument = f( t.argument );
            }
            catch(...) {
                t.valid = false;
            }
        }

        return (t.valid) ? terminal_maybe( just( t.argument ) ) : terminal_maybe( nothing<decltype(t.argument)>() );
    };
};

auto unwrap = []( auto const & f ) {
    return f;
};

#endif
