# Minihell

### Useful links:
-> <a href="https://harm-smits.github.io/42docs/projects/minishell" target="_blank">42 Docs</a>
-> <a href="https://www.cs.purdue.edu/homes/grr/SystemsProgrammingBook/Book/Chapter5-WritingYourOwnShell.pdf" targrt="_blank">Writing your own shell cheatsheet</a>
-> <a href="https://pubs.opengroup.org/onlinepubs/009695399/utilities/xcu_chap02.html" target="_blank">Shell syntax</a>
-> <a href="https://www.linux.org/threads/bash-03-–-command-line-processing.38676/" target="_blank">Command-line processing</a>
-> <a href="https://github.com/Swoorup/mysh" target="_blank">Practice example from 42 Decs</a>

Valgrind: run it with this flag to suppress all readline leaks: --suppressions=readline.supp

### Monika's Task
    - Builtins.
    - Implement error handling.

### Lilie's Task
    - remove libft and adjust makefile
    - free, close etc: remove list nodes

    - error handling
    - streamline error handling
    - questions: sigpipe handling?


### PROB
```Bash
 ./minishell
msimic@@c2r5p11:~/Documents/[01]_GitHub/Minihell$ pwd
/home/msimic/Documents/[01]_GitHub/Minihell
bash: pwd: No such file or directory
msimic@@c2r5p11:~/Documents/[01]_GitHub/Minihell$ cd ..
bash: cd: No such file or directory
msimic@@c2r5p11:~/Documents/[01]_GitHub/Minihell$ cd inc
bash: cd: No such file or directory
msimic@@c2r5p11:~/Documents/[01]_GitHub/Minihell$ ls
" fjsdl ' fasdjkl "   inc   input   Makefile   minishell   minishell_tester   obj   readline.supp   README.md   src   tmp   utils_test
msimic@@c2r5p11:~/Documents/[01]_GitHub/Minihell$ 
```





