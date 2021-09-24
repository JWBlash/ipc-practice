#### Resources

**listen() function backlog param**
https://tangentsoft.net/wskfaq/advanced.html#backlog

If you're not sure, it's generally advised to use SOMAXCONN, defined in socket.h:

https://docs.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-listen?redirectedfrom=MSDN:
"A value for the backlog of SOMAXCONN is a special constant that instructs the underlying service provider responsible
for socket s to set the length of the queue of pending connections to a maximum reasonable value."
