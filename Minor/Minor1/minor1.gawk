#Name: Yafet Kubrom
#EUID: 11334602
BEGIN {print("Student Average Test Scores");i=0;FS=","} 
NR == 1 {printf("%-20s%s\n","Name","Average")}
NR > 1 && $3 == "A" {
			j=($4+$5+$6)/3
			s = $2 " " $1
			printf("%-20s%.1f\n",s,j)
			k+=j
			++i
	}
END {printf("%d students with average score of %.1f\n",i,(k/i))}
