#!/bin/bash

for i in {1..6}
do
	echo "Batch for example$i.png"
	echo "tournament-longrun.toml"
	./gekon/automated_tests tournament-longrun.toml "samples_offic/example${i}.png" "samples_offic/example${i}_E.png" | tee -a output/example$i.log
    cp output/* ~/Knock\ Knock\ Cloud/GeKon
	echo "roulette-longrun.toml"
	./gekon/automated_tests roulette-longrun.toml "samples_offic/example${i}.png" "samples_offic/example${i}_E.png" | tee -a output/example$i.log
    cp output/* ~/Knock\ Knock\ Cloud/GeKon
	echo "ranksel-longrun.toml"
	./gekon/automated_tests ranksel-longrun.toml "samples_offic/example${i}.png" "samples_offic/example${i}_E.png" | tee -a output/example$i.log
    cp output/* ~/Knock\ Knock\ Cloud/GeKon
done
