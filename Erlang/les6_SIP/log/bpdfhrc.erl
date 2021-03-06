-module(bpdfhrc).

-compile([export_all]).

nks_sip_transport_uas_sent(A1) ->
    nksip_callbacks:nks_sip_transport_uas_sent(A1).

nks_sip_uac_pre_response(A1, B1, C1) ->
    nksip_callbacks:nks_sip_uac_pre_response(A1, B1, C1).

sip_dialog_update(A1, B1, C1) ->
    nksip_callbacks:sip_dialog_update(A1, B1, C1).

sip_invite(A2, B2) ->
    case nksip_tutorial_client_callbacks:sip_invite(A2, B2)
	of
      continue ->
	  [A1, B1] = [A2, B2], nksip_callbacks:sip_invite(A1, B1);
      {continue, [A1, B1]} ->
	  nksip_callbacks:sip_invite(A1, B1);
      Other -> Other
    end.

api_server_event(A1, B1, C1) ->
    nkservice_callbacks:api_server_event(A1, B1, C1).

sip_session_update(A1, B1, C1) ->
    nksip_callbacks:sip_session_update(A1, B1, C1).

nks_sip_route(A1, B1, C1, D1) ->
    nksip_callbacks:nks_sip_route(A1, B1, C1, D1).

service_handle_call(A1, B1, C1) ->
    nkservice_callbacks:service_handle_call(A1, B1, C1).

api_subscribe_allow(A1, B1, C1, D1, E1) ->
    nkservice_callbacks:api_subscribe_allow(A1, B1, C1, D1,
					    E1).

nks_sip_make_uac_dialog(A1, B1, C1, D1) ->
    nksip_callbacks:nks_sip_make_uac_dialog(A1, B1, C1, D1).

nks_sip_uas_timer(A1, B1, C1) ->
    nksip_callbacks:nks_sip_uas_timer(A1, B1, C1).

service_init(A1, B1) ->
    nkservice_callbacks:service_init(A1, B1).

sip_bye(A1, B1) -> nksip_callbacks:sip_bye(A1, B1).

sip_register(A1, B1) ->
    nksip_callbacks:sip_register(A1, B1).

nks_sip_transport_uac_headers(A1, B1, C1, D1, E1, F1) ->
    nksip_callbacks:nks_sip_transport_uac_headers(A1, B1,
						  C1, D1, E1, F1).

sip_get_user_pass(A1, B1, C1, D1) ->
    nksip_callbacks:sip_get_user_pass(A1, B1, C1, D1).

api_server_reg_down(A1, B1, C1) ->
    nkservice_callbacks:api_server_reg_down(A1, B1, C1).

error_code(A1) -> nkservice_callbacks:error_code(A1).

api_server_get_user_data(A1) ->
    nkservice_callbacks:api_server_get_user_data(A1).

nks_sip_connection_sent(A1, B1) ->
    nksip_callbacks:nks_sip_connection_sent(A1, B1).

sip_publish(A1, B1) ->
    nksip_callbacks:sip_publish(A1, B1).

nks_sip_uas_method(A1, B1, C1, D1) ->
    nksip_callbacks:nks_sip_uas_method(A1, B1, C1, D1).

sip_update(A1, B1) ->
    nksip_callbacks:sip_update(A1, B1).

api_server_cmd(A1, B1) ->
    nkservice_callbacks:api_server_cmd(A1, B1).

nks_sip_uas_dialog_response(A1, B1, C1, D1) ->
    nksip_callbacks:nks_sip_uas_dialog_response(A1, B1, C1,
						D1).

sip_cancel(A1, B1, C1) ->
    nksip_callbacks:sip_cancel(A1, B1, C1).

sip_notify(A1, B1) ->
    nksip_callbacks:sip_notify(A1, B1).

api_server_code_change(A1, B1, C1) ->
    nkservice_callbacks:api_server_code_change(A1, B1, C1).

nks_sip_connection_recv(A1, B1) ->
    nksip_callbacks:nks_sip_connection_recv(A1, B1).

