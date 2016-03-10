#!/bin/bash
rm -rf match_results
mkdir match_results

echo "Ce script est susceptible de tourner pendant quelques minutes, soyez patient"

for i in 10 15 20
do
    echo "rand vs line" > match_results/rand_line.log
    randline=0
    echo "rand vs eat" > match_results/rand_eat.log
    randeat=0
    echo "line vs eat" > match_results/line_eat.log
    lineeat=0
    for j in `seq 1 3`
    do
	./banquise 4 full-hexa $i $i ./lib_strat_rand.so ./lib_strat_line.so >> match_results/rand_line.log
	if [ $? -eq 0 ]
	then
	    ((randline = randline - 1))
	else
	    ((randline = randline + 1))
	fi

	./banquise 4 full-hexa $i $i ./lib_strat_rand.so ./lib_strat_eat.so >> match_results/rand_eat.log
	if [ $? -eq 0 ]
	then
	    ((randeat = randeat - 1))
	else
	    ((randeat = randeat + 1))
	fi

	./banquise 4 full-hexa $i $i ./lib_strat_line.so ./lib_strat_eat.so >> match_results/line_eat.log
	if [ $? -eq 0 ]
	then
	    ((lineeat = lineeat - 1))
	else
	    ((lineeat = lineeat + 1))
	fi
    done
    echo map hexa $i fois $i
    echo strat_rand vs strat_line : $(( randline/2 + 5)) - $(( 5 - randline/2))
    echo strat_rand vs strat_eat : $(( randeat/2 + 5)) - $(( 5 - randeat/2))
    echo strat_line vs strat_eat : $(( lineeat/2 + 5)) - $(( 5 - lineeat/2))
done
