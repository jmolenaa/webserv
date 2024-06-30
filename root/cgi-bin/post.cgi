#!/usr/bin/env python3

import cgi, cgitb
import sys

try:
    form = cgi.FieldStorage()
except Exception as e:
    sys.exit(12)

try:
    starter=form.getvalue("starter")
    main=form.getvalue("main")
    dessert=form.getvalue("dessert")
except Exception as e:
    sys.exit(12)

print("<!DOCTYPE html>")
print("<html>")
print("<head>")
print("<title> YOUR ORDER </title>")
print("</head>")
print("<body>")
print("<h3> Your Order is Served! </h3> <br/>")
print(f"This is your starter: {starter} <br/>")
print(f"Here is your main: {main} <br/>")
print(f"And finally, your dessert: {dessert} <br/>")
print("<br/> ENJOY YOUR MEAL!")
print("</body>")
print("</html>")