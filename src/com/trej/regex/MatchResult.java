// Copyright (c) 2019 Ajado Aduragbemi Joseph
// 
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

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
     * Get a List containing substrings of the input match.
     * 
     * @return a List of substrings if matchArray is valid, null otherwise.
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

    /**
     * Get substring of the match at a given index.
     *
     * @param index
     * @return a substring of the match at the given index.
     */
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
     * Check to see if compiled regex pattern had an approximate expression
     * 
     * @return true if compiled expression is an approximate expression.
     */
    public boolean isApproximate() {
        return approximate;
    }

}