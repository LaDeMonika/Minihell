# Minihell

### Useful links:
-> <a href="https://harm-smits.github.io/42docs/projects/minishell" target="_blank">42 Docs</a>
-> <a href="https://www.cs.purdue.edu/homes/grr/SystemsProgrammingBook/Book/Chapter5-WritingYourOwnShell.pdf" targrt="_blank">Writing your own shell cheatsheet</a>
-> <a href="https://pubs.opengroup.org/onlinepubs/009695399/utilities/xcu_chap02.html" target="_blank">Shell syntax</a>
-> <a href="https://www.linux.org/threads/bash-03-–-command-line-processing.38676/" target="_blank">Command-line processing</a>
-> <a href="https://github.com/Swoorup/mysh" target="_blank">Practice example from 42 Decs</a>

Valgrind: run it with this flag to suppress all readline leaks:
--suppressions=readline.supp

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
- improve error message: include bash and command argument if necessary
- quote behaviour in heredoc
- export and env should show the exact same elements as bash?

### PROB



big tester m:
TOTAL TEST COUNT: 871  TESTS PASSED: 871  LEAKING: 0
                     STD_OUT: ✓   STD_ERR: ✓   EXIT_CODE: ✓
                         TOTAL FAILED AND PASSED CASES:
                                     ❌ 0
                                     ✅ 2613
TOTAL TEST COUNT: 881  TESTS PASSED: 868  LEAKING: 0
                     STD_OUT: 6  STD_ERR: 4  EXIT_CODE: 6
                         TOTAL FAILED AND PASSED CASES:
                                     ❌ 16
                                     ✅ 2627
 TOTAL TEST COUNT: 889  TESTS PASSED: 863  LEAKING: 0
                     STD_OUT: 19  STD_ERR: 10  EXIT_CODE: 16
                         TOTAL FAILED AND PASSED CASES:
                                     ❌ 45
                                     ✅ 2622
TOTAL TEST COUNT: 891  TESTS PASSED: 755  LEAKING: 0
                     STD_OUT: 129  STD_ERR: 68  EXIT_CODE: 63
                         TOTAL FAILED AND PASSED CASES:
                                     ❌ 260
                                     ✅ 2413
 TOTAL TEST COUNT: 894  TESTS PASSED: 750  LEAKING: 0
                     STD_OUT: 133  STD_ERR: 72  EXIT_CODE: 71
                         TOTAL FAILED AND PASSED CASES:
                                     ❌ 276
                                     ✅ 2406
 TOTAL TEST COUNT: 894  TESTS PASSED: 728  LEAKING: 0
                     STD_OUT: 137  STD_ERR: 97  EXIT_CODE: 91
                         TOTAL FAILED AND PASSED CASES:
                                     ❌ 325
                                     ✅ 2357
TOTAL TEST COUNT: 931  TESTS PASSED: 717  LEAKING: 0
                     STD_OUT: 114  STD_ERR: 117  EXIT_CODE: 111
                         TOTAL FAILED AND PASSED CASES:
                                     ❌ 342
                                     ✅ 2451
 TOTAL TEST COUNT: 931  TESTS PASSED: 711  LEAKING: 0
                     STD_OUT: 113  STD_ERR: 124  EXIT_CODE: 118
                         TOTAL FAILED AND PASSED CASES:
                                     ❌ 355
                                     ✅ 2438
TOTAL TEST COUNT: 931  TESTS PASSED: 670  LEAKING: 0
                     STD_OUT: 152  STD_ERR: 127  EXIT_CODE: 122
                         TOTAL FAILED AND PASSED CASES:
                                     ❌ 401
                                     ✅ 2392
 TOTAL TEST COUNT: 696  TESTS PASSED: 614  LEAKING: 0
                     STD_OUT: 50  STD_ERR: 55  EXIT_CODE: 29
                         TOTAL FAILED AND PASSED CASES:
                                     ❌ 134
                                     ✅ 1954
TOTAL TEST COUNT: 740  TESTS PASSED: 612  LEAKING: 0
                     STD_OUT: 96  STD_ERR: 55  EXIT_CODE: 29
                         TOTAL FAILED AND PASSED CASES:
                                     ❌ 180
                                     ✅ 2040
 TOTAL TEST COUNT: 740  TESTS PASSED: 604  LEAKING: 0
                     STD_OUT: 97  STD_ERR: 56  EXIT_CODE: 35
                         TOTAL FAILED AND PASSED CASES:
                                     ❌ 188
                                     ✅ 2032
TOTAL TEST COUNT: 866  TESTS PASSED: 663  LEAKING: 0
                     STD_OUT: 119  STD_ERR: 121  EXIT_CODE: 150
                         TOTAL FAILED AND PASSED CASES:
                                     ❌ 390
                                     ✅ 2208
 TOTAL TEST COUNT: 868  TESTS PASSED: 606  LEAKING: 0
                     STD_OUT: 129  STD_ERR: 175  EXIT_CODE: 192
                         TOTAL FAILED AND PASSED CASES:
                                     ❌ 496
                                     ✅ 2108
TOTAL TEST COUNT: 871  TESTS PASSED: 606  LEAKING: 0
                     STD_OUT: 131  STD_ERR: 177  EXIT_CODE: 191
                         TOTAL FAILED AND PASSED CASES:
                                     ❌ 499
                                     ✅ 2114
TOTAL TEST COUNT: 871  TESTS PASSED: 597  LEAKING: 0
                     STD_OUT: 140  STD_ERR: 177  EXIT_CODE: 191
                         TOTAL FAILED AND PASSED CASES:
                                     ❌ 508
                                     ✅ 2105
TOTAL TEST COUNT: 878  TESTS PASSED: 593  LEAKING: 0
                     STD_OUT: 149  STD_ERR: 179  EXIT_CODE: 192
                         TOTAL FAILED AND PASSED CASES:
                                     ❌ 520
                                     ✅ 2114
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


