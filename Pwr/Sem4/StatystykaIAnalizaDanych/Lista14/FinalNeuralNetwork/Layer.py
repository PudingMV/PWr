import numpy as np
import math


class Layer:

    def __init__(self, numNodesIn, numNodesOut, activation="relu"):

        self.numNodesIn = numNodesIn
        self.numNodesOut = numNodesOut
        self.activation = activation

        self.dropoutRate = 0.0
        self.dropoutMask = None

        # weights: input x output
        self.weights = np.zeros(
            (numNodesIn, numNodesOut)
        )

        self.biases = np.zeros(
            numNodesOut
        )


        # gradients
        self.costGradientW = np.zeros(
            (numNodesIn, numNodesOut)
        )

        self.costGradientB = np.zeros(
            numNodesOut
        )

        # Adam parameters
        self.mW = np.zeros_like(self.weights)
        self.vW = np.zeros_like(self.weights)

        self.mB = np.zeros_like(self.biases)
        self.vB = np.zeros_like(self.biases)

        self.t = 0

        self.beta1 = 0.9
        self.beta2 = 0.999
        self.epsilon = 1e-8

        # cache for backprop
        self.inputs = None
        self.weightedInputs = None
        self.activations = None


        self.InitialRandomWeights()



    # ==========================
    # Forward pass
    # ==========================

    def CalculateOutputs(
        self,
        inputs,
        training=False
    ):

      self.inputs = np.asarray(inputs)

      self.weightedInputs = (
          np.dot(self.inputs, self.weights)
          + self.biases
      )

      self.activations = self.ActivationFunction(
          self.weightedInputs
      )

      if (
          training
          and
          self.dropoutRate > 0
      ):

          self.dropoutMask = (
              np.random.rand(
                  *self.activations.shape
              )
              >
              self.dropoutRate
          )

          self.activations = (
              self.activations
              *
              self.dropoutMask
              /
              (1 - self.dropoutRate)
          )

      return self.activations



    # ==========================
    # Activation functions
    # ==========================

    def ActivationFunction(self, x):

        if self.activation == "relu":

            return np.where(
                x > 0,
                x,
                0.01 * x
            )


        elif self.activation == "linear":

            return x


        else:
            raise ValueError(
                f"Unsupported activation: {self.activation}"
            )



    def ActivationDerivative(self, x):

        if self.activation == "relu":

            return np.where(
                x > 0,
                1,
                0.01
            )


        elif self.activation == "linear":

            return np.ones_like(x)


        else:
            raise ValueError(
                f"Unsupported activation: {self.activation}"
            )



    # ==========================
    # Cost (kept for compatibility)
    # ==========================

    def NodeCost(self, outputActivation, expectedOutput):

        error = outputActivation - expectedOutput

        return error * error



    def NodeCostDerivative(self, outputActivation, expectedOutput):

        return 2 * (
            outputActivation -
            expectedOutput
        )



    # ==========================
    # Initialization
    # ==========================

    def InitialRandomWeights(self):

        # He initialization for ReLU

        self.weights = np.random.normal(
            0,
            math.sqrt(2 / self.numNodesIn),
            (self.numNodesIn, self.numNodesOut)
        )


        self.biases = np.random.uniform(
            -0.1,
            0.1,
            self.numNodesOut
        )



    # ==========================
    # Gradient update
    # ==========================

    def UpdateGradients(self, nodeValues):

      nodeValues = np.asarray(nodeValues)

      self.costGradientW += (
          self.inputs.T @ nodeValues
      )

      self.costGradientB += (
          np.sum(nodeValues, axis=0)
      )



    # ==========================
    # Apply update
    # ==========================

    def ApplyGradient(self, learnRate, batchSize):

        self.weights -= (
            learnRate *
            self.costGradientW /
            batchSize
        )


        self.biases -= (
            learnRate *
            self.costGradientB /
            batchSize
        )


        # reset gradients

        self.costGradientW.fill(0)

        self.costGradientB.fill(0)

    def ApplyAdam(self, learnRate, batchSize):

      gW = self.costGradientW / batchSize
      gB = self.costGradientB / batchSize

      self.mW = (
        self.beta1 * self.mW
        + (1 - self.beta1) * gW
      )

      self.mB = (
        self.beta1 * self.mB
        + (1 - self.beta1) * gB
      )

      self.vW = (
        self.beta2 * self.vW
        + (1 - self.beta2) * (gW ** 2)
      )

      self.vB = (
        self.beta2 * self.vB
        + (1 - self.beta2) * (gB ** 2)
      )

      self.t += 1

      mW_hat = self.mW / (
        1 - self.beta1 ** self.t
      )

      mB_hat = self.mB / (
        1 - self.beta1 ** self.t
      )

      vW_hat = self.vW / (
        1 - self.beta2 ** self.t
      )

      vB_hat = self.vB / (
        1 - self.beta2 ** self.t
      )

      l2 = 0.0001

      self.weights -= (
        learnRate *
        (
          mW_hat /
          (np.sqrt(vW_hat) + self.epsilon)
          +
          l2 * self.weights
        )
      )

      self.biases -= (
        learnRate
        * mB_hat
        / (np.sqrt(vB_hat) + self.epsilon)
      )

      
      # reset gradients

      self.costGradientW.fill(0)

      self.costGradientB.fill(0)