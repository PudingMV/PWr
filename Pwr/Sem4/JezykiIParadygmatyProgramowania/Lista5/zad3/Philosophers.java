import java.util.Random;
import java.util.concurrent.Semaphore;
import java.util.concurrent.locks.ReentrantLock;

public class Philosophers {

    static final int DEFAULT_PHILOSOPHERS = 5;
    static final int DEFAULT_MEALS = 5;

    // ===== STATYSTYKI =====
    static class Stats {
        private final int[] data;
        private final Object lock = new Object();

        Stats(int n) {
            data = new int[n];
        }

        void set(int id, int value) {
            synchronized (lock) {
                data[id] = value;
            }
        }

        int get(int id) {
            synchronized (lock) {
                return data[id];
            }
        }
    }

    // ===== FORK =====
    static class Fork {
        final ReentrantLock lock = new ReentrantLock();
    }

    // ===== WAITER =====
    static class Waiter {
        private final Semaphore seats;

        Waiter(int max) {
            seats = new Semaphore(max);
        }

        void requestSeat() throws InterruptedException {
            seats.acquire();
        }

        void leaveSeat() {
            seats.release();
        }
    }

    // ===== PHILOSOPHER =====
    static class Philosopher extends Thread {
        private final int id;
        private final Fork left;
        private final Fork right;
        private final Stats stats;
        private final Waiter waiter;
        private final Random rand;
        private final int meals;

        Philosopher(int id, Fork left, Fork right,
                     Stats stats, Waiter waiter, int meals) {
            this.id = id;
            this.left = left;
            this.right = right;
            this.stats = stats;
            this.waiter = waiter;
            this.meals = meals;
            this.rand = new Random();
        }

        void think() throws InterruptedException {
            System.out.println("Filozof " + (id + 1) + " myśli nad sensem życia.");
            Thread.sleep((rand.nextInt(3) + 1) * 100L);
        }

        void eat(int meal) throws InterruptedException {
            System.out.println("Filozof " + (id + 1)
                    + " zjada crispy chicken bacon burgera numer " + meal);
            Thread.sleep((rand.nextInt(3) + 1) * 100L);
        }

        @Override
        public void run() {
            int failed = 0;

            for (int meal = 1; meal <= meals; meal++) {
                try {
                    think();

                    System.out.println("Filozof " + (id + 1) + " jest głodny i próbuje zjeść.");
                    waiter.requestSeat();

                    boolean leftTaken = left.lock.tryLock();
                    if (!leftTaken) {
                        failed++;
                        System.out.println("Filozof " + (id + 1) + " nie podniósł lewego widelca.");
                        waiter.leaveSeat();
                        continue;
                    }

                    boolean rightTaken = right.lock.tryLock();
                    if (!rightTaken) {
                        failed++;
                        System.out.println("Filozof " + (id + 1) + " nie podniósł prawego widelca.");
                        left.lock.unlock();
                        waiter.leaveSeat();
                        continue;
                    }

                    eat(meal);

                    left.lock.unlock();
                    right.lock.unlock();
                    waiter.leaveSeat();

                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                }
            }

            stats.set(id, failed);
        }
    }

    // ===== MAIN =====
    public static void main(String[] args) throws InterruptedException {

        int numPhilosophers = DEFAULT_PHILOSOPHERS;
        int meals = DEFAULT_MEALS;

        if (args.length >= 2) {
            numPhilosophers = Integer.parseInt(args[0]);
            meals = Integer.parseInt(args[1]);
        }

        Fork[] forks = new Fork[numPhilosophers];
        for (int i = 0; i < numPhilosophers; i++) {
            forks[i] = new Fork();
        }

        Stats stats = new Stats(numPhilosophers);
        Waiter waiter = new Waiter(numPhilosophers - 1);

        Philosopher[] philosophers = new Philosopher[numPhilosophers];

        for (int i = 0; i < numPhilosophers; i++) {
            philosophers[i] = new Philosopher(
                    i,
                    forks[i],
                    forks[(i + 1) % numPhilosophers],
                    stats,
                    waiter,
                    meals
            );
            philosophers[i].start();
        }

        for (Philosopher p : philosophers) {
            p.join();
        }

        System.out.println("\nStatystyki");
        for (int i = 0; i < numPhilosophers; i++) {
            System.out.println("Filozof " + (i + 1)
                    + " tyle razy obszedł się smakiem = " + stats.get(i));
        }
    }
}