nks_sip_parse_uac_opts(A2, B2) ->
    case
      nksip_uac_auto_auth_callbacks:nks_sip_parse_uac_opts(A2,
							   B2)
	of
      continue ->
	  [A1, B1] = [A2, B2],
	  nksip_callbacks:nks_sip_parse_uac_opts(A1, B1);
      {continue, [A1, B1]} ->
	  nksip_callbacks:nks_sip_parse_uac_opts(A1, B1);
      Other -> Other
    end.

sip_authorize(A1, B1, C1) ->
    nksip_callbacks:sip_authorize(A1, B1, C1).

nks_sip_authorize_data(A1, B1, C1) ->
    nksip_callbacks:nks_sip_authorize_data(A1, B1, C1).

sip_route(A1, B1, C1, D1, E1) ->
    nksip_callbacks:sip_route(A1, B1, C1, D1, E1).

nks_sip_uas_send_reply(A1, B1, C1) ->
    nksip_callbacks:nks_sip_uas_send_reply(A1, B1, C1).

nks_sip_uac_pre_request(A1, B1, C1, D1) ->
    nksip_callbacks:nks_sip_uac_pre_request(A1, B1, C1, D1).

sip_options(A2, B2) ->
    case nksip_tutorial_client_callbacks:sip_options(A2, B2)
	of
      continue ->
	  [A1, B1] = [A2, B2],
	  nksip_callbacks:sip_options(A1, B1);
      {continue, [A1, B1]} ->
	  nksip_callbacks:sip_options(A1, B1);
      Other -> Other
    end.

api_server_init(A1, B1) ->
    nkservice_callbacks:api_server_init(A1, B1).

api_allow(A1, B1) ->
    nkservice_callbacks:api_allow(A1, B1).

nks_preparse(A1, B1) ->
    nksip_callbacks:nks_preparse(A1, B1).

sip_ack(A1, B1) -> nksip_callbacks:sip_ack(A1, B1).

service_handle_info(A1, B1) ->
    nkservice_callbacks:service_handle_info(A1, B1).

api_server_login(A1, B1, C1) ->
    nkservice_callbacks:api_server_login(A1, B1, C1).

nks_sip_dialog_update(A1, B1, C1) ->
    nksip_callbacks:nks_sip_dialog_update(A1, B1, C1).

nks_sip_uas_sent_reply(A1) ->
    nksip_callbacks:nks_sip_uas_sent_reply(A1).

nks_sip_call(A1, B1, C1) ->
    nksip_callbacks:nks_sip_call(A1, B1, C1).

sip_subscribe(A1, B1) ->
    nksip_callbacks:sip_subscribe(A1, B1).

nks_sip_uac_response(A2, B2, C2, D2) ->
    case
      nksip_uac_auto_auth_callbacks:nks_sip_uac_response(A2,
							 B2, C2, D2)
	of
      continue ->
	  [A1, B1, C1, D1] = [A2, B2, C2, D2],
	  nksip_callbacks:nks_sip_uac_response(A1, B1, C1, D1);
      {continue, [A1, B1, C1, D1]} ->
	  nksip_callbacks:nks_sip_uac_response(A1, B1, C1, D1);
      Other -> Other
    end.

api_syntax(A1, B1, C1, D1) ->
    nkservice_callbacks:api_syntax(A1, B1, C1, D1).

sip_refer(A1, B1) -> nksip_callbacks:sip_refer(A1, B1).

nks_sip_uac_reply(A1, B1, C1) ->
    nksip_callbacks:nks_sip_uac_reply(A1, B1, C1).

nks_sip_uac_proxy_opts(A1, B1) ->
    nksip_callbacks:nks_sip_uac_proxy_opts(A1, B1).

api_server_handle_call(A1, B1, C1) ->
    nkservice_callbacks:api_server_handle_call(A1, B1, C1).

sip_info(A1, B1) -> nksip_callbacks:sip_info(A1, B1).

nks_sip_parse_uas_opt(A1, B1, C1) ->
    nksip_callbacks:nks_sip_parse_uas_opt(A1, B1, C1).

api_server_forward_event(A1, B1, C1) ->
    nkservice_callbacks:api_server_forward_event(A1, B1,
						 C1).

nks_sip_debug(A1, B1, C1) ->
    nksip_callbacks:nks_sip_debug(A1, B1, C1).

