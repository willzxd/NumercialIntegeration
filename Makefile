numerics: main.cpp Functions.h Functions.cpp Expr.h Expr.cpp strtonum.h helper.h helper.cpp
	g++ -o numerics -std=c++11 -Wall -Werror -ggdb3 Functions.cpp Expr.cpp helper.cpp main.cpp

clean:
	rm -f *~ numerics *.o
