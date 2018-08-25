-module(caller).

-export([work/0]).

work() ->
    nksip:start(test, #{sip_listen=>"sip:all, sips:all"}).



