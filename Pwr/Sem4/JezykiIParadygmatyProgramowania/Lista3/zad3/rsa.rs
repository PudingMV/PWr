use crate::ring::Ring;

pub struct RSA<const N: i32> {
    n: i32,
    phi: i32,
    e: i32,
    d: i32,
}

impl<const N: i32> RSA<N> {
    pub fn new(p: i32, q: i32) -> Self {
        let n = p * q;
        let phi = (p - 1) * (q - 1);

        let e = Self::find_coprime(phi);
        let d = Self::mod_inverse(e, phi);

        Self { n, phi, e, d }
    }

    pub fn get_modulo(&self) -> i32 {
        self.n
    }

    pub fn get_public_key(&self) -> i32 {
        self.e
    }

    pub fn encrypt(&self, m: Ring<N>) -> Ring<N> {
        Self::mod_pow(m, self.e)
    }

    pub fn decrypt(&self, c: Ring<N>) -> Ring<N> {
        Self::mod_pow(c, self.d)
    }

    fn mod_pow(mut base: Ring<N>, mut exp: i32) -> Ring<N> {
        let mut result = Ring::<N>::new(1);

        while exp > 0 {
            if exp % 2 == 1 {
                result = result * base;
            }
            base = base * base;
            exp /= 2;
        }

        result
    }

    fn gcd(mut a: i32, mut b: i32) -> i32 {
        while b != 0 {
            let t = b;
            b = a % b;
            a = t;
        }
        a
    }

    fn find_coprime(phi: i32) -> i32 {
        for i in 3..phi {
            if Self::gcd(i, phi) == 1 {
                return i;
            }
        }
        panic!("No coprime found");
    }

    fn mod_inverse(a: i32, m: i32) -> i32 {
        let (mut t, mut new_t) = (0, 1);
        let (mut r, mut new_r) = (m, a);

        while new_r != 0 {
            let q = r / new_r;

            let temp_t = t - q * new_t;
            t = new_t;
            new_t = temp_t;

            let temp_r = r - q * new_r;
            r = new_r;
            new_r = temp_r;
        }

        if r != 1 {
            panic!("No inverse");
        }

        if t < 0 {
            t += m;
        }

        t
    }
}