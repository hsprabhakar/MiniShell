#include "includes.h"


// Creats Job class to use in printJob()
class Job {
    public:
        pid_t pid;
        string status = "";
        string command = "";
        string time_elaped = "";

        // Constructor
        Job(pid_t a_pid, string a_status, string a_duration, string a_cmd) {
            pid = a_pid;
            status = a_status;
            time_elaped = a_duration;
            command = a_cmd;
        }

        // to use
        Job(pid_t a_pid, string a_cmd) {
            pid = a_pid;
            command = a_cmd;
        }

        // Simply lay out
        void print_job() {
            cout << pid << " " << status << " " << time_elaped << " " << command << endl;
        }
        

};