package com.trej.regex;

public
class RegexException extends RuntimeException {
    
    /**
     *
     */
    private static final long serialVersionUID = 6528401244022001586L;

    public RegexException() {
        super();
    }

    public RegexException(String message) {
        super(message);
    }

    /**
     *
     * @param message
     * @param cause
     */
    public RegexException(String message, Throwable cause) {
        super(message, cause);
    }

    /**
     *
     * @param cause
     */
    public RegexException(Throwable cause) {
        super(cause);
    }
    
}