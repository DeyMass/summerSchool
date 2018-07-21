-module(my_reverse).

-export([reverse/1]).

%%reverses List

reverse([H]) ->
    [H];
reverse([H|T]) ->
    reverse(T) ++ [H];
reverse([]) ->
    [].
