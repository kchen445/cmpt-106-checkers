// Script for creating a fully connected random network.
//
// To run...
//  $ swift createNetwork > <filename>
//
// *** Only will run on macOS *** 

import Darwin // for arc4random_uniform

// Configuration

let numInNodes = 64
let numOutNodes = 32
let hiddenLayers = [40, 34]

// Objects and functions

struct Node {
    private static var nextId = 0

    let id: Int

    init () {
        self.id = Node.nextId
        Node.nextId += 1
    }
}

class Conn {
    private static var __nextInnov = -1

    static var nextInnov: Int {
        __nextInnov += 1
        return __nextInnov
    }
}

func randomWeight () -> Double {
    return 2 * ((Double(arc4random_uniform(100)) / 100.0) - 0.5)
}

// Execution Start

var hiddenNodeCount = 0
for i in hiddenLayers {
    hiddenNodeCount += i
}
let totalNodeCount = numInNodes + numOutNodes + hiddenNodeCount

print("\(totalNodeCount) \(numInNodes) \(hiddenNodeCount) \(numOutNodes)")

var layers = [[Node]]()

// Add input nodes

layers.append([Node]())
for _ in 0..<numInNodes {
    layers[0].append(Node())
}

// Add hidden nodes
for i in 0..<hiddenLayers.count {
    layers.append([Node]())
    for _ in 0..<hiddenLayers[i] {
        layers[i + 1].append(Node())
    }
}

// Add output nodes
layers.append([Node]())
for _ in 0..<numOutNodes {
    layers[hiddenLayers.count + 1].append(Node())
}

// Create connections and print them out
for i in 0..<(layers.count - 1) {
    let baseLayer = layers[i]
    let targetLayer = layers[i + 1]
    for baseNode in baseLayer {
        for targetNode in targetLayer {
            let weight = randomWeight()
            let innov = Conn.nextInnov
            print("\(baseNode.id) \(targetNode.id) \(weight) \(innov) 1")
        }
    }
}
