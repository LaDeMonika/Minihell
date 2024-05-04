#!/bin/bash

cd ..
make

#files for storing outputs
output_bash="output.bash.txt"
output_minishell="output_minishell.txt"

#piping
command="ls | grep m | grep n | grep i"
eval $command > $output_bash
echo "$command" | ./minishell | sed '1d;$d' > $output_minishell

diff_output=$(diff $output_bash $output_minishell)
echo "test command: "$command""
if $diff_output; then
	echo "output OK"
else
	echo "output difference:"
	echo "$diff_output"
fi
echo ""

#redirection
command="wc -l < input.txt"
command="wc -l < input.txt > output.txt"
command="wc -l < input.txt < input2.txt"
command="wc -l <> output.txt"

#heredoc
command="wc -l << a << b"

#redirection + heredoc
command="wc -l < input.txt >> output.txt"
command="wc -l << EOF > output.txt"
command="wc-l < input.txt >> output.txt >> output2.txt"
command="wc -l >> output.txt << a << b < input.txt > output2.txt"
command="echo "hello" > output yes.txt <<eof"

#input + heredoc 4 combinations
command="wc << a << b"
command="wc < input1 << b"
command="wc << b < input1"
command="wc < input1 < input2"

#redirection + piping
command="grep i < input.txt | wc -l > output.txt"

#redirection + heredoc + piping
command="wc -l << a << b | wc -l > output.txt"