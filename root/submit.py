import cgi, cgitb

form = cgi.FieldStorage()

cake=form.getvalue("cake")
food=form.getvalue("food")
print("Content-type:text/html")
print("Favorite cake is: %s" %(cake))
print("Favorite food is: %s" %(food))