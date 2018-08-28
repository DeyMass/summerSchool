-module(caller).

-export([work/0]).

work() ->
    nksip:start(client1, #{
        sip_from => "sip:1010@sauron.domain",
	plugins => [nksip_uac_auto_auth],
        sip_listen => "sip:all, sips:all"
    }),
    nksip_uac:register(client1, "sip:192.168.2.81", [{sip_pass, "111"}, contact, {supported, ""}]),
	nksip_uac:invite(client1, "sip:enp2s0f2@sauron.domain", [{route, "<sips:192.168.2.81>"}, [auto_2xx_ack], {body, nksip_sdp:new()}]).

