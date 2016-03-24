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
void assigning(vector<string> parsed_vector, vector<CommandLine> &commands)

{
    struct CommandLine line;
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
    commands.push_back(line);
       
    

}

void reading()
{
    string readline;
    
    

    while (true) {
        getline(cin, readline);

        if(readline.compare("quit") == 0)
            break;
        else
        {
            vector<string> parsed_vector;
            vector<CommandLine> final_commands;
            parsing(readline, parsed_vector);
            assigning(parsed_vector, final_commands);

            /**********************************************************PRINT**************************************************************/
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
      
            
        }
            
        
        
    }
    
   
    
}




int main(int argc, char *argv[]) {

    
    reading();
    	

    
        
    return 0;	
    	

}
   
