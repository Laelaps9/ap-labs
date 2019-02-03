package main

import (
	"fmt"
	"math"
)

type Point struct{ x, y float64 }

func Distance(p, q Point) float64 {
	return math.Hypot(q.x-p.x, q.y-p.y)
}
func (p Point) Distance(q Point) float64 {
	return math.Hypot(q.x-p.x, q.y-p.y)
}

type Path []Point

func (path Path) Distance() float64 {
	sum := 0.0
	for i := range path {
		if i > 0 {
			sum += path[i-1].Distance(path[i])
		}
	}
	return sum
}

func (p Point) X() float64 {
	return p.x
}

func (p Point) Y() float64 {
	return p.y
}

func printPoint(p Point) {
	fmt.Println(p.X())
	fmt.Println(p.Y())
}

func main() {
	p := Point{1, 3}
	printPoint(p)
}
