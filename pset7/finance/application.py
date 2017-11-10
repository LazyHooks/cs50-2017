from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import mkdtemp

from helpers import *

# configure application
app = Flask(__name__)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"] = usd

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

@app.route("/")
@login_required
def index():
    #symbol selection user owns and respective amount
    portfolio_symbols = db.execute("SELECT shares, symbol \
                                    FROM portfolio WHERE id = :id", \
                                    id=session["user_id"])
    # total_cash stores net worth of stocks and cash temporarily
    total_cash = 0
    
    # update each symbol prices and total
    for pf_symbol in portfolio_symbols:
        symbol = pf_symbol["symbol"]
        shares = pf_symbol["shares"]
        stock = lookup(symbol)
        total = shares * stock["price"]
        total_cash += total
        db.execute("UPDATE portfolio SET price=:price, \
                    total=:total WHERE id=:id AND symbol=:symbol", \
                    price=usd(stock["price"]), \
                    total=usd(total), id=session["user_id"], symbol=symbol)
    
    # update cash value in portfolio
    updated_cash = db.execute("SELECT cash FROM users \
                               WHERE id=:id", id=session["user_id"])
    
    # total cash = share value + cash value
    total_cash += updated_cash[0]["cash"]
    
    # show portfolio
    updated_portfolio = db.execute("SELECT * from portfolio \
                                    WHERE id=:id", id=session["user_id"])
    #return rendered template (currency=usd)                                
    return render_template("index.html", stocks=updated_portfolio, \
                            cash=usd(updated_cash[0]["cash"]), total= usd(total_cash) )


    
@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock."""
    #renders the buy webpage
    if request.method == "GET":
        return render_template("buy.html")
    else:
        # checks symbol against valid stock
        stock = lookup(request.form.get("symbol"))
        if not stock:
            return apology("Invalid Symbol")
        
        # ensures the user does not enter negative no. of shares to buy
        # uses exception catching block
        try:
            shares = int(request.form.get("shares"))
            if shares < 0:
                return apology("Shares must be positive integer")
        except:
            return apology("Shares must be positive integer")
        
        # select cash of particular user based on id
        money = db.execute("SELECT cash FROM users WHERE id = :id", \
                            id=session["user_id"])
        
        # ensures user has enougn cash to buy shares
        if not money or float(money[0]["cash"]) < stock["price"] * shares:
            return apology("Not enough money")
        
        # keeps account of events
        db.execute("INSERT INTO histories (symbol, shares, price, id) \
                    VALUES(:symbol, :shares, :price, :id)", \
                    symbol=stock["symbol"], shares=shares, \
                    price=usd(stock["price"]), id=session["user_id"])
                       
        # debits cash              
        db.execute("UPDATE users SET cash = cash - :purchase WHERE id = :id", \
                    id=session["user_id"], \
                    purchase=stock["price"] * float(shares))
                        
        # select shares of chosen symbol
        user_shares = db.execute("SELECT shares FROM portfolio \
                           WHERE id = :id AND symbol=:symbol", \
                           id=session["user_id"], symbol=stock["symbol"])
                           
        #creates new stock object for new shares
        if not user_shares:
            db.execute("INSERT INTO portfolio (name, shares, price, total, symbol, id) \
                        VALUES(:name, :shares, :price, :total, :symbol, :id)", \
                        name=stock["name"], shares=shares, price=usd(stock["price"]), \
                        total=usd(shares * stock["price"]), \
                        symbol=stock["symbol"], id=session["user_id"])
                        
        # if object already exists, simply increments no. of particular shares
        else:
            shares_total = user_shares[0]["shares"] + shares
            db.execute("UPDATE portfolio SET shares=:shares \
                        WHERE id=:id AND symbol=:symbol", \
                        shares=shares_total, id=session["user_id"], \
                        symbol=stock["symbol"])
        
        # redirects user to index
        return redirect(url_for("index"))


@app.route("/history")
@login_required
def history():
    """Show history of transactions."""
    #gets list of libraries format from db
    histories = db.execute("SELECT * from histories WHERE id=:id", id=session["user_id"])
    #renders them on web page
    return render_template("history.html", histories=histories)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        # ensure username exists and password is correct
        if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
            return apology("invalid username and/or password")

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")

@app.route("/logout")
def logout():
    """Log user out."""

    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))

@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    #post queries lookup for the quote
    if request.method == "POST":
        rows = lookup(request.form.get("symbol"))
        
        #if query fails
        if not rows:
            return apology("Invalid Symbol")
        #shows quoted webpage    
        return render_template("quoted.html", stock=rows)
    #returns to quote webpage
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user."""
    
    if request.method == "POST":
        
         # verifies username was input
        if not request.form.get("username"):
            return apology("Must provide username")
            
         # verifies user typed in a password   
        elif not request.form.get("password"):
             return apology("Must provide password")
        
            
        
        # ensure password and verified password is the same
        elif request.form.get("password") != request.form.get("passwordagain"):
            return apology("password doesn't match")
            
            
        
            
        
        # new user gets inserted into users
        #password is hashed and stored
        result = db.execute("INSERT INTO users (username, hash) \
                             VALUES(:username, :hash)", \
                             username=request.form.get("username"), \
                             hash=pwd_context.encrypt(request.form.get("password")))
                 
        if not result:
            return apology("Username already exist")
        
        # store ID of current user logged in 
        session["user_id"] = result

        # redirect user to index
        return redirect(url_for("index"))
    
    #render register page again
    else:
        return render_template("register.html")                



