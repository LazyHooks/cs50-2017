import nltk

class Analyzer():
    #"""Implements sentiment analysis."""

    def __init__(self, positives, negatives):
       # """Initialize Analyzer."""
        self.positives_set=set()
        file = open(positives,"r") # opens positive words dictionary
        for line in file:
            if line.startswith(';') != True:
                self.positives_set.add(line.rstrip("\n"))
        file.close() 
            
        self.negatives_set = set()
        file = open(negatives, "r") #opens negative words dictionary
        for line in file:
            if line.startswith(';') != True:
                self.negatives_set.add(line.rstrip("\n"))
        file.close()

    def analyze(self, text):
        #"""Analyze text for sentiment, returning its score."""

        sum = 0
        tokenizer = nltk.tokenize.TweetTokenizer()
        tokens = tokenizer.tokenize(text)
        for word in tokens:
            if word.lower() in self.positives_set:
                sum = sum + 1
                
            elif word.lower() in self.negatives_set:
                sum = sum - 1
                
            else:
                continue
            
        return sum
