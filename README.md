# Math Server

#### Simple math server for expressions like "321+32-(73\*12)".

##### Python client connects to the server (ip and port have to be changed) and reads query as RPN notation separated with spaces like "10 5 + 6 * 2 * 3 / 45 + 5 - log" ( = 2), server reads it and send answer to the client.
------------------------------------------
Server: __C__

Parser & Interpreter: __Haskell__

Client: __Python__

Valid operations:

- +, - , * , /
- ^
- ln (natural logarithm)
- log (logarithm base 10)
