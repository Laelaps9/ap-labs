package main

import (
	"fmt"
	"log"
	"os"
	"strings"
	"strconv"
	"gopl.io/ch5/links"
)

var tokens = make(chan struct{}, 20)

func crawl(url string) []string {
	fmt.Println(url)
	tokens <- struct{}{} // acquire a token
	list, err := links.Extract(url)
	<-tokens // release the token

	if err != nil {
		log.Print(err)
	}
	return list
}

var seen = make(map[string] bool)

func crawler(depth int, url string, done chan bool) {
	visited, ok := seen[url]
	if depth <= 0 || (visited && ok) {
		done <- true
		return
	}
	seen[url] = true
	linkList := crawl(url)
	linksVisited := make(chan bool)

	for _, link := range linkList {
		go crawler (depth - 1, link, linksVisited)
		<-linksVisited
	}
	done <- true
}

func main() {
	if len(os.Args) <= 2 {
		log.Fatal("Usage: ./crawl3.go -depth=3 https:://google.com")
	}
	depthInput := strings.Split(os.Args[1], "=")
	if (strings.Compare(depthInput[0], "-depth") != 0) || (len(depthInput) != 2) {
		log.Fatal("Usage: ./crawl3.go -depth=3 https:://google.com")
	}
	depth, err := strconv.Atoi(depthInput[1])
	if err != nil {
		log.Fatal(err)
	}
	url := os.Args[2]
	done := make(chan bool)
	seen[url] = false
	go crawler(depth, url, done)
	<-done
}
