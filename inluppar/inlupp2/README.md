How to compile and run:
write make clean, make, make run - in this order. Then the website menu will popup and the program is ready for user input.

how to build tests:
Simply write make test in the terminal


What the program does:
This program is a fully functional website. The website allows the user to create carts, remove shopping carts, add items to carts, calculate the total cost of carts, check out, add items, remove items, edit items, list all currently available merchandise, view the current stock of an item, and leave the website (quit).
After the user completes a specific action, for example creating a new shopping cart, they are returned to the main menu.
While in the menu, the user can choose from the previously listed actions. If an invalid option is selected, the user is simply taken back to the menu with a minor error message.
The only way to exit the program (without using Ctrl + C) is to choose the “quit” option. When doing so, a confirmation message will appear, and based on the user's reponse, they will either be returned to the action menu or exit the website. The files used are backend.c, backend.h, common.h, hash_table.c, hash_table.h, iterator.c, iterator.h, linked_list.c, linked_list.h, ui.c, ui.h, utils.c and utils.h

Error handling:
We have e few error handling parts of our code, for example:
Some of our hash-functions return true or false depending on the outcome of the operation
We always have Error messages for wrongfull user input, if an int is needed the program must receve an int or it will not continue.
Memory allocations must work, otherwise we print another error message

Assumptions:
The program doesn't save your actions if you decide to quit it.

Design decision:

* For the hashtable lookup and remove functions we decided to make them return a bool, depending on if the lookup or removal of said entry was successful. If the caller wants to attain which entry was removed or successful looked up, the caller must send a pointer to a elem_t. Here the entry's value will be stored by the functions upon successful removal or lookup. The benefits of this design decision is that the caller can choose to only remove an entry or seeing if a key has a value without seeing the actual value. This can be done by simply sending NULL as the last argument to the functions.

* For our struct linked list we choose to include a tail parameter to allow for append in O(1) time complexity. 

* In common.h we decided to create macros for the elem_t type to improve both the writing and reading comprehension of the struct.

* In our database-struct we save an int called "next_cart_id". This is a designchoise that made making shoppingcarts easier. With this solution we can give a new shoppingcart a new, unique ID even if we have ramoved carts.

* We used an iteratior in our calc_cost function so we could go through the linked list of keys(items) in the cart in the simplest way possible.