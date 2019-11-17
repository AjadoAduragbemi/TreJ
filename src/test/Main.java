package test;

import com.trej.regex.MatchResult;
import com.trej.regex.Regex;
import com.trej.regex.RegexException;

public class Main {

    public static void main(String[] args) {
        try {
            Regex regex = Regex.compile("^(test)([a-z]+)\\d{3}$", Regex.REG_ICASE);
            MatchResult result = regex.exec("teststRing123");
            if(result != null) {
                System.out.println(result.groups());
                System.out.println(result.group(1));
                System.out.println(result.isApproximate());
            }
            regex.free();
            result = Regex.exec("{~2}(tast)([a-z]+)\\d{3}$", "tastString123");
            if(result != null) {
                System.out.println(result.groups());
                System.out.println(result.group(1));
                System.out.println(result.isApproximate());
            }
        } catch(RegexException ex) {
            ex.printStackTrace();
        }
    }
}