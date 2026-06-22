import numpy as np
import random
import copy
import time

from sklearn.datasets import fetch_20newsgroups

from sklearn.model_selection import (
    train_test_split,
    GridSearchCV,
    ParameterGrid
)

from sklearn.pipeline import Pipeline

from sklearn.feature_extraction.text import (
    CountVectorizer,
    TfidfTransformer,
    TfidfVectorizer
)

from sklearn.linear_model import LogisticRegression
from sklearn.naive_bayes import MultinomialNB
from sklearn.svm import LinearSVC
from sklearn.ensemble import RandomForestClassifier
from sklearn.neighbors import KNeighborsClassifier

from NeuralNetwork import NeuralNetwork
from DataPoint import DataPoint

print("\nLoading dataset...")

dataset = fetch_20newsgroups(
    subset="all",
    shuffle=True,
    random_state=42
)

print("Samples:", len(dataset.data))
print("Classes:", len(dataset.target_names))

X_temp, X_test, y_temp, y_test = train_test_split(
    dataset.data,
    dataset.target,
    test_size=0.2,
    random_state=42,
    stratify=dataset.target
)

X_train, X_val, y_train, y_val = train_test_split(
    X_temp,
    y_temp,
    test_size=0.2,
    random_state=42,
    stratify=y_temp
)

print("\nDataset split")
print("Train:", len(X_train))
print("Validation:", len(X_val))
print("Test:", len(X_test))

majority = np.bincount(y_train).argmax()

baseline = np.mean(
    y_test == majority
)

print(
    "\nMajority baseline:",
    round(baseline,4)
)

models = {
"Logistic Regression":
(
Pipeline([
    ("vect", CountVectorizer()),
    ("tfidf", TfidfTransformer()),
    ("classifier", LogisticRegression(max_iter=1000))
]),

{
    "vect__stop_words":[None,"english"],

    "vect__ngram_range":[
        (1,1),
        (1,2)
    ],

    "vect__max_features":[
        5000,
        10000
    ],

    "tfidf__use_idf":[
        True,
        False
    ],

    "classifier__C":[
        0.1,
        1,
        10
    ]
}
),

"Linear SVM":
(
Pipeline([
    ("vect", CountVectorizer()),
    ("tfidf", TfidfTransformer()),
    ("classifier", LinearSVC(max_iter=20000))
]),

{
    "vect__stop_words":[None,"english"],

    "vect__ngram_range":[
        (1,1),
        (1,2)
    ],

    "vect__max_features":[
        5000,
        10000
    ],

    "tfidf__use_idf":[
        True,
        False
    ],

    "classifier__C": [
        0.001,
        0.01,
        0.1,
        1,
        10,
        100
    ]
}
),

"Naive Bayes":
(
Pipeline([
    ("vect", CountVectorizer()),
    ("tfidf", TfidfTransformer()),
    ("classifier", MultinomialNB())
]),

{
    "vect__stop_words":[None,"english"],

    "vect__ngram_range":[
        (1,1),
        (1,2)
    ],

    "tfidf__use_idf":[
        True,
        False
    ],

    "classifier__alpha":[
        0.1,
        0.5,
        1.0
    ]
}
),

"Random Forest":
(
Pipeline([
    ("vect", CountVectorizer()),
    ("tfidf", TfidfTransformer()),
    ("classifier",
        RandomForestClassifier(
            random_state=42
        )
    )
]),

{
    "vect__stop_words":[None,"english"],

    "vect__max_features":[
        3000,
        5000
    ],

    "tfidf__use_idf":[
        True,
        False
    ],

    "classifier__n_estimators":[
        100,
        200
    ],

    "classifier__max_depth":[
        20,
        50,
        None
    ]
}
),

"KNN":
(
Pipeline([
    ("vect", CountVectorizer()),
    ("tfidf", TfidfTransformer()),
    ("classifier",
        KNeighborsClassifier()
    )
]),

{
    "vect__stop_words":[None,"english"],

    "vect__max_features":[
        3000,
        5000
    ],

    "tfidf__use_idf":[
        True,
        False
    ],

    "classifier__n_neighbors":[
        3,
        5,
        7,
        11
    ],

    "classifier__weights":[
        "uniform",
        "distance"
    ]
}
)
}

