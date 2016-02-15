arc(Node, M, Seed, PCost, Target, Cost) :- M is Node*Seed, h(Node, Hvalue, Target), Cost is 1 + PCost + Hvalue.
arc(Node, M, Seed, PCost, Target, Cost) :- M is Node*Seed + 1, h(Node, Hvalue, Target), Cost is 2 + PCost + Hvalue.

goal(Node, Target) :- 0 is Node mod Target.

h(Node, Hvalue, Target) :-  goal(Node, Target), !, Hvalue is 0
                            ;
                            Hvalue is 1/Node.
                            
search([[Node, Cost] | _FRest], _Seed, Target, [Node, Cost]) :- goal(Node, Target).
search([[Node, Cost] | FRest], Seed, Target, Found) :- 
    setof([N, C], arc(Node, N, Seed, Cost, Target, C), FNode),
    add_to_frontier(FNode, FRest, FNew),
    search(FNew, Seed, Target, Found).

insert(CostX, [], [CostX]).
insert([NodeX, CostX], [[NodeY, CostY] | Tail], [[NodeX, CostX], [NodeY, CostY] | Tail]) :- less_than(CostX, CostY).
insert([NodeX, CostX], [[NodeY, CostY] | Tail], [[NodeY, CostY] | NewTail]) :- CostX > CostY, insert([NodeX, CostX], Tail, NewTail).

insertion_sort([],Acc,Acc).
insertion_sort([Head | Tail], Acc, Sorted) :- insert(Head, Acc, NewAcc), insertion_sort(Tail, NewAcc, Sorted).

add_to_frontier([], Frontier, Frontier).
add_to_frontier([Head | Tail], FRest, FNew) :-
    insertion_sort([Head | FRest], [], FwHead),
    add_to_frontier(Tail, FwHead, FNew).

less_than(Cost1, Cost2) :-
    F1 is Cost1, F2 is Cost2,
    F1 =< F2.

a_star(Start, Seed, Target, Found) :-
    setof([N, C], arc(Start, N, Seed, 0, Target, C), FNode),
    add_to_frontier(FNode, [], FNew),
    search(FNew, Seed, Target, Found).
