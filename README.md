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

- correct exit status code when parsin fails (now it's always 2)
- next: exit status, errno and perror of cd
- maybe need to update PWD in env

### PROB



big tester m:
after moving expand env to tokenize:
 TOTAL TEST COUNT: 928  TESTS PASSED: 558  LEAKING: 0
                     STD_OUT: 132  STD_ERR: 249  EXIT_CODE: 244
                         TOTAL FAILED AND PASSED CASES:
                                     ❌ 625
                                     ✅ 2159
TOTAL TEST COUNT: 928  TESTS PASSED: 587  LEAKING: 0
                     STD_OUT: 133  STD_ERR: 214  EXIT_CODE: 218
                         TOTAL FAILED AND PASSED CASES:
                                     ❌ 565
                                     ✅ 2219
TOTAL TEST COUNT: 928  TESTS PASSED: 412  LEAKING: 0
                     STD_OUT: 133  STD_ERR: 215  EXIT_CODE: 303
                         TOTAL FAILED AND PASSED CASES:
                                     ❌ 651
                                     ✅ 2133
TOTAL TEST COUNT: 941  TESTS PASSED: 406  LEAKING: 0
                     STD_OUT: 144  STD_ERR: 224  EXIT_CODE: 305
                         TOTAL FAILED AND PASSED CASES:
                                     ❌ 673
                                     ✅ 2150
TOTAL TEST COUNT: 941  TESTS PASSED: 402  LEAKING: 0
                     STD_OUT: 148  STD_ERR: 224  EXIT_CODE: 305
                         TOTAL FAILED AND PASSED CASES:
                                     ❌ 677
                                     ✅ 2146
TOTAL TEST COUNT: 941  TESTS PASSED: 354  LEAKING: 0
                     STD_OUT: 203  STD_ERR: 241  EXIT_CODE: 305
                         TOTAL FAILED AND PASSED CASES:
                                     ❌ 749
                                     ✅ 2074
TOTAL TEST COUNT: 941  TESTS PASSED: 319  LEAKING: 0
                     STD_OUT: 238  STD_ERR: 241  EXIT_CODE: 305
                         TOTAL FAILED AND PASSED CASES:
                                     ❌ 784
                                     ✅ 2039

TOTAL TEST COUNT: 941  TESTS PASSED: 309  LEAKING: 0
                     STD_OUT: 248  STD_ERR: 241  EXIT_CODE: 305
                         TOTAL FAILED AND PASSED CASES:
                                     ❌ 794
                                     ✅ 2029


