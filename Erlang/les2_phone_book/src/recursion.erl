-module (recursion).

-export ([fact/1]).
-export ([fact_tail/2]).
-export ([fact_rec/1]).
-import (timer, [tc/2]).

fact_tail(Acc, 1) -> Acc;
fact_tail(Acc, N) when N >= 0 ->
    fact_tail(Acc*N, N-1);
fact_tail(_, N) when N < 0 ->
    io:format("Wrong value").

fact_rec(1) ->
    1;
fact_rec(N) ->
    fact_rec(N-1) * N.

fact(N) ->
    {R1, _} = timer:tc(fact_rec, [N]),
    {R2, _} = timer:tc(fact_tail, [1, N]),
    {R1, R2}.
