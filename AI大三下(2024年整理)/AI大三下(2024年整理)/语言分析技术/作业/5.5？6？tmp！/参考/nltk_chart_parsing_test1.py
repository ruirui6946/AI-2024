import nltk
grammar = nltk.CFG.fromstring("""
S -> NP VP
NP -> ART ADJ N | ART N | ADJ N
VP -> AUX VP | V NP
ART -> 'the'
ADJ -> 'large'
N -> 'can' | 'hold' | 'water'
AUX -> 'can'
V -> 'can' | 'hold' | 'water'
""")
tokens = ['the', 'large', 'can', 'can', 'hold', 'the', 'water']
parser = nltk.ChartParser(grammar, trace=1)
for tree in parser.parse(tokens):
    print(tree)
    tree.draw()
