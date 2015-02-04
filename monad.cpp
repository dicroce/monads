
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

        // terminal(stdout);                 When terminal returns, it returns a lambda that it wants to pass stdout
        //                                   to.
        //
        // terminal(stdout)();               By putting some parens on the end, I can invoke that lambda. BUT the
        //                                   lambda that terminal returns TAKES a lambda as its argument.
        //
        // terminal(stdout)(fmap(hello))     Luckily, invoking fmap() returns such a lambda.
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

        // So, what has all this accomplished? Well, we have a new way to write code that models computation as a
        // sequence of steps to be performed one after the other. What we have above is a monad that threads state (in
        // the above case that state is stdout) through a sequence of steps.
        //
        // Why not simply do something like this:
        //
        //     int arg = 42;
        //     arg = func1( arg );
        //     arg = func2( arg );
        //     arg = func3( arg );
        //
        // One reason that comes to mind is that the above is open to corruption because there is nothing stopping the
        // programmer from inserting a line of code between calls to funcs() that mutates arg. This is literally
        // impossible with our monadic code above because the chained calls are a single statement.
        //
        // Is that it? Are monads simply about threading state?
        //
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
