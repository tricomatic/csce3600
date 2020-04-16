#Name....Yafet Kubrom
#EUID....11334602
#Course..CSCE3600.01
#Date....2/23/2020
#.....Minor 2....
#!/bin/bash

#trap will handle signal (SIGINT) using kil() function
trap 'kil' 2

kil()
{
	echo " (SIGINT) recieved"
	read -p "Terminate Program? (Y/N) " choice	#variable choice will accept requist for continuing or stoping.
	if [ $choice = "Y" ] || [ $choice = "y" ]
	then
		echo "Terminating Proram"
		echo
		kill -9 $$
	else
		echo "Continuing Execution"
	fi
}

while true		#this will run continuesly until killed.
do
	echo
	echo `date`
	printf "%-25s %s\n" "User ID" "Count"		#this will be header
	if [ $# -gt 0 ]
	then
		for parm				#parm will traverse through all parameters into program
		do
			printf "%-25s" $parm
			ps -u $parm | gawk 'END {printf(" %d\n",NR-2)}'		#gawk command to find each parameter's # of process
			let x=$(ps -u $parm | wc -l)-2		#count the amount of processes
			let y=$y+$x				#y will increment every time new user enters
		done
		let y=$y-1					#-1 will subtract the header line.
		printf "%d USERS, TOTAL PROCESSES %d\n" "$#" "$y"
		y=0		#restart y for new test after 5 sec.
	else
		ps aux | gawk 'NR>1 {total[$1]++;}
			END {for(id in total) printf "%-25s %d\n", id, total[id];
						printf("%d USERS, TOTAL PROCESSES %d\n",length(total),NR-2)}'
	fi
	sleep 5
done
