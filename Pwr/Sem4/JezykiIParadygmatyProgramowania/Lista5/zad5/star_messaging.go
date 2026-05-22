package main

import (
	"fmt"
	"math/rand"
	"os"
	"strconv"
	"sync"
	"time"
)

var (
	NUM_USERS         int
	MESSAGES_PER_USER int

	rng = rand.New(rand.NewSource(time.Now().UnixNano()))
)

// --- small delay ---
func SmallDelay() {
	time.Sleep(30 * time.Millisecond)
}

// --- COUNTER (protected) ---
type Counter struct {
	mu   sync.Mutex
	data []int
}

func NewCounter(size int) *Counter {
	return &Counter{
		data: make([]int, size),
	}
}

func (c *Counter) Inc(id int) {
	c.mu.Lock()
	c.data[id-1]++
	c.mu.Unlock()
}

func (c *Counter) Get(id int) int {
	c.mu.Lock()
	defer c.mu.Unlock()
	return c.data[id-1]
}

// --- SERVER ---

type SendRequest struct {
	From int
	To   int
	Msg  int
}

type Server struct {
	sendCh  chan SendRequest
	stopCh  chan struct{}
	counts  *Counter
	wg      sync.WaitGroup
	stopped bool
}

func NewServer(c *Counter) *Server {
	return &Server{
		sendCh: make(chan SendRequest),
		stopCh: make(chan struct{}),
		counts: c,
	}
}

func (s *Server) Start() {
	s.wg.Add(1)

	go func() {
		defer s.wg.Done()

		for {
			select {
			case req := <-s.sendCh:
				fmt.Printf("SERVER: %d -> %d msg = %d\n",
					req.From, req.To, req.Msg)

				s.counts.Inc(req.To)
				SmallDelay()

			case <-s.stopCh:
				fmt.Println("SERVER STOPPED")
				return
			}
		}
	}()
}

func (s *Server) Send(from, to, msg int) {
	s.sendCh <- SendRequest{From: from, To: to, Msg: msg}
}

func (s *Server) Stop() {
	close(s.stopCh)
	s.wg.Wait()
}

// --- COMPLETION ---

type Completion struct {
	mu       sync.Mutex
	count    int
	expected int
}

func (c *Completion) Init(expected int) {
	c.mu.Lock()
	c.count = 0
	c.expected = expected
	c.mu.Unlock()
}

func (c *Completion) OneDone() {
	c.mu.Lock()
	c.count++
	c.mu.Unlock()
}

func (c *Completion) Done() bool {
	c.mu.Lock()
	defer c.mu.Unlock()
	return c.count == c.expected
}

// --- USER TASK ---

func userTask(id int, server *Server, done *Completion) {
	for i := 1; i <= MESSAGES_PER_USER; i++ {
		target := (rng.Intn(NUM_USERS) % NUM_USERS) + 1

		fmt.Printf("USER %d -> USER %d\n", id, target)

		server.Send(id, target, i)
		SmallDelay()
	}

	fmt.Printf("USER %d finished\n", id)
	done.OneDone()
}

// --- MAIN ---

func main() {
	if len(os.Args) != 3 {
		fmt.Println("Usage: ./star_messaging <users> <messages>")
		return
	}

	NUM_USERS, _ = strconv.Atoi(os.Args[1])
	MESSAGES_PER_USER, _ = strconv.Atoi(os.Args[2])

	fmt.Println("START")

	counts := NewCounter(NUM_USERS)
	server := NewServer(counts)
	server.Start()

	done := &Completion{}
	done.Init(NUM_USERS)

	// start users
	for i := 1; i <= NUM_USERS; i++ {
		go userTask(i, server, done)
	}

	// wait until all users finish
	for !done.Done() {
		time.Sleep(10 * time.Millisecond)
	}

	time.Sleep(100 * time.Millisecond)

	server.Stop()

	fmt.Println("\nRESULTS")
	for i := 1; i <= NUM_USERS; i++ {
		fmt.Printf("USER %d received: %d\n", i, counts.Get(i))
	}

	fmt.Println("DONE")
}