#### Use a digispark or similar (any attiny85) as an PPM-to-USBJoystick adapter using any ppm source.

There are some simulator cables out there that you can buy from nearly any shop which names 
are well known. But as the name tells everything: cable. 
Get rid of the cable! Okay, you probably want to get rid of it and now you can! 
I'm using a "MCS digistump digispark" (lsusb) and the corresponding arduino library and also
a Flysky FS-A8S with my FS-i6x. As this project uses the ppm protocol, any ppm receiver should 
work out of the box. Even any transmitter with ppm output should work.


### tested
- fs-a8s in ubuntu and windows
- fs-i6x trainerport (8-ch only; need to read more about ppm)

### changes
- changed some variable names
- added attiny85 code for digispark/trinket
- added more channels (still not sure if stable and supported by the protocol itself)
- changed gap timing to 2500us


### to-do
- *get rid of the jitter*
- find more precise (small) timer-library for the attiny85
- test with other ppm input (DX6i)
- show up as joystick with two analog sticks
- change gamecontroller name
- external components for ???
