rm -rf ./cpp_plugin;
xtp plugin init \
    --path ./cpp_plugin \
    --extension-point ext_01j9hn0bshfh2shvdxk9xz7zys \
    --feature stub-with-code-samples \
    -t ./cpp_bundle -y && \
./rebuild.sh;
