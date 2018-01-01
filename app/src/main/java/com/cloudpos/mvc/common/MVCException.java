package com.cloudpos.mvc.common;

public class MVCException extends RuntimeException {
	private static final long serialVersionUID = -5923896637917336703L;
	
	public MVCException() {
	}
	
	public MVCException(String msg) {
		super(msg);
	}
	
	public MVCException(Throwable cause) {
		super(cause);
	}
	
	public MVCException(String msg, Throwable cause) {
		super(msg, cause);
	}

}
