:- initialization(main, main).

merge([], L, L).
merge(L, [], L).
merge([X|Xs], [Y|Ys], [X|Z]) :-
    X =< Y,
    merge(Xs, [Y|Ys], Z).
merge([X|Xs], [Y|Ys], [Y|Z]) :-
    X > Y,
    merge([X|Xs], Ys, Z).

split([], [], []).
split([X], [X], []).
split([X,Y|T], [X|Xs], [Y|Ys]) :-
    split(T, Xs, Ys).

mergesort([], []).
mergesort([X], [X]).
mergesort(L, S) :-
    split(L, L1, L2),
    mergesort(L1, S1),
    mergesort(L2, S2),
    merge(S1, S2, S).

run(Argv) :-
    maplist(atom_number, Argv, L),
    mergesort(L, S),
    write(S), nl.

main :-
    current_prolog_flag(argv, Argv),
    run(Argv),
    halt.