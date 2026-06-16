"""
$ python feature.py \
largedata/train_large.tsv \
largedata/val_large.tsv \
largedata/test_large.tsv \
word2vec.txt \
largeoutput/formatted_train_large.tsv \
largeoutput/formatted_val_large.tsv \
largeoutput/formatted_test_large.tsv

$ python feature.py \
smalldata/train_small.tsv \
smalldata/val_small.tsv \
smalldata/test_small.tsv \
word2vec.txt \
smalloutput/formatted_train_small.tsv \
smalloutput/formatted_val_small.tsv \
smalloutput/formatted_test_small.tsv
"""

import csv
import numpy as np
import argparse

VECTOR_LEN = 300   # Length of word2vec vector
MAX_WORD_LEN = 64  # Max word length in dict.txt and word2vec.txt

################################################################################
# We have provided you the functions for loading the tsv and txt files. Feel   #
# free to use them! No need to change them at all.                             #
################################################################################


def load_tsv_dataset(file):
    """
    Loads raw data and returns a tuple containing the reviews and their ratings.

    Parameters:
        file (str): File path to the dataset tsv file.

    Returns:
        An np.ndarray of shape N. N is the number of data points in the tsv file.
        Each element dataset[i] is a tuple (label, review), where the label is
        an integer (0 or 1) and the review is a string.
    """
    dataset = np.loadtxt(file, delimiter='\t', comments=None, encoding='utf-8', dtype='l,O')
    return dataset


def load_feature_dictionary(file):
    """
    Creates a map of words to vectors using the file that has the word2vec
    embeddings.

    Parameters:
        file (str): File path to the word2vec embedding file.

    Returns:
        A dictionary indexed by words, returning the corresponding word2vec
        embedding np.ndarray.
    """
    word2vec_map = dict()
    with open(file) as f:
        read_file = csv.reader(f, delimiter='\t')
        for row in read_file:
            word, embedding = row[0], row[1:]
            word2vec_map[word] = np.array(embedding, dtype=float)
    return word2vec_map

# TODO
# -------------------------------
def extract_features(dataset, word2vec_map):
    """
    Performs feature extraction using word2vec embeddings.

    Parameters:
        dataset (np.ndarray): Raw data containing (label, review) tuples.
        word2vec_map (dict): Dictionary mapping words to their word2vec embeddings.

    Returns:
        An np.ndarray containing formatted data points (label, feature_vector).
    """
    formatted_data = []
    for label, review in dataset:
        words = review.split()
        # Trim words not in word2vec_map
        trimmed_words = [word for word in words if word in word2vec_map]
        # If no words are in word2vec_map, use a zero vector
        if len(trimmed_words) == 0:
            feature_vector = np.zeros(VECTOR_LEN)
        else:
            feature_vector = np.mean([word2vec_map[word] for word in trimmed_words], axis=0)
        formatted_data.append((label, feature_vector))
    return np.array(formatted_data, dtype=object)


def write_to_file(formatted_data, file):
    """
    Writes the formatted data to a tsv file.

    Parameters:
        formatted_data (np.ndarray): Formatted data containing (label, feature_vector) tuples.
        file (str): File path to write the formatted data.
    """
    with open(file, 'w') as f:
        for label, feature_vector in formatted_data:
            # Format label and feature_vector to 6 decimal places
            label_str = "{:.6f}".format(label)
            features_str = "\t".join("{:.6f}".format(val) for val in feature_vector)
            f.write(f"{label_str}\t{features_str}\n")
# -------------------------------

if __name__ == "__main__":
    # Initialize argument parser
    parser = argparse.ArgumentParser(description="Process input and output files for feature extraction and machine learning.")

    # Add arguments
    parser.add_argument('train_input', type=str, help="Path to the training input .tsv file.")
    parser.add_argument('validation_input', type=str, help="Path to the validation input .tsv file.")
    parser.add_argument('test_input', type=str, help="Path to the test input .tsv file.")
    parser.add_argument('feature_dictionary_input', type=str, help="Path to the word2vec feature dictionary .txt file.")
    parser.add_argument('formatted_train_out', type=str, help="Path to the output .tsv file where extracted features from the training data will be written in.")
    parser.add_argument('formatted_validation_out', type=str, help="Path to the output .tsv file where extracted features from the validation data will be written in.")
    parser.add_argument('formatted_test_out', type=str, help="Path to the output .tsv file where extracted features from the test data will be written in.")

    # Parse arguments
    args = parser.parse_args()
    
    # TODO
    # -------------------------------
    # Load datasets
    train_dataset = load_tsv_dataset(args.train_input)
    validation_dataset = load_tsv_dataset(args.validation_input)
    test_dataset = load_tsv_dataset(args.test_input)

    # Load feature dictionary
    word2vec_map = load_feature_dictionary(args.feature_dictionary_input)

    # Extract features
    formatted_train_data = extract_features(train_dataset, word2vec_map)
    formatted_validation_data = extract_features(validation_dataset, word2vec_map)
    formatted_test_data = extract_features(test_dataset, word2vec_map)

    # Write formatted data to output files
    write_to_file(formatted_train_data, args.formatted_train_out)
    write_to_file(formatted_validation_data, args.formatted_validation_out)
    write_to_file(formatted_test_data, args.formatted_test_out)
    # -------------------------------