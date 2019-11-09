package com.trej.regex;

import java.util.ArrayList;
import java.util.List;

public class MatchResult {

    private final Regex regexParent;
    private final Match[] matchArray;
    private boolean approximate;

    public MatchResult(Regex regex, Match[] matchArray, boolean approximate) {
        this.regexParent = regex;
        this.matchArray = matchArray;
        this.approximate = approximate;
    }

    /**
     *
     */
    public final List<String> groups() {
        List<String> groups = null;
        if (regexParent != null && matchArray != null) {
            groups = new ArrayList<>(matchArray.length);
            for (Match match : matchArray) {
                if (match != null) {
                    groups.add(regexParent.getInput().substring(match.getStartOffset(), match.getEndOffset()));
                }
            }
        }
        return groups;
    }

    public final String group(int index) {
        if (regexParent != null && matchArray != null && index < matchArray.length) {
            Match match = matchArray[index];
            if (match != null) {
                return regexParent.getInput().substring(match.getStartOffset(), match.getEndOffset());
            }
        }
        return null;
    }

    /**
     * @return boolean return the isApproximate
     */
    public boolean isApproximate() {
        return approximate;
    }

}