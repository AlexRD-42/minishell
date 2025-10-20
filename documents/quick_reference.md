# Special Characters
'<<' : Heredoc
Allows you to create an input FD using the input text of the shell (delimited by an EOF). The maximum length of the heredoc will be limited to 64kb, because that is the maximum size a pipe can hold without flowing.

Example: 
Constraints: 64kb buffer

'<'  : Input Redirection
'>'  : Output Redirection (Overwrites)
'>>' : Output Redirection (Appends)
'|'  : Pipe

'"'  : Quotes
Single Quotes: Meta Characters are not interpreted
Double Quotes: Meta Characters are not interpreted (except for $)

$ARGS="A B C D" 
1) ./main "A B C D" 1 ARG
2) ./main $ARGS		4 ARGS


Bonus:
&& and || for priorities
* for the current directory