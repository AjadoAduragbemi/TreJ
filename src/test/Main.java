package test;

import com.trej.regex.MatchResult;
import com.trej.regex.Regex;
import com.trej.regex.RegexException;

public class Main {

    public static void main(String[] args) {
        java.util.regex.Pattern pattern = java.util.regex.Pattern.compile("^(test)([a-z]+)\\d{3}$");
        java.util.regex.Matcher matcher = pattern.matcher("teststring123");
        if (matcher.find()) {
            System.out.println(matcher.groupCount());
            System.out.println(matcher.matches());
        }

        Regex regex = com.trej.regex.Regex.compile("(test)([a-z]+)\\d{3}$", Regex.REG_ICASE);
        try {
            MatchResult result = regex.match("teststRing123");
            System.out.println(result.getRegexParent().getErrorMessage());
            if(result != null) {
                System.out.println(result.groups());
                System.out.println(result.group(1));
            }
        } catch(RegexException ex) {
            ex.printStackTrace();
        }
    }
}