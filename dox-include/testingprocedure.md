Testing 					{#testingprocedure}
=======

Graphics Performance Experiments
--------------------------------

One of the configurable options included is the ability to print the frames
per second from within the display library. This allowed the team to measure
the impact of tick frequency and number of objects in the game world on the
frame rate. Unfortunately, running the application with the debug flag affects
the testing cycle itself as output to stdout makes a noticeable difference on
performance. 

The structure of the TCP module also affected the graphics display.
Experimentation revealed that the server was capable of sending faster than
the client could receive, deserialize and draw the frame. The messages were
buffered and due to time constraints the TCP module was designed to drop the
messages and draw the most recent frame. This resulted in jumps that were
detectable to the viewer. 

This was patched by manually setting the client applications to update with
a greater frequency than the server. The resulting frame rate that we recorded
was in the range of 45-50 frames per second, providing a visually smooth
experience for the viewer.


timeloop_lib Experiments
------------------------

The timeloop lib provided the ability to call a particular function on a set
interval. During the course of our development we performed an experiment to
establish the accuracy of using nanosleep for timing.  This experiment was
configured to call a function that printed out the time difference between the
current and previous tick. During our test we found that the time taken to
perform a loop resulted in the accuracy of the timing drifting accordingly. We
modified the experiment to attempt to include the running time of the current
tick in the sleep time, but even in this case, the timing accuracy drifted
either consistently in the positive or consistently in the negative direction.

Despite this outcome, we concluded that for the purpose of creating a smooth
animation, if the timing varied slightly, it would not be perceptible to the
human eye.


UDP Experiments
---------------

The UDP component made use of UDP broadcast to perform one way messaging. To
test this functionality, we developed a simple application that sends out
messages a particular interval; at the same time we setup a client version of
the application to receive and print those messages. Using this test setup, we
first slowly increased the size of the message until the message failed to be
received. We found that above 512 bytes message delivery would begin to be
unreliable. 

Next using a simple short message, we slowly increased the rate at which
messages where dispatched until the client began to miss messages. We found
that message delivery became inconsistent about 20 messages per second. As we
increased the rate, message loss increased as well.

