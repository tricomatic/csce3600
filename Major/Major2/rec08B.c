// Name.....Yafet Kubrom
// EUDI.....11334602
// Date.....04/03/2020
// Course...CSCE3600.001
// ......Rec08.c.....


/*
 * usage: ./a.out input_file text_pattern
 * Executes the command "cat input_file | grep text_pattern | cut -b 1-10".
 * Note only minimal error checking is done for simplicity/shortness of code.
 */

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char **argv)
{
	int status, i;
	int k = 2;

	if (argc == 3)
	{
		// arguments for commands
		char *cat_args[]  = {"cat", argv[1], NULL};
		char *grep_args[] = {"grep", argv[2], NULL};
		char *cut_args[]  = {"cut", "-b", "1-10", NULL};

		// file descriptors for 2 pipes: fd1 for cat-to-grep, fd2 for grep-to-cut
		int fd1[2], fd2[2];

        // make pipe for cat to grep
		pipe(fd1);
		// fd1[0] = read  end of cat->grep pipe (read by grep)
		// fd1[1] = write end of cat->grep pipe (written by cat)

        // make pipe for grep to cut
		// fd2[0] = read  end of grep->cut pipe (read by cut)
		// fd2[1] = write end of grep->cut pipe (written by grep)
		pipe(fd2);
		// fork the first child (to execute cat)
		if (fork() == 0)
		{
			// duplicate write end of cat->grep pipe to stdout
			dup2(fd1[1], fileno(stdout));
			close(fd1[0]);
			close(fd1[1]);
			close(fd2[0]);
			close(fd2[1]);

			// close both ends of all created fd# pipes (very important!)


            execvp(*cat_args, cat_args);
 	 	}
  		else // parent (assume no error)
        {
            // fork second child (to execute grep)
            if (fork() == 0)
			{
	  			// duplicate read end of cat->grep pipe to stdin (of grep)
				dup2(fd1[0], fileno(stdin));

	  			// duplicate write end of grep->cut pipe to stdout (of grep)
				//dup2(fd2[1], fileno(stdout));
	  			// close both ends of all created fd# pipes (very important!)
				close(fd1[0]);
				close(fd1[1]);
				close(fd2[0]);
				close(fd2[1]);
	  			execvp(*grep_args, grep_args);
			}
            else // parent (assume no error)
			{
	  			// fork third child (to execute cut)
					if (k <= 2) goto label;
					if (fork() == 0)
                {
                    // duplicate read end of grep->cut pipe to stadin (of cut)
			dup2(fd2[0], fileno(stdin));
                    // close both ends of all created fd# pipes (very important!)
			close(fd1[0]);
			close(fd1[1]);
			close(fd2[0]);
			close(fd2[1]);
                    execvp(*cut_args, cut_args);
                }
			}
        }

  		// only the parent gets here, close all pipes and wait for 3 children to finish

		label:
			close(fd1[0]);
  		close(fd1[1]);
  		close(fd2[0]);
  		close(fd2[1]);

  		for (i = 0; i < k; i++)
		{
    			wait(&status);
		}
	}
	else
	{
		printf("usage: %s input_file text_pattern\n", argv[0]);
	}

	return 0;
}