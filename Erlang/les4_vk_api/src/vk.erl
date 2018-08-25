%%%-------------------------------------------------------------------
%%% @author michail
%%% @copyright (C) 2018, <COMPANY>
%%% @doc
%%%
%%% @end
%%% Created : 03. Aug 2018 16:24
%%%-------------------------------------------------------------------
-module(vk).
-author("michail").

%% API
-export([connectvk/2]).
-export([get_friends/1]).


recursiveWalk([], _, _, _) ->
    gb_sets:empty();

recursiveWalk(_, -1, _, _) ->
    gb_sets:empty();


recursiveWalk([H|_T], 0, ResSet, Dst) ->
    io:format("~p ~p  H == Dst (~p) ~n ", [H, Dst, H == Dst]),
    case H == Dst of
        true  ->
            gb_sets:add_element(H, ResSet);
        false ->
            case H > Dst of
                false ->
                    recursiveWalk(_T, 0, ResSet, Dst);
                true ->
                    gb_sets:empty()
            end
    end;

recursiveWalk([H|T], Depth, ResSet, Dst) ->
    EmptySet = gb_sets:empty(),
    case H == Dst of
        true  ->
            gb_sets:add_element(H, ResSet);
        false ->
            Result = recursiveWalk(T, Depth, ResSet, Dst),
            case Result == EmptySet of
                true ->
                    case gb_sets:is_element(H, ResSet) of
                        false ->
                            NewSet = gb_sets:add_element(H, ResSet),
                            Parsed = get_friends(H),
                            recursiveWalk(Parsed, Depth - 1, NewSet, Dst);
                        _ ->
                            Result
                    end;
                _ ->
                    Result
            end
    end.

get_friends(Uid) ->
    io:format("request for user# ~p~n", [Uid]),
    Code = httpc:request(
        lists:flatten(
            io_lib:format("~s~s~s",
                ["https://api.vk.com/method/friends.get?user_id=",
                io_lib:format("~p", [Uid]),

                "&access_token=" ++ ?ACC_TOK ++ "&v=5.80"]))),
    case Code of
        {ok, {_, _, Result}} ->
            Answer = jiffy:decode(Result),
            case Answer of
                {[{_,{[{_,_},{_, BinList}]}}]} -> BinList;
                _ ->
                    []
            end;
        _ ->
            io:format("socket error"),
            get_friends(Uid)
    end.


connectvk(Uid, Did) ->
    connectvk(Uid, Did, 1).

connectvk(Uid, Did, Hop) ->
    ssl:start(),
    inets:start(),
    List = get_friends(Uid),
    Result = gb_sets:new(),
    recursiveWalk(List, 6, gb_sets:add_element(Uid, Result), , Did).
