/*
AUTHOR: Harish Prabhakar
Course: CMPUT 379
Instructor: Dr. David Wishart

https://www.tutorialspoint.com/c_standard_library/c_function_signal.htm
https://github.com/Pk13055/cpp-shell Helped teach how to use SIGs
https://stackoverflow.com/questions/18925363/why-does-o-creat-create-files-only-accessible-by-administrators
https://www.geeksforgeeks.org/command-line-arguments-in-c-cpp/
https://stackoverflow.com/questions/13439363/whats-the-difference-between-char-and-const-castcharstring-c-str
*/

/*
*/

#include "includes.h"
#include "helpers.cpp"
#include "Job.cpp"

vector<Job> jobs; // Global vector of jobs
// Update jobs as we add and delete jobs from vector
// 


/*

*/
bool hasPid(string command_pid, vector<Job> a_jobs) {
    int i = 0;
    for (i; i < a_jobs.size(); i++) { // https://cplusplus.com/reference/string/stoi/
        if (a_jobs[i].pid == stoi(command_pid)) {
            return true;
        }
    }
    return false;
}

void show_jobs() {
    // Going to create a vector of current jobs, 
    // Copy them in
    // Clear our jobs vector
    // use pipe open to use ps -o command
    // use be the same regex to parse input and tokenize
    // add to jobs
    // *** TO DO IF TIME***
    int job_num = 0;
    cout << "Running processes: " << endl;
    for (int x = 0; x < jobs.size(); x++) {
        job_num += 1;

        //print_job(jobs[x], x);
    }


}

void signal_function(vector<string> arguments, int signal, bool is_background_proc) {

    // Set signals
    if ((signal) == 0) {
        signal = SIGKILL;
    }
    else if ((signal) == 1) {
        signal = SIGCONT;
    }
    else if ((signal) == 2) {
        signal = SIGSTOP;
    }

    // for (int x = 0; x < arguments.size(); x++) {
    //     cout << arguments[x] << arguments.size() << endl;
    
    if (arguments.size() >= 2) {
        if (hasPid(arguments[1], jobs)) {
            kill(stoi(arguments[1]), signal); // kill it with the resulting signal from input arguments
            if (is_background_proc) {
                wait(0);
            }
            return;
        } else {
            cout << "Invalid PID, not in table" << endl; // PID not in the vector table
        }
    }

    else {
        cout << "PID input missing" << endl;
    }
}  

void handle_input(vector<string> cmd_args, string full_input) {

    bool background_proc;
    if (cmd_args[cmd_args.size() - 1] == "&") {
        background_proc = true;
    }
    else {
        background_proc = false;
    }


    int outfile = index_finder('>', cmd_args);
    int infile = index_finder('<', cmd_args);

    bool back_or_index_no_ex = false;
    if (outfile != -1 || background_proc) {
        back_or_index_no_ex = true;
    }
    // come back
    string main_cmd = cmd_args[0];
    int status;
    switch (CommandEnum(main_cmd)) {
        case JOBS:
            show_jobs();
            break;

        case KILL:
            signal_function(cmd_args, 0, background_proc);
            break;
    

        case SUSPEND:
            signal_function(cmd_args, 2, background_proc);
            break;

        case RESUME:
            signal_function(cmd_args, 1, background_proc);
            break;
        
        case WAIT:
            
            pid_t a_pid;
            // https://www.tutorialspoint.com/c_standard_library/c_function_signal.htm
            signal(SIGCHLD, SIG_DFL); // Default Action specified  for this signal call
                while ((a_pid = wait(&status)) > 0);
                    {//Do nothing
                    };
                return;
            break;
        
        case NONE:
        default:

            // Use normal commands on the shell
            
            int In, Out, status; // create pipe opens and status int
            pid_t pid, pid2;
            string dir;
            // Fork procedure from notes
            if (( pid = fork()) < 0) {
                fprintf(stderr, "Fork Failed");
                break; // ???
            }
            // Child
            
            else if (pid == 0) {
                if (back_or_index_no_ex) {
                    
                    if (outfile == -1) {
                        dir = "/dev/null"; // https://linuxhint.com/what_is_dev_null/ forredirecting w/o errors 
                    }
                    else {
                        dir = cmd_args[outfile];
                        dir = dir.substr(1, dir.size()); // get rid of first char
                    }
                    //https://stackoverflow.com/questions/18925363/why-does-o-creat-create-files-only-accessible-by-administrators
                    Out = open(dir.c_str(), O_RDWR | O_CREAT, S_IRUSR | S_IWUSR); //open for reading and writing, permissions given to user
                    dup2(Out, 1); 
                    close(Out); // Close output pipe
                }

                if (infile != -1) {
                    string dir = cmd_args[infile];
                    dir = dir.substr(1, dir.size());
                    In = open(dir.c_str(), O_RDWR | O_CREAT, S_IRUSR | S_IWUSR); // In-Pipe
                    dup2(In, 0);
                    close(In);
                }
                
                string processed_output = OutPutParser(cmd_args, 0, ((infile == -1) ? outfile: infile));
                vector<string> cmd = tokenizer(processed_output, " ");

                int command_len = ((background_proc && outfile == -1) ? cmd.size() - 1: cmd.size()); // If it is a background proc AND > not there
                // then take out one character else its the whole thing

                // Now need to execvp the command
                char *argv[command_len + 1];
                // https://www.geeksforgeeks.org/command-line-arguments-in-c-cpp/
                // https://stackoverflow.com/questions/13439363/whats-the-difference-between-char-and-const-castcharstring-c-str
                for (int i = 0; i < command_len; i++) {
                    argv[i] = const_cast<char *>(cmd[i].c_str()); // Takes command and casts it into argument for argv[i]. Got help from TA
                }

                // https://linux.die.net/man/3/execvp
                argv[command_len] = (char *) nullptr; //... The list of arguments must be terminated by a NULL pointer, and, since these are variadic functions, this pointer must be cast (char *) NULL.
                execvp(argv[0], argv);
                exit(1); // exit

            } 
            else {
                Job job = Job(pid, full_input); // Create job FOR PROCESS TABLE
                jobs.push_back(job);

                // Check if backgrouund process and >> exists OR atleast >> exists
                if ((background_proc && outfile != -1) || outfile != -1) {
                    signal(SIGCHLD, SIG_IGN); // Ignore signal that says child process has been ended. It is a background proc
                }
                else if (background_proc) {
                    signal(SIGCHLD, SIG_IGN);
                    return;
                }
                else {
                    signal(SIGCHLD, SIG_DFL); // Continue with default action for this
                    while ((pid2 = wait(&status)) >0) {
                    }
                }
                break;
            }
    }
}
void exit_shell() { // Discord agreed on this
    for (auto &jobs: jobs) {
        int stat;
        waitpid(jobs.pid, &stat, WNOHANG);// waitpid must be immediately returned instead of waiting. NO HANGING
    }
}

int main() {
    string cmd;
    while (cmd != "exit") {
        printPrompt();
        getline(cin, cmd);
        if (cmd == "exit") {
            exit_shell();
            return 0;
        }

        else {
            cmd = regex_replace(cmd, regex("^ +| +$|( ) +"), "$1"); // replace with spaces or space + end of string
            vector<string> command_args = tokenizer(cmd, " ");
            handle_input(command_args, cmd);
        }
    }
}