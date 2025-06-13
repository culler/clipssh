# clipssh

The clipssh package is a bibary Tk extension, currently only supporting macOS.

The package adds one command with signature *clipssh text* which has no return value.

The effect of the command is:
  - To set the string value of the first item on the general NSPasteboard to the string
    given as the argument *text*.  The pasteboard type is set to public.utf8-plain-text.
    This means that other applications, such as web browsers, will paste the text.
    However, the copy is done without incrementing the changeCount property of the
    general NSPasteboard.
  - To arrange that the pasteboard will be cleared shortly after the text is pasted;

The fact that the changeCount is not incremented means that most clipboard managers
will not be aware of the copy and hence will not archive the string copied by the
command.

The intended application is for copying a password from a Tk-based application and
pasting it into a browser without leaving the password in any archive files created
by a clipboard manager.
