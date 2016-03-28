#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <algorithm>
using namespace std;


void parsing(string s, vector<string> &parsed)
{
    
    string sub;
    istringstream iss(s);

    while (iss >> sub)
    {
        
	//cout << "sub: " << sub << endl;        
	parsed.push_back(sub);

    }
    
}
struct CommandLine
{
    string command;
    vector<string> arguments;
    string input_pipe;
    vector<string> output_pipes;

};

struct Pipeline
{
    int fd[2];
    string pipeID;
};


void assigning(vector<string> parsed_vector, struct CommandLine &line)

{
    
    int i;
    line.command = parsed_vector[0];
    //cout << "command: " << line.command << endl;
    //cout << "parsed_vector.size(): " << parsed_vector.size() << endl;
    if( parsed_vector.size() > 1)
    {
              
        i = 1;        
        while((parsed_vector[i].compare("<|") != 0) && (parsed_vector[i].compare(">|") != 0))
	{
                    
		//cout<< "while girdim ve i =  " << parsed_vector[i] << endl; 
		line.arguments.push_back(parsed_vector[i]);
		//cout << "arguments = " << line.arguments[i-1] << endl;
		i++;
		//cout << "i = " << i << endl;
		if(i >= (parsed_vector.size()))
			break;
		
		           
	}
	
	//cout <<"i = " << i << endl;
        if(i < parsed_vector.size())
	{
		if(parsed_vector[i].compare("<|") == 0)
        	{
		    i++;
		    line.input_pipe = parsed_vector[i];
		    i = i + 2;

                    
		    while( i < parsed_vector.size())
		    {
		                
		        line.output_pipes.push_back(parsed_vector[i]);
		        i++;
		    }
                    
                    
        	}
		else if(parsed_vector[i].compare(">|") == 0)
		{
		    i++;
		            
		    while( i < parsed_vector.size())
		    {
		                
		        line.output_pipes.push_back(parsed_vector[i]);
		        i++;
		    }
		            
		}
	}       
        
         //cout << "cikiyo" << endl;         
                    
    }
    
    

}

void repeat(int index, vector<CommandLine> &commands, vector<Pipeline> &pipesForAll)
{
    
    int i = 0,j = 0, len;
    char buf[99999];
    int r;
    int outputFlag = 0;
    for(j = 0; j < pipesForAll.size(); j++)
    {
                  
        if(commands[index].input_pipe.compare(pipesForAll[j].pipeID) == 0)
        {
            //cout << "pipesForAll[ "<< j<< "].fd[1]" << pipesForAll[j].fd[1] << endl;
            close(pipesForAll[j].fd[1]);   
        }
        else
        {
            for(i = 0; i < commands[index].output_pipes.size(); i++)
            {
                if(commands[index].output_pipes[i] .compare(pipesForAll[j].pipeID) == 0)
                {
                    outputFlag = 1;
                }
            }
            if(outputFlag)
            {
                outputFlag = 0;
                close(pipesForAll[j].fd[0]);    
            }
            else
            {
                close(pipesForAll[j].fd[0]);
                close(pipesForAll[j].fd[1]);
            }

        }
                
    }  
            
        /*close(pipesForAll[0].fd[0]);
        close(pipesForAll[0].fd[1]);
        close(pipesForAll[1].fd[0]);
        close(pipesForAll[2].fd[0]);
        close(pipesForAll[3].fd[1]);*/
            
        len = read(0, buf, 99999);

        

        for(i = 0; i < commands[index].output_pipes.size(); i++)
        {
             
            for(j = 0; j < pipesForAll.size(); j++)
            {
                if( commands[index].output_pipes[i].compare(pipesForAll[j].pipeID ) == 0)
                {
                  
                  dup2(pipesForAll[j].fd[1],1);
                  close(pipesForAll[j].fd[1]);
                  close(pipesForAll[j].fd[0]);
 
                }
            }
            write(1, buf, len);
                       
        } 
        
        
    

}

