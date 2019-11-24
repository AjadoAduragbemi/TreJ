package test;

import com.trej.regex.MatchResult;
import com.trej.regex.Regex;
import com.trej.regex.RegexException;

public class Main {

    public static void main(String[] args) {
        MatchResult result;
        try {
            Regex regex = Regex.compile("(test)([a-z]+)\\d{3}$", Regex.REG_ICASE);
            result = regex.exec("teststRing123");
            printResults(result);
            regex.free();
        } catch(RegexException ex) {
            ex.printStackTrace();
        }

        try {
            // an approximate match with 2 substitutions
            // tast -> test
            // String -> string
            result = Regex.exec("{~2}(test)([a-z]+)\\d{3}$", "tastString123");
            printResults(result);
        } catch(RegexException ex) {
            ex.printStackTrace();
        }
        
        try {
            result = Regex.exec("(test)([a-zA-Z]+)\\d{3}(À)$", "testString123À");
            printResults(result);
        } catch(RegexException ex) {
            ex.printStackTrace();
        }

        try {
            result = Regex.exec("{+1#1}(test)([a-z]+)\\d{3}(À)$", "tewstString123À");
            printResults(result);
        } catch(RegexException ex) {
            ex.printStackTrace();
        }
    }

    private static void printResults(MatchResult result) {
        if(result != null) {
            System.out.println(result.groups());
            if(result.isApproximate()) {
                System.out.println("Approximate Match:");
                System.out.println("    Match Cost: " + result.getMatchCost());
                System.out.println("    Insert Count: " + result.getInsertCount());
                System.out.println("    Delete Count: " + result.getDeleteCount());
                System.out.println("    Substitution Count: " + result.getSubstitutionCount());
            }
        }
    }
}