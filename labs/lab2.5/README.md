# Lab 2.5
## crawl3.go
**Crawl3.go** receives a depth and a link, the program goes into that link and retrieves all the links found in that site,
then it enters each of those links recursively until the depth specified is reached.  
## Compiling and running
To compile this program, you could use `go build`, then to run it type `./crawl3 -depth=[your specified depth] [url to visit]`. If you would like to run it without having a binary file created you can use `go run crawl3.go -depth=[your specified depth] [url to visit]`. You must follow this syntax, otherwise the program will return an error with an example of the correct syntax.
