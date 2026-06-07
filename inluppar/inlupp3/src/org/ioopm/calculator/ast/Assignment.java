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

    public SymbolicExpression eval(Environment vars){
        SymbolicExpression lhs = this.getLhs().eval(vars);
        SymbolicExpression rhs = this.getRhs();

        if(rhs.isConstant()){
            throw new IllegalAssignmentException("Error: can't redefine named constant");
        } else if (rhs instanceof Variable) {
            vars.put((Variable) rhs, lhs);
            return lhs;
        } else {
            throw new RuntimeException("Right hand side of assignment must be a variable");
        }
    }
}