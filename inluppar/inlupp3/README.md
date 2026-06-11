**Instructions**

To compile and run: enter the following into the terminal in this order:
make clean
make
make run 

The calculator will start and is ready.

To run tests: 
make test

**General**

The program is a symbolic calculator, the user can write mathematical expressions in the terminal. These expressions will be parsed into an abstract syntax tree which then is evaluated. Commands can also be used. The command vars prints the current variables that are stored, clear removes all the stored varaibles and quit exits the calculator. 

**Assumptions**

The program does not save variables after the program has been closed. Commands and functions are written in lowercase, for example quit, vars, clear, sin and cos.

**Design Decisions**

For priorities when printing expressions, i chose to override getPriority() in the classes that need a special priority. The assignment also suggested using a priority field, however overriding felt easier to follow because each operator class defines its own priority directly.

For named constants, Constants stores a HashMap from String to Double. The assignment also mentioned storing NamedConstant objects directly in the map, but we kept the String to Double version because it was simpler and close to the suggested code.