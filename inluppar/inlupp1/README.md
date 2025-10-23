# Initial Profiling Results
For each input, what are the top 3 functions?
Our top 3 functions were string_eq, string_sum_hash och ioopm_hash_table_lookup for all 3 inputs. It makes sense that string_eq is dominant since its used in the lookup function. 
For each input, are the top 3 functions in your code (that you have written), or is it in library functions?
We have written the ioopms_hast_table_lookup function

We have written all of the functions our selves, however the whole string_eq function is really all about using strcmp and returning whether they match or not. Strcmp is indeed a library function. This 

Are the top 3 functions in your code consistent across the inputs? Why? Why not?
Yes they are, but in varying degrees, this is because we call lookup when we add any word to our hashtable .To find a certain key we have to compare our key with the one we are currently evaluating to check for equality.

Is there some kind of trend? (Possibly several)
Yes, similar top functions, every create/ destroy is used once

Do the results correspond with your expectations?
Yes for the most part, perhaps we didn't foresee string_eq being this strikingly dominant in the 16k_word.txt. But after looking at all the function-calls it makes perfect sense.

Based on these results, do you see a way to make your program go faster?
Yes, probalbly having more buckets. Then we wouldn't have to compare as many entrys(strings) inside the buckets and we would have less collisions aswell, so our string_eq would be used less. Less comparisons would lead to a faster running program! We could also use a better hash function that distributes our key more even. If our buckets are evenly filled the comparisons would take less time than the "overfilled" buckets from before. The "overfilled" buckets are always going to be worst for our performance for 2 reasons: the first being more comparisons, the second being that the "overfilled" bucket is the most commonly used bucket aswell -> more comparisons.