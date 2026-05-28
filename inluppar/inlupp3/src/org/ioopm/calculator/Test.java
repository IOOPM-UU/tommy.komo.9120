package org.ioopm.calculator;

import org.ioopm.calculator.ast.*;

public class Test{
    public static void main(String[] args){
        Constant c1 = new Constant(5);
        Constant c2 = new Constant(2);
        Variable v = new Variable("x");
        Addition a = new Addition(c1, v);
        Multiplication m = new Multiplication(a, c2);

        testPrinting("(5.0 + x) * 2.0", m);
        testPrinting("7.0", new Constant(7));
        testPrinting("y", new Variable("y"));
        testPrinting("cos y", new Cos(new Variable("y")));
        testPrinting("10.0 / z", new Division(new Constant(10), new Variable("z")));
        testPrinting("x + y", new Addition(new Variable("x"), new Variable("y")));

        testEvaluating(new Constant(12), new Addition(new Constant(5), new Constant(7)), new Environment());
        testEvaluating(new Addition(new Variable("x"), new Constant(2)),
                       new Addition(new Variable("x"), new Constant(2)), new Environment());
        testEvaluating(new Cos(new Variable("y")), new Cos(new Variable("y")), new Environment());

        Environment vars = new Environment();
        vars.put(new Variable("x"), new Constant(8));

        Addition fivePlusX = new Addition(new Constant(5), new Variable("x"));
        Multiplication multiply = new Multiplication(fivePlusX, new Constant(2));

        testEvaluating(new Constant(26), multiply, vars);

        testCommand(Vars.instance());
        testCommand(Quit.instance());
    }

    public static void testPrinting(String expected, SymbolicExpression e) {
        if (expected.equals("" + e)) {
            System.out.println("Passed: " + e);
        } else {
            System.out.println("Error: expected '" + expected + "' but got '" + e + "'");
        }
    }

    public static void testEvaluating(SymbolicExpression expected, SymbolicExpression e, Environment vars) {
        SymbolicExpression result = e.eval(vars);
        if (result.equals(expected)) {
            System.out.println("Passed: " + e);
        } else {
            System.out.println("Error: expected '" + expected + "' but got '" + result + "'");
        }
    }

    public static void testCommand(SymbolicExpression e){
        if (e.isCommand()){
            System.out.println("Passed: command");
        } else {
            System.out.println("Error: expected command");
        }
    }
}