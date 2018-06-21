# Evolving Graphs by Graph Programming

A public repository for EGGP, maintained by Timothy Atkinson. This short FAQ should help you get started - otherwise, my contact information is at the bottom.

### What is EGGP?

*Evolving Graphs by Graph Programming* is a type of evolutionary algorithm where individuals are *graphs* and mutation operators are defined as *graph programs*. Graph programs are automated, sequential algorithms composed of graph transformations (which are themselves grounded in category theory). In this repository we are using the probabilistic graph programming language P-GP 2.

This iteration of EGGP is designed to evolve acyclic graphs with sharing (e.g. for solving digital circuit problems, genetic programming problems etc). Our unique use of graph programming means that, unlike other graph-based EA's (e.g. the remarkable [Cartesian Genetic Programming](http://www.cartesiangp.co.uk/)) we can directly preserve acylicity by designing a graph program which preserves that property when mutating an individual - this is the top-level eggp_mutate_edge.gp2 file. In contrast, Cartesian Genetic Programming (and other traditional GP approaches) guarantee acyclicity through some encoding which typically means a loss of some part of the landscape. Our results on digital circuit problems indicate that our more generalised landscape given by graph-program based mutation operators is in some cases highly useful.

To read more about EGGP and read about our promising results, check out our paper at EuroGP 2018:

- [Evolving Graphs by Graph Programming](https://link.springer.com/chapter/10.1007/978-3-319-77553-1_3)
Timothy Atkinson, Detlef Plump, Susan Stepney. EuroGP 2018.

To learn about the probabilistic graph programming technique that we use to implement EGGP, check out our paper at ICGT 2018:

- [Probabilistic Graph Programs for Randomised and Evolutionary Algorithms](https://link.springer.com/chapter/10.1007%2F978-3-319-92991-0_5)
Timothy Atkinson, Detlef Plump, Susan Stepney. ICGT 2018.


### How do I use the code?

*Please note, these instructions are designed for Linux only. For other operating systems get in touch with me and I'll try to help you build the code.*

The top-level demo.c file should be largely self explanatory for a casual user. If you want to get deeper into the code and implement your own ideas then hopefully the comments see you through. Otherwise, throw me an email and I'll do my best to help.

To actually *build* the code, you'll need to do 3 things:

1. Install a local version of P-GP 2. [The instructions on the github page](https://github.com/UoYCS-plasma/P-GP2) should hopefully be clear enough.
2. Replace the string "/YOUR/PGP2/INSTALL/DIR/" on line 1 of the top-level Makefile with the path to your P-GP 2 installation.
3. Build your EGGP program by calling *make mainfile=demo.c*. This simply means 'build EGGP using demo.c as the main file'.

Now you can run the code by executing *./eggp*.

### How can I contact you?

Feel free to send an email to my university address; tja511@york.ac.uk
