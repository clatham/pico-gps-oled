# save the current working directory
CWD="$PWD"

# get the directory the script is in, get the project directory, and cd
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
PROJECT_DIR="$SCRIPT_DIR"

cd "$PROJECT_DIR"


# if the build directory exists, empty it
if [[ -d build ]]; then
    rm -rf ./build/*
fi


# build everything
cmake -S . -B build
cmake --build build --config Release


# restore the original directory
cd "$CWD"
