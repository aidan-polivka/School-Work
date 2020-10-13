#!bash run.sh
# Aidan's shell script

threads=$((3))
while [ $threads -gt 0 ]
do
	export OMP_NUM_THREADS=$threads
	counter=$((100))
	while [ $counter -gt 0 ]
	do
		./openmp-matrix
		counter=$(( $counter - 1 ))
	done
	threads=$(( $threads - 1 ))
done

