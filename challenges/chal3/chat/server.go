// Copyright © 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/

// See page 254.
//!+

// Chat is a server that lets clients chat with each other.
package main

import (
	"bufio"
	"fmt"
	"log"
	"net"
	"os"
	"strings"
	"time"
)

//!+broadcaster
type client chan<- string // an outgoing message channel

type user struct {
	ip_port string
	channel chan string
}

var (
	entering = make(chan client)
	leaving  = make(chan client)
	messages = make(chan string) // all incoming client messages
	users = make(map[string]user)
)

func broadcaster() {
	clients := make(map[client]bool) // all connected clients
	for {
		select {
		case msg := <-messages:
			// Broadcast incoming message to all
			// clients' outgoing message channels.
			for cli := range clients {
				cli <- msg
			}

		case cli := <-entering:
			clients[cli] = true

		case cli := <-leaving:
			delete(clients, cli)
			close(cli)
		}
	}
}

//!-broadcaster

//!+handleConn
func handleConn(conn net.Conn) {
	ch := make(chan string) // outgoing client messages
	go clientWriter(conn, ch)

	input := bufio.NewScanner(conn)
	ip := conn.RemoteAddr().String()
	input.Scan()
	who := input.Text()

	if _, ok := users[who]; ok {
		ch <- who + " is already in use. Please choose another username"
	} else {
		users[who] = user{ip_port: ip, channel: ch}

		fmt.Println("irc-server > New connected user [" + who + "]")
		ch <- "irc-server > Welcome to the Simple IRC Server"
		ch <- "irc-server > Your user [" + who + "] is succesfully logged"
		messages <- "New connected user [" + who + "]"
		entering <- ch

		for input.Scan() {
			msg := strings.Split(input.Text(), " ")
			if msg[0] == "/users" {
				if len(msg) == 1 {
					var userList string
					for key, _ := range users  {
						userList += key + ", "
					}
					ch <- "irc-server > " + userList[:len(userList) - 2]
				} else {
					ch <- "Usage: /users"
				}

			} else if msg[0] == "/msg" {
				if len(msg) >= 3 {
					if _, ok := users[msg[1]]; ok {
						users[msg[1]].channel <- "[Direct message from " + who + "] > " + strings.Join(msg[2:], " ")
					} else {
						ch <- "irc-server > User not found"
					}
				} else {
					ch <- "Usage: /msg <user> <message>"
				}

			} else if msg[0] == "/time" {
				if len(msg) == 1 {
					ch <- "irc-server > " + time.Now().Format("15:04:05")
				} else {
					ch <- "irc-server > Usage: /time"
				}

			} else if msg[0] == "/user" {
				if len(msg) == 2 {
					if _, ok := users[msg[1]]; ok {
						ch <- "irc-server > username: " + msg[1] + ", IP: " + users[msg[1]].ip_port
					} else {
						ch <- "irc-server > User not found"
					}
				} else {
					ch <- "irc-server > Usage: /user <username>"
				}

			} else {
				messages <- who + " > " + input.Text()
			}

		}
		delete(users, who)
		fmt.Println("[" + who + "] left")
		leaving <- ch
		messages <- "irc-server > [" + who + "] left"
	}
	conn.Close()
}

func clientWriter(conn net.Conn, ch <-chan string) {
	for msg := range ch {
		_, err := fmt.Fprintln(conn, msg)
		if err != nil {
			log.Fatal(err)
		}
	}
}

//!-handleConn

//!+main
func main() {
	if len(os.Args) != 5 {
		log.Fatal("Usage: ./server -host <host> -port <port #>")
	}

	listener, err := net.Listen("tcp", os.Args[2] + ":" + os.Args[4])
	if err != nil {
		log.Fatal(err)
	}

	fmt.Println("irc-server > Simple IRC Server started at " + os.Args[2] + ":" + os.Args[4])

	go broadcaster()
	fmt.Println("irc-server > Ready for receiving new clients")
	for {
		conn, err := listener.Accept()
		if err != nil {
			log.Print(err)
			continue
		}
		go handleConn(conn)
	}
}

//!-main
