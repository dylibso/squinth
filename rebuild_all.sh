rm vcv_module/res/*.wasm;

echo "\nRUST"
(cd templates/rust_template && ./rebuild.sh;) || true;
cp templates/rust_template/rust_plugin/target/wasm32-unknown-unknown/release/plugin.wasm vcv_module/res/rust_plugin.wasm;

echo "\nTypeScript"
(cd templates/ts_template && ./rebuild.sh);
cp templates/ts_template/ts_plugin/dist/plugin.wasm vcv_module/res/ts_plugin.wasm; 

echo "\nGolang"
(cd templates/go_template && ./rebuild.sh) || true;
cp templates/go_template/go_plugin/dist/plugin.wasm vcv_module/res/go_plugin.wasm;

echo "\nZig"
(cd templates/zig_template && ./rebuild.sh);
cp templates/zig_template/zig_plugin/zig-out/bin/plugin.wasm vcv_module/res/zig_plugin.wasm;

echo "\nCpp"
(cd templates/cpp_template && ./rebuild.sh);
cp templates/cpp_template/cpp_plugin/dist/plugin.wasm vcv_module/res/cpp_plugin.wasm;

echo "\nPython"
(cd templates/python_template && ./rebuild.sh);
cp templates/python_template/py_plugin/plugin.wasm vcv_module/res/py_plugin.wasm;

echo "\nInstall Module to User VCV Folder";
(cd vcv_module && make clean && make install);

# Install to dev location
echo "\nInstall Module to User VCV Folder";
(cd vcv_module && ./dev_build_and_install)
