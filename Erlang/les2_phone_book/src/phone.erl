-module(phone).

-record(user, {num, name, phone}).

-export ([quicksort_tail/1]).
-export([search/2]).
-export([book_read/0]).

translate([Num, Name, Phone]) ->
    {Num, Name, Phone}.

parseData(Data) ->
    translate(string:tokens(Data, " ")).

%%reads data stored in 'File' and converts it into a list
%%that looks like [{Num, Name, Phone}, {Num, Name, Phone}, ...]
read_raw(File, List) ->
    case file:read_line(File) of
        {ok, Data} ->
            read_raw(File, List ++ [parseData(Data -- "\n")]);
        eof ->
            List
        end.

%%Returns selected position in list of 3 elements...
%%I need a doctor...
getVal([_,_,Phone], 3) ->
    Phone;
getVal([_,Name,_], 2) ->
    Name;
getVal([Num,_,_], 1) ->
    Num.

%%sorts list like [{1,2,3}, {3,4,5}, {5,6,7}] by field number 2
quicksort([]) -> [];
quicksort([H|Tail]) ->
    quicksort([X || X <- Tail, getVal(erlang:tuple_to_list(X), 2)
        < getVal(erlang:tuple_to_list(H), 2)])
    ++ [H] ++
   quicksort([X || X <- Tail, getVal(erlang:tuple_to_list(X), 2)
        >= getVal(erlang:tuple_to_list(H), 2)]).

%%sorts only lists similiar to [1,2,3,4,5,6...]
quicksort_tail([]) ->
    [];
quicksort_tail([H|Tail]) ->
    {Small, Great} = quicksort_tail(H, [], [], Tail),
    Smaller = quicksort_tail(Small),
    Greater = quicksort_tail(Great),
    Smaller ++ [H] ++ Greater.

quicksort_tail(_, Small, Great, []) ->
    {Small, Great};
quicksort_tail(H, Small, Great, [First|Rem]) when H > First ->
    quicksort_tail(H, [First | Small], Great, Rem);
quicksort_tail(H, Small, Great, [First|Rem]) when H =< First ->
    quicksort_tail(H, Small, [First|Great], Rem).


%%shows records from 'raw_book.txt' text file
book_read() ->
    {ok, File} = file:open("raw_book.txt", [read]),
    read_raw(File, []).

search(Value, Type) ->
    List = book_read(),
    search(List, Value, Type).

search([TupleList | T], Key, Type) ->
    List = erlang:tuple_to_list(TupleList),
    case Type of
        phone ->
            ValPos = 3;
        name ->
            ValPos = 2;
        num ->
            ValPos = 1;
        _ ->
            ValPos = 1,
            io:format("Unknown type~n")
        end,
    Condition = getVal(List, ValPos),
    case Condition of
        Key ->
            List;
        _ ->
            search(T, Key, Type)
    end;
search([],_,_) -> io:format("NotFound ~n").
