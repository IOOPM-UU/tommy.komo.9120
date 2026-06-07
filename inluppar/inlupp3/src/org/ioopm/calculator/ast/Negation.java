package org.ioopm.calculator.ast;

public class Negation extends Unary{
    public Negation(SymbolicExpression argument){
        super(argument);
    }

    public String getName(){
        return "-";
    }

    public int getPriority(){
        return 200;
    }

    public SymbolicExpression eval(Environment vars){
        SymbolicExpression argument = this.getArgument().eval(vars); 

        if(argument.isConstant()){
            return new Constant(-argument.getValue());
        } else {
            return new Negation(argument);
        }
    }
}