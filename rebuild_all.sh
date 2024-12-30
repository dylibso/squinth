rm vcv_module/res/*.wasm;

(cd templates/rust_template && ./rebuild.sh;) || true;
cp templates/rust_template/rust_plugin/target/wasm32-unknown-unknown/release/plugin.wasm vcv_module/res/rust_plugin.wasm;

(cd templates/ts_template && ./rebuild.sh);
cp templates/ts_template/ts_plugin/dist/plugin.wasm vcv_module/res/ts_plugin.wasm; 

(cd templates/go_template && ./rebuild.sh) || true;
cp templates/go_template/go_plugin/dist/plugin.wasm vcv_module/res/go_plugin.wasm;

(cd templates/zig_template && ./rebuild.sh);
cp templates/zig_template/zig_plugin/zig-out/bin/plugin.wasm vcv_module/res/zig_plugin.wasm;

# Use the "clang" that comes with Wasi-SDK package, set it in your $PATH
#(cd templates/c_template; clang -o c_template.wasm --target=wasm32-wasi -Wl,--no-entry c_template.c);
#cp templates/c_template/c_template.wasm vcv_module/res;

(cd vcv_module; ./dev_build_and_install.sh;);
