CC=gcc
CFLAGS=-std=c99 -Wall -g -O0

.PHONY: default all clean mrproper strats

default: all

###############################
#           SERVER            #
###############################

# server name
SERV=banquise

# sources
SERV_SRC = server.c main.c
SERV_OBJ = $(SERV_SRC:.c=.o)
SERV_OBJ_NO_GRAPHICS = $(SERV_OBJ:.o=.nographics.o)

# rules
$(SERV): $(SERV_OBJ) display.o
	$(CC) $(CFLAGS) -o $@ $^ -ldl `sdl2-config --cflags --libs` -lSDL2_image -lSDL2_ttf -lm

display.o: display.c
	$(CC) $(CFLAGS) -o $@ -c $^ 

$(SERV_OBJ): %.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $^ 

NOGRAPHICS: $(SERV_OBJ_NO_GRAPHICS)
	$(CC) $(CFLAGS) -o $(SERV) $^ -ldl -lm

$(SERV_OBJ_NO_GRAPHICS): %.nographics.o: %.c
	$(CC) $(CFLAGS) -DNO_GRAPHICS -o $@ -c $^ 





###############################
#         STRATEGIES          #
###############################

strat.o: strat.c
	$(CC) -o $@ -c $^ -fPIC $(CFLAGS)

graph.o: graph.c
	$(CC) -o $@ -c $^ -fPIC $(CFLAGS)

queue.o: queue.c
	$(CC) -o $@ -c $^ -fPIC $(CFLAGS)

ford_fulkerson.o: graph.c
	$(CC) -o $@ -c $^ -fPIC $(CFLAGS)

# RANDOM STRATEGY
# ===============

# strategy name
STRAT_RAND=lib_strat_rand.so

# add it to strats list
STRATS += $(STRAT_RAND)

# sources
STRAT_RAND_SRC = strat_rand.c
STRAT_RAND_OBJ = $(STRAT_RAND_SRC:.c=.o)

# rules
$(STRAT_RAND): $(STRAT_RAND_OBJ) strat.o graph.o
	$(CC) -o $@ $^ -shared

$(STRAT_RAND_OBJ): %.o: %.c
	$(CC) -o $@ -c $^ -fPIC $(CFLAGS)

# LINE STRATEGY
# =============

# strategy name
STRAT_LINE=lib_strat_line.so

# add it to strats list
STRATS += $(STRAT_LINE)

# sources
STRAT_LINE_SRC = strat_line.c
STRAT_LINE_OBJ = $(STRAT_LINE_SRC:.c=.o)

# rules
$(STRAT_LINE): $(STRAT_LINE_OBJ) strat.o graph.o
	$(CC) -o $@ $^ -shared

$(STRAT_LINE_OBJ): %.o: %.c
	$(CC) -o $@ -c $^ -fPIC $(CFLAGS)

# EAT THEM ALL
# ============

# strategy name
STRAT_EAT=lib_strat_eat.so

# add it to strats list
STRATS += $(STRAT_EAT)

# sources
STRAT_EAT_SRC = strat_eat.c
STRAT_EAT_OBJ = $(STRAT_EAT_SRC:.c=.o)

# rules
$(STRAT_EAT): $(STRAT_EAT_OBJ) strat.o graph.o
	$(CC) -o $@ $^ -shared

$(STRAT_EAT_OBJ): %.o: %.c
	$(CC) -o $@ -c $^ -fPIC $(CFLAGS)

# # DISSEKATOR STRATEGY
# # ===================

# # strategy name
# STRAT_DISS=lib_strat_diss.so

# # add it to strats list
# STRATS += $(STRAT_DISS)

# # sources
# STRAT_DISS_SRC = strat_diss.c
# STRAT_DISS_OBJ = $(STRAT_DISS:.c=.o)

# # rules
# $(STRAT_DISS): $(STRAT_DISS_OBJ) strat.o graph.o queue.o ford_fulkerson.o
# 	$(CC) -o $@ $^ -shared

# $(STRAT_DISS_OBJ): %.o: %.c
# 	$(CC) -o $@ -c $^ -fPIC $(CFLAGS)

################################
#             TESTS            #
################################

TESTS = test_server #test_strats

test_strats: $(STRATS)
	gcc $(CFLAGS) test_print.c -l_strat_rand -L. -I. -o $@
ifeq ($(shell uname -s), Linux) # Mac doesn't need it
	@printf "\n"
	@printf "\033[0;33m[REMINDER]\033[0m Before launching, don't forget:\n"
	@printf "export LD_LIBRARY_PATH=\$$LD_LIBRARY_PATH:$(shell pwd)"
	@printf "\n\n"
endif

test_server: test_server.o server.o
	$(CC) $(CFLAGS) -o $@ $< -ldl 
#ifeq ($(shell uname -s), Linux) # Mac doesn't need it
#	@printf "\n"
#	@printf "\033[0;33m[REMINDER]\033[0m Before launching, don't forget:\n"
#	@printf "export LD_LIBRARY_PATH=\$$LD_LIBRARY_PATH:$(shell pwd)"
#	@printf "\n\n"
#endif

test_server.o: test_server.c
	$(CC) $(CFLAGS) -o $@ -c $^

test_queue.o: test_queue.c
	$(CC) $(CFLAGS) -o $@ -c $^	

test_queue : test_queue.o queue.o
	$(CC) $(CFLAGS) -o $@ $^

test_ford_fulkerson : test_ford_fulkerson.o queue.o ford_fulkerson.o
	$(CC) $(CFLAGS) -o $@ $^

test_strat_dissekator.o : test_strat_dissekator.c
	$(CC) $(CFLAGS) -o $@ -c $^

test_strat_dissekator : test_strat_dissekator.o strat_dissekator.o ford_fulkerson.o queue.o graph.o
	$(CC) $(CFLAGS) -o $@ $^

################################
#             TOOLS            #
################################

all: $(SERV) $(STRATS) $(TESTS) 

double: all $(STRATS)
	cp lib_strat_line.so lib_strat_line2.so
	cp lib_strat_rand.so lib_strat_rand2.so
	cp lib_strat_eat.so lib_strat_eat2.so
#cp lib_strat_diss.so lib_strat_diss2.so

nographics: NOGRAPHICS $(STRATS) $(TESTS)

strats: $(STRATS)

clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(SERV)
	rm -rf $(STRATS)
	rm -rf *~
	rm -rf \#*
	rm -rf test_strats
	rm -rf test_server 
	rm -rf test_strats.dSYM
	rm -rf lib_strat*
	rm -rf test_queue
