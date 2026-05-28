package org.ioopm.calculator.ast;

public abstract class Command extends SymbolicExpression{
    public SymbolicExpression eval(Environment vars){
        throw new RuntimeException("Commands can't be evaluated");
    }

    public boolean isCommand(){
        return true;
    }
}