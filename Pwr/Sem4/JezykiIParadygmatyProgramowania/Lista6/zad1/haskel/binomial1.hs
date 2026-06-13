import System.Environment (getArgs)

binomial :: Integer -> Integer -> Integer
binomial n k
    | k == 0 = 1
    | k == n = 1
    | otherwise = binomial (n - 1) k + binomial (n - 1) (k - 1)

main :: IO ()
main = do
    args <- getArgs
    let n = read (args !! 0) :: Integer
    let k = read (args !! 1) :: Integer
    print (binomial n k)