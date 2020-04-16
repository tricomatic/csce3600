#Name: Yafet Kubrom
#EUID: 11334602

#!/bin/bash
y=$(((RANDOM)%10+1))

while true
do
	read -p "Enter a number between 1 and 10: " num
	if [ $num = $y ];then
	  	echo "Congratulations, the number is $num."
	  	break
	fi
	echo -e "Sorry, you are not correct. Try again!\n"
done

