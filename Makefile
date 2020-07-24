all: server client

server: solver Server/main.c
	ghc --make -no-hs-main -optc-O Server/main.c Server/solver -o Executables/server

solver: Server/solver.hs
	ghc -c -O Server/solver.hs

client: Client/client.py
	cp Client/client.py Executables/client.py

.Phony: clean

clean:
	rm Server/main.o Server/solver.hi Server/solver.o Server/solver_stub.h 
