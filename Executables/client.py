import socket
import textwrap

IP = '127.0.0.1'
Port = 20202

MaxInputSize = 512 #512 is used in C server
MaxResponseSize = 256 #more should not be needed

query = input('Your query: ')
textwrap.shorten(query, width = MaxInputSize-1)
query = query + '\0'
query = str.encode(query)

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((IP, Port))

s.send(query)
response = s.recv(MaxResponseSize)

s.close()

response = response.decode('utf-8')
print(response)
    