#cargo build --target wasm32-unknown-unknown --release

rm -rf ./rust_plugin;
xtp plugin init --path ./rust_plugin --extension-point ext_01j9hn0bshfh2shvdxk9xz7zys -t ./bundle_v0 -y && \
(cd rust_plugin && xtp plugin build);
# xtp plugin push in order to push up to the xtp instance
