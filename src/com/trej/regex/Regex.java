package com.trej.regex;

/**
 * description of class
 * 
 *
 * @author Aduragbemi Joseph
 * @version 1.0.0
 */
public class Regex implements java.io.Serializable {

    /**
     *
     */
    private static final long serialVersionUID = -2801431994164874288L;

    private static final int REG_OK = 0;
    private static final int REG_EXTENDED = 1;

    public static final int REG_ICASE = (REG_EXTENDED << 1);
    public static final int REG_NEWLINE = (REG_ICASE << 1);
    public static final int REG_NOSUB = (REG_NEWLINE << 1);
    public static final int REG_LITERAL = (REG_NOSUB << 1);
    public static final int REG_RIGHT_ASSOC = (REG_LITERAL << 1);
    public static final int REG_UNGREEDY = (REG_RIGHT_ASSOC << 1);

    /**
     * The compile pattern flags.
     *
     * @serial
     */
    private int cflags;

    /**
     * The original regular-expression pattern string.
     *
     * @serial
     */
    private String pattern;

    private int errorValue;
    private String errorMessage;

    private String input;

    /**
     *
     * @param pattern
     * @param cflags
     */
    private Regex(String pattern, int cflags) {
        this.pattern = pattern;
        this.cflags = cflags;
    }

    /**
     *
     * @param regex The expression to be compiled.
     */
    public static Regex compile(String regex) {
        return compile(regex, 0);
    }

    /**
     *
     * @param regex  The expression to be compiled.
     * @param cflags Compile flags, a bit mask that may include [REG_ICASE,
     *               REG_NEWLINE, REG_NOSUB, REG_LITERAL, REG_RIGHT_ASSOC,
     *               REG_UNGREEDY].
     */
    public static Regex compile(String regex, int cflags) {
        return new Regex(regex, REG_EXTENDED | cflags);
    }

    /**
     * @return int return the cflags
     */
    public int getCflags() {
        return cflags;
    }

    private native MatchResult match() throws RegexException;

    /**
     *
     * @param input
     * @throws RegexException
     */
    public MatchResult match(String input) throws RegexException {
        this.input = input;
        return match();
    }

    /**
     *
     * @param regex
     * @param input
     * @throws RegexException
     */
    public static MatchResult match(String regex, String input) throws RegexException {
        return Regex.compile(regex).match(input);
    }

    /**
     *
     * @param regex
     * @param input
     * @param cflags
     * @throws RegexException
     */
    public static MatchResult match(String regex, String input, int cflags) throws RegexException {
        return Regex.compile(regex, cflags).match(input);
    }

    /**
     * @param cflags the cflags to set
     */
    public void setCflags(int cflags) {
        this.cflags = cflags;
    }

    /**
     * @return int return the errorValue
     */
    public int getErrorValue() {
        return errorValue;
    }

    /**
     * @return String return the errorMessage
     */
    public String getErrorMessage() {
        return errorMessage;
    }

    /**
     *
     */
    public String getInput() {
        return input;
    }

    static {
        System.loadLibrary("trej");
    }

}