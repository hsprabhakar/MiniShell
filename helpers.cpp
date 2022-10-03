#include "includes.h"


// https://gist.github.com/LovelyBuggies/6dafc6f3ad6d27c17d8c0dd81d23a1b8
vector<string> tokenizer(string input, string delim) {
    vector<string> tokens;
    size_t pos = 0;
    string token;
    while ((pos = input.find(delim)) != string::npos) {
        token = input.substr(0, pos);

        tokens.push_back(token); // add token
        input.erase(0, pos + delim.length());
    }

    // Deal with & ending
    bool input_size_check = (input.size() >= 2);
    if (input_size_check && (input.back() == '&')) {
        input.pop_back();
        tokens.push_back(input);
        tokens.emplace_back("&"); // APpends back & to token

    }
    else {
        tokens.push_back(input);

    }
    // for(int i = 0; i < tokens.size(); i++)
    //     std::cout << tokens[i] << ' ';
    return tokens;
} 


int index_finder(char character, vector<string> input_string) {
    for (int x = 0; x < input_string.size(); x++) {
        if (input_string[x][0] == character) {
            return x;
        }
    }
    return -1; // If character not found
}
enum Command {
    KILL,
    JOBS,
    RESUME,
    WAIT,
    SUSPEND,
    NONE
};

Command CommandEnum(string input) {
    static const map<string, Command> Strings{
        {"kill",    KILL},
        {"jobs",    JOBS},
        {"resume",  RESUME},
        {"wait",    WAIT},
        {"suspend", SUSPEND},
        {"none",    NONE}
    };

    // find which one https://stackoverflow.com/questions/3136520/determine-if-map-contains-a-value-for-a-key
    auto i = Strings.find(input);
    if (i != Strings.end()) {
        return i->second;
    }
    return NONE;
}

int getInput(string cmd) {
    getline(cin, cmd);
    if (cmd != "exit") {
        cmd = regex_replace(cmd, regex("^ +| +$|( ) +"), "$1"); // using this version
        vector<string> command_arguments = tokenizer(cmd, " ");
        return 0;
        //handle_input(command_arguments);
        // returns back to 
    }
    else {
        return 1;
    }
}

void printPrompt() {

    static int init = 1;
    if ( init ) {
        const char* CLEAR_SCREEN_ANSI = " \e[1;H\e[2J"; // https://www.youtube.com/watch?v=z4LEuxMGGs8&t=191s How to clear the screem
        write(STDOUT_FILENO,CLEAR_SCREEN_ANSI, 12);
        init = 0;
    }
    printf("SHELL379: ");
}

string OutPutParser(vector<string> cmd_args, int start, int end) {
    string output_str;
    int i = ((start == -1) ? 0: start);
    for (i; i < ((end == -1) ? cmd_args.size(): end); i++) {
        output_str.append(cmd_args[i]); // append each cjaracter tp output_str
        output_str.append(" ");
    }
    cout << output_str << endl;
    output_str.pop_back();
    return output_str;
}
