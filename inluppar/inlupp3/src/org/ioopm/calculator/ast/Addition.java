package org.ioopm.calculator.ast;

public class Addition extends Binary{
    public Addition(SymbolicExpression lhs, SymbolicExpression rhs){
        super(lhs, rhs);  // för att när vi skapar en Addition skapar vi en Binary också som vi behöver constructa
    }

    public SymbolicExpression eval(Environment vars){
        SymbolicExpression lhs = this.getLhs().eval(vars);
        SymbolicExpression rhs = this.getRhs().eval(vars);

        if (lhs.isConstant() && rhs.isConstant()){
            return new Constant(lhs.getValue() + rhs.getValue());
        } else {
            return new Addition(lhs, rhs);
        }
    }

    public String getName(){
        return "+";
    }

    public int getPriority(){
        return 50;
    }
}