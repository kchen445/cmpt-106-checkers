// 
// OS independent functions for interacting with the terminal.
//

#ifndef SHELL_HPP
#define SHELL_HPP

#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <vector>

#ifdef _WIN32   // for windows systems
    #define POPEN   _popen
    #define PCLOSE  _pclose
#else           // for unix systems
    #define POPEN   popen
    #define PCLOSE  pclose
#endif

namespace shell {

    // Executs and returns the output to stdout from a commmand as a string.
    // This is the variable buffer size version of this function. Use of
    // shell::exec() is recommended.
    //
    // Function referenced from: (has been modified from original)
    // https://stackoverflow.com/questions/478898/how-to-execute-a-command-and-get-output-of-command-within-c-using-posix
    template<size_t N>
    inline std::string bufferedExec (const char* cmd) {
        std::array<char, N> buffer;
        std::string result;
        std::shared_ptr<FILE> pipe(POPEN(cmd, "r"), PCLOSE);
        if (!pipe) throw std::runtime_error("shell::bufferedExec<N>: unable to open pipe");
        while (!feof(pipe.get())) {
            if (fgets(buffer.data(), N, pipe.get()) != nullptr)
                result += buffer.data();
        }
        return result;
    }


    // Same as above, but with a fixed buffer size.
    inline std::string exec (const char* cmd) {
        return bufferedExec<128>(cmd);
    }

    inline std::string exec (std::string const &cmd) {
        return exec(cmd.c_str());
    }


    // Runs a given system command.
    // Unlike exec, this does not return the output of the command.
    // Any printing to stdout done by the command will still be
    // printed to stdout.
    //
    // Example:
    //      shell::run("echo hello") 
    //          will print hello to screen.
    //
    //      shell::exec("echo hello") 
    //          will not print anything to the screen, but
    //          instead will return "hello" as a string.
    inline void run (const char* cmd) {
        system(cmd);
    }

    inline void run (std::string const &cmd) {
        system(cmd.c_str());
    }


    // Parses a \n separated string into individual elements.
    inline std::vector<std::string> parseNLSeparatedString (std::string const &str) {
        std::vector<std::string> v;
        std::string stringBuilder;
        for (char c : str) {
            if (c == '\n') {
                v.push_back(stringBuilder);
                stringBuilder = "";
            } else {
                stringBuilder.push_back(c);
            }
        }
        // cmdOut may not end in \n
        if (stringBuilder != "") {
            v.push_back(stringBuilder);
        }
        return v;
    }


    // Returns all files in a given directory using shell::exec().
    // Default dir is the executing directory for the program.
    inline std::vector<std::string> ls (std::string const &dir = ".") {
        std::string command = "";

        // Create OS specific command.
        #ifdef _WIN32
            // e.g. > dir . /b
            command = "dir " + dir + " /b";
        #else
            // e.g. $ ls .
            command = "ls " + dir;
        #endif

        return parseNLSeparatedString(exec(command));
    }


    // Searches the current directory for any files or
    // directories that satisfy a given wildcard.
    //
    // Example:
    //      shell::find("*.cpp") will return all .cpp files
    //
    // This is equivalent to calling $ find . -name '*.cpp' on unix
    inline std::vector<std::string> find (std::string const &wildcard) {
        std::string command = "";
        
        // Create OS specific command
        #ifdef _WIN32
            // e.g. > dir *.cpp /b
            command = "dir " + wildcard + " /b";
        #else
            // e.g. $ ls *.cpp
            command = "ls " + wildcard;
        #endif

        return parseNLSeparatedString(exec(command));
    }


    // Searches a given directory for files that match the wildcard.
    inline std::vector<std::string> findin (std::string const &dir, std::string const &wildcard) {
        std::string command = "";

        #ifdef _WIN32
            // e.g. > cd src & dir *.cpp /b & cd ..
            // Jump to specified directory, call the dir command.
            command = "cd " + dir + " & dir " + wildcard + " /b";
        #else
            // e.g. $ ls src/*.cpp
            command = "ls " + dir + "/" + wildcard;
        #endif

        return parseNLSeparatedString(exec(command));
    }


    // Recusivly search for files in the current directory.
    // On unix:
    //  File names contain relative directories.
    //      e.g. ./src/a.cpp
    // On windows:
    //  File names contain absolute directories.
    //      e.g. C:\Users\Me\Desktop\Project\src\a.cpp
    inline std::vector<std::string> findrec (std::string const &wildcard) {
        std::string command = "";

        #ifdef _WIN32
            // e.g. > dir *.cpp /b/s
            command = "dir " + wildcard + " /b/s";
        #else
            // e.g. $ find . -name '*.cpp'
            command = "find . -name '" + wildcard + "'";
        #endif

        return parseNLSeparatedString(exec(command));
    }


    // Calls shell::findrec from a given directory.
    inline std::vector<std::string> findrec (std::string const &dir, std::string const &wildcard) {
        std::string command = "";

        #ifdef _WIN32
            command = "cd " + dir + " & dir " + wildcard + "/b/s";
        #else
            command = "find " + dir + " -name '" + wildcard + "'";
        #endif
        
        return parseNLSeparatedString(exec(command));
    }


    // Clears the terminal by calling the OS specific clear command.
    inline void clear () {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
    }

}

#endif
