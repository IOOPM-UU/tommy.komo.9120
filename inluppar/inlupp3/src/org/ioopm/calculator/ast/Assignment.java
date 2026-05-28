package org.ioopm.calculator.ast;

public class Assignment extends Binary{
    public Assignment(SymbolicExpression lhs, SymbolicExpression rhs){
        super(lhs, rhs);
    }

    public String getName(){
        return "=";
    }

    public int getPriority(){
        return 0;
    }

    public SymbolicExpression eval(){
        SymbolicExpression lhs = this.getLhs().eval(vars);
        SymbolicExpression rhs = this.getRhs();

        if (rhs instanceof Variable){    // om den faktiskt är en variabel, lagra variabeln i environment
            vars.put((Variable) rhs, lhs);  // castar som variable för att vars lagrar keys av typen variable
            return lhs;
        } else {
            throw new RuntimeException("Right hand side of assignment must be a variable");
        }
    }
}