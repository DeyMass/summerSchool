-module (power).

-export ([test/2]).

 -define (POW (Base, Exp), begin
              Fun = fun
                  Pow(_, 0, Res) -> Res;
                  Pow(B, E, Res) -> Pow(B, E-1, Res*B)
              end,
              Fun(Base, Exp, 1)
         end).


test(Base, Exp) ->
    ?POW(Base, Exp).
