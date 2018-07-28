-module(fsm).

-export([messageHandler/0]).
-export([fsm_start/0]).

%%Message handler, which prints messages, received by FSM
messageHandler() ->
	receive 
		{Type, Reason} ->
			case Type of
				error ->
					io:format("error because ~s~n",[Reason]),
					messageHandler();
				info ->
					io:format("info: ~s~n", [Reason]),
					messageHandler();
				_ ->
					io:format("warning: ~s~n", [Reason]),
					messageHandler()
				end
	end.

fsm_start() ->
	MsgHandler = spawn(fsm, messageHandler, []),
	fsm(initiate, MsgHandler, [], 0).

%%after initialization starts reading port #7777
%%after receiving message sends to MessageHandler
fsm(State, MsgHandler, List, Socket) ->
    case State of
		initiate ->
			%%%TCP
			{Status, ListenSocket} = gen_tcp:listen(7777, [{active, false}]),
			{Status, Sock} = gen_tcp:accept(ListenSocket),
			%%%/TCP
			%%%UDP
			%{Status, Sock} = gen_udp:open(7777, [{active, false}]),
			%%%/UDP
			case Status of
				ok ->
					MsgHandler ! {info, "socket initiated successfully"};
				error ->
					MsgHandler ! {error, "ERROR OCCURED"};
				_ ->
					MsgHandler ! {warning, "WARNING! SOMETHING WRONG"}
				end,
			{ok, File} = file:open("base.txt",[read]),
			case file:read_line(File) of
				{ok, Data} ->
					fsm(work, MsgHandler, Data, Sock);
				{Status, Reason}  ->
					MsgHandler ! {error, Reason}
			end;
		work ->
			%%%UDP
			%DrawBack = gen_udp:recv(Socket, 256),

			%%%TCP
			DrawBack = gen_tcp:recv(Socket, 0),
			case DrawBack of
				%{_, {ClientAddr, ClientPort, Data}} ->
				{ok, Data} ->
					MsgHandler ! {info, "received"},
					MsgHandler ! {info, Data},
					%%%UDP
					gen_tcp:send(Socket, List),
					%%%TCP
					%gen_udp:send(Socket, ClientAddr, ClientPort, List),

					fsm(work, MsgHandler, List, Socket);
				{error, Stat} ->
					MsgHandler ! {error, Stat}
			end
    end.
