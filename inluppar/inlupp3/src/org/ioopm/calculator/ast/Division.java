package org.ioopm.calculator.ast;

public class Division extends Binary{
    public Division(SymbolicExpression lhs, SymbolicExpression rhs){
        super(lhs, rhs);
    }

    public SymbolicExpression eval(Environment vars){
        SymbolicExpression lhs = this.getLhs().eval(vars);
        SymbolicExpression rhs = this.getRhs().eval(vars);

        if (lhs.isConstant() && rhs.isConstant()){
            return new Constant(lhs.getValue() / rhs.getValue());
        } else {
            return new Division(lhs, rhs);
        }
    }

    public String getName(){
        return "/";
    }

    public int getPriority(){
        return 100;
    }
}