:- initialization(main, main).

is_prime(N) :-
    N > 1,
    \+ has_divisor(N, 2).

has_divisor(N, D) :-
    D * D =< N,
    (N mod D =:= 0 ;
     D1 is D + 1,
     has_divisor(N, D1)).

primes(N, X) :-
    findall(P, (between(2, N, P), is_prime(P)), X).

run([A|_]) :-
    atom_number(A, N),
    primes(N, X),
    write(X), nl.

main :-
    current_prolog_flag(argv, Argv),
    run(Argv),
    halt.