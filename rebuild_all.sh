rm vcv_module/res/*.wasm;

(cd templates/rust_template && ./rebuild.sh;) || true;
cp templates/rust_template/rust_plugin/target/wasm32-unknown-unknown/release/plugin.wasm vcv_module/res/rust_plugin.wasm;

(cd templates/ts_template && ./rebuild.sh);
cp templates/ts_template/ts_plugin/dist/plugin.wasm vcv_module/res/ts_plugin.wasm; 

(cd templates/go_template && ./rebuild.sh) || true;
cp templates/go_template/go_plugin/dist/plugin.wasm vcv_module/res/go_plugin.wasm;

(cd templates/zig_template && ./rebuild.sh);
cp templates/zig_template/zig_plugin/zig-out/bin/plugin.wasm vcv_module/res/zig_plugin.wasm;

(cd templates/cpp_template && ./rebuild.sh);
cp templates/cpp_template/cpp_plugin/dist/plugin.wasm vcv_module/res/cpp_plugin.wasm;

(cd templates/python_template && ./rebuild.sh);
cp templates/python_template/py_plugin/plugin.wasm vcv_module/res/py_plugin.wasm;

(cd vcv_module; ./dev_build_and_install.sh;);
