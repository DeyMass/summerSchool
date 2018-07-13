-module(my_pow).

-export([mypow/2]).

%%my_pow is equivalent to x^a function

mypow(Res, _, 0)->
    Res;
mypow(Res, Base, Exp) when Exp > 0 ->
    mypow(Res*Base, Base, Exp-1).


%%user friendly interface
mypow(_, 0) ->
    1;
mypow(Base, Exp) when Exp > 0 ->
    mypow(Base, Base, Exp-1);
mypow(Base, Exp) when Exp < 0 ->
    1 / mypow(Base, Base, -(Exp + 1)).
