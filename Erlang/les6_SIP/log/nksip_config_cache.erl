-module(nksip_config_cache).

-compile([export_all]).

sync_call_time() -> 30000.

msg_routers() -> 16.

max_calls() -> 100000.

sip_defaults() -> [].

re_content_length() ->
    {re_pattern, 2, 0, 0,
     <<69, 82, 67, 80, 140, 0, 0, 0, 1, 0, 0, 0, 81, 8, 0, 0,
       255, 255, 255, 255, 255, 255, 255, 255, 13, 0, 10, 0, 0,
       0, 2, 0, 0, 0, 64, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 125,
       0, 72, 30, 13, 30, 10, 46, 32, 127, 0, 7, 0, 1, 30, 108,
       113, 0, 31, 30, 99, 30, 111, 30, 110, 30, 116, 30, 101,
       30, 110, 30, 116, 30, 45, 30, 108, 30, 101, 30, 110, 30,
       103, 30, 116, 30, 104, 114, 0, 38, 55, 32, 30, 58, 46,
       32, 127, 0, 7, 0, 2, 86, 12, 114, 0, 7, 55, 32, 30, 13,
       30, 10, 114, 0, 72, 0>>}.

re_call_id() ->
    {re_pattern, 2, 0, 0,
     <<69, 82, 67, 80, 126, 0, 0, 0, 1, 0, 0, 0, 81, 8, 0, 0,
       255, 255, 255, 255, 255, 255, 255, 255, 13, 0, 10, 0, 0,
       0, 2, 0, 0, 0, 64, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 125,
       0, 58, 30, 13, 30, 10, 46, 32, 127, 0, 7, 0, 1, 30, 105,
       113, 0, 17, 30, 99, 30, 97, 30, 108, 30, 108, 30, 45,
       30, 105, 30, 100, 114, 0, 24, 55, 32, 30, 58, 46, 32,
       127, 0, 7, 0, 2, 86, 12, 114, 0, 7, 55, 32, 30, 13, 30,
       10, 114, 0, 58, 0>>}.

global_id() ->
    <<50, 81, 115, 115, 117, 118, 90, 85, 50, 101, 105, 117,
      81, 99, 79, 73, 97, 107, 114, 104, 120, 73, 97, 54, 57,
      90, 102>>.

