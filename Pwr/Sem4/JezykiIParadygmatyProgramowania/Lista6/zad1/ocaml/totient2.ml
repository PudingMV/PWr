let rec prime_factors n d =
  if n = 1 then []
  else if d * d > n then [n]
  else if n mod d = 0 then d :: prime_factors (n / d) d
  else prime_factors n (d + 1)

let rec unique = function
  | [] -> []
  | x :: xs -> x :: unique (List.filter (fun y -> y <> x) xs)

let totient2 n =
  let factors = unique (prime_factors n 2) in
  List.fold_left (fun acc p -> acc * (p - 1) / p) n factors

let () =
  let n = int_of_string Sys.argv.(1) in
  Printf.printf "%d\n" (totient2 n)