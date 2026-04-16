package body Euler is
  function gcd(a, b : Integer) return Integer is
    temp : Integer; a_copy : Integer := a; b_copy : Integer := b;
  begin
    if a_copy < b_copy then
      temp := b_copy;
      b_copy := a_copy;
      a_copy := temp;
    end if;

    while b_copy /= 0 loop
      temp := b_copy;
      b_copy := a_copy mod b_copy;
      a_copy := temp;
    end loop;

    return a_copy;
  end gcd;

  function minimalDivisor(n : Integer) return Integer is
    i : Integer := 3;
  begin
    if n mod 2 = 0 then
      return 2;
    end if;

    while i * i <= n loop
      if n mod i = 0 then
        return i;
      end if;

      i := i+2;
    end loop;

    return n;
  end minimalDivisor;

  function Totient(n : Integer) return Integer is
    ans : Integer := 1;
  begin
    for i in 2 .. n-1 loop
      if gcd(i, n) = 1 then
        ans := ans + 1;
      end if;
    end loop;
    
    return ans;
  end Totient;

  function diophantic(a, b : Integer) return Pair is
      c, test, x_candidate, a_copy, b_copy : Integer;
    begin

      a_copy := a;
      b_copy := b;

      c := gcd(a, b);
      for y in 0 .. 100000 loop
        test := c + b_copy*y;

        if test mod a_copy = 0 then

          x_candidate := test / a_copy;

          if x_candidate > 0 and y > 0 then
            if a_copy*x_candidate - b_copy*y = c then
              return (First => x_candidate, Second => y);
            end if;
          end if;
        end if;
      end loop;
      return (First => -1, Second => -1);
    end diophantic;

end Euler;

