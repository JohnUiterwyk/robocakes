How Each Learning Objective is Addressed 	{#objectivesaddressed}
========================================


Complex System-Level Development
--------------------------------

The development of the Hydra application allowed us to gain a deep
understanding of building real-world c applications for a Linux target.
Although the application was targeted at the pi, and parts of it use
embedded-system-specific components, the abstraction provided by Autotools
allowed the project to build on multiple platforms with varying degrees of
capability. The choice to model the simulation on an entirely headless process
meant that Hydra can offload the more significant processing to a more capable
platform, leaving the Pi to manage it's own display.


Furthermore, the use of c, coupled with a well-known style guide should ensure
that other Linux embedded system developers will be comfortable adopting Hydra
and improving upon it. Licensing the project under the GPL should also help
this.


The use of Doxygen for documentation generation, git for version control and
github for hosting also demonstrate an appreciation of the culture surrounding
pi hacking, and linux development generally.


Analysis Of Hardware and Software Performance and Trade-Offs
------------------------------------------------------------

We demonstrate this learning objective through a portfolio that includes
detailed descriptions, explanations, and diagrams. Our source code is
documented in depth using Doxygen-compatible comments, and doxygen is
integrated into our build process.  We performed performance testing and
analysis on multiple components and used the results to inform our
development. We configured the system to boot without a GUI to avoid overhead,
disabled unused services such as Alsa, lightdm, and nfs, and minimized the
number of libraries the application was dependent on. However, the performance
gains from disabling services were minimal, and unneeded, since our design
suited the Pi's architecture and capabilities. The low powered nature of the
pi only became apparent during compilation time, and any performance
bottlenecks during runtime could be mitigated by adjusting the tick period
slightly, while still maintaining an acceptable frame rate.

We developed the application to allow for a headless server to run as
a separate process from the client on a single Pi. Even a Pi running as both
a server and client achieved good performance.

We considered the impact of using OpenGL and GLUT vs. the relatively
lightweight openVG library for graphics and after considerable research
decided to opt for libraries optimised to run on the low-powered Pi hardware.
This also avoided the inherent overhead of using a full 3D library to create
a 2D visualization. OpenVG also displays using built in vector graphics rather
than external models, further eliminating the need for extra third party
libraries. While we didn't end up creating graphics that required more than
basic types (models in openGL) this was a consideration even in the planning
stages of our application as there was always the potential to develop this
into a game.  We analysed the impact of using UDP versus TCP and used the
resulting data to inform our development.

Key OS Concepts
---------------

Our development of the Hydra application allowed us to demonstrate clear
understanding of kernel and hardware interaction, process and interprocess
communication, concurrency and synchronization, scheduling, as well as memory
hierarchy, management, and cost-performance trade-offs.


### Kernel and Hardware Interaction

The Hydra client spawns separate threads for interacting with the display and
listening/sending to the network and the Hydra server uses three threads for
networking. These system calls demonstrate Hydra's Kernel interaction.


Additionally, the networking component component connected to new sockets
directly, demonstrating additional system calls.


OpenVG allowed Hydra to interact with the embedded GPU, providing all the
performance benefits and developmental challenges associated with
hardware-specific code, but demonstrating hardware interaction.


### Process and Interprocess Communication

Within the context of our application, we demonstrate a mastery of
interprocess communication through the implementation of  TCP and UDP
networking functionality. In this context interprocess communication entails
passing information, data, and state between two or more processes running
either locally or on separate computers.


Our applications server module serializes the simulation state contained in
a sim_data_t into a char string. This character string is dispatched over the
network using an established TCP connection. The client module of the
application receives that message, deserializes the char string into an
sim_data_t structure and then proceeds to draw the state using the display
module.


To achieve this result, our application first establishes a TCP communication
channel between two processes. The server module first creates a TCP stream
socket and binds to that socket on a specified local port using the first
available network interface. Network interfaces were acquired using the
getaddrinfo() function passing in a pointer to a properly formatted struct
addrinfo. 

The server module then creates 3 threads for networking purposes. One thread
is used to begin accepting new connections using a blocking accept() call.
Upon accepting a connection from a client, the socket file descriptor for the
new client is saved to a set.  A second thread polls a mutex controlled shared
memory struct containing a message buffer; upon being notified of changes to
that shared memory via a conditional variable, the message buffer in the
shared memory is copied to a local buffer. This message is then dispatched to
all connected clients using send(). A third thread makes use of the select()
function to block while waiting for a message sent by any client to the
server. The select() function makes use of timeout variable to break the block
and add any recently connected clients to the select set. If a socket becomes
active, the socket file descriptor set is checked for which socket is active.
The recv() function is then used to read any waiting data into a char buffer.


The client module similarly creates a TCP stream socket, and then attempts to
connect to the server using a given ip address and port. Upon successfully
connecting to the server, the client creates two threads for network purposes.
The first thread is used to block on a recv() call, that wakes up upon
receiving a message from the server. Similar to the server, the second thread
polls a mutex controlled shared memory struct containing a message buffer;
upon being notified of changes to that shared memory via a conditional
variable, the message buffer in the shared memory is copied to a local buffer.
This message is then dispatched to the server using send(). 


Before implementing TCP networking, we first implemented one way UDP messaging
using broadcast datagram sockets. In this implementation, the server
dispatched messages to the local subnet's broadcast address and given port
using the sendto() call. All clients would listen on the given broadcast
address and port for messages using the blocking recvfrm() function.


Through the use of serialization and low level network based communication,
our project demonstrates a deep understanding of how to enable communication
between processes both locally and remotely. 




### Concurrency and Synchronization

The Hydra application demonstrates our team's mastery of concurrency and
synchronization through the use of multithreading, shared memory, and mutexes.
Concurrency, which is defined as having multiple executions occurring
simultaneously, is demonstrated in our application through the use of
pthreads. 


Our application server module uses three threads for networking functions in
addition to the main thread for performing the simulation, while our client
module used two threads for networking in addition to the main thread which
handled drawing. For more information about how these threads are used, please
review the “Process and Interprocess Communication” section.


Synchronization is demonstrated in the Hydra application in several different
ways. The Hydra application performs synchronization of data between threads,
it coordinated events between threads, it synchronized data between processes
on different computers, and it created a visually synchronized display of the
simulation. 


Synchronization of data between threads was achieved using shared memory with
memory access controlled using mutexes. Synchronization of events between
threads was achieved with the shared memory structures use of a conditional
variable indicating that a message had been received or that a message was
ready to be sent. Data was synchronized between processes using serialization
of state to a char string which was transmitted over a TCP connection.
Finally, displays were synchronized by using a strategy of only drawing the
most recently received serialized state message.


The following provides an outline of synchronization of data and state between
threads and processes in the Hydra application:


1. The application makes passes a pointer to each thread which references a struct shared_buffer_t, which includes the following variables. 
	* char * buffer
	* int is_changed
	* pthread_mutex_t mutex


2. Within the server’s simulation thread:
	* Timer util calls sim_tick every 16 ms
	* Simulation is advanced one tick
	* Simulation serializes the sim_data_t struct to string
	* Copies string to shared buffer using thread_copy_to_buffer


3. The function `thread_copy_to_buffer(shared_buffer_t *, char * message)` performs the following:
	* Lock mutex
	* Copy message to the buffer with memcpy
	* Set is_changed to 1
	* Unlock mutex


4. Within the server's send thread:
	* The thread uses a while loop
	* Each loop, thread_copy_from_buffer is called
	* If result  = 1, then send the copied message to all clients
	* The function calls nanosleep for 1ms to free up the cpu for other processes


4. The function `thread_copy_from_buffer(shared_buffer_t *, char * message)` performs the following:
	* Lock mutex
	* Check to see if is_changed is true
	* If changed, copy buffer to message using memcpy
	* Set is_changed to 0
	* Unlock mutex
	* Return result = 1 if copy done, otherwise 0


Within the client's receive thread, the following is performed
* While loop blocking on recv()
* When message received calls thread_copy_to_buffer


Within the client's main draw thread, the following is performed
* Timer utility calls draw_tick() function every 16 ms
* Call thread_copy_from_buffer 
* If result  = 1, then a new message was copied to the local buffer from the shared memory buffer
* Deserialize message to a sim_data_t struct
* Draw updated state using sim_data_t struct


Through the intelligent use of multiple threads to enable multiple blocking
actions to occur without impacting the time dependent simulation ticks as well 


### Memory Hierarchy, Management, and Cost-Performance Trade-offs

Deep understanding of memory management  was demonstrated in several ways.
First we developed a shared memory buffer that allowed for efficient cross
thread messaging. We designed the functions that used that memory section to
make use of memcpy() to ensure that minimal time was spent within a locked
state.  Furthermore, memory management was demonstrated through the efficient
reuse of memory structures. For example each message received by a client was
parsed and copied into a single struct that was reused for each message.  


### Preemptive and Nonpreemptive scheduling
Scheduling was demonstrated through the development of a Timerloop library
which allowed us to schedule a function call to occur on a regular interval
with sub millisecond accuracy. Using this library we were able to create
a graphics display with a smooth steady framerate. Furthermore, this library
allowed us to drive a physics simulation, generating new state messages at
a regular interval. 

Further understanding and mastery of scheduling was demonstrated through the
use of the select function which allowed a thread to wait until a particular
socket recieved a message.

Finally, we ensured that whenever a while loop was used, we included
a nanosleep() for 1ms to release the cpu for use by other threads or
processes. The result of these implementations and optimizations, is that we
here able to achieve a high level of performance with low cpu usage.


### File System Design and Implementation

Although the Hydra project did not require any file system design, the team
utilized a minimal Raspbian filesystem within a chroot environment, allowing
for simple and precise tracking of all Hydra's dependencies.


Additionally, the use of Autotools and a custom macro or two allowed the Make
files to identify the default locations for configuration files on the target
architecture, ensuring a more platform-agnostic deployment.


Considerations for Effective and Efficient Teamwork.
---------------------------------------------------

Our group functioned exceedingly well as a team.  Our skill sets and
individual experience complimented each other. We each found a role suited to
our capabilities, and everyone worked equally hard to contribute to the
success of our project.

We made use of Agile development methodologies to tackle goals and task in an
efficient, timely, and fair manner. We divided our project into multiple
sprints lasting 2 weeks. We held a sprint planning meeting before each sprint
to add tasks to our Backlog and select the task to be tackle in the new
sprint. We held biweekly online Scrum meetings to keep up to date on each
others progress, and we held weekly Sunday Hackathons to ensure we made steady
progress. Please see the Development Methodology section for more information
on this. We made use of industry standard tools as for collaboration,
communication, and coordination. Our team achieved this professional level of
operation using Trello for task tracking and assignment, Toggl for time
tracking, GitHub for distributed development, made use of video conferencing
for online meetings, and made use of a discussion forum for threaded
messaging. Please see the Collaboration Tools section for more information. 

We have provided extensive documentation of our the highly efficient and
effective operation of our team in the form of team communication log, task
board screenshots, time logs, gantt chart, and git commits.
