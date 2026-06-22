import random
import numpy as np
import pickle
import copy

from sklearn.datasets import fetch_20newsgroups
from sklearn.feature_extraction.text import TfidfVectorizer
from sklearn.linear_model import LogisticRegression

from NeuralNetwork import NeuralNetwork
from DataPoint import DataPoint


# ==========================
# 1. Pobranie danych
# ==========================

print("Loading dataset...")

train = fetch_20newsgroups(
    subset="train",
    shuffle=True,
    random_state=42
)

test = fetch_20newsgroups(
    subset="test",
    shuffle=True,
    random_state=42
)


print(
    "Train size:",
    len(train.data)
)

print(
    "Test size:",
    len(test.data)
)


# ==========================
# 2. TF-IDF
# ==========================

print("\nVectorizing text...")


vectorizer = TfidfVectorizer(
    stop_words="english",
    ngram_range=(1,2),
    max_features=10000,
    min_df=2,
    max_df=0.95,
    sublinear_tf=True
)


X_train = vectorizer.fit_transform(
    train.data
)

X_test = vectorizer.transform(
    test.data
)


print(
    "Input size:",
    X_train.shape[1]
)


# ==========================
# 3. Zamiana danych na DataPoint
# ==========================


print("\nPreparing DataPoints...")


trainingData = []

for x, y in zip(
        X_train.toarray(),
        train.target
):

    trainingData.append(
        DataPoint(
            x,
            y
        )
    )

random.shuffle(trainingData)
trainingData = trainingData


testData = []

for x, y in zip(
        X_test.toarray(),
        test.target
):

    testData.append(
        DataPoint(
            x,
            y
        )
    )

testData = testData  # Limit to 200 samples for faster testing


# ==========================
# 4. Tworzenie sieci
# ==========================

def Accuracy(network, data):
    correct = 0

    for dp in data:

        pred = network.Classify(dp.inputs)

        trueLabel = dp.expectedOutputs.index(1)

        if pred == trueLabel:
            correct += 1

    return correct / len(data)


inputSize = X_train.shape[1]


network = NeuralNetwork(
    [
        inputSize,
        256,
        128,
        20
    ]
)


print("\nNetwork created")


# ==========================
# 5. Trening
# ==========================


epochs = 300
learningRate = 0.00005

bestTestAcc = 0
patience = 10
epochsWithoutImprovement = 0

bestNetwork = None


for epoch in range(epochs):

    random.shuffle(trainingData)

    network.Learn(
        trainingData,
        learningRate,
        batchSize=128
    )


    cost = network.TotalCost(
        trainingData
    )

    trainAcc = Accuracy(
        network,
        trainingData
    )

    testAcc = Accuracy(
        network,
        testData
    )


    print(
        f"Epoch {epoch+1}: "
        f"cost={cost:.4f} "
        f"train={trainAcc:.4f} "
        f"test={testAcc:.4f}"
    )


    # ==========================
    # Early stopping
    # ==========================

    if testAcc > bestTestAcc:

        print("New best model!")

        bestTestAcc = testAcc
        epochsWithoutImprovement = 0

        bestNetwork = copy.deepcopy(network)


    else:

        epochsWithoutImprovement += 1


    if epochsWithoutImprovement >= patience:

        print(
            "Early stopping!"
        )

        break



# ==========================
# 6. Test
# ==========================

if bestNetwork is not None:
    network = bestNetwork

print(
    "Restored best model with test accuracy:",
    bestTestAcc
)


print("\nTesting...")


correct = 0


for dataPoint, realLabel in zip(
        testData,
        test.target
):

    prediction = network.Classify(
        dataPoint.inputs
    )

    if prediction == realLabel:
        correct += 1


accuracy = correct / len(testData)


print(
    "Test accuracy:",
    accuracy
)

model = {
    "network": network,
    "vectorizer": vectorizer,
    "classes": train.target_names
}


with open("news_classifier.pkl", "wb") as f:
    pickle.dump(model, f)


print("Model saved")

clf = LogisticRegression(
    max_iter=1000
)

clf.fit(X_train, train.target)

print(
    clf.score(X_test, test.target)
)