package org.ioopm.calculator.ast;

public class Log extends Unary{
    public Log(SymbolicExpression argument){
        super(argument);
    }

    public String getName(){
        return "log";
    }

    public int getPriority(){
        return 200;
    }

    public SymbolicExpression eval(Environment vars){
        SymbolicExpression argument = this.getArgument().eval(vars);  
        if(argument.isConstant()){
            return new Constant(Math.log(argument.getValue()));
        } else {
            return new Log(argument);
        }
    }
}