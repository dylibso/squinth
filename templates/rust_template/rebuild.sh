#cargo build --target wasm32-unknown-unknown --release

(cd rust_plugin && xtp plugin build);
# xtp plugin push in order to push up to the xtp instance
