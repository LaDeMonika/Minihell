#!/bin/bash

cd ..
make

#make outputdir if necessary
output_dir="./test_output"
mkdir -p $output_dir

#files for storing outputs
output_bash=$output_dir/"output_bash.txt"
output_minishell=$output_dir/"output_minishell.txt"
commands_file="test_commands.txt"

#clear previous outputs
echo "" > $output_bash
echo "" > $output_minishell

#read from file until end
#IFS= prevents trimming
#read-r prevents escaping backslashes
while IFS= read -r command
do
	#is true if command is empty
	if [ -z "$command" ]; then
		continue
	fi
	eval "$command" &> $output_bash
	echo "$command" | ./minishell | sed '1d;$d' &> $output_minishell

	#compare bash and minishell output
	diff_output=$(diff $output_bash $output_minishell)

	echo "Testing command: "$command""
	if $diff_output; then
		echo "Output OK"
	else
		echo "Output difference: "
		echo "$diff_output"
	fi
	echo ""
done < "$commands_file"