#!/usr/bin/env python3

import cgi, cgitb

form = cgi.FieldStorage()

filename=form.getvalue("filename")

print("<!DOCTYPE html>")
print("<html>")
print("<head>")
print("<title> ORDER CANCELLATION </title>")
print("</head>")
print("<body>")
print("<h3> Cancellation successful! </h3> <br/>")
print("Order number: %s has been cancelled <br/>" %(filename))
print("<br/> TCH, YOUR LOSS!")
print("</body>")
print("</html>")