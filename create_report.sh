#!/bin/bash

scripts=$(ls | grep mpi)
#echo "$scripts"
n="100000000"
report="report_all.txt"
echo "INÍCIO DO PROGRAMA: $(date "+ %H:%M:%S")" > $report
echo "Rodando os programas com N = $n" >> $report
for i in {1,2,3}; do
	echo "--- RODADA $i ---" >> $report
	for p in {8,4,2}; do
		echo -e "\nRodando os programas com $p processos\n" >> $report
		for s in $scripts; do
			echo "--> $s" >> $report
			mpicc "$s" -O3 -o program_run -lm
			mpirun -np $p -oversubscribe ./program_run $n  >> $report
			rm program_run
			sleep 30
		done
	done
done

n="1000000000"
echo "Rodando os programas com N = $n" >> $report
for i in {1,2,3}; do
	echo "--- RODADA $i ---" >> $report
	for p in {8,4,2}; do
		echo -e "\nRodando os programas com $p processos\n" >> $report
		for s in $scripts; do
			echo "--> $s" >> $report
			mpicc "$s" -O3 -o program_run -lm
			mpirun -np $p -oversubscribe ./program_run $n  >> $report
			rm program_run
			sleep 30
		done
	done
done
echo "FIM DO PROGRAMA: $(date "+ %H:%M:%S")" >> $report
