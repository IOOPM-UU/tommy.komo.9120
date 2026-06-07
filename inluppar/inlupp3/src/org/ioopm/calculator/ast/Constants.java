package org.ioopm.calculator.ast;

import java.util.HashMap;

public class Constants {
    public static final HashMap<String, Double> namedConstants = new HashMap<String, Double>();

    static {
        namedConstants.put("e", Math.E);
        namedConstants.put("pi", Math.PI);
        namedConstants.put("Answer", 42.0);
        namedConstants.put("L", 6.022140857e23);
    }
}