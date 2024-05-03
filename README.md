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
    - free, close etc
    - error handling
    - add bash warning for EOF signal during heredoc: also what happens if there are several heredoc childs
    - streamline error handling

heredoc:
- remove error log

not working commands yet:
-

seems to work now:
- wc -l << t | grep i
- wc -l << t | wc -l < a
- ls | grep i > output | wc -l < outputfdsafds
- wc -l < a | wc -l << b | wc -l < t
- wc -l c | wc -l << b
- wc -l c | wc -l << b > output
- wc -l < a | wc -l << b
- wc -l << a | wc -l << b
- wc -l c | wc - l << a | head << b