let rec prime_factors n d =
  if n = 1 then []
  else if d * d > n then [n]
  else if n mod d = 0 then d :: prime_factors (n / d) d
  else prime_factors n (d + 1)

let prime_factors n =
  prime_factors n 2

let () =
  let n = int_of_string Sys.argv.(1) in
  let res = prime_factors n in
  List.iter (Printf.printf "%d ") res;
  print_newline ()