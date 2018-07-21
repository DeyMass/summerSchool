-module(receiving).

-export ([start/0]).

start() ->
    receive
        Message ->
            Message,
            start()
    end.
