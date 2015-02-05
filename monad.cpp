
#include <stdio.h>

#include "state.h"
#include "maybe.h"

int main( int argc, char* argv[] )
{
    {
        // First, define some very simple worker lambdas. Note: they take and return the same type.

        auto hello = [] ( auto s ) { fprintf( s, "Hello " ); return s; };
        auto world = [] ( auto s ) { fprintf( s, "World!" ); return s; };
        auto newline = [] ( auto s ) { fprintf( s, "\n" ); return s; };

        // terminal_state(stdout);           When terminal returns, it returns a lambda that it wants to pass stdout
        //                                   to.
        //
        // terminal_state(stdout)();         By putting some parens on the end, I can invoke that lambda. BUT the
        //                                   lambda that terminal returns TAKES a lambda as its argument.
        //
        // terminal_state(stdout)
        //   (fmap_state(hello))             Luckily, invoking fmap() returns such a lambda.
        //
        //                                   Internally, fmap() captures its argument lambda and returns a lambda. This
        //                                   lambda wants as an argument the argument to fmap()'s argument lambda
        //                                   (stdout), which is convenient because that is exactly what the lambda
        //                                   returned by terminal wants to give it.
        //

        terminal_state(stdout)
            (fmap_state(hello))
            (fmap_state(world))
            (fmap_state(newline));
    }

    {
        auto plus_2 = [] ( auto arg ) { return arg + 2; };
        auto minus_2 = [] ( auto arg ) { printf("IN MINUS 2\n"); return arg - 2; };
        auto failer = [] ( auto arg ) -> int { throw -1; };

        auto result = terminal_maybe(just(40))
            (fmap_maybe( plus_2 ))
            (fmap_maybe( plus_2 ))
            (fmap_maybe( plus_2 ))
            (fmap_maybe( plus_2 ))
            (fmap_maybe( failer ))
            (fmap_maybe( minus_2 ))
            (unwrap);

        printf("result = %d\n",result.argument);
    }

    return 0;
}
