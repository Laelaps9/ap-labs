// Copyright © 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/

// See page 227.

// Netcat is a simple read/write client for TCP servers.
package main

import (
	"io"
	"log"
	"net"
	"os"
)

//!+
func main() {
	if len(os.Args) != 5 {
		log.Fatal("Usage: ./client -user <username> -server <host>:<port>")
	}

	conn, err := net.Dial("tcp", os.Args[4])
	if err != nil {
		log.Fatal(err)
	}
	done := make(chan struct{})
	_, err2 := io.WriteString(conn, os.Args[2] + "\n")
	if err2 != nil {
		log.Fatal(err2)
	}
	go func() {
		_, err := io.Copy(os.Stdout, conn)
		if err != nil {
			log.Fatal(err)
		}

		log.Println("done")
		done <- struct{}{} // signal the main goroutine
	}()
	go func() {
		mustCopy(conn, os.Stdin)
	}()

	<-done // wait for background goroutine to finish
	conn.Close()

}

//!-

func mustCopy(dst io.Writer, src io.Reader) {
	if _, err := io.Copy(dst, src); err != nil {
		log.Fatal(err)
	}
}
