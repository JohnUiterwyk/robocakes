Development Methodology   				{#devmethodology}
=======================

Our team operated using a methodology based on core Agile principles. We
borrowed several concepts and methods from Scrum and XP to facilitate an
iterative and incremental approach to developing the Hydra application. 

We made use of a sprint based development schedule which timeboxed each
development effort to 2 weeks. We held sprint planning meetings before each
sprint, during which we would add new tasks or user stories to the Product
Backlog, agree upon the goals for the next sprint, and select the tasks and
user stories for the next sprint, which would become our Sprint Backlog. Task
and user story tracking was facilitated using the online tool Trello. We then
would self assign to different tasks, updating their status in Trello as we
made progress. 

We held Bi-weekly Scrum meetings using the Google Hangout video conferencing
tool to update each other on our progress or any issues we were having. Each
Sunday we met in person for a 6-8 hour "hackathons". During these times we
would often make use of the XP practice of pair programming, especially when
tackling challenging and complex problems such as working with multithreading
networking, openVG, or configuring Automake. 

Programming Languages
---------------------


The Robocakes team decided to implement the Hydra project in C, giving us the
opportunity to work as close to the operating system and hardware as possible,
while still giving us a realistic chance of achieving our goals. C is the
lingua-franca of the embedded Linux world, so the team felt C was the most
appropriate language from a cultural point of view. The few test
implementations of OpenVG projects that the team were able to find for the Pi
were implemented in C, which gave us an additional incentive choose that
language as opposed to other, less involved languages.

However, some incidental coding, not related to the main work, was required.
Configuring build environments required a minor amount of bash scripting to
automate the process for each team member. The team created display mockups in
flash and python early in development to test message de-serialization.


Coding Style
------------

Coding style for this project has been guiding by the Linux Kernel coding
style, since this is a project targeted at Linux. The advantage of using the
Kernel style guide is that while it may be occasionally barmy (8 spaces per
tab?), it is *very* opinionated, so there is never any cause for confusion.

Development Tools
-----------------

### VIM

VIM was the primary source code editor when working on a Linux host. It
allowed the team to use the same development environment when working on
a development machine or on a Raspberry Pi and share indentation rules between
all team members to ensure adherence to the agreed-upon styleguide.

A selection of plugins were added to VIM (using pathogen) to allow git
integration, build the project and run tests all from inside the editor.


### Xcode
Given that some team members used the Mac OSX for their primary operating
system, they chose to make use of the Xcode IDE. Xcode allowed these team
members to take advantage of the code completion and debugging features of the
IDE. For some circumstances, Xcode was configured to make use of the Makefile
to perform compilation.


### gcc
gcc with the -Wall option provided a good selection of warnings on
compilation, but the team decided to increase the coverage by adding several
other warning flags at compilation. The use of Autotools allowed us to pass
a flag at compile time that would enable debugging information in the code and
compile with the additional warnings enabled. This flag also disabled
optimizations performed by the compiler, which made for easier debugging in
GDB.


### Autotools
Autotools allowed the team to conditionally import functionality based on
platform availability. Several of the libraries used for Hydra were installed
in non-standard locations on the Pi, so the libtool component of Autotools
allowed the team to simply import the libraries as normal, and let Autotools
deal with the library locations.

AutoMake provided incremental builds, which led to faster compile times on the
Pi, since only sources that had changed had to be recompiled. However, the
incremental builds with explicit dependencies (provided by AutoMake) meant
that compilation was performed on all updated source files.


### GDB
GDB was used to examine program crashes and logical errors during development.
The ability to print a backtrace after a crash was invaluable in understanding
the circumstances in which crashes occurred.


### Valgrind
Valgrind was used to check memory usage in Hydra. With our testing programs,
each module could be checked in isolation to isolate memory leaks very
specifically.


### chroot and qemu
To provide each team member with an emulated Pi environment, a chroot
filesystem based on Raspbian was created, and qemu was used to emulate the
armhf architecture. This allowed the team to work on their own workstations,
rapidly make and test code without the inconvenience of copying files over to
another device. It also allowed each member to use their preferred development
environment on the platform they were most comfortable working on.

Collaboration Tools
-------------------

### Trello

Trello was used for tracking the Product Backlog, Sprint Backlog, current
tasks, completed tasks, and task assignment. An archive of our Trello board
can be found in the appendix.


### Git and GitHub
Git was used for source control and to support distributed development, as
well as for deploying code to the Raspberry Pi. Additionally GitHub Issues was
used to track bugs and errors. Bugs and error resolution were handled using
self assignment. 


### Google Groups
A dedicated Google Group was created to provided threaded discussions.
Additionally we made use of this to enable a group email list functionality.
This discussion forum was used to share links, record brain dumps of project
ideas, announce important information to team members, and to occasionally
coordinate meeting times. An archive of our Google group can be found in the
appendix.


### Google Chat
Google chat was used extensively for group communication. We created a group
chat channel that allowed all messages to be received by all team members.
Chat was used regularly to discuss ongoing issues, plan meetings, and notify
team members of important information.  An archive of our Google Chats can be
found in the appendix.


### Google Hangout
Google Hangouts were used regularly to hold online video conference meetings.
We made use of this for our bi-weekly Scrum meetings as well as for times when
we would be collaborating on code remotely. 


### Google Drive
Google Drive was used extensively for file sharing and collaborative document
editing. This enabled all team members to simultaneously work on documents.
The ability to see other team members typing in real time combined with google
hangout video conferencing allowed for collaborative authoring of content.


###Byobu
Byobu is a wrapper for screen and tmux. This tool allowed multiple team
members to simultaneously work on the same terminal prompt. This enabled team
members who had more linux experience and knowledge to assist others.

