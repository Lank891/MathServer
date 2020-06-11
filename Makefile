all: server

server: solver Server/main.c
	ghc --make -no-hs-main -optc-O Server/main.c Server/solver -o server

solver: Server/solver.hs
	ghc -c -O Server/solver.hs

.Phony: clean

clean:
	rm Server/main.o Server/solver.hi Server/solver.o Server/solver_stub.h Server/server
