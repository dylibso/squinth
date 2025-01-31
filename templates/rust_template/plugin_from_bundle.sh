rm -rf ./rust_plugin;
xtp plugin init \
    --path ./rust_plugin \
    --extension-point ext_01j9hn0bshfh2shvdxk9xz7zys \
    --feature stub-with-code-samples \
    -t ./rust_bundle -y && \
./rebuild.sh;
