from keras.models import Sequential
from keras.layers import Dense
import math
import numpy

numpy.random.seed(11)

dataX = []
dataY = []
for i in range(1, 56):
    dataX.append(numpy.loadtxt("out/n{}.txt".format(i), delimiter=' '))
    dataY.append(numpy.loadtxt("out/h{}.txt".format(i), delimiter=' '))

result = 0.
e = 5.
for i in range(len(dataX)):
    rTemp = 0.
    for j in range(len(dataX[i])):
        if math.hypot(dataX[i][j][0] - dataY[i][j][0], dataX[i][j][1] - dataY[i][j][1]) > e:
            rTemp += 1
    result += rTemp / len(dataX[i])
result /= len(dataX)
print("\tPourcentage des points non-déplacés : {} selon e = {} pixels".format(1-result, e))

X = numpy.zeros((len(dataX), len(dataX[0])*2))
for i in range(len(X)):
    for j in range(len(dataX[i])):
        X[i][j*2] = (dataX[i][j][0])
        X[i][j*2+1] = (dataX[i][j][1])

Y = numpy.zeros((len(dataY), len(dataY[0])*2))
for i in range(len(Y)):
    for j in range(len(dataY[i])):
        Y[i][j*2] = (dataY[i][j][0])
        Y[i][j*2+1] = (dataY[i][j][1])

#print(X, Y)

model = Sequential()
model.add(Dense(136, input_dim=136, activation='relu'))
model.add(Dense(204, activation='relu'))
model.add(Dense(272 , activation='relu'))
model.add(Dense(204, activation='relu'))
model.add(Dense(136, activation='relu'))

#print(model.output_shape)
print(model.summary())

model.compile(loss='mean_absolute_error', optimizer='sgd', metrics=['accuracy'])

model.fit(X, Y, epochs=15000, batch_size=55)

score = model.evaluate(X, Y)
print("\n{}: {}".format(model.metrics_names[1], score[1]*100))

print("Sauvegarder ce model ? [taper le nom du fichier où sauvegarder pour sauvegarder] ")
response = input()
if response != "":
    model.save_weights(response)
