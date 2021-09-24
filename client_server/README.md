# Building a web server in C

From: https://stackoverflow.com/questions/176409/build-a-simple-http-server-in-c

1. Get your basic TCP sockets layer running (listen on port/ports, accept client connections and send/receive data).
2. Implement a buffered reader so that you can read requests one line (delimited by CRLF) at a time.
3. Read the very first line. Parse out the method, the request version and the path.
4. Implement header parsing for the "Header: value" syntax. Don't forget unfolding folded headers.
5. Check the request method, content type and content size to determine how/if the body will be read.
6. Implement decoding of content based on content type.
7. If you're going to support HTTP 1.1, implement things like "100 Continue", keep-alive, chunked transfer.
8. Add robustness/security measures like detecting incomplete requests, limiting max number of clients etc.
9. Shrink wrap your code and open-source it :)

# Building Client/Server

- Write a client and server program that opens a TCP socket between them
  - Will require socket(), connect(), listen(), and accept()

_Note:_ I want to store data in the "server" with some data structure I write in C, like a queue or linked list or something,
for the sake of data structure practice.
