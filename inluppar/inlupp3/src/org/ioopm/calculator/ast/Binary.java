package org.ioopm.calculator.ast;

public abstract class Binary extends SymbolicExpression{
    private SymbolicExpression lhs = null;
    private SymbolicExpression rhs = null;

    public Binary(SymbolicExpression lhs, SymbolicExpression rhs){
        this.lhs = lhs;
        this.rhs = rhs;
    }

    public SymbolicExpression getLhs(){
        return this.lhs;
    }

    public SymbolicExpression getRhs(){
        return this.rhs;
    }

    public String toString(){
        String left = this.lhs.toString();
        String right = this.rhs.toString();

        if (this.lhs.getPriority() < this.getPriority()){
            left = "(" + left +")";
        }
        if (this.rhs.getPriority() < this.getPriority()) {
            right = "(" + right + ")";
        }

        return left + " " + this.getName() + " " + right;
    }

    public boolean equals(Object other){
        if (other instanceof Binary){
            return this.equals((Binary) other);
        } else {
            return false;
        }
    }

    public boolean equals(Binary other){
        return this.getName().equals(other.getName())
            && this.lhs.equals(other.lhs)
            && this.rhs.equals(other.rhs);
    }
}