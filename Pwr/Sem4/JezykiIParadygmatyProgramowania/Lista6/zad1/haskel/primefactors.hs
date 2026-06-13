import System.Environment (getArgs)

primeFactors :: Integer -> [Integer]
primeFactors n = factor n 2
  where
    factor 1 _ = []
    factor n f
        | f * f > n      = [n]
        | n `mod` f == 0 = f : factor (n `div` f) f
        | otherwise       = factor n (f + 1)

main :: IO ()
main = do
    args <- getArgs
    let n = read (head args)
    print (primeFactors n)