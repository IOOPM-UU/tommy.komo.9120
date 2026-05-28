package org.ioopm.calculator.ast;

public abstract class SymbolicExpression{
    public boolean isConstant() {
        return false;
    }

    public String getName(){
        throw new RuntimeException("getName() called on expression with no operator");
    }

    public int getPriority(){
        return 100;
    }

    public double getValue(){
        throw new RuntimeException("getValue() called on expression which isn't a constant");
    }

    public boolean isCommand(){
        return false;
    }

    public abstract SymbolicExpression eval(Environment vars); // Det här säger att de andra subklasserna måste säga hur de ska evaluera 
}