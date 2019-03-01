// Clock2 is a concurrent TCP server that periodically writes the time.
package main

import (
	"io"
	"log"
	"net"
	"time"
	"os"
	"strings"
)

func handleConn(c net.Conn) {
	defer c.Close()
	tz := os.Getenv("TZ")
	timestamp, err := time.LoadLocation(tz)

	if(err != nil) {
		log.Fatal(err)
	}

	for {
		_, err := io.WriteString(c, tz + strings.Repeat(" ", 15 - len(tz)) + ":  " + time.Now().In(timestamp).Format("15:04:05\n"))
		if err != nil {
			return // e.g., client disconnected
		}
		time.Sleep(1 * time.Second)
	}
}

func main() {
	port := os.Args[2]
	listener, err := net.Listen("tcp", "localhost:" + port)
	if err != nil {
		log.Fatal(err)
	}
	for {
		conn, err := listener.Accept()
		if err != nil {
			log.Print(err) // e.g., connection aborted
			continue
		}
		go handleConn(conn) // handle connections concurrently
	}
}
