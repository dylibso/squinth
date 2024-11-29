package main

import (
	"encoding/json"
	"fmt"
	"io"
	"net/http"
	"time"
)

type xtpBindingInfo struct {
	ID             string    `json:"id"`
	UpdatedAt      time.Time `json:"updatedAt"`
	ContentAddress string    `json:"contentAddress"`
}

// func epPlugins(pluginName string, xtpToken string) (bool, error) {
// 	url := "https://xtp.dylibso.com/api/v1/extension-points/ext_01j9hn0bshfh2shvdxk9xz7zys/bindings/"
// 	client := &http.Client{}

// 	req, err := http.NewRequest("GET", url, nil)
// 	if err != nil {
// 		return false, fmt.Errorf("error creating request: %v", err)
// 	}

// 	req.Header.Add("Authorization", "Bearer "+xtpToken)

// 	resp, err := client.Do(req)
// 	if err != nil {
// 		return false, fmt.Errorf("error making request: %v", err)
// 	}
// 	if resp.StatusCode != 200 {
// 		fmt.Printf("\nResponse code: %v encountered while trying to fetch from, %v\n", resp.StatusCode, url)
// 		return false, fmt.Errorf("code %d returned in response to ", resp.StatusCode)
// 	}

// 	defer resp.Body.Close()

// 	var result map[string]any
// 	var json_bytes []byte
// 	resp.Body.Read(json_bytes)
// 	json.Unmarshal(json_bytes, &result)
// 	for key, value := range result {
// 		plugins := result["foo"].(map[string]any)
// 	}
// }

func getWasmFile(contentAddress string, xtpToken string) ([]byte, error) {
	url := "https://xtp.dylibso.com/api/v1/c/" + contentAddress
	client := &http.Client{}

	req, err := http.NewRequest("GET", url, nil)
	if err != nil {
		return nil, fmt.Errorf("error creating request: %v", err)
	}

	req.Header.Add("Authorization", "Bearer "+xtpToken)

	resp, err := client.Do(req)
	if err != nil {
		return nil, fmt.Errorf("error making request: %v", err)
	}
	if resp.StatusCode != 200 {
		fmt.Printf("\nResponse code: %v encountered while trying to fetch from, %v\n", resp.StatusCode, url)
		return nil, fmt.Errorf("code %d returned from request for %s", resp.StatusCode, contentAddress)
	}

	defer resp.Body.Close()

	// Read the response body
	wasmBytes, err := io.ReadAll(resp.Body)
	if err != nil {
		return nil, fmt.Errorf("error reading response: %v", err)
	}

	fmt.Println("SUCCESS")
	return wasmBytes, nil
}

func getWasmByPluginName(pluginName string, extensionID string, xtpToken string) ([]byte, error) {
	url := "https://xtp.dylibso.com/api/v1/extension-points/" + extensionID + "/bindings/"
	client := &http.Client{}

	req, err := http.NewRequest("GET", url, nil)
	if err != nil {
		return nil, fmt.Errorf("error creating request: %v", err)
	}

	req.Header.Add("Authorization", "Bearer "+xtpToken)

	resp, err := client.Do(req)
	if err != nil {
		return nil, fmt.Errorf("error making request: %v", err)
	}
	defer resp.Body.Close()

	// Parse the JSON response
	var data map[string]struct {
		ContentAddress string `json:"contentAddress"`
	}

	if err := json.NewDecoder(resp.Body).Decode(&data); err != nil {
		return nil, fmt.Errorf("error decoding JSON: %v", err)
	}

	fmt.Println(data)

	for name, metadata := range data {
		fmt.Println(name)
		if pluginName == name {
			fmt.Printf("\nFOUND: %s - Downloading its .wasm module file...\n", pluginName)
			wasmBytes, err := getWasmFile(metadata.ContentAddress, xtpToken)
			if err != nil {
				return nil, fmt.Errorf("error getting wasm file: %v", err)
			}
			return wasmBytes, nil
		}
	}

	return nil, fmt.Errorf("error: no match for %s found in the list of available plugins", pluginName)
}
