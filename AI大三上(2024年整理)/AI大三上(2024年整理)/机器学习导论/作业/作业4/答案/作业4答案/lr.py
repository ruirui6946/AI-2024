"""
$ python lr.py \
largeoutput/formatted_train_large.tsv \
largeoutput/formatted_val_large.tsv \
largeoutput/formatted_test_large.tsv \
largeoutput/formatted_train_labels.txt \
largeoutput/formatted_test_labels.txt \
largeoutput/formatted_metrics.txt \
500 \
0.001

$ python lr.py \
smalloutput/formatted_train_small.tsv \
smalloutput/formatted_val_small.tsv \
smalloutput/formatted_test_small.tsv \
smalloutput/formatted_train_labels.txt \
smalloutput/formatted_test_labels.txt \
smalloutput/formatted_metrics.txt \
500 \
0.001
"""

import numpy as np
import argparse

def sigmoid(x):
    """
    Implementation of the sigmoid function.

    Parameters:
        x (str): Input np.ndarray.

    Returns:
        An np.ndarray after applying the sigmoid function element-wise to the
        input.
    """
    e = np.exp(x)
    return e / (1 + e)


def train(theta, X, y, num_epoch, learning_rate):
    # TODO: Implement `train` using vectorization
    # ---
    # Loop through the dataset for 'num_epoch' times
    for _ in range(num_epoch):
        # Loop through each data point in the dataset
        for i in range(len(X)):
            xi = X[i]  # Feature vector for the i-th data point
            yi = y[i]  # Label for the i-th data point
            h = sigmoid(np.dot(xi, theta))  # Compute the prediction
            gradient = (h - yi) * xi  # Compute the gradient
            theta -= learning_rate * gradient  # Update the model parameters
    return theta
    # ---


def predict(theta, X):
    # TODO: Implement `predict` using vectorization
    # ---
    # Compute the sigmoid of the dot product and round to get binary output
    return np.round(sigmoid(X.dot(theta)))
    # ---

def compute_error(y_pred, y):
    # TODO: Implement `compute_error` using vectorization
    # ---
    # Compute the mean of incorrect predictions
    return np.mean(y_pred != y)
    # ---
    
def load_vectorized_data(file):
    # TODO
    # ---
    # Load the data from the .tsv file
    data = np.loadtxt(file, delimiter='\t')
    
    # Split the data into features (X) and labels (y)
    y = data[:, 0]
    X = data[:, 1:]
    
    return X, y
    # ---

if __name__ == "__main__":
    # Initialize argument parser
    parser = argparse.ArgumentParser()

    # Add arguments
    parser.add_argument('formatted_train_input', type=str, help="Path to the vectorized training input .tsv file.")
    parser.add_argument('formatted_validation_input', type=str, help="Path to the vectorized validation input .tsv file.")
    parser.add_argument('formatted_test_input', type=str, help="Path to the vectorized test input .tsv file.")
    parser.add_argument('train_out', type=str, help="Path to the output .txt file where predictions on the training data will be written.")
    parser.add_argument('test_out', type=str, help="Path to the output .txt file where predictions on the test data will be written.")
    parser.add_argument('metrics_out', type=str, help="Path to the output .txt file where metrics like training and testing error will be written.")
    parser.add_argument('num_epoch', type=int, help="Number of epochs for SGD to iterate through the training data.")
    parser.add_argument('learning_rate', type=float, help="Learning rate for the SGD algorithm.")

    # Parse arguments
    args = parser.parse_args()

    
    # TODO
    # ---
    # Load training and test data
    X_train, y_train = load_vectorized_data(args.formatted_train_input)
    X_test, y_test = load_vectorized_data(args.formatted_test_input)

    # Initialize model parameters to zeros
    theta = np.zeros(X_train.shape[1])

    # Train the model
    theta = train(theta, X_train, y_train, args.num_epoch, args.learning_rate)

    # Make predictions on training and test data
    y_train_pred = predict(theta, X_train)
    y_test_pred = predict(theta, X_test)

    # Compute error rates
    train_error = compute_error(y_train_pred, y_train)
    test_error = compute_error(y_test_pred, y_test)

    # Write predictions and metrics to files
    with open(args.train_out, 'w') as f:
        for label in y_train_pred:
            f.write(f"{int(label)}\n")

    with open(args.test_out, 'w') as f:
        for label in y_test_pred:
            f.write(f"{int(label)}\n")

    with open(args.metrics_out, 'w') as f:
        f.write(f"error(train): {train_error:.6f}\n")
        f.write(f"error(test): {test_error:.6f}\n")
    # ---