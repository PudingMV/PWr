import System.Environment (getArgs)

isPrime :: Integer -> Bool
isPrime n = n > 1 && null [x | x <- [2..floor (sqrt (fromIntegral n))], n `mod` x == 0]

primes :: Integer -> [Integer]
primes n = filter isPrime [2..n]

main :: IO ()
main = do
    args <- getArgs
    print (primes (read (head args)))