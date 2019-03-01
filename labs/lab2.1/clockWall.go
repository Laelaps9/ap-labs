package main

import (
	"io"
	"log"
	"net"
	"os"
	"strings"
	"sync"
)

func connect(conn net.Conn, zone string, wait *sync.WaitGroup) {
	for {
		_, err := io.Copy(os.Stdout, conn)
		if err == nil {
			break
		} else {
			log.Fatal(err)
		}
	}
	wait.Done()
}

func main () {
	var wait sync.WaitGroup

	// Loops to get all the parameters passed on as timezones and ports
	for _, param := range os.Args[1:] {
		// Splits each parameter into zone and port to broadcast to
		splitParam:= strings.Split(param, "=")
		zone := splitParam[0]
		port := splitParam[1]
		conn, err := net.Dial("tcp", port)
		if err != nil {
			log.Fatal(err)
		}
		// Adds a goroutine to the waiting group
		wait.Add(1)
		go connect(conn, zone, &wait)
	}
	// Waits for all the goroutines to end
	wait.Wait()
}
