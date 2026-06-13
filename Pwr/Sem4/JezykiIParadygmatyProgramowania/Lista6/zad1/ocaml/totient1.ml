let rec gcd' a b =
  if b = 0 then a
  else gcd' b (a mod b)

let coprime a b =
  gcd' a b = 1

let rec count i n acc =
  if i > n then acc
  else
    let acc =
      if coprime i n then acc + 1 else acc
    in
    count (i + 1) n acc

let totient n =
  count 1 n 0

let () =
  let n = int_of_string Sys.argv.(1) in
  Printf.printf "%d\n" (totient n)