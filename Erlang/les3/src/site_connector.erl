-module(site_connector).

-export([parse/4]).

%%carefull, there is no error checking! 
%%Cuz i'm bad and lazy programmer

%%expects call like 
%%parse({X,X,X,X}, "http://host/path", "host", "filename"
parse(Ip, Site, Host, File) ->
	{ok, Soc} = gen_tcp:connect(Ip, 80, [{active, false}]),
	gen_tcp:send(Soc, "GET " ++ Site ++ "\r\nHost: " ++ Host),
	{ok, Text} = gen_tcp:recv(Soc, 0),
	{ok, Fd} = file:open(File, [write, binary]),
	file:write(Fd, Text).
