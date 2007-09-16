function assert(str,exp){
    if(exp)
        return;
    throw ("assertion failed:" + str+ ":");
}

// This function returns the name of a given function. It does this by
// converting the function to a string, then using a regular expression
// to extract the function name from the resulting code.
function funcname(f) {
    var s = f.toString().match(/function (\w*)/)[1];
    if ((s == null) || (s.length == 0)) return "anonymous";
    return s;
}

// This function returns a string that contains a "stack trace."
// Doesn't seem to work for me in FF2 or FF3
function stacktrace() {
    var s = "";  // This is the string we'll return.
    // Loop through the stack of functions, using the caller property of
    // one arguments object to refer to the next arguments object on the
    // stack.
    for(var a = arguments.caller; a != null; a = a.caller) {
        // Add the name of the current function to the return value.
        s += funcname(a.callee) + "\n";

        // Because of a bug in Navigator 4.0, we need this line to break.
        // a.caller will equal a rather than null when we reach the end 
        // of the stack. The following line works around this.
        if (a.caller == a) break;
    }
    return s;
}
