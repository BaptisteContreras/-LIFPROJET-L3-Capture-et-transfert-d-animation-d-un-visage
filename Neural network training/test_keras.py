from keras import Input, Model
from keras.models import Sequential
from keras.layers import Dense
import numpy

numpy.random.seed(7)

data = numpy.loadtxt("pima-indians-diabetes.csv.txt", delimiter=",")
X = data[:, 0:8]
Y = data[:, 8]

print(X, Y)
"""
model = Sequential()
model.add(Dense(12, input_dim=8, activation='relu'))
model.add(Dense(8, activation='relu'))
model.add(Dense(1, activation='sigmoid'))
"""
input = Input(shape=(8,))
l1 = Dense(12, activation='relu')(input)
l2 = Dense(8, activation='relu')(l1)
l3 = Dense(1, activation='sigmoid')(l2)
model = Model(inputs=input, outputs=[l3])

model.compile(loss='binary_crossentropy', optimizer='adam', metrics=['accuracy'])

model.fit(X, Y, epochs=150, batch_size=10)

score = model.evaluate(X, Y)
print("\n{}: {}".format(model.metrics_names[1], score[1]*100))
