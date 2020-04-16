#Name...Yafet Kubrom
#EUID...11334602
#Date...2/21/2020
#Cource..CSCE3600.001

#!/bin/bash

curve()
{
	incr=$1
	shift		#shift all arguement one to left.. shift 2 shifts twice
	arr=("$@")
	index=0
	for i in ${arr[@]}
	do
		let grades[$index]=$i+$incr
		index=$index+1
	done
}

if [ $# -ne 1 ]
then
	echo "usage: ./rec05.sh <curve amount>"
	exit 1
fi

for i in 1 2 3 4 5
do
	read -p "Enter QUIZ #$[$i]: " grades[i-1]
done

curve $1 "${grades[@]}"

echo "Curved Grades: "

for ((i=0 ; $i<${#grades[*]} ; i=$i+1))
do
	echo "grades[$i] = ${grades[i]}"
done
