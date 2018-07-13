-module(my_ackerman).

-export([ackerman/2]).

%%if you run it with m = 4, and n = 4,
%%then you can boil some water on your laptop 

ackerman(0, N) ->
    N + 1;
ackerman(M, 0) when M > 0 ->
    ackerman(M - 1, 1);
ackerman(M, N) when M > 0, N > 0 ->
    ackerman(M - 1, ackerman(M, N - 1)).
