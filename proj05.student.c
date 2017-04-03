/*
Andrew Gilbertson gilbe244@arctic
2017-02-22
proj05.student.c
* 
FIXED IN THIS VERSIION:
* Faulty DLIST
* Cleaned up (a very small bit) of cd
* Added the external command ability
* */

#include <sys/wait.h>
#include <iostream>
#include <string.h>
#include <string>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <cstdio>
#include <sstream>
#include <stdio.h>
#include <deque>

using std::cout; using std::endl; using std::string;

int main(int argc, char *argv[], char *env[]){
	
	//line number to output
	int line_number = 1;
	
	extern char **environ;
	
	//variable block: current user, working directory, timer stuff
	std::string user = getlogin();
	std::string wd = get_current_dir_name();
	time_t timer;
	struct tm * time_local;
	
	//first output with line 1
	cout<<"<"<<line_number<<" "<<user<<">";
	
	//input, capped at 128 as stated in specs
	char input[128];
	
	//getline, storing in the char array
	std::cin.getline(input,128);
	std::deque<string> list;
	std::deque<string> directories;
	
	directories.push_front(wd);

	
	//loop to checck if getline is 0, to not increment line
	while(input[0]=='\0')
	{
			cout<<"<"<<line_number<<" "<<user<<">";
			std::cin.getline(input,128);

	}
	
	//to hold all strtok tokens, split by tabs and new lines and spaces
	//size 12 because of assumption there won't be more than 12 arguments
	//ever entered
	char *tokens[12];
	
	char *external[10];
	
	
	//loop to split up all tokens with specified delims
	int i = 0;
	tokens[i] = strtok(input, " \t \n");
	
	//this loop accounts for only a space being entered, and just re
	//grabs data and attempts to add it to the array
	while(tokens[0] == NULL){
			cout<<"<"<<line_number<<" "<<user<<">";
			std::cin.getline(input,128);
			tokens[i] = strtok(input, " \t \n");

		
	}
	string command = "";

	//loop to go through all of array, assuming not null; will place
	//each string token into the array at i
	while (tokens[i] != NULL){
		command+=" "+string(tokens[i]);
		i++;
		tokens[i] = strtok( NULL, " \t \n" );   
		       
	}
	//start off with putting command in front 
	list.push_front(command);


	
	//MAIN WHILE LOOP, breaks if quit is entered
	while(std::string(tokens[0]) != "quit"){
		

		//we are testing to see if the command is the !# command,
		//to go back to a specific point
	    if(string(tokens[0]).at(0) == '!')
		{
			string number = string(tokens[0]);
			string real_num;
			for(int i = 1; number[i] !='\0';i++)
			{
				real_num+=number[i];
				
			}
			unsigned int dex;
			//a quick check to make sure we have a number
			if (!isalpha(real_num[0])){
			std::istringstream (real_num) >>dex;
			
			//this will make sure we can actually go in at that point
			if (dex < list.size()){
			//loop to split up all tokens with specified delims
			int i = 0;
			tokens[i] = strtok(strdup(list[list.size()-dex].c_str()), " \t \n");
	

			//loop to go through all of array, assuming not null; will place
			//each string token into the array at i
			while (tokens[i] != NULL){
				i++;
				tokens[i] = strtok( NULL, " \t \n" );   
		       
									}
									
				//print your shiny new token
				cout<<tokens[0]<<endl;
				
				//continue to re go through loop with newly loaded 
				//tokens[0]
				continue;
			}
			else{
				
				cout<<"You entered a number that was too large"<<endl;
				
			}
			
		}
		else{
			
			cout<<"you didn't enter a number!"<<endl;
		}
	}
		
		//date functionality, using timer
		else if (string(tokens[0]) == "date"){
			if(tokens[1] != NULL)
			{
				cout<<"Ignoring extra arguments"<<endl;
				
			}
			//getting time and converting to actual time readable
			//by humans
			time(&timer);
			time_local = localtime(&timer);
			cout<<asctime(time_local);		
		}
		
		//current directory, pretty straightforward. gets new before
		//outputting, in case it has changed
		else if (string(tokens[0]) == "curr"){
			if(tokens[1] != NULL)
			{
				cout<<"Ignoring extra arguments"<<endl;
				
			}
			
			//re gets current dir name and prints it 
			wd = get_current_dir_name();
			cout<<wd<<endl;	
		}
		
		//env functionality, loops through env given in MAIN
		else if (string(tokens[0]) == "env"){

			//was getting weird feedback so i had to do a test 
			//for both
			 if(tokens[2] != NULL && tokens[1] != NULL)
			{
				int check;
				check = setenv(tokens[1],tokens[2],1);
				if(check < 0)
					cout<<"Error changing value"<<endl;
				
			}
			
			//continued, tacked on both cause of some weird cased
			else if(tokens[1] != NULL && tokens[2] == NULL)
			{
				const char* get = getenv(tokens[1]);
				if(get!=NULL){
				
				cout<<get<<endl;
				}
				else
				{
				cout<<"Invalid env variable get attempt"<<endl;
				
				}
				}

			else{
				
			for(int i = 0; environ[i]!= NULL;i++)
			{
				cout<<environ[i]<<endl;
			}	
				
			}

		}
		
		//CD, a bit tricky; nested if statements 
		else if (string(tokens[0]) == "cd")
		{
			//a check to make sure we dont enter the same 
			//working directory into dlist
			string temp = get_current_dir_name();
			
			//checks to see if only input is CD, if it is, goes to 
			//home directory with some string manipulation
			if (tokens[1] == NULL)
			{
				int return_val = 0;
				string home = "/user/"+user;
				const char * c = home.c_str();
				return_val = chdir(c);
				if(return_val < 0){
				}
					
			std::string wd = get_current_dir_name();
			if (temp != wd){
			//adding to directoies list 
			directories.push_front(wd);
}
				
			}
			//else statement to handle when an argument given
			else{
			if (tokens[2] != NULL)
			{
				cout<<"Ignoring extra arguments and attempting to"<<
				"change dir to "<<tokens[1]<<endl;
			}
			
			//all of this was done before realizing i could check
			//the string with a .at(). I may go back and change it 
			//between weeks, but it works for now so I am going to
			//stick with it
			if (tokens[1])
			{
					int check = 0;
					char * direc = tokens[1];
					check = chdir(direc);
			
				//error if unable to change directory
		if (check < 0){
			
				//a loop to grab the input to check 
				string number = "";
				for (int i = 0; tokens[1][i] !='\0';i++)
				{
					number+=tokens[1][i];
				}
				//testing the possibility of dlist entry
				if(number[0]=='#'){
					
					
					//nested loop to grab characters after #
					string new_number = "";
				
					for(int i = 1;number[i] !='\0';i++)
					{
						new_number+=number[i];
					
					}
					unsigned int spot;

					std::istringstream (new_number) >> spot;
				
					if (spot > directories.size())
					{
					cout<<"index doesnt exist"<<endl; 
					}
					//attempt to cd if it was valid 
					else{
					const char * c = directories[directories.size()-spot].c_str();
					int return_val = chdir(c);
					if(return_val < 0){

						}
					std::string wd = get_current_dir_name();
					if (temp != wd){
					directories.push_front(wd);
				}
				}
				}
				
				//checking possiblity of ~and user
				else if (number[0] == '~')
				{
					string user = "";
				
					for(int i = 1;number[i] !='\0';i++)
					{
					user+=number[i];
					
					}
					string to_home = "/user/"+user;
					const char * c = to_home.c_str();
					int check = chdir(c);
					if (check < 0)
					{
						cout<<"couldn't change directory"<<endl;
					}
					
								//a quick check to make sure we aren't going to change 
			//to one we are already in
			std::string wd = get_current_dir_name();
			if (temp != wd){
			directories.push_front(wd);
		}
					
					}
				//none of those were the case, error
				else{
					cout<<"Invalid directory input"<<endl;
					
				}
			}
			else{
			std::string wd = get_current_dir_name();
			if (temp != wd){
			directories.push_front(wd);
		}
		}
			
		
					
				}
				else if(tokens[0] && tokens[1] == NULL)
				{
					
								//a quick check to make sure we aren't going to change 
			//to one we are already in
			std::string wd = get_current_dir_name();
			if (temp != wd){
			directories.push_front(wd);
		}
			
		}

}

					}	


		//will just add all commands to a deque 
		else if (string(tokens[0]) == "clist")
		{
			//was having trouble getting the numbers to work,
			//so i broke it up into 2 loops. not efficient 
			//but it worked
			if (list.size() <= 10)
			{
				//because of front insertion, i had to get a little
				//clever with the numbers
				int line = 1;
				for (unsigned int i = list.size(); i >0;i--)
				{
					cout<<line<<": "<<list[i-1]<<endl;
					line++;
				}
			}
			//same thing
			else{
				
				int line = list.size()-10;

				for(unsigned int i = 10;i >0;i--)
				{
					cout<<line<<": "<<list[i]<<endl;
					line++;
					
				}
				
			}
			
		}
		

		//dlist, same set up as clist 
		else if(string(tokens[0]) == "dlist")
		{
			if(directories.size()<=10)
			{
			int line = 1;
			for (unsigned int i = directories.size(); i > 0; i--)
			{
				
				cout<<line<<": "<<directories[i-1]<<endl;
				line++;
			}
			}
			else{
				
				int line = directories.size()-10;;
				for(unsigned int i = 10; i > 0; i--)
				{
						cout<<line<<": "<<directories[i]<<endl;
						line++;
				}
				
			}
				
			
		}
		//the else here is where the external commands will be 
		//handled, with 3 distince casees
		else{
			
			//starting off by putting all arguments into a new
			//char pointer array to check against
			int count = 0;
			for(int i = 0; tokens[i]!=NULL; i++)
			{
				count++;
				external[i] = (char*)tokens[i];
				
			}
			
			//sets the last part to NULL to satisfy requireent of 
			//exec
			external[count] = NULL;
			
			
			//FIRST CHECK if last token is ampersand 
			if (string(tokens[count-1]).at(0) == '&')
			{
				
			//replacing ampersand with NULL so it doesn't get
			//processed
			external[count-1] = NULL;
			int flag;
			
			//fork call
			int pid = fork();
			if (pid < 0)
			{
				cout<<"Fork fail"<<endl;
				flag = -1;
				exit(flag);
			}
			else if (pid > 0)
			{
				flag = 0;
				
			}
			//child process exec call
			else{
				flag = execvp(external[0],external);
				if (flag < 0){
					cout<<string(tokens[0])<<": not a real command"<<endl;
			}
				exit(flag);

		}	
				
				}
				
			//checking if the last character of the last argument is &, which is valid
			//in this project
			else if(string(tokens[count-1]).at(string(tokens[count-1]).length()-1) =='&')
			{
			//this took me FOREVER, I had to chop off the &. did it with an array,
			//a loop, and a string token
			char temp[string(tokens[count-1]).length()];
			for(unsigned int i = 0;i<string(tokens[count-1]).length();i++)
			{
				temp[i] = tokens[count-1][i];
			}
			
			external[count-1] = strtok(temp,"&");
			int flag;
			int pid = fork();
			if (pid < 0)
			{
				cout<<"Fork fail"<<endl;
				flag = -1;
				exit(flag);
			}
			//parent process with NO WAIT (background process)
			else if (pid > 0)
			{
				flag = 0;
				
			}
			//same child process call
			else{
				flag = execvp(external[0],external);
				//checks will see if external was able to work or not
				if (flag < 0){
					cout<<string(tokens[0])<<": not a real command"<<endl;
			}
				exit(flag);

		}	
						
			}
				
				
			//this else is the standard handle, will be called with no &
			//because of the wait inside of it 
			else{
			int flag;
			int pid = fork();
			if (pid < 0)
			{
				cout<<"Fork fail"<<endl;
				flag = -1;
				exit(flag);
			}
			//parent process WITH WAIT
			else if (pid > 0)
			{
				waitpid(pid,NULL,0);
				flag = 0;
				
			}
			//child process; standard 
			else{
				flag = execvp(external[0],external);
				if (flag < 0){
					cout<<string(tokens[0])<<": not a real command"<<endl;
			}
					exit(flag);

		}
			
		}
	}
		
		//increment line number, output, and re grab input with same
		//loop
		line_number++;
		cout<<"<"<<line_number<<" "<<user<<">";
	int i = 0;	
	std::cin.getline(input,128);


	
	//okay, i put this while loop here specifically AGAIN to 
	//account for an empty enter. with no command, it won't loop back
	//to the top, and keeps the line number constant. 
	while(input[0]=='\0')
	{
			cout<<"<"<<line_number<<" "<<user<<">";

			std::cin.getline(input,128);

	}
	//re putting input back into array
	tokens[i] = strtok(input, " \t \n");
	
	
	//this loop accounts for only a space being entered, and just re
	//grabs data and attempts to add it to the array
	while(tokens[0] == NULL){
			cout<<"<"<<line_number<<" "<<user<<">";
			std::cin.getline(input,128);
			tokens[i] = strtok(input, " \t \n");

		
	}
	
	
	string command = "";
	//loop to go through all of array, assuming not null; will place
	//each string token into the array at i
	while (tokens[i] != NULL){
		command+=" "+string(tokens[i]);

		i++;
		tokens[i] = strtok( NULL, " \t \n" );          

		
	}
	//adding the command to the front of the deque 
	list.push_front(command);

	
		
	}

}

