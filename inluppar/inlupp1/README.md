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



how to build tests:
Simply write make test or make memtest in the terminal

Coverege of our code by tests;
We used gcov and lcov to measure our data
hash_table.c: Line coverege: about 92% and branch coverege: about 85%
linked_list.c: Line coverege: about 88% and branch coverege: about 80%
iterator.c: Line coverege: about 95% and branch coverege: about 90%
frec-count.c: Line coverege: about 90% and branch coverege: about 82%

How to compile and run:
compile by simply typing make frec-count

then in the terminal write ./frec-count (your textfile).txt (can add multiple textfiles)

What the program does:
This program counts how many words that exist in a textfile (.txt). We use our hash-functions to store each word as a key and the amount of times it has been written as a value. The result is a printed list of all the words in the textfile (in lexological order) and how many times it has been written. You can also insert more than one textfile at a time!

Error handling:
We have e few error handling parts of our code, for example:
Some of our hash-functions return true or false depending on the outcome of the operation
If a file can't be opened the program prints an error message
Memory allocations must work, otherwise we print another error message

Assumptions:
Usege of files with normal text and a strict amount of delimiters

Design decision:

* For the hashtable lookup and remove functions we decided to make them return a bool, depending on if the lookup or removal of said entry was successful. If the caller wants to attain which entry was removed or successful looked up, the caller must send a pointer to a elem_t. Here the entry's value will be stored by the functions upon successful removal or lookup. The benefits of this design decision is that the caller can choose to only remove an entry or seeing if a key has a value without seeing the actual value. This can be done by simply sending NULL as the last argument to the functions.

* For our struct linked list we choose to include a tail parameter to allow for append in O(1) time complexity. 

* In common.h we decided to create macros for the elem_t type to improve both the writing and reading comprehension of the struct. 