void compute(vector<string> input, vector<CommandLine> &commands)
{
    int fdes[2];
    vector<Pipeline> pipesForAll;
    int max = 0;
    int numOfProcess = commands.size();
    //cout << "compute a girdi" << endl;

    //input_pipe.size() kadar pipe yap.
    for(int i = 0; i < input.size(); i++)
    {
        //cout << "1. fora girdi" << endl;
        Pipeline pipeline;
        pipe(fdes);
        pipeline.pipeID = input[i];
        pipeline.fd[0] = fdes[0];
        pipeline.fd[1] = fdes[1];
        pipesForAll.push_back(pipeline);

    }
    //cout << "1. fordan cıktı" << endl;
    //cout << "pipesForAll.size():  " << pipesForAll.size() << endl;

    //REPEATER ICIN DE PIPE OLUSTUR

    for(int k = 0; k < pipesForAll.size(); k++)
    {
        //cout << "max pipeID bulma for u" << endl;
        if(max < atoi((pipesForAll[k].pipeID).c_str()))
            max = atoi((pipesForAll[k].pipeID).c_str());
        //cout << "max :" << max << endl;
    }

    //cout << "commands.size(): " << commands.size() << endl; 

    for(int i = 0; i < numOfProcess; i++)
    {
        if(commands[i].output_pipes.size() > 1)
        {
            //cout << "output pipe size > 1 ve i = " <<  i << endl;
            struct CommandLine temp;
           
            
            //change orginal command's outgoing edges to repeater and repeaters outgoing to original
            for(int j = 0; j < commands[i].output_pipes.size(); j++)
                temp.output_pipes.push_back(commands[i].output_pipes[j]); // onceki process in outgoinglerini repeater a yazıyor

            max++;
            
            
            ostringstream ostr; //output string stream
            ostr << max; //use the string stream just like cout,
                //except the stream prints not to stdout but to a string.

            temp.input_pipe =ostr.str(); 
            commands[i].output_pipes.clear(); //onceki processin outgoinglerini siliyo ve repeatera baglıyo
            commands[i].output_pipes.push_back(ostr.str());      //onceki processle repetar arasına yeni eklediği pipe ın pipe no sunu outgoing olarak ekliyo
            
            struct Pipeline temp2; //process ve repeater arasında yaratılan pipe ı pipe vectorune ekliyo
            temp2.pipeID = ostr.str();
            pipe(temp2.fd);
            pipesForAll.push_back(temp2);
            
            commands.push_back(temp);  //repeater ı processlerin tutulduğu vectore ekliyo
        }           
    }
    numOfProcess = commands.size();

    /*for(int i = 0; i < numOfProcess; i++)
    {
        cout << "command: " << commands[i].command << " ";
        for(int j = 0; j < commands[i].arguments.size(); j++)
        {
            cout << "arguments: " << commands[i].arguments[j] << ", ";
        }
        cout << " ";
        cout << "input pipe: " << commands[i].input_pipe<< " ";
        for(int m = 0; m < commands[i].output_pipes.size(); m++)
        {
            cout << " output pipes: " << commands[i].output_pipes[m] << ", ";    
        }
        cout<< endl; 

    } 

    for(int j = 0; j < pipesForAll.size(); j++)
    {
        cout << "pipeID: " << pipesForAll[j].pipeID << " ";
        cout << "fd[0]: " << pipesForAll[j].fd[0] << " ";
        cout << "fd[1]: " << pipesForAll[j].fd[1] << " ";
    }
    cout << endl;*/


    // SONRA FORK() VE 3 CASE VAR. INPUT > 0, BIR OUTPUT, BIRDEN COK OUTPUT

    bool flagForRepeater = false;
    for(int i = 0; i < numOfProcess; i++)
    {
        
        flagForRepeater = false;

        if(fork() == 0)
        {
            if(!(commands[i].input_pipe.empty()))
            {
                for( int j = 0; j < pipesForAll.size(); j++)
                {
                    
                    if(commands[i].input_pipe.compare(pipesForAll[j].pipeID) == 0)
                    {
                        //cout << "input pipe pipeID: " << pipesForAll[j].pipeID  << endl;
                        //cout << "commands[i].command: " << commands[i].command << endl;
                        //close(pipesForAll[j].fd[1]);
                        dup2(pipesForAll[j].fd[0], 0);
                        //close(pipesForAll[j].fd[0]);
                        
                    }
                }
            }
           

            if(commands[i].output_pipes.size() == 1)
            {
                for(int j = 0; j < pipesForAll.size(); j++)
                {
                    
                    if(commands[i].output_pipes[0].compare(pipesForAll[j].pipeID) == 0)
                    {
                        
                        //cout << "output pipe pipeID: " << pipesForAll[j].pipeID  << endl;
                        //cout << "commands[i].command: " << commands[i].command << endl;
                        //close(pipesForAll[j].fd[0]);
                        dup2(pipesForAll[j].fd[1],1);
                        //close(pipesForAll[j].fd[1]);
                        
                    }
                }
                
            }
            else if(commands[i].output_pipes.size() > 1)
            {
                //cout << "commands[i].command: " << commands[i].command << endl;
                flagForRepeater = true;
                repeat(i, commands, pipesForAll);
                
            }

            for (int j = 0; j < pipesForAll.size(); j++)
            {
                close(pipesForAll[j].fd[0]);
                close(pipesForAll[j].fd[1]);
            }

            if (flagForRepeater)
                exit(0);

            // EXEC THE PROCESS


            //EXECVP ARGUMANLARI ICIN CHAR * ARRAYI YARAT

           
            char* args[commands[i].arguments.size() + 2];
            int a = 0;
            //COMMANDI CHAR POINTERINA CEVIRDIM
            char* char_command = new char[ commands[i].command.length() + 1];
            strcpy(char_command, commands[i].command.c_str());
            args[a] = char_command;

            //ARGUMENTS CHAR POINTERLARINA CEVIR
            for(int k = 0; k < commands[i].arguments.size(); k++)
            {
                char* char_arg = new char[ commands[i].arguments[k].length() + 1];
                strcpy(char_arg, commands[i].arguments[k].c_str() );
                a++;
                args[a] = char_arg;
            }

            args[++a] = NULL;

            /*for(int i = 0; i < sizeof(args); i++)
                cout<< "args[" << i <<"]: " << args[i] << endl;*/
            execvp(args[0], args);
           
        }
        
    }

    for (int j = 0; j < pipesForAll.size(); j++)
    {
        close(pipesForAll[j].fd[0]);
        close(pipesForAll[j].fd[1]);
    }
    
    for(int i = 0; i < numOfProcess; i++)
    {   
        int w;
        wait(&w);
    }   

}

