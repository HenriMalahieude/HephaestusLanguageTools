## Hephaestus Language Tools
This is both an exploration of compilation through the C Programming Language, as well as an attempt to make a simple interface for making programming languages.

I've used Lexer, Yacc, and Bison to do some Language Definitions but I find it... annoyingly complex.

So. The idea is a simplified tool kit for interacting with "languages" or text, whatever it may be.


---
That means:
* Easy Interoperibility: You should be able to pick and choose the tool you'd like to use.
    * Hate my lexer, but love my parsers? This project should be able to interact with others fine. 
    * This will be last on my TODO list though, considering the scale of what I'm trying here :skull:
* Simple Interface: That means that defining vocab or parse files is simply that, a vocab or parse file. 
    * No programming random actions in C attached to each step.
    * While I do know this lowers the amount of actions open to a developer, this is meant to be a simple Yacc/Bison/Whatever. 
    * Devs with more complicated needs can switch to one of those or contribute their need into this codebase.
* Simple and Clear Options:
    * Still defining your language? Need to be loose with your rules? Keep them in separate .vocab, .parse, .semants files.
    * Want to ship a stand-alone compiler? Either define it as a string in your code, or manually define your rules.

I also would like to make this a modern toolset:
* w/ Tests to ensure correctness
* w/ Proper documentation
* w/ Ease to learn/master
* w/ Can be used to teach language concepts
    * TBH, the entire reason for this project was my annoyance at my college class using these outdated tools like yacc and bison with their 90s ass websites.
    * Those don't EVEN COMPILE INTO A SINGLE FILE, THEY COMPILE INTO C FIRST MAKING ERRORS SO DIFFICULT TO FUCKING FIND!!!!!
    * This is a skill issue. Moving on.

---
TODO List:
1. ~~"Lexic" Tokenizer~~
2. "Syntac" Parsers
    1. Top-Down LL(1)
    2. Bottom-Up LR(0)
    3. Bottom-Up SLR --Not Sure I want to actually make this one
    4. Bottom-Up LR(1)
3. "Semant" Abstract Tree Generator
4. (maybe) Backend. Though perhaps I will simply point you guys to QBE? This would get 100x bigger if I decided to take that on.
