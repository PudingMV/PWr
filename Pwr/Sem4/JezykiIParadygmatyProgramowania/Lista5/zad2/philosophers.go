package main

import (
	"fmt"
	"math/rand"
	"os"
	"strconv"
	"sync"
	"time"
)

const DefaultNumPhilosophers = 5
const DefaultMealsPerPhilosopher = 5

type Stats struct {
	mu   sync.Mutex
	data []int
}

func NewStats(n int) *Stats {
	return &Stats{
		data: make([]int, n),
	}
}

func (s *Stats) Set(id int, value int) {
	s.mu.Lock()
	defer s.mu.Unlock()
	s.data[id] = value
}

func (s *Stats) Get(id int) int {
	s.mu.Lock()
	defer s.mu.Unlock()
	return s.data[id]
}

type Fork struct {
	mu sync.Mutex
}

type Waiter struct {
	seat chan struct{}
}

func NewWaiter(max int) *Waiter {
	return &Waiter{
		seat: make(chan struct{}, max),
	}
}

func (w *Waiter) RequestSeat() {
	w.seat <- struct{}{}
}

func (w *Waiter) LeaveSeat() {
	<-w.seat
}

type Philosopher struct {
	id     int
	left   *Fork
	right  *Fork
	stats  *Stats
	waiter *Waiter
	rng    *rand.Rand
}

func (p *Philosopher) think() {
	fmt.Printf("Filozof %d myśli nad sensem życia.\n", p.id+1)
	time.Sleep(time.Duration(p.rng.Intn(3)+1) * 100 * time.Millisecond)
}

func (p *Philosopher) eat(meal int) {
	fmt.Printf("Filozof %d zjada burgera numer %d\n",
		p.id+1, meal)
	time.Sleep(time.Duration(p.rng.Intn(3)+1) * 100 * time.Millisecond)
}

func (p *Philosopher) run(meals int, wg *sync.WaitGroup) {
	defer wg.Done()

	failed := 0

	for meal := 1; meal <= meals; meal++ {
		p.think()

		fmt.Printf("Filozof %d jest głodny i próbuje zjeść.\n", p.id+1)
		p.waiter.RequestSeat()

		leftCh := make(chan bool, 1)
		rightCh := make(chan bool, 1)

		go func() {
			locked := p.left.mu.TryLock()
			leftCh <- locked
		}()

		select {
		case ok := <-leftCh:
			if !ok {
				failed++
				fmt.Printf("Filozof %d nie podniósł lewego widelca.\n", p.id+1)
				p.waiter.LeaveSeat()
				continue
			}
		case <-time.After(100 * time.Millisecond):
			failed++
			fmt.Printf("Filozof %d za wolny (lewy widelec).\n", p.id+1)
			p.waiter.LeaveSeat()
			continue
		}

		go func() {
			locked := p.right.mu.TryLock()
			rightCh <- locked
		}()

		select {
		case ok := <-rightCh:
			if !ok {
				failed++
				fmt.Printf("Filozof %d nie podniósł prawego widelca.\n", p.id+1)
				p.left.mu.Unlock()
				p.waiter.LeaveSeat()
				continue
			}
		case <-time.After(100 * time.Millisecond):
			failed++
			fmt.Printf("Filozof %d za wolny (prawy widelec).\n", p.id+1)
			p.left.mu.Unlock()
			p.waiter.LeaveSeat()
			continue
		}

		p.eat(meal)

		p.left.mu.Unlock()
		p.right.mu.Unlock()
		p.waiter.LeaveSeat()
	}

	p.stats.Set(p.id, failed)
}

func main() {
	numPhilosophers := DefaultNumPhilosophers
	meals := DefaultMealsPerPhilosopher

	if len(os.Args) >= 3 {
		numPhilosophers, _ = strconv.Atoi(os.Args[1])
		meals, _ = strconv.Atoi(os.Args[2])
	}

	rng := rand.New(rand.NewSource(time.Now().UnixNano()))

	forks := make([]*Fork, numPhilosophers)
	for i := range forks {
		forks[i] = &Fork{}
	}

	stats := NewStats(numPhilosophers)
	waiter := NewWaiter(numPhilosophers - 1)

	var wg sync.WaitGroup

	for i := 0; i < numPhilosophers; i++ {
		p := &Philosopher{
			id:     i,
			left:   forks[i],
			right:  forks[(i+1)%numPhilosophers],
			stats:  stats,
			waiter: waiter,
			rng:    rand.New(rand.NewSource(rng.Int63())),
		}

		wg.Add(1)
		go p.run(meals, &wg)
	}

	wg.Wait()

	fmt.Println("\nStatystyki")
	for i := 0; i < numPhilosophers; i++ {
		fmt.Printf("Filozof %d tyle razy obszedł się smakiem = %d\n",
			i+1, stats.Get(i))
	}
}