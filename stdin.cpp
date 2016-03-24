#include <iostream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

void parsing(string s, vector<string> &parsed)
{
    
    string sub;
    istringstream iss(s);

    while (iss >> sub)
    {
        
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
void assigning(vector<string> parsed_vector, struct CommandLine &line)

{
    
    int i;
    line.command = parsed_vector[0];

    if( parsed_vector.size() > 1)
    {
                
                
        for( i = 1; (parsed_vector[i].compare("<|") != 0) && (parsed_vector[i].compare(">|") != 0) && i < parsed_vector.size(); i++)
        {
                    

            line.arguments.push_back(parsed_vector[i]);
        }

                
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
            vector<string> parsed_vector;
            struct CommandLine command;
            parsing(readline, parsed_vector);
            assigning(parsed_vector, command);

            cout<< "Geldi" << endl;
            // checks if input pipe is empty. If not, checks if the pipe was pushed the vector before. If not pushes the pipe id to the vector
            if (!(command.input_pipe.empty()))
            {
               cout<< "Girdi input pipe a" << endl;
               if(!(find(input_pipes.begin(), input_pipes.end(), command.input_pipe) != input_pipes.end()) ){
                    /* input_pipes does not contain input_pipe */

                    input_pipes.push_back(command.input_pipe);
                    cout << "input pipe pushlandı: " << command.input_pipe << endl;
                } 
            }
            // checks if output pipe is empty. If not, checks if the out pipes were pushed the vector before. If not pushes the pipe id to the vector
            if(!(command.output_pipes.empty()))
            {
               cout << "Girdi output pipe a" << endl;
               for(int i = 0; i < command.output_pipes.size(); i++)
               {
                    if(!(find(outputpipes.begin(), outputpipes.end(), command.output_pipes[i]) != outputpipes.end()) ){
                        /* input_pipes does not contain input_pipe */
                        outputpipes.push_back(command.output_pipes[i]);
                        cout<< "Output pipe a pushlandı: " << command.output_pipes[i] << endl;
                    } 
               }
               
            }

            //checks if the input pipe vector equals to the output pipe vectors. If they are equal, stops reading line( stops the program for now!!! change!!)
            if (!(input_pipes.empty()) && !(outputpipes.empty()))
            {
                cout << "pipelarım bos degil" << endl;
                if(input_pipes == outputpipes)
                {
                    cout << "start graph!" << endl;
                    break;
                }
             
            }
           

            
            final_commands.push_back(command);


        }

    }

    /**********************************************************PRINT**************************************************************/

    cout << "final_commands size:" << final_commands.size() << endl;
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
                
    }  
    /***************************************************************************************************************************/
}




int main(int argc, char *argv[]) {

    
    reading();
        

    
        
    return 0;   
        

}
   