@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock."""
   
    #if GET render sell again as get is not secure
    if request.method == "GET":
        return render_template("sell.html")
    else:
        # if POST check symbol against valid list of symbols
        stock = lookup(request.form.get("symbol"))
        #if invalid, returns apology page
        if not stock:
            return apology("Invalid Symbol")
        
        # take in no of shares to sell and that it is positive or 0
        #use try except exception catching block
        try:
            shares = int(request.form.get("shares"))
            if shares < 0:
                return apology("Shares must be positive integer")
        except:
            return apology("Shares must be positive integer")
        
        # select shares (of preferred symbol) of the user
        user_shares = db.execute("SELECT shares FROM portfolio \
                                 WHERE id = :id AND symbol=:symbol", \
                                 id=session["user_id"], symbol=stock["symbol"])
        
        # ensures user has enough shares to sell the input amount of those shares
        if not user_shares or int(user_shares[0]["shares"]) < shares:
            return apology("Not enough shares")
        
        # add transaction to history
        db.execute("INSERT INTO histories (symbol, shares, price, id) \
                    VALUES(:symbol, :shares, :price, :id)", \
                    symbol=stock["symbol"], shares=-shares, \
                    price=usd(stock["price"]), id=session["user_id"])
                       
        # credit the cash recieved by user to their account             
        db.execute("UPDATE users SET cash = cash + :purchase WHERE id = :id", \
                    id=session["user_id"], \
                    purchase=stock["price"] * float(shares))
                        
        # update share count to current after selling
        shares_total = user_shares[0]["shares"] - shares
        
        # if current shares is zero , delete object of that symbol from database
        if shares_total == 0:
            db.execute("DELETE FROM portfolio \
                        WHERE id=:id AND symbol=:symbol", \
                        id=session["user_id"], \
                        symbol=stock["symbol"])
        # if shares >0 then ensure current amount of sshares is updated on database
        else:
            db.execute("UPDATE portfolio SET shares=:shares \
                    WHERE id=:id AND symbol=:symbol", \
                    shares=shares_total, id=session["user_id"], \
                    symbol=stock["symbol"])
        
        # return to homepage
        return redirect(url_for("index"))

#EXTRA FEATURE
#Add more cash to existing wallet
@app.route("/addMoney", methods=["GET", "POST"])
@login_required
def addMoney():
    """Add Money to existing wallet"""
    
    if request.method == "POST":
        
        # ensure amount must be integers
        try:
            amount = int(request.form.get("amount"))
            if amount < 0:
                return apology("Money added must be positive amount")
            
        except:
            return apology("Money added must be positive amount")
            
        # update user cash (increase)              
        db.execute("UPDATE users SET cash = cash + :amount WHERE id = :id", \
                    amount=amount, id=session["user_id"])
        
        # return to index
        return apology("Amount added Sucessfully")
    
    else:
        return render_template("addMoney.html")
