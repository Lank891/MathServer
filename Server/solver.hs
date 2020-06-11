module Rpn where

import Data.List
import Text.Read
import Foreign.C.String

solveRPN :: String -> Maybe [Float]
solveRPN = foldl ff (Just []) . words
    where ff  Nothing _   = Nothing
          ff (Just (x:y:ys)) "+" = Just( (y + x):ys )
          ff (Just (x:y:ys)) "-" = Just( (y - x):ys )
          ff (Just (x:y:ys)) "*" = Just( (y * x):ys )
          ff (Just (x:y:ys)) "/" = if x == 0 then Nothing else Just( (y / x):ys )
          ff (Just (x:y:ys)) "^" = Just( (y ** x):ys )
          ff (Just (x:ys)) "ln"  = if x <= 0 then Nothing else Just( log x:ys )
          ff (Just (x:ys)) "log" = if x <= 0 then Nothing else Just( logBase 10 x:ys )
          ff (Just (xs)) number  = case readMaybe number of
                                    Just num  -> Just(num:xs)
                                    Nothing -> Nothing 

getHeadJust :: Maybe [Float] -> Maybe Float
getHeadJust Nothing = Nothing
getHeadJust (Just xs) = Just $ head xs

rpnFromList :: String -> Maybe Float
rpnFromList = getHeadJust . solveRPN 

returnAnswer :: String -> IO CString
returnAnswer x = case rpnFromList x of
                    Just n -> newCString $ (show n) ++ "\n"
                    Nothing -> newCString $ "Invalid Calculations\n"

rpn :: CString -> IO CString
rpn x = do k <- peekCString x
           returnAnswer k

foreign export ccall rpn :: CString -> IO CString
