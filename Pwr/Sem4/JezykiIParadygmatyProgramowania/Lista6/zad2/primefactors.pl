:- initialization(main, main).

prime_factors(N, X) :-
    pf(N, 2, X).

pf(1, _, []).
pf(N, D, [D|Xs]) :-
    N > 1,
    0 is N mod D,
    N1 is N // D,
    pf(N1, D, Xs).
pf(N, D, X) :-
    D * D =< N,
    D1 is D + 1,
    pf(N, D1, X).
pf(N, D, [N]) :-
    D * D > N,
    N > 1.

run([A|_]) :-
    atom_number(A, N),
    prime_factors(N, X),
    write(X), nl.

main :-
    current_prolog_flag(argv, Argv),
    run(Argv),
    halt.