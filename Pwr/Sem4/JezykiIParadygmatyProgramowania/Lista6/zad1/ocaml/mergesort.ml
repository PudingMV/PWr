let rec merge a b =
  match a, b with
  | [], ys -> ys
  | xs, [] -> xs
  | x::xs, y::ys ->
      if x <= y then x :: merge xs (y::ys)
      else y :: merge (x::xs) ys

let rec split lst =
  let rec aux i left right = function
    | [] -> (List.rev left, List.rev right)
    | x::xs ->
        if i mod 2 = 0
        then aux (i+1) (x::left) right xs
        else aux (i+1) left (x::right) xs
  in aux 0 [] [] lst

let rec merge_sort lst =
  match lst with
  | [] -> []
  | [_] -> lst
  | _ ->
      let left, right = split lst in
      merge (merge_sort left) (merge_sort right)

let () =
  let lst = List.tl (Array.to_list Sys.argv) |> List.map int_of_string in
  let sorted = merge_sort lst in
  List.iter (Printf.printf "%d ") sorted;
  print_newline ()