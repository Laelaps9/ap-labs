package main

import (
	"fmt"
	"os"
	"path/filepath"
)

// scanDir stands for the directory scanning implementation
func scanDir(dir string) error {

	directories, symbolic, other := 0, 0, 0
	filepath.Walk(dir, func(path string, info os.FileInfo, err error) error {
		fmt.Println("File: ", path)
		file, _ := os.Lstat(path)
		if info.IsDir() {
			directories++
		} else if file.Mode() & os.ModeSymlink == os.ModeSymlink{
			symbolic++
		} else {
			other++
		}
		return nil
	})
	fmt.Printf("Directories %d\nSymbolic Links %d\nOther files %d\n", directories, symbolic, other)
	return nil
}

func main() {
	if len(os.Args) < 2 {
		fmt.Println("Usage: ./dir-scan <directory>")
		os.Exit(1)
	}

	scanDir(os.Args[1])
}
