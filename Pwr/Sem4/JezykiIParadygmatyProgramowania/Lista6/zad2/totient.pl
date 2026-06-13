:- initialization(main, main).

gcd(A, 0, A).
gcd(A, B, G) :-
    B > 0,
    R is A mod B,
    gcd(B, R, G).

coprime(A, B) :-
    gcd(A, B, 1).

totient(N, T) :-
    totient(1, N, N, T).

totient(I, N, Acc, Acc) :-
    I > N.
totient(I, N, Acc, T) :-
    I =< N,
    (coprime(I, N) ->
        Acc1 is Acc - 1
    ;
        Acc1 = Acc
    ),
    I1 is I + 1,
    totient(I1, N, Acc1, T).

run([A|_]) :-
    atom_number(A, N),
    totient(N, T),
    write(T), nl.

main :-
    current_prolog_flag(argv, Argv),
    run(Argv),
    halt.