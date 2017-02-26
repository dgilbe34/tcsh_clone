/*
Andrew Gilbertson gilbe244@arctic
2017-02-16
proj03.cpp

Your Comments
*/

#include <iostream>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <cstdio>
#include <sstream>
#include <stdio.h>
#include <vector>

using std::cout; using std::endl; using std::string;

int main(int argc, char *argv[], char *env[]){
	
	//line number to output
	int line_number = 1;
	
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
	string command = string(input);
	std::vector<string> list;

	list.push_back(command);
	
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
	//loop to go through all of array, assuming not null; will place
	//each string token into the array at i
	while (tokens[i] != NULL){
		i++;
		tokens[i] = strtok( NULL, " \t \n" );          
	}
	
	//MAIN WHILE LOOP, breaks if quit is entered
	while(std::string(tokens[0]) != "quit"){
		

		
		//date functionality, using timer
		if (string(tokens[0]) == "date"){
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
			
			//two different loops, its the same thing. 
			//first if statement prints ignored arguments AFTER
			//the loop, so the user sees (and it isn't behind a ton of
			//arguments)
			if(tokens[1] != NULL)
			{
			for(int i = 0; env[i] != NULL;i++)
			{
				cout<<env[i]<<endl;
			}	
				cout<<"Ignored extra arguments"<<endl;
			}
			else{
			for(int i = 0; env[i]!= NULL;i++)
			{
				cout<<env[i]<<endl;
			}	
		}

		}
		
		//CD, a bit tricky; nested if statements 
		else if (string(tokens[0]) == "cd")
		{
			
			//checks to see if only input is CD, if it is, goes to 
			//home directory with some string manipulation
			if (tokens[1] == NULL)
			{
				int return_val = 0;
				string home = "/user/"+user;
				const char * c = home.c_str();
				return_val = chdir(c);
				if(return_val < 0)
					continue;
				
			}
			//else statement to handle when an argument given
			else{
			if (tokens[2] != NULL)
			{
				cout<<"Ignoring extra arguments and attempting to"<<
				"change dir to "<<tokens[1]<<endl;
			}
			int check = 0;
			char * direc = tokens[1];
			check = chdir(direc);
			
			//error if unable to change directory
			if (check < 0)
				cout<<"Error, unable to change dir"<<endl;
			}
			
			
		}
		else if (string(tokens[0]) == "clist")
		{
			for (int i = 0; i <list.size();i++)
			{
				cout<<list[i]<<endl;
			}
			
			
		}
		//else statement outputting an error for an incorrect 
		//input
		else{
			cout<<string(tokens[0])<<": command not found"<<endl;
			
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
	//loop to go through all of array, assuming not null; will place
	//each string token into the array at i
	while (tokens[i] != NULL){
		i++;
		tokens[i] = strtok( NULL, " \t \n" );          

		
	}
	
		
	}

}

