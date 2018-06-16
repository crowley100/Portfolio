numeral(0).
numeral(s(X)) :- numeral(X).
numeral(p(X)) :- numeral(X). 
numeral(X+Y) :- numeral(X), numeral(Y).
numeral(-X) :- numeral(X).
numeral(X-Y) :- numeral(X), numeral(Y).

resolve(0,0).
resolve(s(X),s(Y)) :- resolve(X,Y), Y \=p(_).
resolve(s(X),Y) :- resolve(X,p(Y)).

resolve(p(X),p(Y)) :- resolve(X,Y), Y \=s(_).
resolve(p(X),Y) :- resolve(X,s(Y)).

add2(0,X,Y) :- resolve(X,Y).
add2(s(X),Y,s(Z)) :- add2(X,Y,Z), Z \=p(_).
add2(s(X),Y,Z) :- add2(X,Y,p(Z)).

add2(p(X),Y,p(Z)) :- add2(X,Y,Z), Z \=s(_).
add2(p(X),Y,Z) :- add2(X,Y,s(Z)).

add2(X+Y,Z,R) :-
	add2(X,Y,R1),
	add2(R1,Z,R),
	numeral(R).
add2(X,Y+Z,R) :-
	add2(Y,Z,R1),
	add2(X,R1,R),
	numeral(R).
add2(W+X,Y+Z,R) :-
	add2(W,X,R1),
	add2(Y,Z,R2),
	add2(R1,R2,R),
	numeral(R).
add2(-X,Y,R) :-
	minus(X,R1),
	add2(R1,Y,R).
add2(X,-Y,R) :-
	minus(Y,R1),
	add2(X,R1,R).
add2(X-Y,Z,R) :-
	subtract(X,Y,R1),
	add2(R1,Z,R).
add2(X,Y-Z,R) :-
        subtract(Y,Z,R1),
        add2(X,R1,R).

minus(0,0).
minus(s(X),p(Z)) :- minus(X,Z), Z \=s(_).
minus(s(X),Z) :- minus(X,s(Z)).
minus(p(X),s(Z)) :- minus(X,Z), Z \=p(_).
minus(p(X),Z) :- minus(X,p(Z)).
minus(X-Y,Z) :-
	minus(Y,R1),
	add2(X,R1,R2),
	minus(R2,Z).

subtract(X,-Y,Z) :-
	add2(X,Y,Z).
subtract(X,Y,Z) :-
	minus(Y,R1),
	add2(X,R1,Z).
subtract(X-Y,Z,R) :-
	subtract(X,Y,R1),
	subtract(R1,Z,R).
subtract(X,Y-Z,R) :-
        subtract(Y,Z,R1),
        subtract(X,R1,R).
