# Minihell

### Useful links:
-> <a href="https://harm-smits.github.io/42docs/projects/minishell" target="_blank">42 Docs</a>
-> <a href="https://www.cs.purdue.edu/homes/grr/SystemsProgrammingBook/Book/Chapter5-WritingYourOwnShell.pdf" targrt="_blank">Writing your own shell cheatsheet</a>
-> <a href="https://pubs.opengroup.org/onlinepubs/009695399/utilities/xcu_chap02.html" target="_blank">Shell syntax</a>
-> <a href="https://www.linux.org/threads/bash-03-–-command-line-processing.38676/" target="_blank">Command-line processing</a>
-> <a href="https://github.com/Swoorup/mysh" target="_blank">Practice example from 42 Decs</a>

Valgrind: run it with this flag to suppress all readline leaks:
make && valgrind --suppressions=readline.supp --leak-check=full --show-leak-kinds=all --track-origins=yes ./minishell

### Monika's Task
    - refactoring
    - norminette
    - test some cases

### Lilie's Task
    - remove libft and adjust makefile
    - free, close etc: remove list nodes

    - error handling
    - streamline error handling
    - questions: sigpipe handling?

- correct exit status code when parsin fails (now it's always 2)
- next: exit status, errno and perror of cd
- improve error message: include bash and command argument if necessary
- quote behaviour in heredoc
- export and env should show the exact same elements as bash?

### PROB

LEAKS CHECK

```bash
vaigrind -s ./minishell
valgrind --leak-check=full --suppressions=readline.supp ./minishell
```

```bash
ls -a
```

```bash
s
```

And it shows where leaks coming from


# Heredoc
<br>
1)

### Bash

        ```bash
        $ ls | cat << stop > herdoc2.txt
        > is this good
        > stop
        $
        ```

### Minishell
        ```bash
        $ ls | cat << stop > herdoc1.txt
        > is this good
        > stop

        $
        ```
<br>
2)

### Bash
        ```bash
        $ ls | cat << stop | ls -la | cat << stop1
        > 12
        > 32232
        > 23
        > stop
        > awdaw
        > daswd
        > stop1
        awdaw
        daswd
        $
        ```

### Minishell
        ```bash
        $ ls | cat << stop | ls -la | cat << stop1
        > 12
        > 32232
        > 23
        > stop
        > awdaw
        > daswd
        > stop1
        awdaw
        daswd

        $
        ```

- so what is happening on all of those cases is that our here dock add extra new line [need to be fixed]
- after finishing _inputfiles are staying