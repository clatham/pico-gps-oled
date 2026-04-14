# Lesson 3 - Basic BASH build script

BASH is a complex scripting language, and we're barely going to scratch the surface of it with our simple little build script.


## Step 1 - Save and restore the current working directory

An ill-behaved script will dump you back at the command prompt somewhere in the file system other than where you started.  For this reason, I always begin my scripts by saving the current working directory, and I always end them by restoring it.

Add the following line to the beginning of your script:
```
CWD=$PWD
```

Add the following line to the end of your script, and make sure it says there when we add future lines in between:
```
cd $CWD
```

The first line sets the variable "CWD" equal to the value of the variable "PWD".  In this case, $PWD is a special variable that contains the current working directory.  There is also a bash command called "pwd" that performs the same function.

Note that in BASH, a dollar sign is used to access the value of a variable.  In Windows, the variable would have a percent sign on both sides for the same effect.


## Step 2 - Change the directory to the expected project directory

Sometimes a script is run from another directory rather than the one it was intended, which can cause it fail.  To prevent this, we'll change directory to the project directory.

Add the following lines to your BASH script:
```
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
PROJECT_DIR=$SCRIPT_DIR

cd $PROJECT_DIR
```

The first line is complicated, but we'll read it from the center out and left to right.  First, it gets the name of the script.  This is then run through the "dirname" command to give us just the directory.  We then "cd" to that directory while redirecting stdout to the /dev/null device.  /dev/null, also called the bit bucket, is a device that just throws away whatever you send it.  In this way, it can be used to prevent random warnings or status from making it to the terminal when we do things in a script.

"SCRIPT_DIR" now contains the directory of this script, which happens to be the project directory.  Therefore, we assign its value to "PROJECT_DIR".  This isn't always the case.  Often we might have our scripts in a project/scripts/ directory, which would then make the path to the project directory "$SCRIPT_DIR/..".  I usually leave the use of the intermediate "SCRIPT_DIR" variable in case I move things later.

Finally, we change directory to that held in "PROJECT_DIR".


## Step 5 - Clean the build directory

Next we'll delete the contents of the build directory, but only if that directory exists.  Otherwise, it would produce an error, because the direcotry is missing.

To do this, add the following lines:
```
if [[ -d build ]]; then
    rm -rf ./build/*
fi
```

This is a BASH conditional, and the "-d" evaluates to true if the given item exists and is a directory.

The "rm" command removes files.  The "-r" option removes files recursively, and the "-f" option forces the removal even if files are directories or protected.  "rm -rf" is therefore a **very** powerful command that can obliterate your file system.  Always use it carefully.  In particular, I like to use the "./\*" type of relative path to ensure "rm" is constrained within a directory.  Be careful, as "/\*" would nuke your file system from the root.


## Step 4 - Build it

We previously learned how to use CMake, and now we must invoke it to do our build for us.

Add the following lines:
```
cmake -S . -B build
cmake --build build --config Release
```

The first line invokes CMake to generate the build system, as we've seen before.  The second line invokes CMake to run the build system.  If everything is successful, we should find our binary under the build/ directory.

This script now does a complete clean and build each time you run it.  We could have instead made it just invoke the build system without cleaning and generating every time.  Then we could've made a "rebuild" script that cleaned and generated when we wanted that behavior.


## Step 5 - Running the script

My preferred way of running a script are either to "source" the script if I'm alright with it modifying my environment or invoking a new BASH shell to run it if I'm not.

Use the following command to source "build.sh":
```
source build.sh
```

This simply tells BASH to execute each line of the script as though the user had typed it.

Use the following command to invoke a new BASH shell to run "build.sh":
```
bash -c build.sh
```

This will cause a new BASH shell to be run on top of the current one.  It will have a copy of the parent environment.  It will execute the single command, then it will return to the parent shell.  Any changes to the child environment will be thrown away when it closes.


## Step 6 - The other way to "run" a script

Many people prefer to "execute" the script by marking it executable and adding a "shebang" to the first line of the script.  Shebang is a portmanteau of the words "hash" and "bang", which make up the shebang.  In UNIX parlance, the octothorp or pound sign is called a hash, and the exclamation mark is called a bang.

The shebang should be immediately followed by the path to the program that should be used to run the file.  In the case of a BASH script that program is /bin/bash, not /bin/sh.  /bin/sh points to whatever the current shell is, which may not be BASH.  This would cause your program to fail.

Add the following to the first line of your script:
```
#!/bin/bash
```

Now mark the file executable by invoking the following command:
```
chmod a+x build.sh
```

This will make build.sh executable for **all** users.  Now you can simply run it with the following command:
```
./build.sh
```

This works with most programs.  For instance, you could add #!/bin/vi to a text file, mark it executable, and run it.  It should invoke Vi with the file loaded.  I find this to be a gimmick, and I prefer to make my intentions clear by using source or bash to run a script.


## Conclusion

BASH is a Turing complete language that can be used to write whole programs, but we're just using it to get our stuff build in a single command.  We may learn more if it as we go along.
