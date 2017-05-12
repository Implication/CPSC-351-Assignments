#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/shm.h>



using namespace std;

//Any exit with the status of 1 means their was an error when calling a system call

struct  message_buf
{
	long mtype;	//Priority of the message
	int mtext;	//Type of ipc we will be doing
	//For reference, 0 for named pipes, 1 for shared memory, 2 for termination
	bool success; //True when operation is sucessfull, false when data is inaccurate
	int size; //Size of the data being sent over
}; 

//Struct used to store the bounded buffer for shared memory
struct bound_buf
{
	char buffer[3000];
	int in;
	int out;
};



int in = 0;
int out = 0;

// Generates a string of ASCII characters for use as data
// Takes in an integer that specifies how many bytes/chars the string contains 
char * GenerateData (int numberOfBytes)
{
    char * result = new char[numberOfBytes];

    char value = 65;
    for (int i = 0; i < numberOfBytes; i++)
    {
        result[i] = value++;

        if (value > 126)
            value = 65;
    }

    result[numberOfBytes-1] = '\0';

    return result;
}

void parent (int size,string msg, pid_t pid)
{	
	//Global variables 
	int mqid; //Unique id of the message queue being used for parent/child
	key_t key = 1235;	//Unique key for the parent
	int msgflg = (IPC_CREAT | 0666);	//flag for messages 
	//Set the message queue here
	if(mqid = msgget(key, msgflg) == -1)
	{
		perror("msgget error in parent"); 
		exit(1);
	}
	//Create the struct for our parent to use in the message queue
	message_buf sbuf;
	int x = 0; //Change back later to zero
	sbuf.mtype = 1;	//Indicates the first message whilch will be to do a named pipe
	//We loop to send a number to represent the ipc call we want to do
	sbuf.size = size;
	while(x < 3)
	{
		sbuf.mtext = x;
		if(x == 0)
		{	//Zero is for named pipe
			int fd;	//File descriptor, basicaly the unique integer that holds location of the file
			char * myfifo = "CPSC351-PIPE";	//This is the name pipe of the file	
			//Make the pipe 
			mkfifo(myfifo,0666);	
			//Open the pipe for writing, if the fd is -1, then print the error	
			//Before opening the pipe we need to send a message to the child.
			if(msgsnd(mqid, &sbuf, sizeof(sbuf) - sizeof(long), msgflg) < 0)
			{
				perror("msgsnd in parent");
				exit(1);		
			}
			//Open the pipe for writing, if it is -1, then prints an error, creates a block
			fd = open(myfifo, O_WRONLY | O_CREAT);
			if (fd < 0)
			{
				perror("File descriptor in parent");
				exit(1);
			}
			//Holds our string created in main to pass to write
			const char *m = msg.c_str();	
			ssize_t t = 1;	//Used for error checking
			//Write into the pipe based on the size of our string
			if ((t = write(fd,m,size) < 0))
			{
				perror("write");
				exit(1);
			}
			//Then we send a message to tell the child to be ready to receive the data
			//Close the file then wait for the child	
			close(fd);			
			//Recive the message of whether this operation was successful
			if((msgrcv(mqid, &sbuf, sizeof(sbuf) - sizeof(long), 2, 0)) < 0)
			{
				perror("msgrcv on parent for named pipes sucess");
				exit(1);
			}
			if(sbuf.success = true)
				cout << "[Parent]: Sent data sucessfully through named pipes" << endl;
			else
				cout << "[Parent]: Sent data unsucessfully through named pipes" << endl;
			//Set sucess back to false for next operation
			sbuf.success = false;
		} //End of named pipe
		if(x == 1)
		{	//Beginning of shared memory
			key_t smkey = 1366;	//Unique key for shared memory
			sbuf.mtype = 3;		//send with the priority of 3 to signal shared memory
			int shmid;
			bound_buf * p;
			//Send a message to prepare for shared memory
			if(msgsnd(mqid, &sbuf, sizeof(sbuf) - sizeof(long), msgflg) < 0)
			{
				perror("msgsnd in shared memory for parent");
				exit(1);
			}
			//Create the segment of memory
			if((shmid = shmget(smkey, sizeof(p), IPC_CREAT | 0666)) < 0){
				perror("shmget in parent");
				exit(1);			
			}	
			//Attach the segment to our data
			if((p = (bound_buf*) shmat(shmid, 0, 0)) == (bound_buf *) -1) {
				perror("shmat in parent");
				exit(1);
			}
			int i = 0; //Use this to access characters and put them in the buffer
			//Loop through the message string and place it in the buffer
			while(i < size-1){
				while(((p->in + 1) % 200) == p->out);
				p->buffer[p->in] = msg.at(i);
				p->in = (p->in+1) % 200;
				i++;
			}
			//Receive a message indicating if shared memory was successful
			if((msgrcv(mqid, &sbuf, sizeof(sbuf) - sizeof(long), 4, 0)) < 0){
				perror("msgrcv for shared memory sucess in parent");
				exit(1);
			}
			shmdt(p);	//Detach the struct from the shared memory
			if(sbuf.success = true)
				cout << "[Parent]: Sent data sucessfully through shared memory" << endl;
			else
				cout << "[Parent]: Sent data unsucessfully through shared memory" << endl;
		}	//End of shared memory
		if(x == 2)
		{
			cout << "[Parent]: Terminating child..." << endl;
			kill(pid,SIGKILL);
			cout << "Exiting Program..." << endl;
			exit(0);
		} // End of program
		x++;
	}
}

