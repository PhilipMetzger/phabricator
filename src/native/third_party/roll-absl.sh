#!usr/bin/bash
# This shell script re-clones absl and adds all the changes to git.
log "Creating workdir"
mkdir -p absl-roll
cd absl-roll
log "Cloning absl source"
git clone https://github.com/abseil/abseil-cpp.git

# add all changes 
git add absl/
git commit
