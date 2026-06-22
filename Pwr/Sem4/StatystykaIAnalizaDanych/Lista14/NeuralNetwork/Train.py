import random
import pickle
from NeuralNetwork import NeuralNetwork
from DataPoint import DataPoint
from MnistLoader import MnistLoader, augmentImage

inputPath = "/home/puding/Projects/pythonNeuralNetwork/MNIST_DataSet"

trainImagesPath = inputPath + "/train-images-idx3-ubyte/train-images-idx3-ubyte"
trainLabelsPath = inputPath + "/train-labels-idx1-ubyte/train-labels-idx1-ubyte"
testImagesPath = inputPath + "/t10k-images-idx3-ubyte/t10k-images-idx3-ubyte"
testLabelsPath = inputPath + "/t10k-labels-idx1-ubyte/t10k-labels-idx1-ubyte"

print("Loading MNIST...")
(iTrain, lTrain), (iTest, lTest) = MnistLoader.loadDataset(
  trainImagesPath, trainLabelsPath, testImagesPath, testLabelsPath)



print("Preparing DataPoint...")
trainingData = [DataPoint(iTrain[i], lTrain[i]) for i in range(len(iTrain))]
testData = [DataPoint(iTest[i], lTest[i]) for i in range(len(iTest))]

network = NeuralNetwork([784, 256, 256, 10])

epochs = 15
batchSize = 100
initialLearningRate = 0.1
#decayedLearningRate = 0.05
#decay = 0.9

for epoch in range(epochs):
  #learningRate = initialLearningRate * (decay ** epoch)
  learningRate = initialLearningRate

  """if epochs > 10:
    learningRate = decayedLearningRate"""

  print(f"\nEpoch {epoch + 1}/{epochs}")

  random.shuffle(trainingData)

  for i in range(0, len(trainingData), batchSize):
    batch = trainingData[i:i + batchSize]

    augmentedBatch = []
    for dP in batch:
      if random.random() < 0.5:
        augmentedInputs = augmentImage(dP.inputs)
      else:
        augmentedInputs = dP.inputs
      augmentedBatch.append(DataPoint(augmentedInputs, dP. expectedOutputs.index(1.0)))

    network.Learn(augmentedBatch, learningRate)


  correct = 0

  for dP in testData:
    prediction = network.Classify(dP.inputs)
    actual = dP.expectedOutputs.index(1.0)

    if prediction == actual:
      correct += 1

  accuracy = correct / len(testData) * 100
  print(f"Test Accuracy: {accuracy:.2f}%")

with open("trainedNetwork.pkl", "wb") as f:
  pickle.dump(network, f)
