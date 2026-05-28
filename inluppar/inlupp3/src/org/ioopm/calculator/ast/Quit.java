package org.ioopm.calculator.ast;

public class Quit extends Command{
    private static final Quit theInstance = new Quit();

    private Quit(){   // inte låta andra skapa fler quit objekt
    }

    public static Quit instance(){   // samtidigt låta de få tillgång till denna instance
        return theInstance;
    }

    public boolean equals(Object other){
        return other instanceof Quit;
    }
}