void reading()
{
    string readline;
    vector<CommandLine> final_commands;
    vector<string> input_pipes;
    vector<string> outputpipes;

    while (true) {
        getline(cin, readline);

        if(readline.compare("quit") == 0)
            break;
        else
        {
            //cout<< "Geldi" << endl;
            vector<string> parsed_vector;
            struct CommandLine command;
            parsing(readline, parsed_vector);
            assigning(parsed_vector, command);
            //cout<< "Geldi command: " << endl;
            final_commands.push_back(command);

            
            // checks if input pipe is empty. If not, checks if the pipe was pushed the vector before. If not pushes the pipe id to the vector
            if (!(command.input_pipe.empty()))
            {
               //cout<< "Girdi input pipe a" << endl;

               if(!(find(input_pipes.begin(), input_pipes.end(), command.input_pipe) != input_pipes.end()) ){
                    /* input_pipes does not contain input_pipe */

                    input_pipes.push_back(command.input_pipe);
                    //cout << "input pipe pushlandı: " << command.input_pipe << endl;
                } 
            }
            // checks if output pipe is empty. If not, checks if the out pipes were pushed the vector before. If not pushes the pipe id to the vector
            if(!(command.output_pipes.empty()))
            {
               //cout << "Girdi output pipe a" << endl;
               for(int i = 0; i < command.output_pipes.size(); i++)
               {
                    if(!(find(outputpipes.begin(), outputpipes.end(), command.output_pipes[i]) != outputpipes.end()) ){
                        /* input_pipes does not contain input_pipe */
                        outputpipes.push_back(command.output_pipes[i]);
                        //cout<< "Output pipe a pushlandı: " << command.output_pipes[i] << endl;
                    } 
               }
               
            }

            //checks if the input pipe vector equals to the output pipe vectors. If they are equal, stops reading line( stops the program for now!!! change!!)
           
                //cout << "pipelarım bos degil" << endl;
		if( input_pipes.size() == 0 && outputpipes.size() == 0)
		{
			 //cout << "pipelarım bos degil" << endl;
			 compute(input_pipes, final_commands);
		         //FOR THE NEW START CLEAN ALL THE CONTENTS TO PREVENT TO BE WRITTEN ON THE TERMINAL AGAIN
		         input_pipes.clear();
		         outputpipes.clear();
		         final_commands.clear();
		         parsed_vector.clear();	
		}
		else 
		{
			 
			 sort(input_pipes.begin(), input_pipes.end());
                	 sort(outputpipes.begin(), outputpipes.end());
		         if(input_pipes == outputpipes)
		         {
		            //cout << "start graph!" << endl;
		            //COMPUTE FONKSIYONUNU CAGIR ve input_pipes.size() ve final_commands ı compute() a arguman olarak ver!!!!
		            compute(input_pipes, final_commands);
		            //FOR THE NEW START CLEAN ALL THE CONTENTS TO PREVENT TO BE WRITTEN ON THE TERMINAL AGAIN
		            input_pipes.clear();
		            outputpipes.clear();
		            final_commands.clear();
		            parsed_vector.clear();

		            
		         }
		}
                
             
            
           

            
            


        }

    }

    /**********************************************************PRINT**************************************************************/

    /*cout << "final_commands size:" << final_commands.size() << endl;
    for(int m = 0; m < final_commands.size(); m++)
    {
        cout << "Command: " << final_commands[m].command << endl;
        for(int j = 0; j < final_commands[m].arguments.size(); j++)
            cout << "Args: " << final_commands[m].arguments[j] << ",";
        cout << endl;
        cout << "Input: " << final_commands[m].input_pipe << endl;
        for(int g = 0; g < final_commands[m].output_pipes.size(); g++)
            cout << "Output: " << final_commands[m].output_pipes[g] << ",";
        cout << endl;
                
    }*/ 
    /***************************************************************************************************************************/
}




int main(int argc, char *argv[]) {

    
    reading();
        

    
        
    return 0;   
        

}
   
