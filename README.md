Dissatisfied with my Python server, I've decided to write a much better one in C. Goodness, I've never read so many man pages in my life.

Differences:

- Written in C
- Support persistent TCP connections
- Support multiple connections (still single-threaded)
- Support SSL

Coded on my  laptop, but this should compile on any machine that implements BSD sockets.

`make` to just get a simple binary mbs-server (my better server server).
`make debug` for print statements and GDB/LLDB debugging.

I intend to write good comments so that curious onlookers can easily follow the structure of the server and, more importantly, the rationale behind decisions made. Code is too immature for this atm.

Copyright © 2016 Ali Homafar
You can do what you want with this, I guess. I don't think students from the 2002 CMU 15-441 course are gonna cheat with this repo.
