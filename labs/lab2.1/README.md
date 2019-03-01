# Lab 2.1
## clock2.go
**clock2.go** initializes a server in the specified port and with the specified timezone.  
## clockWall.go
**clockWall.go** receives n number of parameters that specify the city and port to listen to.  
## Compiling and running  
To compile type `make` to compile in terminal, this creates 2 binaries. **clock2** and **clockWall**. To open a port with a specified port follow
the format `TZ=[Zone] .clock2 -port [port to open] &`. To listen to a port type `clockWall [City]=localhost:[port number]`.  
## Warning
The *&* symbol at the end of the **clock2** will make the process to continue running in the background. This means that they will continue to send information
to a port even if no one is listening.