void child (pid_t pid)
{
	//Create access to the existing message queue for the child
	int mqid;
	key_t key = 1235;
	int msgflg = (IPC_CREAT | 0666);
	message_buf rbuf;
	if(mqid = msgget(key, IPC_CREAT | 0666) == -1)
	{
		perror("msgget error in child"); 
		exit(1);
	}
	//Message receive to indicate were doign Named pipes
	if((msgrcv(mqid, &rbuf, sizeof(rbuf) - sizeof(long), 1, 0)) < 0)
	{
		perror("msgrcv");
		exit(1);
	}
	rbuf.mtype = 2;	//Change the priority to indicate to parent the success of reading the data
		if(rbuf.mtext == 0)
		{
			int fd;	//Special file descriptor location of our pip
			char * myfifo = "CPSC351-PIPE";	//This specifies the name of our pipe
			char rdmsg[rbuf.size];	//This receives the data from our pipe
			//Open the data to read into the child, and check for errors
			fd = open(myfifo, O_RDONLY);
			if (fd < 0)
			{
				perror("fd in child");
				exit(1);
			}
			//Read and check for any errors while reading the data
			ssize_t t;
			if((t = read(fd,rdmsg,rbuf.size)) < 0)
			{
				perror("read");
				exit(1);		
			}
			int ds = strlen(rdmsg);
			//Check if the length of our message is equal to the size sent by the parent
			if(ds == (rbuf.size - 1))
			{
				rbuf.success = true;
				cout << "[Child]: Named pipe reading sucessful" << endl;
			}
			else
			{
				cout << "[Child]: Named pipe reading unsucessful" << endl;
				rbuf.success = false;
			}
			//Send a message back to the parent to indicate the sucess of named pipes
			if((msgsnd(mqid, &rbuf, sizeof(rbuf) - sizeof(long), msgflg)) < 0)
			{
				perror("msgsnd");
				exit(1);
			}
		}	//End of named pipes
		//Receive next message to do shared memory
	if((msgrcv(mqid, &rbuf, sizeof(rbuf) - sizeof(long), 3, 0)) < 0)
	{
		perror("msgrcv");
		exit(1);
	}
	rbuf.mtype = 4; //Indicate we are done receiving shared memory with this
	if(rbuf.mtext == 1)
	{
		key_t smkey = 1366;
		int shmid;
		char *shm, *s;
		bound_buf* c;
		//Locate the segmant of shared memory
		if((shmid = shmget(smkey,sizeof(c),0666)) < 0) {
			perror("shmget in child");
			exit(1);
		}
		//Attack the segment to our data
		if((c = (bound_buf*) shmat(shmid,0,0)) == (bound_buf *) -1) {
			perror("shmat");
			exit(1);
		}
		//Read what the server puts in the memory
		string data;
		while (data.length() < rbuf.size - 1) {
			while (c->in == c->out);
			data += c->buffer[out];
			c->out = (c->out + 1) % 200;		
		}
		//If they are equal then the data was sent successfully
		int ds = data.length();
		if(ds == (rbuf.size - 1)){
			cout << "[Child]: Read data through shared memory sucessfully" << endl;
			rbuf.mtext = 1;
		}
		//Otherwise we were unsucessful
		else{
			cout << "[Child]: Read data through shared memory unsuccessful" << endl;
			rbuf.mtext = 0;
		}
		//Send a message back into parent indicating whether we were sucessful
		if(msgsnd(mqid, &rbuf, sizeof(rbuf) - sizeof(long), msgflg) < 0){
			perror("msgsnd");
			exit(1);
		}
	}
}

int main(int argc, char **argv){
	if(argv[1] == NULL)
	{
	std::cout << "NO arguments were given, program will now exit" << std::endl;
	}
	else{
		int size = atoi(argv[1]) + 1; //Converts the number characters into a type int for the size of the 			character string in bits
		string s = GenerateData(size);
		//Create a child process
		pid_t pid = fork();
		//Error Checking: if the pid is less than 0, than an error occured when forking
		if(pid < -1)
		{
			perror ("fork");
		}
		//pid is zero, then the process is a child, so launch the child function
		if (pid == 0)
		{
			cout << "[Child]: " << pid << endl;
			child(pid);
		}
		//pid is greater than zero, then its the original process or the parent
		if (pid > 0){
			cout << "[Parent]: " << pid << endl;
			cout << "[Parent]: Generating a string of " << size - 1 << " bytes" << endl;
			parent(size,s,pid);
		}
	}	//End of forking statements
	return 0;
}
