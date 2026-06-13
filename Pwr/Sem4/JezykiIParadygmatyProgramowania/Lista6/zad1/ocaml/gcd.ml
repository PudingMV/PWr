let rec egcd a b =
  if b = 0 then
    (1, 0, a)
  else
    let (x1, y1, g) = egcd b (a mod b) in
    let x = y1 in
    let y = x1 - (a / b) * y1 in
    (x, y, g)

let () =
  let a = int_of_string Sys.argv.(1) in
  let b = int_of_string Sys.argv.(2) in
  let (x, y, g) = egcd a b in
  Printf.printf "(%d, %d, %d)\n" x y g