-module(connection).

-export ([search_in/1]).

search_in([Key, Type]) ->
    {Key,Type}.
    %А как собственно ему писать то...
    %rpc:call('node2@127.0.0.1', phone, search, [Key,Type]).