nks_sip_uas_process(A1, B1) ->
    nksip_callbacks:nks_sip_uas_process(A1, B1).

service_code_change(A1, B1, C1) ->
    nkservice_callbacks:service_code_change(A1, B1, C1).

sip_resubscribe(A1, B1) ->
    nksip_callbacks:sip_resubscribe(A1, B1).

sip_message(A1, B1) ->
    nksip_callbacks:sip_message(A1, B1).

service_handle_cast(A1, B1) ->
    nkservice_callbacks:service_handle_cast(A1, B1).

service_terminate(A1, B1) ->
    nkservice_callbacks:service_terminate(A1, B1).

api_server_terminate(A1, B1) ->
    nkservice_callbacks:api_server_terminate(A1, B1).

sip_reinvite(A1, B1) ->
    nksip_callbacks:sip_reinvite(A1, B1).

api_server_handle_cast(A1, B1) ->
    nkservice_callbacks:api_server_handle_cast(A1, B1).

api_cmd(A1, B1) -> nkservice_callbacks:api_cmd(A1, B1).

api_server_handle_info(A1, B1) ->
    nkservice_callbacks:api_server_handle_info(A1, B1).

uuid() ->
    <<99, 100, 98, 54, 52, 50, 51, 98, 45, 55, 52, 50, 56,
      45, 53, 53, 56, 49, 45, 97, 55, 56, 51, 45, 50, 48, 99,
      102, 51, 48, 55, 101, 57, 51, 97, 51>>.

timestamp() -> 1535091567841715.

plugins() ->
    [nkservice, nksip, nksip_uac_auto_auth,
     nksip_tutorial_client_callbacks].

name() -> client.

log_level() -> notice.

listen_ids() ->
    {map,
     <<131, 116, 0, 0, 0, 1, 100, 0, 5, 110, 107, 115, 105,
       112, 108, 0, 0, 0, 2, 100, 0, 6, 51, 65, 87, 101, 69,
       67, 100, 0, 6, 51, 121, 100, 77, 75, 106, 106>>}.

listen() ->
    {map,
     <<131, 116, 0, 0, 0, 2, 100, 0, 9, 110, 107, 115, 101,
       114, 118, 105, 99, 101, 106, 100, 0, 5, 110, 107, 115,
       105, 112, 108, 0, 0, 0, 2, 104, 2, 108, 0, 0, 0, 1, 104,
       4, 100, 0, 14, 110, 107, 115, 105, 112, 95, 112, 114,
       111, 116, 111, 99, 111, 108, 100, 0, 3, 117, 100, 112,
       104, 4, 97, 192, 97, 168, 97, 0, 97, 60, 98, 0, 0, 19,
       206, 106, 116, 0, 0, 0, 5, 100, 0, 5, 99, 108, 97, 115,
       115, 104, 2, 100, 0, 5, 110, 107, 115, 105, 112, 100, 0,
       7, 98, 112, 100, 102, 104, 114, 99, 100, 0, 4, 104, 111,
       115, 116, 109, 0, 0, 0, 10, 49, 57, 50, 46, 49, 54, 56,
       46, 54, 48, 100, 0, 14, 117, 100, 112, 95, 115, 116, 97,
       114, 116, 115, 95, 116, 99, 112, 100, 0, 4, 116, 114,
       117, 101, 100, 0, 14, 117, 100, 112, 95, 115, 116, 117,
       110, 95, 114, 101, 112, 108, 121, 100, 0, 4, 116, 114,
       117, 101, 100, 0, 11, 117, 100, 112, 95, 115, 116, 117,
       110, 95, 116, 49, 98, 0, 0, 1, 244, 104, 2, 108, 0, 0,
       0, 1, 104, 4, 100, 0, 14, 110, 107, 115, 105, 112, 95,
       112, 114, 111, 116, 111, 99, 111, 108, 100, 0, 3, 116,
       108, 115, 104, 4, 97, 192, 97, 168, 97, 0, 97, 60, 98,
       0, 0, 19, 207, 106, 116, 0, 0, 0, 2, 100, 0, 5, 99, 108,
       97, 115, 115, 104, 2, 100, 0, 5, 110, 107, 115, 105,
       112, 100, 0, 7, 98, 112, 100, 102, 104, 114, 99, 100, 0,
       4, 104, 111, 115, 116, 109, 0, 0, 0, 12, 49, 57, 50, 46,
       49, 54, 56, 46, 48, 46, 54, 48, 106>>}.

