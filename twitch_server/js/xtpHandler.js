async function get_wasm_file(content_address, xtp_token) {
    const url = 'https://xtp.dylibso.com/api/v1/' + content_address;
    const options = { method: 'GET', headers: { Authorization: 'Bearer ' + xtp_token } };

    try {
        const response = await fetch(url, options);

        // Get the binary data as ArrayBuffer
        const wasmBuffer = await response.arrayBuffer();
        // Optional: Convert to Uint8Array if needed
        const wasmBytes = new Uint8Array(wasmBuffer);

        console.log("SUCCESS");

        return wasmBytes
    } catch (error) {
        console.error(error);
    }
}

export async function get_wasm_by_plugin_name(plugin_name, extension_id, xtp_token) {
    // Fetch all artifacts at this extension point
    // TODO: Will this list the artifacts associated with multiple guest keys?

    const url = 'https://xtp.dylibso.com/api/v1/extension-points/' + extension_id + '/bindings/';
    const options = { method: 'GET', headers: { Authorization: 'Bearer ' + xtp_token } };

    try {
        const response = await fetch(url, options);
        const data = await response.json();
        console.log(data)
        for (const [name, metadata] of Object.entries(data)) {
            console.log(name)
            if (plugin_name == name) {
                console.log("FOUND: ", plugin_name, " - Downloading its .wasm module file...")
                get_wasm_file(metadata.contentAddress, xtp_token)
            }
        }
    } catch (error) {
        console.error(error);
    }
}
