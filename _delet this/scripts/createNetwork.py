from random import uniform

#
# Script for creating a fully connected, random network.
#
# There is no file manipluation in this script so the output
# must be feed into a file using the the unix pipeline and
# dos equivalent.
#
# Usage:
#   Configure the desired values in the configuration section.
#   Run command (Unix):
#       $ python createNetwork.py > <filename>
#   where <filename> is the file to output the data.
#
#   Run command (DOS): (probably...)
#       python createNetwork.py > <filename>
#   where <filename> is the file to output the data.
#

# Configuration
numInNodes = 3
numOutNodes = 3
hiddenLayers = [7, 5] # must have at least one element
# End of configuration

def randWeight (): 
    return uniform(-1, 1)

__nextNodeId = -1
__nextInnov = -1

def nextId ():
    global __nextNodeId
    __nextNodeId += 1
    return __nextNodeId

def nextInnov ():
    global __nextInnov
    __nextInnov += 1
    return __nextInnov

# Print header line
numHiddenNodes = 0
for layerCount in hiddenLayers:
    numHiddenNodes += layerCount

totalNodeCount = numInNodes + numOutNodes + numHiddenNodes

print totalNodeCount, numInNodes, numOutNodes, numHiddenNodes

# List of all layers in the network
nodes = []

# List of the input nodes as id values
inNodes = []

for i in range(0, numInNodes):
    inNodes.append(nextId())

nodes.append(inNodes)

# Append the hidden layers to the node list
for layer in hiddenLayers:
    l = []
    for i in range(0, layer):
        l.append(nextId())

    nodes.append(l)

# List of the output nodes as id values
outNodes = []

for i in range(0, numOutNodes):
    outNodes.append(nextId())

nodes.append(outNodes)

# Create connection and print them to stdout

for i in range(0, len(nodes) - 1):
    baseLayer = nodes[i]
    targetLayer = nodes[i + 1]

    for baseNode in baseLayer:
        for targetNode in targetLayer:
            # Printing of connections
            print baseNode, targetNode, nextInnov(), randWeight(), 1
