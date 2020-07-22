# Math Server

#### Simple math server for expressions like "321+32-(73\*12)".

##### For now it opens a serer using port (first program argument) and when client connects it reads one request as RPFor now it opens a serer using port (first program argument) and when client connects it reads one request  asFor now it opens a serer using port (first program argument) and when client connects it reads one request as RPN notation separated with spaces like "10 5 + 6 * 2 * 3 / 45 + 5 - log" ( = 2), sends the answer back and closes the client.
------------------------------------------
Server: __C__

Parser & Interpreter: __Haskell__

Client: __Python__

Valid operations:

- +, - , * , /
- ^
- ln (natural logarithm)
- log (logarithm base 10)
