use rand::rngs::StdRng;
use rand::{Rng, SeedableRng};
use std::time::{SystemTime, UNIX_EPOCH};

use crate::ring::Ring;

pub struct DHSetup<T> {
    generator: T,
}

impl<const N: u64> DHSetup<Ring<N>> {
    pub fn new() -> Self {
        let seed = SystemTime::now()
            .duration_since(UNIX_EPOCH)
            .unwrap()
            .as_secs();

        let mut rng = StdRng::seed_from_u64(seed);

        let generator = Self::find_generator(&mut rng);

        Self { generator }
    }

    pub fn get_generator(&self) -> Ring<N> {
        self.generator
    }

    pub fn power(&self, a: Ring<N>, mut b: u64) -> Ring<N> {
        let mut result = Ring::new(1);
        let mut base = a;

        while b > 0 {
            if b & 1 == 1 {
                result = result * base;
            }
            base = base * base;
            b >>= 1;
        }

        result
    }

    fn prime_factors(mut n: u64) -> Vec<u64> {
        let mut factors = Vec::new();

        let mut i = 2;
        while i * i <= n {
            if n % i == 0 {
                factors.push(i);
                while n % i == 0 {
                    n /= i;
                }
            }
            i += 1;
        }

        if n > 1 {
            factors.push(n);
        }

        factors
    }

    fn random_element<R: Rng>(rng: &mut R) -> Ring<N> {
        let v = rng.gen_range(2..(N - 1)) as i64;
        Ring::new(v)
    }

    fn find_generator<R: Rng>(rng: &mut R) -> Ring<N> {
        for _ in 0..10000 {
            let candidate = Self::random_element(rng);
            if Self::is_generator_static(candidate) {
                return candidate;
            }
        }
        panic!("No generator found");
    }

    fn is_generator_static(a: Ring<N>) -> bool {
        let p_minus_1 = N - 1;
        let factors = Self::prime_factors(p_minus_1);

        for q in factors {
            let exp = p_minus_1 / q;
            let mut r = Ring::new(1);
            for _ in 0..exp {
                r = r * a;
            }
            if r.get() == 1 {
                return false;
            }
        }
        true
    }
}