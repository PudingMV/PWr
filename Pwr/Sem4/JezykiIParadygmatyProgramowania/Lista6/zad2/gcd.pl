:- initialization(main, main).

egcd(A, 0, 1, 0, A).
egcd(A, B, X, Y, G) :-
    B > 0,
    R is A mod B,
    egcd(B, R, X1, Y1, G),
    X is Y1,
    Y is X1 - (A // B) * Y1.

run([A,B|_]) :-
    atom_number(A, A1),
    atom_number(B, B1),
    egcd(A1, B1, X, Y, G),
    write((X,Y,G)), nl.

main :-
    current_prolog_flag(argv, Argv),
    run(Argv),
    halt.