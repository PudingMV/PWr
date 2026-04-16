mod ring;
use ring::Ring;

fn main() {
    type R5 = Ring<5>;

    let mut a = R5::new(4);
    let b = R5::new(2);

    println!("Initial values:");
    println!("a = {}, b = {}\n", a, b);

    //mathematical operations
    println!("Mathematical operations:");
    println!("a + b = {}", a + b);
    println!("a - b = {}", a - b);
    println!("a * b = {}", a * b);
    println!("a / b = {}\n", a / b);

    //assignment operations
    println!("Assignment operators:");

    a += b;
    println!("a += b -> {}", a);

    a -= b;
    println!("a -= b -> {}", a);

    a *= b;
    println!("a *= b -> {}", a);

    a /= b;
    println!("a /= b -> {}\n", a);

    //comparisons
    println!("Comparisons:");
    println!("a == b -> {}", a == b);
    println!("a != b -> {}", a != b);
    println!("a < b  -> {}", a < b);
    println!("a > b  -> {}", a > b);
    println!("a <= b -> {}", a <= b);
    println!("a >= b -> {}\n", a >= b);

    //Test: -1
    let c = R5::new(-1);

    println!("Test with -1:");
    println!("c -> value assigned: -1 -> value inside ring: {}", c);
}