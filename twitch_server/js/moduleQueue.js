class WasmModuleQueue {
    constructor(maxSize = 100, maxMemoryMB = 500) {
        this.queue = [];
        this.maxSize = maxSize;
        this.maxMemoryBytes = maxMemoryMB * 1024 * 1024;
        this.currentMemoryUsage = 0;
    }

    enqueue(wasmModule) {
        const moduleSize = wasmModule.byteLength;

        // Check if adding this module would exceed our memory limit
        if (this.currentMemoryUsage + moduleSize > this.maxMemoryBytes) {
            // Remove oldest modules until we have space
            while (this.currentMemoryUsage + moduleSize > this.maxMemoryBytes && this.queue.length > 0) {
                this.dequeue();
            }
        }

        this.queue.push(wasmModule);
        this.currentMemoryUsage += moduleSize;

        // Basic memory stats logging
        console.log(`Queue size: ${this.queue.length}, Memory usage: ${(this.currentMemoryUsage / 1024 / 1024).toFixed(2)}MB`);
    }

    dequeue() {
        if (this.queue.length === 0) return null;

        const module = this.queue.shift();
        this.currentMemoryUsage -= module.byteLength;

        // Explicitly null the reference to help GC
        module = null;

        return module;
    }

    // Method to manually clear the queue
    clear() {
        this.queue.forEach(module => {
            module = null;
        });
        this.queue = [];
        this.currentMemoryUsage = 0;

        // Suggest garbage collection (if --expose-gc flag is enabled)
        if (global.gc) {
            global.gc();
        }
    }

    // Monitor memory usage
    getMemoryStats() {
        const memoryUsage = process.memoryUsage();
        return {
            queueSize: this.queue.length,
            queueMemoryMB: (this.currentMemoryUsage / 1024 / 1024).toFixed(2),
            heapUsedMB: (memoryUsage.heapUsed / 1024 / 1024).toFixed(2),
            heapTotalMB: (memoryUsage.heapTotal / 1024 / 1024).toFixed(2),
            rss: (memoryUsage.rss / 1024 / 1024).toFixed(2)
        };
    }
}

module.exports = {
    WasmModuleQueue // Export object with function
};
