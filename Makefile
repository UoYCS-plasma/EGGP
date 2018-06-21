PGP2DIR = /YOUR/PGP2/INSTALL/DIR/
INCDIR= $(PGP2DIR)/include/
LIBDIR= $(PGP2DIR)/lib/
GP2_OBJECTS = eggp_init eggp_mutate_node eggp_mutate_edge
OBJECTS := common_functions.c evolutionary_algorithm.c generic_operators.c utils.c eggp.c $(mainfile)
CC=gcc

CFLAGS = -I$(INCDIR) -L $(LIBDIR) -O2 -lgp2 -lm -g

default:	$(OBJECTS)
		rm -f -r PGP2Files;	mkdir PGP2Files;
		$(foreach var,$(GP2_OBJECTS),mkdir PGP2Files/$(var);	echo '$(var) compile'; $(PGP2DIR)/bin/gp2 -l $(PGP2DIR) -m $(var) -o PGP2Files/$(var) $(var).gp2; )
		$(CC) $(OBJECTS) $(foreach var,$(GP2_OBJECTS), PGP2Files/$(var)/*.c) $(CFLAGS) -o eggp

clean:
		rm -f eggp
		rm -f -r PGP2Files
