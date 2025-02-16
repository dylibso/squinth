# Function to check if a command exists
command_exists () {
  command -v "$1" >/dev/null 2>&1
}

ls extism >/dev/null || (git clone git@github.com:extism/extism.git extism && (cd extism && make && sudo make install DEST=/usr/local))

curl https://static.dylibso.com/cli/install.sh -s | bash

for dir in ./templates/*     # list directories in the form "/tmp/dirname/"
do
    echo "\n\n\n"${dir}"\n\n\n"
    (cd ${dir} && ./plugin_from_bundle.sh)
done