trained_models = {}
validation_scores = {}

print("\n==============================")
print("CLASSICAL MODELS")
print("==============================")

for name,(pipeline,params) in models.items():

    print("\n")
    print("="*60)
    print(name)
    print("="*60)

    grid = list(
        ParameterGrid(params)
    )

    print(
        "Configurations:",
        len(grid)
    )

    for i,p in enumerate(grid,1):

        print(
            f"{i}/{len(grid)}",
            p
        )

    start = time.time()

    search = GridSearchCV(
        pipeline,
        params,
        cv=5,
        n_jobs=-1,
        verbose=2
    )

    search.fit(
        X_train,
        y_train
    )

    print("\nCV RESULTS")

    results = search.cv_results_

    for mean,std,param in zip(
        results["mean_test_score"],
        results["std_test_score"],
        results["params"]
    ):

        print(
            f"{mean:.4f} (+/- {std:.4f})",
            param
        )

    print("\nBEST PARAMS")
    print(
        search.best_params_
    )

    val_score = search.score(
        X_val,
        y_val
    )

    print(
        "Validation accuracy:",
        round(val_score,4)
    )

    trained_models[name] = (
        search.best_estimator_
    )

    validation_scores[name] = (
        val_score
    )

    print(
        "Time:",
        round(
            time.time()-start,
            2
        ),
        "sec"
    )

print("\n\n==============================")
print("PREPARING NEURAL NETWORK")
print("==============================")


vectorizer = TfidfVectorizer(
    stop_words="english",
    ngram_range=(1,2),
    max_features=100000,
    min_df=2,
    max_df=0.95,
    sublinear_tf=True
)



X_train_nn = vectorizer.fit_transform(
    X_train
)

X_val_nn = vectorizer.transform(
    X_val
)

X_test_nn = vectorizer.transform(
    X_test
)



def MakeData(X,y):

    data=[]

    for x,label in zip(
        X.toarray(),
        y
    ):

        data.append(
            DataPoint(
                x,
                label
            )
        )

    return data



trainData = MakeData(
    X_train_nn,
    y_train
)


valData = MakeData(
    X_val_nn,
    y_val
)


testData = MakeData(
    X_test_nn,
    y_test
)



def Accuracy(network,data):

    correct=0

    for x in data:

        prediction = network.Classify(
            x.inputs
        )

        if prediction == x.expectedOutputs.index(1):
            correct+=1


    return correct/len(data)





# =====================================================
# 6. TRAIN NN WITH VALIDATION
# =====================================================



print("\nTraining Neural Network...")


network = NeuralNetwork(
[
X_train_nn.shape[1],
512,
256,
256,
128,
20
]
)

network.layers[0].dropoutRate = 0.10
network.layers[1].dropoutRate = 0.15
network.layers[2].dropoutRate = 0.15
network.layers[3].dropoutRate = 0.25

bestNetwork=None
bestVal=0

patience=20
withoutImprovement=0


epochs=300



for epoch in range(epochs):


    random.shuffle(trainData)


    network.Learn(
        trainData,
        0.0002,
        batchSize=256
    )


    trainAcc = Accuracy(
        network,
        trainData
    )


    valAcc = Accuracy(
        network,
        valData
    )


    print(
        f"Epoch {epoch+1}: "
        f"train={trainAcc:.4f} "
        f"validation={valAcc:.4f}"
    )



    if valAcc > bestVal:

        print(
            "New best NN model"
        )

        bestVal=valAcc
        bestNetwork=copy.deepcopy(network)

        withoutImprovement=0


    else:

        withoutImprovement+=1



    if withoutImprovement>=patience:

        print(
            "Early stopping"
        )

        break



network=bestNetwork


trained_models["Neural Network"]=network

validation_scores["Neural Network"]=bestVal

print("\n")
print("="*60)
print("FINAL TEST")
print("="*60)

results = []

for name,model in trained_models.items():

    if name == "Neural Network":

        score = Accuracy(
            model,
            testData
        )

    else:

        score = model.score(
            X_test,
            y_test
        )

    results.append(
        (name,score)
    )

results.sort(
    key=lambda x:x[1],
    reverse=True
)

print("\nFINAL RANKING\n")

for name,score in results:

    print(
        f"{name:25s}",
        f"{score:.4f}"
    )