-module(my_sum).

-record(user, {name, money}).

-export([countsum/1]).

%%finds sum of money from all records, that gets in input as list
%%input similiar to [#user{name=<name>, money=<money>}]

countsum([], Sum) ->
    Sum;
countsum([H|T], Sum) ->
    countsum(T, Sum + H#user.money).

%user friendly interface
countsum(List) ->
    countsum(List, 0).
