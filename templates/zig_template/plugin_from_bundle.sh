rm -rf ./zig_plugin;
xtp plugin init \
    --path ./zig_plugin \
    --extension-point ext_01j9hn0bshfh2shvdxk9xz7zys \
    --feature stub-with-code-samples \
    -t ./zig_bundle -y && \
./rebuild.sh;
