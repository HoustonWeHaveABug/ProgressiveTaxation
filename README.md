# ProgressiveTaxation

Link to challenge: https://www.reddit.com/r/dailyprogrammer/comments/cdieag/20190715_challenge_379_easy_progressive_taxation/.

The program reads tax brackets configuration and user requests on standard input.

Tax brackets configuration is expected in the following format:

- Number of tax brackets (n)
- Income cap for tax bracket 1
- Tax rate for tax bracket 1
- ...
- Tax rate for tax bracket n

Income cap is a unsigned integer, tax rate is a double ranging from 0 to 1 inclusive.

For tax bracket n, only a tax rate is read, the income cap is set to UINT_MAX.

User requests can be of four types:

- 0 \<income\>: output \<overall tax\> and \<overall tax rate\> for given \<income\>
- 1 \<overall tax\>: output \<income\> and \<overall tax rate\> for given \<overall tax\>
- 2 \<overall tax rate\>: output \<income\> and \<overall tax\> for given \<overall tax rate\>
- 3: quit the program
