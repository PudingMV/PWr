import numpy as np
import random
import copy
import time

from sklearn.datasets import fetch_20newsgroups
from sklearn.model_selection import train_test_split, GridSearchCV
from sklearn.pipeline import Pipeline
from sklearn.feature_extraction.text import TfidfVectorizer

from sklearn.linear_model import LogisticRegression
from sklearn.naive_bayes import MultinomialNB
from sklearn.svm import LinearSVC
from sklearn.ensemble import RandomForestClassifier
from sklearn.neighbors import KNeighborsClassifier

from NeuralNetwork import NeuralNetwork
from DataPoint import DataPoint



# =====================================================
# 1. DATA
# =====================================================


print("\nLoading dataset...")


dataset = fetch_20newsgroups(
    subset="all",
    shuffle=True,
    random_state=42
)


print(
    "Total samples:",
    len(dataset.data)
)


print(
    "Classes:",
    len(dataset.target_names)
)



# =====================================================
# 2. TRAIN / VALIDATION / TEST SPLIT
# =====================================================


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


print("\nDataset split:")
print("Train:", len(X_train))
print("Validation:", len(X_val))
print("Test:", len(X_test))



# =====================================================
# 3. BASELINE
# =====================================================


majority = np.bincount(
    y_train
).argmax()


baseline = np.mean(
    y_test == majority
)


print(
    "\nMajority baseline:",
    baseline
)




# =====================================================
# 4. CLASSICAL MODELS
# =====================================================


models = {


"Logistic Regression":
(
Pipeline(
[
(
"tfidf",
TfidfVectorizer(
    stop_words="english",
    ngram_range=(1,2),
    max_features=10000,
    sublinear_tf=True
)
),

(
"classifier",
LogisticRegression(
    max_iter=1000
)
)

]),


{
"classifier__C":[0.1,1,10]
}

),



"Linear SVM":
(
Pipeline(
[
(
"tfidf",
TfidfVectorizer(
    stop_words="english",
    ngram_range=(1,2),
    max_features=10000
)
),

(
"classifier",
LinearSVC()
)

]),


{
"classifier__C":[0.01,0.1,1,10]
}

),



"Naive Bayes":
(
Pipeline(
[
(
"tfidf",
TfidfVectorizer(
    stop_words="english"
)
),

(
"classifier",
MultinomialNB()
)

]),


{
"classifier__alpha":[0.1,0.5,1]
}

),



"Random Forest":
(
Pipeline(
[
(
"tfidf",
TfidfVectorizer(
    stop_words="english",
    max_features=5000
)
),

(
"classifier",
RandomForestClassifier(
    n_estimators=200,
    random_state=42
)
)

]),


{
"classifier__max_depth":[20,50,None]
}

)

}



trained_models = {}

validation_scores = {}



print("\n\n==============================")
print("CLASSICAL MODELS")
print("==============================")



for name,(pipeline,params) in models.items():


    print(
        "\nSearching parameters for:",
        name
    )


    start=time.time()


    search = GridSearchCV(
        pipeline,
        params,
        cv=5,
        n_jobs=-1
    )


    search.fit(
        X_train,
        y_train
    )


    val_score = search.score(
        X_val,
        y_val
    )


    trained_models[name]=search.best_estimator_

    validation_scores[name]=val_score


    print(
        "Best parameters:"
    )

    print(
        search.best_params_
    )


    print(
        "Validation accuracy:",
        val_score
    )


    print(
        "Time:",
        round(time.time()-start,2),
        "seconds"
    )





# =====================================================
# 5. NEURAL NETWORK DATA
# =====================================================


print("\n\n==============================")
print("PREPARING NEURAL NETWORK")
print("==============================")


vectorizer = TfidfVectorizer(
    stop_words="english",
    ngram_range=(1,2),
    max_features=10000,
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
256,
128,
20
]
)



bestNetwork=None
bestVal=0

patience=10
withoutImprovement=0


epochs=100



for epoch in range(epochs):


    random.shuffle(trainData)


    network.Learn(
        trainData,
        0.00005,
        batchSize=128
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



# =====================================================
# 7. FINAL TEST
# =====================================================


print("\n\n==============================")
print("FINAL TEST RESULTS")
print("==============================")



for name,model in trained_models.items():


    if name=="Neural Network":

        score=Accuracy(
            model,
            testData
        )


    else:

        score=model.score(
            X_test,
            y_test
        )


    print(
        f"{name:25s}",
        "test accuracy:",
        round(score,4)
    )



print("\nDone.")