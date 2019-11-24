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

    private int matchCost, insertCount, deleteCount, substitutionCount;
    private byte[] inputBytes;

    public MatchResult(Regex regex, Match[] matchArray, boolean approximate) {
        this.regexParent = regex;
        this.matchArray = matchArray;
        this.approximate = approximate;
    }

    private String getInputRange(int start, int end) {
        byte[] inputBytes = regexParent.getInput().getBytes();
        byte[] outputBytes = new byte[end - start];
        int bpos = 0;
        for (int index = start; index < end; index++) {
            outputBytes[bpos++] = inputBytes[index];
        }
        return new String(outputBytes);
    }

    /**
     * Get a List containing substrings of the input match.
     * 
     * @return a List of substrings if matchArray is valid, null otherwise.
     */
    public final List<String> groups() {
        List<String> groups = null;
        if (matchArray != null) {
            groups = new ArrayList<>(matchArray.length);
            for (Match match : matchArray) {
                if (match != null) {
                    groups.add(getInputRange(match.getStartOffset(), match.getEndOffset()));
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
        return this.approximate;
    }

    public int getMatchCost() {
        return this.matchCost;
    }

    public int getInsertCount() {
        return this.insertCount;
    }

    public int getDeleteCount() {
        return this.deleteCount;
    }

    public int getSubstitutionCount() {
        return this.substitutionCount;
    }
}