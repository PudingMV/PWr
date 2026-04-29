mod ring;
mod rsa;

use ring::Ring;
use rsa::RSA;

fn main() {
    const P: i32 = 10007;
    const Q: i32 = 10009;
    const N: i32 = P * Q;

    let rsa = RSA::<N>::new(P, Q);

    let message = Ring::<N>::new(1234);

    let encrypted = rsa.encrypt(message);
    let decrypted = rsa.decrypt(encrypted);

    println!("Modulo n: {}", rsa.get_modulo());
    println!("Public key: {}", rsa.get_public_key());

    println!("Message: {}", message);
    println!("Encrypted: {}", encrypted);
    println!("Decrypted: {}", decrypted);

    if message == decrypted {
        println!("OK");
    } else {
        println!("BLAD");
    }
}