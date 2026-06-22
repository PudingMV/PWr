class DataPoint:
    def __init__(self, inputs, label):

        self.inputs = inputs

        self.expectedOutputs = [0.0] * 20
        self.expectedOutputs[label] = 1