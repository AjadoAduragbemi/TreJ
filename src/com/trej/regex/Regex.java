package com.trej.regex;

/**
 * A Regex class that provides approximate pattern matching by accessing native funtions
 * defined by TRE
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

    private long preg;

    private Regex(String pattern, int cflags) throws RegexException {
        this.pattern = pattern;
        this.cflags = cflags;
        this.preg = 0;

        errorValue = compile();
        if(errorValue != REG_OK) {
            throw new RegexException(errorMessage);
        }
    }

    private native int compile();

    /**
     *
     * @param pattern The regular expression pattern to be compiled.
     * @throws RegexException if object has been initialized with an invalid regex
     *                        pattern
     */
    public static Regex compile(String pattern) throws RegexException {
        return compile(pattern, 0);
    }

    /**
     *
     * @param pattern The regular expression pattern to be compiled.
     * @param cflags Compile flags, a bit mask that may include [REG_ICASE,
     *               REG_NEWLINE, REG_NOSUB, REG_LITERAL, REG_RIGHT_ASSOC,
     *               REG_UNGREEDY].
     * @throws RegexException if object has been initialized with an invalid regex
     *                        pattern
     */
    public static Regex compile(String pattern, int cflags) throws RegexException {
        return new Regex(pattern, REG_EXTENDED | cflags);
    }

    private native MatchResult exec();

    /**
     *
     * @param input String to match against regex pattern
     */
    public MatchResult exec(String input) {
        this.input = input;
        return exec();
    }

    /**
     *
     * @param pattern The expression to be compiled.
     * @param input String to match against regex pattern
     * @throws RegexException if regex pattern is invalid
     */
    public static MatchResult exec(String pattern, String input) throws RegexException {
        return exec(pattern, input, 0);
    }

    /**
     *
     * @param pattern  The expression to be compiled.
     * @param input  String to match against regex pattern
     * @param cflags Compile flags, a bit mask that may include [REG_ICASE,
     *               REG_NEWLINE, REG_NOSUB, REG_LITERAL, REG_RIGHT_ASSOC,
     *               REG_UNGREEDY].
     * @throws RegexException if regex pattern is invalid
     */
    public static MatchResult exec(String pattern, String input, int cflags) throws RegexException {
        Regex regex = Regex.compile(pattern, cflags);
        MatchResult result = regex.exec(input);
        regex.free();
        return result;
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
     * @return String return the errorMessage
     */
    public String getInput() {
        return input;
    }

    public native void free();

    static {
        System.loadLibrary("trej");
    }

}