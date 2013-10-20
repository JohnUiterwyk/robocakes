Difficulties Encountered  		{#difficultiesencountered}
========================

During the course of our development, there are three main areas which
presented significant but not insurmountable challenges. These were challenges
related to graphics, networking, and cross platform development. 

Graphics Challenges
-------------------

The graphics component of the Hydra project was implemented in openVG using an
EGL context to draw each window. OpenVG is maintained by the Khronos group but
as was discovered, there is no comprehensive API documentation for the
library. This proved to be a significant barrier for entry. This was
eventually overcome after a significant amount of research including reading
the source code and several examples. 

The EGL library also provided a stumbling block. EGL uses openGL ES for the
display context which requires an embedded system GPU. This proved too
difficult to emulate, and given the lengthy compilation times on the Raspberry
Pi, a frustratingly slow code, run, debug process developed. 

Networking Challenges
---------------------

Developing an application that spans multiple computers greatly increases the
difficulty of debugging. During the course of creating the UDP and later the
TCP component of the application, we found identifying the source of bugs and
problems to be a challenge. The first was to develop each component as
a module, and then develop test applications that implemented the modules in
a simple straightforward manner. Additional test application each
incrementally added a features, allowing us to identify the point at which an
error began to occur.


Another challenge was that due to differences between the Pi and our
development environments, errors that occurred on the Pi would sometimes not
be reproducible on the main PC. This challenge was overcome by setting up the
Pi's as secondary development environments, complete with installation of Git
and vim customizations.


A third challenge we came across was related to to packet loss with UDP. By
its nature, UDP is not reliable, but we were surprised at how consistently the
UDP implementation lost packets. More information about this can be found in
the Testing section of this document. This should not have been surprising due
to the performance constraints of the Pi. We resolved this issue, by
redeveloping the networking component using TCP. This not only resulted in
zero packet loss, it also enabled unified bidirectional communication.
A fourth challenge was related to messages spanning multiple packets, or
multiple message being buffered before they could be read

Cross-Platform Development Challenge
------------------------------------


Autotools was the collection of tools that  the team used to overcome several
issues that arose. The first of these stemmed from the use of the openVG
library for the display function. Several libraries were installed in
non-standard directories by default which resulted in issues with linking.
While these paths could be hardcoded, it made the build undesirably Pi
specific which in turn would have lengthened debugging and testing times
considerably.

The second issue was that early development prototypes relied on glibc which
proved to be prohibitive to install on OSX. Autotools allowed us to examine
the host system before compilation and automatically generate makefiles
appropriate to the platform.  Autotools itself consumed a disproportionate
amount of team time due to a very steep learning curve but it was eventually
successfully implemented. 
