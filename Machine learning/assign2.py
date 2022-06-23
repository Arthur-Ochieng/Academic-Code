from typing import Text
class CustomAdaline(object):
    
    def __init__(self, n_iterations=100, random_state=1, learning_rate=0.01):

        self.n_iterations = n_iterations

        self.random_state = random_state

        self.learning_rate = learning_rate

    def fit(self, X, y):

        rgen = np.random.RandomState(self.random_state)

        self.coef_ = rgen.normal(loc=0.0, scale=0.01, size=1 + X.shape[1])

        for _ in range(self.n_iterations):

              activation_function_output = self.activation_function(self.net_input(X))

              errors = y - activation_function_output

              self.coef_[1:] = self.coef_[1:] + self.learning_rate*X.T.dot(errors)

              self.coef_[0] = self.coef_[0] + self.learning_rate*errors.sum() 
    '''
    Net Input is sum of weighted input signals
    '''
    def net_input(self, X):
            weighted_sum = np.dot(X, self.coef_[1:]) + self.coef_[0]
            return weighted_sum

    '''
    Activation function is fed the net input. As the activation function is
    an identity function, the output from activation function is same as the
    input to the function.
    '''

    def activation_function(self, X):
            return X

    '''
    Prediction is made on the basis of output of activation function
    '''

    def predict(self, X):

        return np.where(self.activation_function(self.net_input(X)) >= 0.0, 1, 0) 

    

    '''
    Model score is calculated based on comparison of 
    expected value and predicted value
    '''

    def score(self, X, y):

        misclassified_data_count = 0

        for xi, target in zip(X, y):

            output = self.predict(xi)
            if(target != output):

                misclassified_data_count += 1

        total_data_count = len(X)

        self.score_ = (total_data_count - misclassified_data_count)/total_data_count

        return self.score_