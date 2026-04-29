mod ring;
mod dhsetup;
mod user;

use ring::Ring;
use dhsetup::DHSetup;
use user::User;

fn main() {
    type Field = Ring<1234567891>;

    let dh = DHSetup::<Field>::new();

    let mut alice = User::<1234567891>::new(&dh);
    let mut bob = User::<1234567891>::new(&dh);

    let a = alice.get_public_key();
    let b = bob.get_public_key();

    alice.set_key(b);
    bob.set_key(a);

    println!("Public key Alice: {}", a);
    println!("Public key Bob:   {}", b);

    let message = Field::new(12345);

    let encrypted = alice.encrypt(message);
    let decrypted = bob.decrypt(encrypted);

    println!("Original:  {}", message);
    println!("Encrypted: {}", encrypted);
    println!("Decrypted: {}", decrypted);

    if message == decrypted {
        println!("SUCCESS: Diffie-Hellman works!");
    } else {
        println!("FAILURE!");
    }
}