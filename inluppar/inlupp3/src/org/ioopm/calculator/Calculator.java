package org.ioopm.calculator;

import org.ioopm.calculator.ast.*;
import org.ioopm.calculator.parser.*;

import java.io.IOException;

public class Calculator {
    public static void main(String[] args) {
        int expressions = 0;
        int evaluated = 0;
        int fullyEvaluated = 0;

        final CalculatorParser parser = new CalculatorParser();
        final Environment vars = new Environment();

        System.out.println("Redo att räkna, lets go!");

        while (true) {
            String input = System.console().readLine("? ");

            try {
                SymbolicExpression expression = parser.parse(input, vars);  // ta input strängen och gör till AST

                if (expression.isCommand()) {
                    if (expression == Quit.instance()) {
                        System.out.println("Stänger ner, hejdå!");
                        System.out.println("Inmatade uttryck: " + expressions);
                        System.out.println("Lyckade evalueringar: " + evaluated);
                        System.out.println("Fullständiga evalueringar: " + fullyEvaluated);
                        break;
                    } else if (expression == Vars.instance()) {
                        System.out.println(vars);
                    } else if (expression == Clear.instance()) {
                        vars.clear();
                        System.out.println("Rensat variabler");
                    }
                } else {
                    expressions = expressions + 1;

                    Environment backup = new Environment();
                    backup.putAll(vars);     // om evaluation failar kan vi återställa, ibland evaluerar VL, men HL är ogiltig, om man redan skrivit in i vars vid det laget så ska man återställa

                    try {
                        SymbolicExpression result = expression.eval(vars);
                        vars.put(new Variable("ans"), result);
                        System.out.println(result);
                        evaluated = evaluated + 1;
                        if (result.isConstant()) {
                            fullyEvaluated = fullyEvaluated + 1;
                        }
                    } catch (IllegalAssignmentException exception) {  // om man försäker ändra named constant
                        vars.clear();
                        vars.putAll(backup);  //ta tillbaka gamla vars
                        System.out.println(exception.getMessage());
                    } catch (RuntimeException exception) {
                        vars.clear();
                        vars.putAll(backup);
                        System.out.println(exception.getMessage());
                    }
                }
            } catch (IllegalExpressionException exception) {
                System.out.println(exception.getMessage());
            } catch (SyntaxErrorException exception) {
                System.out.println(exception.getMessage());
            } catch (IOException exception) {
                System.out.println("Error: could not read input");
            }
        }
    }
}
