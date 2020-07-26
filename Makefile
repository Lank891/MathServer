all: server client

server: tests Server/main.c
	ghc --make -no-hs-main -optc-O Server/main.c Server/solver -o Executables/server

tests: solver Server/tests.c
	ghc --make -no-hs-main -optc-O Server/tests.c Server/solver -o Server/test
	Server/test || (echo "Build failed $$?"; exit 1)

solver: Server/solver.hs
	ghc -c -O Server/solver.hs

client: Client/client.py
	cp Client/client.py Executables/client.py

.Phony: clean

clean:
	rm Server/tests.o Server/test Server/main.o Server/solver.hi Server/solver.o Server/solver_stub.h -f 
