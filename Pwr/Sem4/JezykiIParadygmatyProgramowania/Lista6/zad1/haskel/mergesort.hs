import System.Environment (getArgs)

merge :: [Int] -> [Int] -> [Int]
merge [] ys = ys
merge xs [] = xs
merge (x:xs) (y:ys)
    | x <= y    = x : merge xs (y:ys)
    | otherwise = y : merge (x:xs) ys

mergeSort :: [Int] -> [Int]
mergeSort []  = []
mergeSort [x] = [x]
mergeSort xs  =
    let half = length xs `div` 2
        left = take half xs
        right = drop half xs
    in merge (mergeSort left) (mergeSort right)

main :: IO ()
main = do
    args <- getArgs
    let list = map read args :: [Int]
    print (mergeSort list)