package org.ioopm.calculator.ast;

public abstract class Unary extends SymbolicExpression{
    private SymbolicExpression argument = null; 

    public Unary(SymbolicExpression argument){
        this.argument = argument;
    }

    public SymbolicExpression getArgument(){
        return this.argument;
    }

    public String toString(){
        return this.getName() + " " + this.argument.toString();
    }

    public boolean equals(Object other){
        if (other instanceof Unary) {
            return this.equals((Unary) other);
        } else{
            return false;
        }
    }

    public boolean equals(Unary other){
        return this.getName().equals(other.getName())
            && this.argument.equals(other.argument);
    }
}