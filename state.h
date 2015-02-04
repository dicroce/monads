
#ifndef __monad_state_h
#define __monad_state_h

// terminal is a lambda that takes an argument, and returns another lambda that captures that argument. This innner
// lambda takes a function as an argument and passes the captured argument to it and returns its result.
auto terminal_state = [] ( auto term ) {
    return [=] ( auto func ) {
        return func( term );
    };
};

// fmap is a lambda that takes a lambda as an argument and returns a lambda that captures that argument. This inner
// lambda takes an argument that it passes to the outer lambdas lambda... Then it calls terminal on the result and
// returns its result.
auto fmap_state = [] ( auto f ) {
    return [=] ( auto t ) {
        return terminal_state( f( t ) );  // internally, using terminal here allows calls to fmap to be chained.
    };
};

#endif
