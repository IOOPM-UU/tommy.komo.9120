package org.ioopm.calculator.ast;

public class Exp extends Unary{
    public Exp(SymbolicExpression argument){
        super(argument);
    }

    public String getName(){
        return "exp";
    }

    public int getPriority(){
        return 200;
    }

    public SymbolicExpression eval(Environment vars){
        SymbolicExpression argument = this.getArgument().eval(vars);  /kolla om argumentet är en konstant och isåfall räkna ut resultat

        if(argument.isConstant()){
            return new Constant(Math.exp(argument.getValue()));
        } else {
            return new Exp(argument);
        }
    }
}