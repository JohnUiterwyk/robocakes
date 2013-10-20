General Constraints 				{#constraints}
===================

Our project was designed specifically for the Raspberry Pi and tailored to be
appropriate considering the nature of the device.

The CPU on the Pi is relatively low-powered and we planned for this by
ensuring that our graphics library took advantage of the onboard GPU, rather
than relying on the CPU. We considered several different options, finally
settling on openVG. This allowed us to take advantage of the GPU for the most
resource intensive module of the project.

Our solution depends on the position of the monitor in relation to other
monitors. Since this is not determinable programmatically, screen position is
set by command line arguments or via a configuration file on each device. 

Time was a significant constraint on our project. Our team initially planned
to implement a playable game, with alternative input devices. During the
initial planning phase however, the team underestimated the amount of
necessary research and the issues inherent in cross platform development.
These issues are examined in more detail in [difficulties].


[difficulites]: @ref difficultiesencoutered "Difficulties Encountered"