id() -> bpdfhrc.

config_nksip_uac_auto_auth() ->
    {nksip_uac_auto_auth, 5, []}.

config_nksip() ->
    {config,
     [<<73, 78, 86, 73, 84, 69>>, <<65, 67, 75>>,
      <<67, 65, 78, 67, 69, 76>>, <<66, 89, 69>>,
      <<79, 80, 84, 73, 79, 78, 83>>, <<73, 78, 70, 79>>,
      <<85, 80, 68, 65, 84, 69>>,
      <<83, 85, 66, 83, 67, 82, 73, 66, 69>>,
      <<78, 79, 84, 73, 70, 89>>, <<82, 69, 70, 69, 82>>,
      <<77, 69, 83, 83, 65, 71, 69>>],
     [<<112, 97, 116, 104>>], 60, 5, 30,
     {uri, sip, <<99, 108, 105, 101, 110, 116>>, <<>>,
      <<49, 57, 50, 46, 49, 54, 56, 46, 48, 46, 54, 48>>, 0,
      <<>>, [], [], [], [], <<>>},
     undefined, [], [], false, 100000,
     <<108, 111, 99, 97, 108, 104, 111, 115, 116>>, auto,
     false, {call_times, 500, 4000, 5000, 180, 900, 1800},
     1300}.

config_nkservice() ->
    {map,
     <<131, 116, 0, 0, 0, 1, 100, 0, 8, 110, 101, 116, 95,
       111, 112, 116, 115, 106>>}.

config() ->
    {map,
     <<131, 116, 0, 0, 0, 3, 100, 0, 8, 115, 105, 112, 95,
       102, 114, 111, 109, 104, 12, 100, 0, 3, 117, 114, 105,
       100, 0, 3, 115, 105, 112, 109, 0, 0, 0, 6, 99, 108, 105,
       101, 110, 116, 109, 0, 0, 0, 0, 109, 0, 0, 0, 12, 49,
       57, 50, 46, 49, 54, 56, 46, 48, 46, 54, 48, 97, 0, 109,
       0, 0, 0, 0, 106, 106, 106, 106, 109, 0, 0, 0, 0, 100, 0,
       10, 115, 105, 112, 95, 108, 105, 115, 116, 101, 110,
       108, 0, 0, 0, 2, 104, 2, 108, 0, 0, 0, 1, 104, 4, 100,
       0, 14, 110, 107, 115, 105, 112, 95, 112, 114, 111, 116,
       111, 99, 111, 108, 100, 0, 3, 117, 100, 112, 104, 4, 97,
       192, 97, 168, 97, 0, 97, 60, 98, 0, 0, 19, 206, 106,
       116, 0, 0, 0, 1, 100, 0, 4, 104, 111, 115, 116, 109, 0,
       0, 0, 10, 49, 57, 50, 46, 49, 54, 56, 46, 54, 48, 104,
       2, 108, 0, 0, 0, 1, 104, 4, 100, 0, 14, 110, 107, 115,
       105, 112, 95, 112, 114, 111, 116, 111, 99, 111, 108,
       100, 0, 3, 116, 108, 115, 104, 4, 97, 192, 97, 168, 97,
       0, 97, 60, 98, 0, 0, 19, 207, 106, 116, 0, 0, 0, 1, 100,
       0, 4, 104, 111, 115, 116, 109, 0, 0, 0, 12, 49, 57, 50,
       46, 49, 54, 56, 46, 48, 46, 54, 48, 106, 100, 0, 14,
       115, 105, 112, 95, 108, 111, 99, 97, 108, 95, 104, 111,
       115, 116, 109, 0, 0, 0, 9, 108, 111, 99, 97, 108, 104,
       111, 115, 116>>}.

class() -> nksip.

callback() -> nksip_tutorial_client_callbacks.

