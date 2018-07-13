-module(my_replace).

-export([replacethird/1]).

%%replaces third element in list

replacethird([H1,H2,_|T])->
    [H1, H2, -1 | replacethird(T)];
replacethird(List)->
    List.
