// Note: run `go doc -all` in this package to see all of the types and functions available.
// ./pdk.gen.go contains the domain types from the host where your plugin will run.
package main

import "errors"

// User-defined function that computes a single sample of a waveform
// It takes WaveArgs as input ()
// And returns float32 (The computed sample)
func Wave(input WaveArgs) (float32, error) {
	// TODO: fill out your implementation here
	panic("Function not implemented.")
}
