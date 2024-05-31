import cgi, cgitb

form = cgi.FieldStorage()

starter=form.getvalue("starter")
main=form.getvalue("main")
dessert=form.getvalue("dessert")

print("Content-type:text/html\r\n\r\n")

print("<html>")
print("<head>")
print("<title> YOUR ORDER </title>")
print("</head>")
print("<body>")
print("<h3> Your Order is Served! </h3><")
print("Please enjoy your starter: %s" %(starter))
print("Here is your main: %s" %(main))
print("And finally, here is your dessert: %s" %(dessert))
print("</body>")
print("</html>")