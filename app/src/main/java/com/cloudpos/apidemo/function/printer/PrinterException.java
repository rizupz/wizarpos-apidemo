package com.cloudpos.apidemo.function.printer;

public class PrinterException extends Exception  {

    private static final long serialVersionUID = 1L;

    public PrinterException() {
    	super();
    }

    public PrinterException(String message) {
    	super(message);
    }

    public PrinterException(String message, Throwable throwable) {
    	super(message, throwable);
    }

    public PrinterException(Throwable throwable) {
    	super(throwable);
    }
}
