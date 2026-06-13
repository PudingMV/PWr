let rec binomial n k =
  if k = 0 || k = n then
    1
  else
    binomial (n - 1) k + binomial (n - 1) (k - 1)

let () =
  let n = int_of_string Sys.argv.(1) in
  let k = int_of_string Sys.argv.(2) in
  Printf.printf "%d\n" (binomial n k)