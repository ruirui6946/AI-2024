import nltk
grammar = nltk.PCFG.fromstring("""

S -> NP VP [1.0]

VP -> v NP [0.7]
VP -> v NP PP [0.3]

NP -> det n [0.4]
NP -> det n PP [0.6]
PP -> p NP [1]
det -> 'the' [1]
n -> 'man' [0.4]
n -> 'girl' [0.3]
n -> 'telescope' [0.3]
v -> 'saw' [1]
p -> 'with' [1]
""")

viterbi_parser = nltk.ViterbiParser(grammar)
for tree in viterbi_parser.parse(['the','man', 'saw','the','girl','with','the', 'telescope']):
    print(tree)
    tree.draw()
