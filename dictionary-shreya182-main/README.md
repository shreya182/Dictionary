[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-24ddc0f5d75046c5622901739e7c5dd533143b0c8e959d652212380cedb1ea36.svg)](https://classroom.github.com/a/oewLDyrD)
# Dependencies
This program communicates with a dictionary API ([dictionaryapi.dev](https://dictionaryapi.dev/)) to retrieve definitions, audio URLs, and pronunciations of words.

The [cpp-httplib](https://github.com/yhirose/cpp-httplib/tree/master) library is used to communicate with the API, and the [json11](https://github.com/dropbox/json11/tree/master) library is used to parse its JSON responses. See the "Building" section below for details on how to build your program with these dependencies in mind.

# Working on the program
The only file you'll need to modify is `main.cpp`. It contains a lot of starter code, including functions that you'll use to interface with the dictionary API (each starting with the prefix `query_`). Read the documentation (block comments) supplied for these functions very carefully along with the commented-out example calls in the skeleton `main()` function. Make sure you understand how to use them. Then, complete the program as described in the assignment document.

# Building
To build your program, you must compile your code (main.cpp) with json11's code (cpp-httplib is "header-only", which means that it doesn't need to be compiled "manually"). You must also link your program against the ssl and crypto DLLs (you don't have to understand what this means---they're just other system-installed libraries that cpp-httplib depends on).

In practice, this means that you should always compile this program like so:

`g++ -o dictionary json11/json11.cpp main.cpp -lssl -lcrypto`

As you know, `-o dictionary` just sets the name of the built executable to be "dictionary". You can change that part to whatever you want.

One option is to run the above command manually from the terminal. However, an alternative, simpler option is to use the GNU make utility. This starter code contains something called a "Makefile". This Makefile is configured to execute the above command when you use it. To use the Makefile, simply run the following terminal command from the project directory:

`make`

Again, this will build the executable and name it "dictionary". Feel free to modify the EXE constant in the Makefile to change the executable name to whatever you'd like.

Note that the `make` command won't do anything if you haven't modified main.cpp since you last built your executable.

# Running
When you run your program, know that the query\_XXX functions provided in the template code can sometimes be a bit slow since they have to communicate with the dictionary API over the internet. In addition, the dictionary API server occasionally goes down momentarily, perhaps for routine maintenance, which will cause these functions to fail in somewhat unpredictable ways. For example, it's been observed that the API will sometimes stop working at exactly noon (PST) and start working again within a minute or two.
