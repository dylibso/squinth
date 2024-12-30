rm -rf ./ts_plugin;
xtp plugin init \
    --path ./ts_plugin \
    --extension-point ext_01j9hn0bshfh2shvdxk9xz7zys \
    --feature stub-with-code-samples \
    -t ./ts_bundle -y && \
./rebuild.sh;
