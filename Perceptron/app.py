class Perceptron:
    def __init__(self):
        self.w1 = 0
        self.w2 = 0
        self.w3 = 0
        self.b = 0

    def activation(self, n):
        if n < 0:
            return -1
        else:
            return 1

    def predict(self, x1, x2, x3):
        n = (self.w1 * x1) + (self.w2 * x2) + (self.w3 * x3) + self.b
        return self.activation(n)

    def train(self, data, iterations):
        for i in range(iterations):
            total_error = 0
            for x1, x2, x3, y0 in data:

                prediction = self.predict(x1, x2, x3)

                error = y0 - prediction
                if prediction != y0:
                    #error = y0 - prediction

                    self.w1 += error * x1
                    self.w2 += error * x2
                    self.w3 += error * x3
                    self.b += error
                total_error += abs(error)

            if total_error == 0:
                print(f'Convergence reached at epoch {i+1}')
                print(f'Optimal values: w1 = {self.w1}, w2 = {self.w2}, w3 = {self.w3}, b = {self.b}')
                return
        print("The perceptron did not converge within the specified number of iterations")


perceptron = Perceptron()

data = [
    [1,-1,-1,-1],
    [1,1,-1,1]
]

perceptron.train(data, iterations=100)
