#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
	//no arguments other than pipe
	if (argc <= 1) {
		exit(EINVAL);
	}

	//only one argument, just run the argument
	else if (argc == 2) {
		pid_t pid = fork(); //fork returns 0 if it is running child process, 1 if it is parent and negative if it is error
		//in child process
		if (pid == 0) {
			if (execlp(argv[1], argv[1], NULL) == -1) {
               exit(errno);
			}
		}
		//in parent process
		else if (pid > 0){
			int status = 0;
			wait(&status);

			if (WIFEXITED(status)) {
				if (WEXITSTATUS(status)) {
					exit(WEXITSTATUS(status));
				}
			}
			return 0;
		}
		//error
		else {
			exit(errno);
		}
	}

	//more than one argument
	else {
		pid_t *processes = (pid_t *)malloc((argc - 1) * sizeof(pid_t));
		
		int fd[2];
		if (pipe(fd) == -1) {
			exit(errno);
		}
		pid_t pid = fork();
		if (pid < 0) {
			exit(errno);
		}
		else if (pid == 0) {
			if (dup2(fd[1], 1) < 0) {
				exit(errno);
			}
			if (close(fd[1]) < 0) {
				exit(errno);
			}
			if (close(fd[0]) < 0) {
				exit(errno);
			}
			if (execlp(argv[1], argv[1], NULL) == -1) {
				exit(errno);
			}
		}
		else {
			//add child's pid into the processes array
			processes[0] = pid;
			if (close(fd[1]) < 0) {
				exit(errno);
			}
		}

		for (int i = 2; i < argc; i++) {
			int read_end = fd[0];

			//last argument
			if (i == argc - 1) {
				pid_t pid = fork();
				if (pid < 0) {
					exit(errno);
				}
				else if (pid == 0) {
					if (dup2(read_end, 0) < 0) {
						exit(errno);
					}
					if (execlp(argv[i], argv[i], NULL) == -1) {
						exit(errno);
					}
				}
				else {
					processes[i-1] = pid;
					if (close(read_end) < 0) {
						exit(errno);
					}
				}

			}
			//middle argument
			else {

				if (pipe(fd) < 0) {
					exit(errno);
				}

				pid_t pid = fork();

				if (pid < 0) {
					exit(errno);
				}
				else if (pid == 0) {
					if (dup2(read_end, 0) < 0) {
						exit(errno);
					}
					if (dup2(fd[1], 1) < 0) {
						exit(errno);
					}
					if (close(fd[1]) < 0) {
						exit(errno);
					}
					if (close(read_end) < 0) {
						exit(errno);
					}
					if (close(fd[0]) < 0) {
						exit(errno);
					}
					if (execlp(argv[i], argv[i], NULL) == -1) {
						exit(errno);
					}
				}
				else {
					processes[i-1] = pid;
					if (close(fd[1]) < 0) {
						exit(errno);
					}
					if (close(read_end) < 0) {
						exit(errno);
					}
 				}
			}
		}

		//wait for processes
		for (int j = 0; j < argc - 1; j++) {
			int status = 0;
			waitpid(processes[j], &status, 0);
			if (WIFEXITED(status)) {
				if (WEXITSTATUS(status)) {
					exit(WEXITSTATUS(status));
				}
			}
		}
	}
	
	/*if (argc == 3) {

		int fd[2];

		if (pipe(fd) == -1) {
			exit(errno);
		}

		pid_t pid = fork();

		if (pid < 0) {
			exit(errno);
		}

		else if (pid == 0) {
			//set up file descriptors
			if (dup2(fd[1], 1) < 0) {
				exit(errno);
			}
			if (close(fd[1]) < 0) {
				exit(errno);
			}
			if (close(fd[0]) < 0) {
				exit(errno);
			}
			//run first argument
			if (execlp(argv[1], argv[1], NULL) == -1) {
				exit(errno);
			}
		}

		else {
			//wait
			int status;
			wait(&status);
			if (WIFEXITED(status)) {
				if (WEXITSTATUS(status)) {
					exit(WEXITSTATUS(status));
				}
			}

			if (close(fd[1]) < 0) {
				exit(errno);
			}
			//create another child
			pid_t pid2 = fork();
			if (pid2 < 0) {
				exit(errno);
			}
			else if (pid2 == 0) {
				//set up file descriptors
				if (dup2(fd[0], 0) < 0) {
					exit(errno);
				}
				if (close(fd[0]) < 0) {
					exit(errno);
				}
				//run second argument
				if (execlp(argv[2], argv[2], NULL) == -1) {
					exit(errno);
				}
			}
			else {
				if (close(fd[0]) < 0) {
					exit(errno);
				}
			}
		}
	} */
	return 0;
}
