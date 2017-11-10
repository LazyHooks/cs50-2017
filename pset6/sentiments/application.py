from flask import Flask, redirect, render_template, request, url_for

import helpers
from analyzer import Analyzer
import sys
import os

app = Flask(__name__)

@app.route("/")
def index():
    return render_template("index.html")

@app.route("/search")
def search():

    # validate screen_name
    screen_name = request.args.get("screen_name", "").lstrip("@")
    if not screen_name:
        return redirect(url_for("index"))

    # get screen_name's tweets
    tweets = helpers.get_user_timeline(screen_name,100)
    
    #if 100 tweets dont exist
    if tweets==None:
        tweets = helpers.get_user_timeline(screen_name,100)
        
    if tweets==None:
        return redirect(url_for("index"))
    
    

    # set all counts to 0.0
    positive, negative, neutral = 0.0, 0.0, 0.0
    
    #list paths
    negatives = os.path.join(sys.path[0], "negative-words.txt")
    positives = os.path.join(sys.path[0], "positive-words.txt")
    
    #analyzer constructor
    analyzer = Analyzer(positives, negatives)
    
    #iterate over tweets classifying them
    for tweet in tweets:
        score = analyzer.analyze(tweet)
        if score > 0.0:
            positive = positive + 1
        elif score < 0.0:
            negative = negative + 1
        else:
            neutral = neutral + 1
        
    # generate chart
    chart = helpers.chart(positive, negative, neutral)

    # render results
    return render_template("search.html", chart=chart, screen_name=screen_name)
