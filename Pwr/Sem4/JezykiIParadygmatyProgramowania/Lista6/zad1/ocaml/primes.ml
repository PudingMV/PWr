let is_prime n =
  let rec check d =
    if d * d > n then true
    else if n mod d = 0 then false
    else check (d + 1)
  in
  n > 1 && check 2

let rec primes_from i n =
  if i > n then []
  else if is_prime i then i :: primes_from (i + 1) n
  else primes_from (i + 1) n

let primes n =
  primes_from 2 n

let () =
  let n = int_of_string Sys.argv.(1) in
  List.iter (Printf.printf "%d ") (primes n);
  print_newline ()