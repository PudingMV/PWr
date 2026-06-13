let next_row row =
  let rec aux prev rest =
    match rest with
    | [] -> [1]
    | x :: xs -> (prev + x) :: aux x xs
  in
  1 :: aux 0 (List.tl row)

let rec build_rows n row =
  if n = 0 then row
  else build_rows (n - 1) (next_row row)

let binomial2 n k =
  List.nth (build_rows n [1]) k

let () =
  let n = int_of_string Sys.argv.(1) in
  let k = int_of_string Sys.argv.(2) in
  Printf.printf "%d\n" (binomial2 n k)