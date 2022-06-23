import numpy as np
from sklearn.model_selection import train_test_split
from sklearn import Datasets



class Adaline(object):

    def __init__(self, number_of_iterations=100, random_state=1, learning_rate=0.01):

        self.number_of_iterations = number_of_iterations
        self.random_state = random_state
        self.learning_rate = learning_rate

    def fit(self, X_train, Y_train):
        rgen = np.random.RandomState(self.random_state)
        self.weight = rgen.normal(loc=0.0, scale=0.01, size=1 + X_train.shape[1])
        for _ in range(self.number_of_iterations):

            activation_function_output = self.activation_function(self.net_input(X_train))

            errors = Y_train - activation_function_output

            #Update the weight
            self.weight[1:] = self.weight[1:] + \
                self.learning_rate * X_train.T.dot(errors)
            
            #Update the Bias
            self.weight[0] = self.weight[0] + self.learning_rate*errors.sum()

    def net_input(self, X):
        
        weighted_sum = np.dot(X, self.weight[1:]) + self.weight[0]
        return weighted_sum

    def activation_function(self, X):
        #Linear activation
        return X

    def predict(self, x_test):
        return np.where(self.activation_function(self.net_input(x_test)) >= 0.0, 1, 0)

    def score(self, x_test, y_test):
        misclassified_data_count = 0
        for x_test_input, target_output in zip(x_test, y_test):
            
            #Try with the test dataset
            produced_output = self.predict(x_test_input)
            if(target_output != produced_output):
                misclassified_data_count += 1
        total_data_count = len(x_test)
        self.accuracy = (total_data_count - misclassified_data_count) / total_data_count
        #Return accuracy as a percentage 
        return self.accuracy