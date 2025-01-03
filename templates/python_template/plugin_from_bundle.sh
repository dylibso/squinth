rm -rf ./py_plugin;
xtp plugin init \
    --path ./py_plugin \
    --extension-point ext_01j9hn0bshfh2shvdxk9xz7zys \
    --feature stub-with-code-samples \
    -t ./py_bundle -y && \
./rebuild.sh;
