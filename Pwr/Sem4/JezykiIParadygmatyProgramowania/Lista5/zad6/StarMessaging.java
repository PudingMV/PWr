import java.util.*;
import java.util.concurrent.*;
import java.util.concurrent.atomic.AtomicInteger;

public class StarMessaging {

    static int NUM_USERS;
    static int MESSAGES_PER_USER;

    static Random rng = new Random();

    // ---------------- SMALL DELAY ----------------
    static void smallDelay() {
        try {
            Thread.sleep(30);
        } catch (InterruptedException ignored) {}
    }

    // ---------------- COUNTER ----------------
    static class Counter {
        private final int[] data;

        public Counter(int size) {
            data = new int[size];
        }

        public synchronized void inc(int id) {
            data[id - 1]++;
        }

        public synchronized int get(int id) {
            return data[id - 1];
        }
    }

    // ---------------- COMPLETION ----------------
    static class Completion {
        private int count = 0;
        private int expected = 0;

        public synchronized void init(int expected) {
            this.expected = expected;
            this.count = 0;
        }

        public synchronized void oneDone() {
            count++;
        }

        public synchronized boolean done() {
            return count == expected;
        }
    }

    // ---------------- SERVER ----------------
    static class Server implements Runnable {

        static class Message {
            int from;
            int to;
            int msg;

            Message(int from, int to, int msg) {
                this.from = from;
                this.to = to;
                this.msg = msg;
            }
        }

        private final BlockingQueue<Message> queue = new LinkedBlockingQueue<>();
        private final Counter counter;
        private volatile boolean running = true;

        public Server(Counter counter) {
            this.counter = counter;
        }

        public void send(int from, int to, int msg) {
            queue.add(new Message(from, to, msg));
        }

        public void stop() {
            running = false;
        }

        @Override
        public void run() {
            try {
                while (running || !queue.isEmpty()) {
                    Message m = queue.poll(50, TimeUnit.MILLISECONDS);

                    if (m != null) {
                        System.out.printf("SERVER: %d -> %d msg = %d%n",
                                m.from, m.to, m.msg);

                        counter.inc(m.to);
                        smallDelay();
                    }
                }
            } catch (InterruptedException ignored) {}

            System.out.println("SERVER STOPPED");
        }
    }

    // ---------------- USER ----------------
    static class User implements Runnable {
        int id;
        Server server;
        Completion completion;

        User(int id, Server server, Completion completion) {
            this.id = id;
            this.server = server;
            this.completion = completion;
        }

        @Override
        public void run() {
            for (int i = 1; i <= MESSAGES_PER_USER; i++) {

                int target = (rng.nextInt(NUM_USERS) % NUM_USERS) + 1;

                System.out.printf("USER %d -> USER %d%n", id, target);

                server.send(id, target, i);
                smallDelay();
            }

            System.out.printf("USER %d finished%n", id);
            completion.oneDone();
        }
    }

    // ---------------- MAIN ----------------
    public static void main(String[] args) throws Exception {

        if (args.length != 2) {
            System.out.println("Usage: java StarMessaging <users> <messages>");
            return;
        }

        NUM_USERS = Integer.parseInt(args[0]);
        MESSAGES_PER_USER = Integer.parseInt(args[1]);

        System.out.println("START");

        Counter counter = new Counter(NUM_USERS);
        Completion completion = new Completion();
        completion.init(NUM_USERS);

        Server server = new Server(counter);
        Thread serverThread = new Thread(server);
        serverThread.start();

        List<Thread> users = new ArrayList<>();

        for (int i = 1; i <= NUM_USERS; i++) {
            Thread t = new Thread(new User(i, server, completion));
            users.add(t);
            t.start();
        }

        // wait for users
        while (!completion.done()) {
            Thread.sleep(10);
        }

        Thread.sleep(100);

        server.stop();
        serverThread.join();

        System.out.println("\nRESULTS");
        for (int i = 1; i <= NUM_USERS; i++) {
            System.out.printf("USER %d received: %d%n", i, counter.get(i));
        }

        System.out.println("DONE");
    }
}