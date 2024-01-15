## Description

This is a simple lamdba calculus inspired language, in order to see how useful it can be for general purpose computing. 

The language right now supports functions that can take in integer or boolean input and evaluate lamda expressions based on semantics described in references [1]. 

Keywords:
1. `lamda`: this starts the definition for lamda function, `lamda(x) {x}` is an identity function.
2. `apply`: this is for function application, for example `apply(lamda(x) {x}, 3)`, calls the identity function with 3 as parameter, and evaluates to 3.
3. Operations: Right now the traditional math operations can be made `op(1, 2)`, for example `+(1,2)` evaluates to 3. Only +, -, /, * are supported. =, Operation evaluates to either true or false
4. `if then else`

Examples:

1. Identity function

`lamda(x){x}`

2. Cumulative sum computation

`let fib = cumulate(x) {if(=(x, 1)) then {1} else {+(x, apply(cumulate, -(x, 1)))}} in apply(cumulate, 10)`


## References
[1]https://www.csa.iisc.ac.in/~raghavan/CleanedPav2011/stlc.pdf

