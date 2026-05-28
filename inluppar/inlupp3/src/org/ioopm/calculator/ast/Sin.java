package org.ioopm.calculator.ast;

public class Sin extends Unary{
    public Sin(SymbolicExpression argument){
        super(argument);
    }

    public String getName(){
        return "sin";
    }

    public int getPriority(){
        return 200;
    }

    public SymbolicExpression eval(Environment vars){
        SymbolicExpression argument = this.getArgument().eval(vars);  /kolla om argumentet är en konstant och isåfall räkna ut resultat

        if(argument.isConstant()){
            return new Constant(Math.sin(argument.getValue()));
        } else {
            return new Sin(argument);
        }
    }
}