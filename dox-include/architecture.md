Architecture  				{#architecture}
============

The Hydra application makes use of a client / server architecture combined
with a modularized approach to application components. The application is
contained in one executable, launching into either client or server mode
depending on configuration parameters. The application is divided into
libraries for TCP, simulation, threading, timing, configuration, and display. 

System Design
-------------

The system is comprised of one or more Raspberry Pi computers. One Raspberry
Pi runs one instance of the application in server mode. This server handles
running the 2D physics simulation, as well as coordinating communication with
each client. The server Raspberry Pi can also simultaneously run an instance
of the application in client mode. Additional Raspberry Pi’s running the
application in client mode can connect via TCP to the server. Each Raspberry
Pi is connected by HDMI to a HD display. 

Data Design
-----------

The Hydra project requires a way for the user to configure the client and
server processes to make running the application as simple as possible. To
achieve maximum flexibility in the number of connected devices and the variety
of display hardware, several options need to be configured by the user, such
as the absolute size of the game map and the position of each client. Although
these can be set using command line arguments, this would make running the
application quite frustrating, as there are a good many options that can be
adjusted.


To address this, Hydra has a configuration parser that reads data from a file
in the form of

	option “value”

Whitespace is ignored, unless enclosed within quotation marks, as are any
lines that start with a # symbol, allowing the configuration file to be
commented. This file structure is fairly common, so source code editors
(including vim, gedit & kate) will provide syntax highlighting automatically.


The other major data design used in Hydra is the format for passing messages
over the network. UDP services often impose limitations on the size of
a packet, so it was important that the message format remain as terse as
possible. This ruled out existing formats with a lot of overhead, such as XML
or json. Ultimately, the approach chosen was to implement an entirely
predictable but inflexible format, using commas to separate values, and
semi-colons to separate logical blocks.


The first block in a message contains the tick number, number of clients (used
by the display to calculate if it should try and draw an object or not), the
dimensions of the game map (which may change dynamically in a future version),
and the number of objects contained in the message.


Each subsequent block in a message contains the unique identifier for an
object in the game world and it's x and y coordinates.


Internal data structures are described in depth in the Data Structure section.

