import numpy as np
import math
import random
from Layer import Layer

class NeuralNetwork:
  def __init__(self, layerSizes):
    self.layers = []

    for i in range(len(layerSizes)-1):

        if i == len(layerSizes)-2:
            activation = "linear"
        else:
            activation = "relu"

        self.layers.append(
            Layer(
                layerSizes[i],
                layerSizes[i+1],
                activation
            )
        )

  def CalculateOutputs(self, inputs):
    for layer in self.layers:
      inputs = layer.CalculateOutputs(inputs)

    inputs = self.softMax(inputs)

    self.layers[-1].activations = inputs

    return inputs

  def Classify(self, inputs):
    outputs = self.CalculateOutputs(inputs)
    return np.argmax(outputs)

  def Cost(self, dataPoint):
    #MSE
    """outputs = self.CalculateOutputs(dataPoint.inputs)
    outputLayer = self.layers[-1]
    cost = 0.0

    for nodeOut in range (len(outputs)):
      cost += outputLayer.NodeCost(outputs[nodeOut], dataPoint.expectedOutputs[nodeOut])

    return cost"""

    #Cross-Entropy
    outputs = self.CalculateOutputs(
        dataPoint.inputs
    )

    cost = 0.0

    for i in range(len(outputs)):

        if dataPoint.expectedOutputs[i] == 1:

            cost -= math.log(
                outputs[i] + 1e-10
            )

    return cost

  def TotalCost(self, data):
    totalCost = 0.0

    for dataPoint in data :
      totalCost += self.Cost(dataPoint)
    return totalCost / len(data)

  def Learn(
        self,
        trainingData,
        learnRate,
        batchSize=32
):



    random.shuffle(trainingData)


    for start in range(
        0,
        len(trainingData),
        batchSize
    ):
        for layer in self.layers:
            layer.costGradientW.fill(0)
            layer.costGradientB.fill(0)

        batch = trainingData[
            start:start+batchSize
        ]


        inputs = np.array(
            [
                x.inputs
                for x in batch
            ]
        )


        expected = np.array(
            [
                x.expectedOutputs
                for x in batch
            ]
        )


        self.UpdateBatchGradients(
            inputs,
            expected
        )


        for layer in self.layers:
            layer.ApplyAdam(
                learnRate,
                len(batch)
            )

  def UpdateBatchGradients(
        self,
        inputs,
        expected
  ):

    current = inputs


    for layer in self.layers:
      current = layer.CalculateOutputs(
          current,
          training=True
      )


    probabilities = self.softMax(current)


    self.layers[-1].activations = probabilities


    nodeValues = (
        probabilities - expected
    )


    self.layers[-1].UpdateGradients(
        nodeValues
    )


    for l in range(
      len(self.layers)-2,
      -1,
      -1
    ):

      currentLayer = self.layers[l]
      nextLayer = self.layers[l+1]


      nodeValues = (
          np.dot(
            nodeValues,
            nextLayer.weights.T
          )
          *
          currentLayer.ActivationDerivative(
            currentLayer.weightedInputs
        ) 
      )


      if currentLayer.dropoutRate > 0:

        nodeValues *= (
            currentLayer.dropoutMask
            /
            (1-currentLayer.dropoutRate)
        )


      currentLayer.UpdateGradients(
        nodeValues
      )

  """def UpdateAllGradients(self, dataPoint):
    self.CalculateOutputs(dataPoint.inputs)

    outputLayer = self.layers[-1]
    nodeValues = []
    for i in range(outputLayer.numNodesOut):
      nodeValues.append(
        outputLayer.activations[i] - dataPoint.expectedOutputs[i])


      nodeValues.append(
        outputLayer.ActivationDerivative(outputLayer.weightedInputs[i]) * 
        outputLayer.NodeCostDerivative(outputLayer.activations[i], dataPoint.expectedOutputs[i]) #for Sigmoid
      )

    outputLayer.UpdateGradients(nodeValues)

    for l in range(len(self.layers) - 2, -1, -1):
      hiddenLayer = self.layers[l]
      nextLayer = self.layers[l+1]
      nodeValuesNext = nodeValues
      nodeValues = []
      for i in range(hiddenLayer.numNodesOut):
        val = 0.0
        for j in range(nextLayer.numNodesOut):
          val += nextLayer.weights[i][j] * nodeValuesNext[j]
        val *= hiddenLayer.ActivationDerivative(hiddenLayer.weightedInputs[i])
        nodeValues.append(val)
      hiddenLayer.UpdateGradients(nodeValues)"""

  def softMax(self, values):

    values = np.asarray(values)

    if values.ndim == 1:

        exp = np.exp(
            values - np.max(values)
        )

        return exp / np.sum(exp)


    exp = np.exp(
        values -
        np.max(values, axis=1, keepdims=True)
    )

    return exp / np.sum(
        exp,
        axis=1,
        keepdims=True
    )
