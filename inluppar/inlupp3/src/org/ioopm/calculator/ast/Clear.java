package org.ioopm.calculator.ast;

public class Clear extends Command {
    private static final Clear theInstance = new Clear();

    private Clear() {
    }

    public static Clear instance() {
        return theInstance;
    }

    public boolean equals(Object other){
        return other instanceof Clear;
    }
}