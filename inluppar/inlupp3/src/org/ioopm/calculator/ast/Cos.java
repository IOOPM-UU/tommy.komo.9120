package org.ioopm.calculator.ast;

public class Cos extends Unary{
    public Cos(SymbolicExpression argument){
        super(argument);
    }

    public String getName(){
        return "cos";
    }

    public int getPriority(){
        return 200;
    }

    public SymbolicExpression eval(Environment vars){
        SymbolicExpression argument = this.getArgument().eval(vars);  //kolla om argumentet är en konstant och isåfall räkna ut resultat

        if(argument.isConstant()){
            return new Constant(Math.cos(argument.getValue()));
        } else {
            return new Cos(argument);
        }
    }   
}