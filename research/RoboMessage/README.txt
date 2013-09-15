So RoboMessage is a simple UDP client server program.
Instructions:
>make
First, open two terminals.
Then start the client:
>./RoboMessage client 8675
Then start the server in another terminal to send messages to the broadcast addrress
>./RoboMessage server 192.168.0.255 8675
RoboMessage server will give you a prompt :::
Type in whatever and see it miraculously appear in the client.

MAGIC! (cue jazz hands)

