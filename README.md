# monads in c++14

####What is "purity" and why should I care?

Consider the following function:
```c++
auto squared = [] ( int a ) -> int {
  return a * a;
};
```
Here we use some fairly new syntax to define a lambda function that squares and returns its results. What can go wrong with this function? The only thing that comes to mind immediately is numeric overflow. Compared to many functions I've written this is a pretty simple function that should almost always work. Now consider this one:
```c++
int ghost = 0;

auto squared = [] ( int a ) -> int {
  return (a * a) + ghost;
};
```
Whether this function works properly is determined by whether "ghost" is 0. If "ghost" is any other value, this lambda will not square it's inputs.

OK, I admit it: this is a contrived example. In real life your probably a much better programmer than the type who would use a global variable in such a horrific way. But have you ever written an object member function that failed because it encountered a member variable with an unexpected value?

This line of thinking leads to an interesting thought: Perhaps the results of functions we write should only depend on their arguments? Our language designers were good enough to create languages that allow us to specify precisely what arguments our functions should take... and quite often we subvert this by passing conglomerate objects that allow our functions to access a whole bunch of state. Perhaps all of our object oriented programming languages would have been better off had they forced us to enumerate precisely what data members a member function needs access to.

A function is pure if its result depends only on its arguments.

###Functional Vs Imperative
An imperative program is one where you tell the computer to perform a series of steps:
```
1)  A = GetA();
2)  B.ProcessA(A);
3)  C = B.CreateC();
```
The order of instructions is very important. You obviously cannot call B.ProcessA(A) if you havent yet called GetA(). Line 3 is a mystery however: it might or might not work if it had been executed before line 2 (it depends on how B is implemented... Does CreateC() depend on some state acquired while ProcessA() runs?).
In a sense, our language allowing us to do these steps as separate statements has opened us up to a range of possible problems. What if our language didn't allow code like the above? What if our language required our functions to consist of only a single statement, but provided a syntax that allowed for explicitly composing a series of function calls?

###Q: What is a monad?
A monad is a design pattern (that comes in a few different flavors) that allows you to model computation as a sequence of steps. **A Monad allows you define what happens in between a series of function calls**. It's as if our language allows us to overload ; and provide whatever glue code is required between two statements.

###Q: Why are there different types of monads?
Because you sometimes want different behaviour between your steps. For example, the maybe monad allows you to short circuit the rest of the sequence if a step fails. It is called maybe because when complete it might or might not hold the result your interested in.

This code requires a compiler that supports C++14. I tested it with clang 3.4.2.

To compile type: clang++ --std=c++1y monad.cpp -omonad
