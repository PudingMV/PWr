.PHONY: zad1h zad1o zad1c zad2

all: zad1h zad1o zad1c zad2

zad1h:
	@echo "zad1 binomial haskell"
	cd zad1 && cd haskel && ghc binomial1.hs && ./binomial1 20 10
	cd zad1 && cd haskel && ghc binomial2.hs && ./binomial2 1000 500
	cd zad1 && cd haskel && ghc mergesort.hs && ./mergesort 5 2 9 1 3
	cd zad1 && cd haskel && ghc gcd.hs && ./gcd 30 20
	cd zad1 && cd haskel && ghc primefactors.hs && ./primefactors 36
	cd zad1 && cd haskel && ghc totient1.hs && ./totient1 10000000
	cd zad1 && cd haskel && ghc totient2.hs && ./totient2 10000000
	cd zad1 && cd haskel && ghc primes.hs && ./primes 20

zad1o:
	@echo "zad1 binomial OCaml"
	cd zad1 && cd ocaml && ocamlc -o binomial1 binomial1.ml && ./binomial1 20 10
	cd zad1 && cd ocaml && ocamlc -o binomial2 binomial2.ml && ./binomial2 200 100
	cd zad1 && cd ocaml && ocamlc -o mergesort mergesort.ml && ./mergesort 5 2 9 1 3
	cd zad1 && cd ocaml && ocamlc -o gcd gcd.ml && ./gcd 30 20
	cd zad1 && cd ocaml && ocamlc -o primefactors primefactors.ml && ./primefactors 36
	cd zad1 && cd ocaml && ocamlc -o totient1 totient1.ml && ./totient1 10000000
	cd zad1 && cd ocaml && ocamlc -o totient2 totient2.ml && ./totient2 10000000
	cd zad1 && cd ocaml && ocamlc -o primes primes.ml && ./primes 20

zad1c:
	@echo "zad1 binomial Common Lisp"
	cd zad1 && cd clisp && sbcl --script binomial1.lisp 20 10
	cd zad1 && cd clisp && sbcl --script binomial2.lisp 1000 500
	cd zad1 && cd clisp && sbcl --script mergesort.lisp 5 2 9 1 3
	cd zad1 && cd clisp && sbcl --script gcd.lisp 30 20
	cd zad1 && cd clisp && sbcl --script primefactors.lisp 36
	cd zad1 && cd clisp && sbcl --script totient1.lisp 10000
	cd zad1 && cd clisp && sbcl --script totient2.lisp 100000
	cd zad1 && cd clisp && sbcl --script primes.lisp 20

zad2:
	cd zad2 && swipl -q mergesort.pl -- 3 1 4 2 5
	cd zad2 && swipl -q gcd.pl -- 30 20
	cd zad2 && swipl -q primefactors.pl -- 360
	cd zad2 && swipl -q totient.pl -- 8
	cd zad2 && swipl -q primes.pl -- 30
