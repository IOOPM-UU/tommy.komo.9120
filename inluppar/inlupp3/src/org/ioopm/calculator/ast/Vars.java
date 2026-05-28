package org.ioopm.calculator.ast;

public class Vars extends Command {
    private static final Vars theInstance = new Vars();

    private Vars(){   // annan kod ska inte kunna skriva new Vars()
    }

    public static Vars instance(){    // annan kod ska dock kunna få tillgång till objektet
        return theInstance;
    }

    public boolean equals(Object other){
        return other instanceof Vars;
    }
}