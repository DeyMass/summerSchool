-module (power).

-export ([test/1]).

 % -define (POW (Base, Exp), begin
 %              Calc = fun(Base, Exp, Res) when Exp > 0 ->
 %                  R = fun(NewBas, NewExp, NewRes) ->
 %                      Calc(NewBas, NewExp - 1, NewRes * Base)
 %                  end,
 %                  R(Base, Exp, Res)
 %              end,
 %              Calc(A, A, 1)
 %             end).


test(A) ->
    % ?POW(A,2).
    A.
