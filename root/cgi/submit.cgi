#!/usr/bin/env python3

import cgi, cgitb

form = cgi.FieldStorage()

starter=form.getvalue("starter")
main=form.getvalue("main")
dessert=form.getvalue("dessert")

print("<!DOCTYPE html>")
print("<html>")
print("<head>")
print("<title> YOUR ORDER </title>")
print("</head>")
print("<body>")
print("<h3> Your Order is Served! </h3> <br/>")
print("This is your starter: %s <br/>" %(starter))
print("Here is your main: %s <br/>" %(main))
print("And finally, your dessert: %s <br/>" %(dessert))
print("<br/> ENJOY YOUR MEAL!")
print("</body>")
print("</html>")