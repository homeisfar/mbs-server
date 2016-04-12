#### What is this?
Dissatisfied with my [Python HTTP server](https://github.com/homeisfar/webserver), I've decided to write a much better one in C. This server has/will have more features:
- Written in C
- Support persistent TCP connections
- Support multiple connections (single-threaded)
- Support SSL

#### How to build
Coded on my  laptop, but this should compile on any machine that implements BSD sockets.  

`make` to just get a simple binary *mbs-server* (Officially named Much Better Server-Server).  
`make debug` to build with with runtime print statements to stdout and GDB/LLDB debugging.

#### Invocation:
`./mbs-server` with optional command line arguments explained below

**• -p <<port>port>**: Overrides the default port the server should run on. The default is port 8000.  
**• -r <<path>path>**: Sets the root directory of the web server. This is where the files are served from. For example, if the server is run with -root ./networks/website, then a request for http://www.example.com/index.html will result in serving the file ./networks/website/index.html. If this option is not present, then use ./www as the default root directory.  
**• -s <<port>port>**: Listens for an SSL (i.e. TLS) connection on the specified port. The default SSL port is 8001.  

#### Extra
I intend to write good code comments so that curious onlookers can easily follow the structure of the server and, more importantly, the rationale behind decisions I've made. As always, there isn't just one way to do the job. My hope is that perhaps this can serve as good code for people to learn from.

I've pulled the webserver's requirements from an old CMU 15-441 computer networks course. The 16 page project description can be found here https://www.cs.cmu.edu/~srini/15-441/F02/Projects/lab01/docs/lab01.pdf

Copyright © 2016 Ali Homafar  
You can do what you want with this, I guess, so long as you don't hold me liable for any damages. I would not recommend using this in a production environment.
