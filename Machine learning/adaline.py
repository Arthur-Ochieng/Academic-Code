from sklearn.datasets import load_breast_cancer
import numpy as np
from sklearn.model_selection import train_test_split
from adaline import Adaline
import pandas as pd
dataframe = pd.read_csv('pimadiabetes_data.txt',delimiter=" ")

#Selects the values in the Diastolic blood pressure column
diastolic_blood_pressure_train = dataframe.values[:5,2]
diastolic_blood_pressure_test = dataframe.values[6:8,2]


#Selects the values in the Body mass index column
body_mass_index_train = dataframe.values[:5,5]
body_mass_index_test = dataframe.values[6:8,5]


x_train = np.stack((diastolic_blood_pressure_train, body_mass_index_train), axis=1)
x_test = np.stack((body_mass_index_test, body_mass_index_test), axis=1)


print(x_train)
#Gets the last column which is the output
y_train = dataframe.values[:5,-1]
y_test = dataframe.values[:5,-1]

print(y_train,y_test)


adaline = Adaline(number_of_iterations=2)
adaline.fit(x_train, y_train)
score = adaline.score(x_test, y_test)
print(score)