-module(gens).

-behaviour(gen_server).

-export([start_link/0, init/1, call/0, handle_call/3]).

start_link() ->
    gen_server:start_link({global, ?MODULE}, ?MODULE, [], []).

call() ->
    gen_server:call({global, ?MODULE}, {say_hello}).

init(_) ->
    io:format("PRIVET", []),
    {ok, work}.

handle_call({say_hello}, _, State) ->
    {reply, "hello", State}.

handle_cast({_, State}) ->
    {noreply, State}.