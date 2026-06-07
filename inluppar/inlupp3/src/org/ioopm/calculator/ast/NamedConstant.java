package org.ioopm.calculator.ast;

public class NamedConstant extends Atom {
    private String name = "";
    private double value = 0.0;

    public NamedConstant(String name, double value){
        this.name = name;
        this.value = value;
    }

    public boolean isConstant(){
        return true;
    }

    public double getValue(){
        return this.value;
    }

    public String toString(){
        return this.name;
    }

    public boolean equals(Object other){
        if (other instanceof NamedConstant) {
            return this.equals((NamedConstant) other);
        } else {
            return false;
        }
    }

    public boolean equals(NamedConstant other){
        return this.name.equals(other.name) && this.value == other.value;
    }

    public SymbolicExpression eval(Environment vars) {
        return this;
    }
}